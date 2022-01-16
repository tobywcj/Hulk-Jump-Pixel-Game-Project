#include "GameEngine.h"

GameEngine::GameEngine() {    // initialization
    _hulk_lives = 4;
    _loki_lives = 4;
    _floor = 1;
    _counter_door = 0;
    _counter_WinBoss_walk = 0;
    _counter_WinBoss_hit = 0;
    _counter_WinBoss_hit_times = 0;
    _g_hulkattack_flag_feedback = 0;
    _JumpStop_flag = 0;
    _WinBoss_flag = 1;
    _bubble1_collision = false;
    _bubble2_collision = false;
    _fireball1_collision = false;
    _fireball2_collision = false;
    _WinBoss_HulkWalk = 1 - 24;
} 

void GameEngine::reset_game(int floor,int hulk_lives,int loki_lives) {
    _floor = floor;
    _hulk_lives = hulk_lives;
    _loki_lives = loki_lives;
}

void GameEngine::hulk_init() {
    Position2D hulk_pos = _hulk.get_hulk_pos();
    _hulk.set_hulk_pos({hulk_pos.x,18});
}

void GameEngine::init(int hulk_x,int hulk_y,int platform_height,int platform_width,int loki_x,int loki_y) {
    _platform.init(platform_height, platform_width);
    _hulk.init(hulk_x, hulk_y);
    _loki.init(loki_x, loki_y);
}

void GameEngine::tutorial2_draw(N5110 &lcd,int &g_hulkattack_flag) {
    // draw the elements in the LCD buffer
    lcd.drawRect(0,0,WIDTH,HEIGHT,FILL_TRANSPARENT);
    
    g_hulkattack_flag = 0;
    _hulk.draw(lcd,g_hulkattack_flag);
}

void GameEngine::tutorial3_draw(N5110 &lcd) {
    // draw the elements in the LCD buffer
    lcd.drawLine(0,46,83,46,1);
    lcd.plotSpecificArray(84,45); // random moving sea wave
    
    srand(time(NULL));
    lcd.drawSprite(5,20,3,3,(int *)star);
    lcd.drawSprite(25,rand()%30,3,3,(int *)star);
    lcd.drawSprite(68,30,3,3,(int *)star);
    lcd.drawSprite(33,36,3,3,(int *)star);
    lcd.drawSprite(rand()%70,41,3,3,(int *)star);
    lcd.drawSprite(48,rand()%12,3,3,(int *)star);
    lcd.drawSprite(52,24,3,3,(int *)star);
    lcd.drawSprite(75,rand()%10,3,3,(int *)star);
    
    _platform.draw(lcd);
}

void GameEngine::tutorial2_update(UserInput input) {
    _hulk.update_BossTransition_jump(input);
}

void GameEngine::tutorial3_update() {
    _platform.update(); 
}

int GameEngine::update(UserInput input) {
    srand(time(NULL));
    
    if ((_floor > 0) && (_floor < 6)) {
        _platform.update(); 
        _hulk.update(input);
        _bubble_x = 3 + rand()%65;
        _bubble_y = 3 + rand()%30;
        // important to update platforms and hulk before checking collisions so can
        // correct for it before updating the display
        check_platform_collision();
        check_PlatformStage_hulk_bubble_collision();
    }
    else if (_floor == 6) {
        _hulk.update_BossTransition_jump(input);
    }
    
    refresh_scene();
    return _floor;
}

int GameEngine::BossStage_update(UserInput input) {
    _hulk.update_BossStage_walk(input);
    check_BossStage_hulk_bubble_collision();
    check_loki_fireball_collision();
    
    return _hulk_lives;
}

void GameEngine::BossStage_update_jump() {
    _hulk.update_BossStage_jump();
    check_BossStage_hulk_bubble_collision();
    check_loki_fireball_collision();
}

void GameEngine::BossStage_AfterCollision_update(int g_JumpStop_flag) {
    _JumpStop_flag = g_JumpStop_flag;
    _hulk.init(2,18);
}

void GameEngine::draw(N5110 &lcd,int &g_hulkattack_flag) {
    
    // draw the elements in the LCD buffer
    lcd.drawRect(0,0,WIDTH,HEIGHT,FILL_TRANSPARENT);
    
    if (_floor == 1) {
        lcd.drawLine(0,46,83,46,1);
        lcd.plotSpecificArray(84,45); // random moving sea wave
    }
    
    if (_floor == 5) {
        lcd.plotSpecificArray(84,1);
        lcd.plotSpecificArray(84,2);
    }
    
    srand(time(NULL));
    lcd.drawSprite(2,2 + rand()%2,3,3,(int *)star);
    lcd.drawSprite(3 + rand()%3,20,3,3,(int *)star);
    lcd.drawSprite(9,32 + rand()%4,3,3,(int *)star);
    lcd.drawSprite(13 + rand()%4,6,3,3,(int *)star);
    lcd.drawSprite(23 + rand()%2,30,3,3,(int *)star);
    lcd.drawSprite(64 + rand()%3,30,3,3,(int *)star);
    lcd.drawSprite(33,32 + rand()%4,3,3,(int *)star);
    lcd.drawSprite(50,39 + rand()%4,3,3,(int *)star);
    lcd.drawSprite(29 + rand()%4,7,3,3,(int *)star);
    lcd.drawSprite(44 + rand()%3,10,3,3,(int *)star);
    lcd.drawSprite(52,20 + rand()%4,3,3,(int *)star);
    lcd.drawSprite(73 + rand()%2,10,3,3,(int *)star);
    lcd.drawSprite(_bubble_x,_bubble_y,11,13,(int *)big_bubble);
    
    g_hulkattack_flag = 0;
    _hulk.draw(lcd,g_hulkattack_flag);
    _platform.draw(lcd);
}

void GameEngine::BossTransition_draw(N5110 &lcd,int &g_hulkattack_flag) {
    
    Position2D hulk_pos = _hulk.get_hulk_pos();
    Position2D hulk_size = _hulk.get_hulk_size();
    
    // motion of the door
    if (_counter_door == 0) {
        lcd.drawSprite(2,15,26,24,(int *)boss_door1);
    }
    else if (_counter_door == 1) {
        lcd.drawSprite(2,15,26,24,(int *)boss_door2);
    }
    else if (_counter_door == 2) {
        lcd.drawSprite(2,15,26,24,(int *)boss_door3);
    }
    else if (_counter_door == 3) {
        lcd.drawSprite(2,15,26,24,(int *)boss_door1);
    }
    else if (_counter_door == 4) {
        lcd.drawSprite(2,15,26,24,(int *)boss_door4);
        _counter_door = 0;
    }
    _counter_door += 1;
    
    // draw the elements in the LCD buffer
    lcd.drawRect(0,0,WIDTH,HEIGHT,FILL_TRANSPARENT);
    
    _hulk.draw(lcd,g_hulkattack_flag);
    
    if ((hulk_pos.x >= 1) && (hulk_pos.x + hulk_size.x <= 35)) {
        lcd.printString("Press buttonA",2,1);
    }
    
}

void GameEngine::BossStage_draw(N5110 &lcd,int g_timer_flag1,int g_timer_flag2,int &g_hulkattack_flag) {
    
    lcd.drawRect(0,0,WIDTH,HEIGHT,FILL_TRANSPARENT); 
    Position2D hulk_pos = _hulk.get_hulk_pos(); // taking the previous frame positions
    Position2D loki_pos = _loki.get_loki_pos();
    _loki_defence_flag = _loki.return_loki_defence_flag();
    
    printf("\n _loki_defence_flag = %u \n", _loki_defence_flag);
    
    // attacks from hulk
    _hulk.fireball_draw(lcd,g_hulkattack_flag);
    
    // loki is under attacked
    if ((_fireball1_collision == true) && (_loki_defence_flag == 1)) {
        printf("\n BOTTOM fireball is defended by Loki !!!!!!!!!!! \n");
        _loki_defence_flag = 0;
        _loki.fireball_defended_draw(loki_pos.x,loki_pos.y,lcd);
        thread_sleep_for(25);
        _fireball1_collision = false;
        _loki.set_loki_pos({WIDTH - 31 - 2,13}); 
    }
    else if ((_fireball1_collision == true) && (_loki_defence_flag == 0)) {
        printf("\n BOTTOM fireball attacked Loki !!!!!!!!!!! \n");
        _loki.fireball_collision_draw(loki_pos.x,loki_pos.y,lcd);
        thread_sleep_for(25);
        _fireball1_collision = false;
        _loki.init(WIDTH - 25,18);
        _loki_lives--;
    }
    else if ((_fireball2_collision == true) && (_loki_defence_flag == 1)) {
        printf("\n UPPER fireball is defended by Loki !!!!!!!!!!! \n");
        _loki_defence_flag = 0;
        lcd.drawSprite(loki_pos.x,loki_pos.y,32,29,(int *)loki_UnderAttack_shield_fly);
        thread_sleep_for(25);
        _fireball2_collision = false;
        _loki.set_loki_pos({WIDTH - 29 - 2,2}); 
    }
    else if ((_fireball2_collision == true) && (_loki_defence_flag == 0)) {
        printf("\n UPPER fireball attacked Loki !!!!!!!!!!! \n");
        _loki.fireball_collision_draw(loki_pos.x,loki_pos.y,lcd);
        thread_sleep_for(25);
        _fireball2_collision = false;
        _loki.init(WIDTH - 25,18);
        _loki_lives--;
    }
    else { _loki.draw(lcd); }
    
    // hulk is under attacked
    if (_bubble1_collision == true) {
        _hulk.bubble1_collision_draw(hulk_pos.x,hulk_pos.y,lcd);
        thread_sleep_for(25);
        _bubble1_collision = false;
    }
    else if (_bubble2_collision == true) {
        _hulk.bubble2_collision_draw(hulk_pos.x,hulk_pos.y,lcd);
        thread_sleep_for(25);
        _bubble2_collision = false;
    } 
    else { _hulk.draw(lcd,g_hulkattack_flag); } 
    
    // attacks from loki
    _loki.bubble_draw(lcd,g_timer_flag1,g_timer_flag2);
    
    _g_hulkattack_flag_feedback = _hulk.get_g_hulkattack_flag();

}

int GameEngine::BossStage_flag_feedback() {
    return _g_hulkattack_flag_feedback;
}

void GameEngine::check_platform_collision()
{
    // read current hulk attributes
    Position2D hulk_pos = _hulk.get_hulk_pos();
    Position2D hulk_velocity = _hulk.get_hulk_velocity();
    Position2D hulk_size = _hulk.get_hulk_size();
    int counter_jumpheight = _hulk.get_counter_jumpheight();
 
    // check platform
    Position2D platform1_pos = _platform.get_pos1(); // bottom
    Position2D platform2_pos = _platform.get_pos2();
    Position2D platform3_pos = _platform.get_pos3();
    Position2D platform4_pos = _platform.get_pos4(); // top
    Position2D platform_size = _platform.get_size();
    
    // see if hulk has hit the platform by checking for overlaps
    // platform 1 collision
    if (
        (hulk_pos.y + hulk_size.y >= platform1_pos.y) && //top
        (hulk_pos.y + hulk_size.y <= platform1_pos.y + platform_size.y) && //bottom
        (hulk_pos.x >= platform1_pos.x - 6) && //left
        (hulk_pos.x + hulk_size.x <= platform1_pos.x + platform_size.x + 6)  //right
    ) {
        // if it has, fix position
        hulk_pos.y = platform1_pos.y - hulk_size.y;
        hulk_velocity.y = -2;
        
    } 
    
    // platform 2 collision
    else if (
        (hulk_pos.y + hulk_size.y >= platform2_pos.y) && //top
        (hulk_pos.y + hulk_size.y <= platform2_pos.y + platform_size.y + 1) && //bottom
        (hulk_pos.x >= platform2_pos.x - 6) && //left
        (hulk_pos.x + hulk_size.x <= platform2_pos.x + platform_size.x + 6)  //right
    ) {
        // if it has, fix position
        hulk_pos.y = platform2_pos.y - hulk_size.y;
        hulk_velocity.y = -3;
        
    }
    
    // platform 3 collision
    else if (
        (hulk_pos.y + hulk_size.y >= platform3_pos.y) && //top
        (hulk_pos.y + hulk_size.y <= platform3_pos.y + platform_size.y) && //bottom
        (hulk_pos.x >= platform3_pos.x - 6) && //left
        (hulk_pos.x + hulk_size.x <= platform3_pos.x + platform_size.x + 6)  //right
    ) {
        // if it has, fix position
        hulk_pos.y = platform3_pos.y - hulk_size.y;
        hulk_velocity.y = -2;
        
    }
    
    // platform 4 collision
    else if (
        (hulk_pos.y + hulk_size.y >= platform4_pos.y) && //top
        (hulk_pos.y + hulk_size.y <= platform4_pos.y + platform_size.y + 2) && //bottom
        (hulk_pos.x >= platform4_pos.x - 6) && //left
        (hulk_pos.x + hulk_size.x <= platform4_pos.x + platform_size.x + 6)  //right
    ) {
        // if it has, fix position
        hulk_pos.y = platform4_pos.y - hulk_size.y;
        hulk_velocity.y = -4;
        
    }
    
    // falling motion
    else if ( 
        (counter_jumpheight == 8) &&
        (hulk_velocity.y == 2) 
    ) {
        // downward velocity
        if (
            (_floor == 1) &&
            (hulk_pos.y > HEIGHT - 40)
        ) {
            hulk_pos.y += 4;
        }
        else { 
            hulk_pos.y += 6; 
            counter_jumpheight = 0;
        }
        
    }
 
    // write new attributes
    _hulk.set_hulk_velocity(hulk_velocity);
    _hulk.set_hulk_pos(hulk_pos);
    _hulk.set_counter_jumpheight(counter_jumpheight);

}

void GameEngine::check_PlatformStage_hulk_bubble_collision()
{
    // read current hulk attributes
    Position2D hulk_pos = _hulk.get_hulk_pos();
    Position2D hulk_size = _hulk.get_hulk_size();
    
    // detection of BOTTOM bubble collision
    if (detect_collision({hulk_pos.x + hulk_size.x/2 ,hulk_pos.y + hulk_size.y},{_bubble_x + 13/2, _bubble_y + 11/2},10) == true) {
        printf("\n << bubble collision >> jump higherrrrrrrrr\n");
        hulk_pos.y -= 6;
    }
    
    // write new attributes
    _hulk.set_hulk_pos(hulk_pos);
}

void GameEngine::check_BossStage_hulk_bubble_collision()
{
    // read current hulk attributes
    Position2D hulk_pos = _hulk.get_hulk_pos();
    Position2D hulk_size = _hulk.get_hulk_size();
    
    // read current bubbles attributes
    Position2D loki_pos = _loki.get_loki_pos();
    Position2D bubble1_pos = _loki.get_bubble1_pos();
    Position2D bubble2_pos = _loki.get_bubble2_pos();
    Position2D bubble_size = _loki.get_bubble_size();
    
    // detection of BOTTOM bubble collision
    if (detect_collision({hulk_pos.x + hulk_size.x/2 ,hulk_pos.y + hulk_size.y/2},{bubble1_pos.x + bubble_size.x/2, bubble1_pos.y + bubble_size.y/2},12) == true) {
        printf("\n << BOTTOM bubble collision >> \n");
        _hulk.set_hulk_pos({2,18}); // setting the position of the hulk
        bubble1_pos.x = WIDTH - 22;
        printf(" --initialize the bottom bubble position after COLLISION-- \n");
        _hulk_lives--;
        _bubble1_collision = true;
    }
    
    // detection of UPPER bubble collision
    else if (detect_collision({hulk_pos.x + hulk_size.x/2 ,hulk_pos.y + hulk_size.y/2},{bubble2_pos.x + bubble_size.x/2, bubble2_pos.y + bubble_size.y/2},12) == true) {
        printf("\n << UPPER bubble collision >> \n");
        _hulk.set_hulk_pos({2,13}); // minimum hulk_pos.y = 11 to avoid collision
        bubble2_pos.x = WIDTH - 22;
        printf(" --initialize the upper bubble position after COLLISION-- \n");
        _hulk_lives--;
        _bubble2_collision = true;
        _JumpStop_flag = 1;
    }
    
    // write new attributes
    _loki.set_bubble1_pos(bubble1_pos);
    _loki.set_bubble2_pos(bubble2_pos);

}

int GameEngine::BossStage_JumpStop_flag() {
    return _JumpStop_flag;
}

void GameEngine::check_loki_fireball_collision()
{ 
    // read current loki attributes
    Position2D loki_pos = _loki.get_loki_pos();
    Position2D loki_size = _loki.get_loki_size();
    // read current fireball attributes
    Position2D fireball1_pos = _hulk.get_fireball1_pos(); // bottom fireball
    Position2D fireball2_pos = _hulk.get_fireball2_pos(); // upper fireball
    Position2D fireball_size = _hulk.get_fireball_size();
    
    // detection of BOTTOM fireball collision
    if (detect_collision({loki_pos.x,loki_pos.y + loki_size.y/2},{fireball1_pos.x + fireball_size.x, fireball1_pos.y + fireball_size.y/2},6) == true) {
        printf("\n << BOTTOM fireball collision >> \n");
        _loki.set_loki_pos({WIDTH - 37 - 2,18});   
        fireball1_pos.x = 23;
        _hulk.set_fireball_flag1(0);
        _fireball1_collision = true;
    }
    
    // detection of UPPER fireball collision
    else if (detect_collision({loki_pos.x, loki_pos.y + loki_size.y/2},{fireball2_pos.x + fireball_size.x, fireball2_pos.y + fireball_size.y/2},9) == true) {
        printf("\n << UPPER fireball collision >> \n");
        _loki.set_loki_pos({WIDTH - 37 - 2,2}); 
        fireball2_pos.x = 23;
        _hulk.set_fireball_flag2(0);
        _fireball2_collision = true;
        _JumpStop_flag = 1;
    }
    
    // write new attributes
    _hulk.set_fireball1_pos(fireball1_pos);
    _hulk.set_fireball2_pos(fireball2_pos);

}

int GameEngine::return_loki_lives() {
    return _loki_lives;
}

bool GameEngine::detect_collision(Position2D player, Position2D target, int range)
{
    // pythagoras
    float distance = sqrt( pow(player.x - target.x, 2) + pow(player.y - target.y,2) );
    
    if (distance < range) { return true; }
    else { return false; }

}

void GameEngine::refresh_scene() {
    Position2D hulk_pos = _hulk.get_hulk_pos();
    Position2D hulk_size = _hulk.get_hulk_size();
    Position2D platform_size = _platform.get_size();
    
    // check if hulk position has gone off the top
    if (hulk_pos.y + hulk_size.y < 0) {
        // reset the scene
        _platform.init(platform_size.y, platform_size.x);
        _hulk.init(WIDTH/2 - hulk_size.x/2, 30);
        _floor++; // jump off the top screen
        
        if (_floor == 6) {
            _hulk.init(WIDTH - hulk_size.x, 41 - hulk_size.y); // walk to the boss door
        }
            
    }
    // falling to the previous floor
    else if (
       (hulk_pos.y >= HEIGHT - 18) &&
       (hulk_pos.y <= HEIGHT)
    ) {
        _hulk.init(hulk_pos.x, 0);
        _floor--;
    }
    
}

void GameEngine::WinBoss_background_draw(N5110 &lcd) {
    
    srand(time(NULL));
    lcd.drawLine(0,46,83,46,1);
    lcd.drawLine(0,45,83,45,1);
    lcd.drawLine(0,44,83,44,1);
    lcd.plotSpecificArray(84,43);
    lcd.drawSprite(2,2 + rand()%2,3,3,(int *)star);
    lcd.drawSprite(3 + rand()%3,20,3,3,(int *)star);
    lcd.drawSprite(9,32 + rand()%4,3,3,(int *)star);
    lcd.drawSprite(13 + rand()%4,6,3,3,(int *)star);
    lcd.drawSprite(23 + rand()%2,30,3,3,(int *)star);
    lcd.drawSprite(64 + rand()%3,30,3,3,(int *)star);
    lcd.drawSprite(33,32 + rand()%4,3,3,(int *)star);
    lcd.drawSprite(50,39 + rand()%4,3,3,(int *)star);
    lcd.drawSprite(29 + rand()%4,7,3,3,(int *)star);
    lcd.drawSprite(44 + rand()%3,10,3,3,(int *)star);
    lcd.drawSprite(52,20 + rand()%4,3,3,(int *)star);
    lcd.drawSprite(73 + rand()%2,10,3,3,(int *)star);
    lcd.drawSprite(75,44 - 25,25,5,(int *)loki_scepter);
    
}
    

void GameEngine::WinBoss_draw(N5110 &lcd,int WinBoss_flag) {
    
    _WinBoss_flag = WinBoss_flag;
    
    if ((_counter_WinBoss_walk >= 0) && (_counter_WinBoss_walk <= 20)) {
        
        lcd.drawSprite(75 - 19 - 5,42 - 25,26,19,(int *)loki_arrested);
        lcd.drawSprite(_WinBoss_HulkWalk,42 - 32,32,24,(int *)big_hulk);
        _WinBoss_HulkWalk += 2;
        _counter_WinBoss_walk += 1;
        
    }
    else if ((_counter_WinBoss_hit_times >= 0) && (_counter_WinBoss_hit_times <= 30)) {
        
        // motion of hulk
        if (_counter_WinBoss_hit == 0) {
            lcd.drawSprite(_WinBoss_HulkWalk,42 - 32,34,49,(int *)Hulk_HitLoki_1);
            _counter_WinBoss_hit += 1;
            printf("air");
        } 
        else if (_counter_WinBoss_hit == 1) {
            lcd.drawSprite(_WinBoss_HulkWalk,42 - 32,34,49,(int *)Hulk_HitLoki_2);
            _counter_WinBoss_hit = 0;
            printf("ground");
        }
        _counter_WinBoss_hit_times += 1;
        printf("\n |||||||||||||| _counter_WinBoss_hit_times = %u |||||||||||| \n",_counter_WinBoss_hit);
        
    }
    else {
        _counter_WinBoss_walk = 0;
        _counter_WinBoss_hit_times = 0;
        _WinBoss_HulkWalk = 1 - 24;
        _WinBoss_flag = 0;
    }
    
}

int GameEngine::return_WinBoss_flag() {
    return _WinBoss_flag;
}