#include "Hulk.h"

// nothing doing in the constructor and destructor
Hulk::Hulk() {}

void Hulk::init(int x, int y) {
    
    // hulk
    _height = 26;
    _width = 20;
    _hulk_x = x;
    _hulk_y = y;
    
    // fireball
    _fireball_sizex = 13;
    _fireball_sizey = 11;
    // the bottom fireball
    _fireball1_x = 2 + _width + 1;
    _fireball1_y = 18 + abs((_height - _fireball_sizey)/2);
    // the upper fireball
    _fireball2_x = 2 + _width + 1;
    _fireball2_y = 3;
    _fireball_speed = 4;
    
    _hulk_velocity.x = 3; // default velocity in x-direction
    _hulk_velocity.y = -3; // default velocity in y-direction
    _hulk_bosstransition_jump = -1;
    _hulk_velocity_walk = 3;
    _hulk_velocity_jump = -4;
    
    _counter_jumpheight = 0;
    _counter_jumpheight_bosstransition = 0;
    _counter_jumpheight_boss = 0;
    _counter_hulkattack = 0;
    _fireball_flag1 = 0;
    _fireball_flag2 = 0;
}

void Hulk::draw(N5110 &lcd,int &g_hulkattack_flag) {
    
    // printf for debugging on coolterm
    printf("\n\n |||||||||||||||||||||||||||| \n");
    printf("\n g_hulkattack_flag = %u \n",g_hulkattack_flag);
    
    // for other scenes
    if (g_hulkattack_flag == 0) {
        lcd.drawSprite(_hulk_x,_hulk_y,26,20,(int *)hulk);
        printf("\n || normal hulk control flag 0 ||\n\n");
    }
    
    // hulk attack
    else if (g_hulkattack_flag == 1) {
        
        if ((_counter_hulkattack >= 0) && (_counter_hulkattack < 2)) {
            lcd.drawSprite(_hulk_x - 3,_hulk_y,26,26,(int *)hulk_attack);
            printf("\n << attack hulk motion >> == 0/1 : \n");
            printf(" _counter_hulkattack = %u \n",_counter_hulkattack);
            _counter_hulkattack += 1;
        }
        else { 
            printf(" << back to normal hulk motion >> == 2 : \n");
            printf(" _counter_hulkattack = %u \n",_counter_hulkattack);
            _counter_hulkattack = 0;
            g_hulkattack_flag = 0;
            lcd.drawSprite(_hulk_x,_hulk_y,26,20,(int *)hulk);
        }
        
    }
    
    _g_hulkattack_flag = g_hulkattack_flag;
    
}

void Hulk::fireball_draw(N5110 &lcd,int &g_hulkattack_flag) {
    
    if (g_hulkattack_flag == 1) {
        
        if ((_hulk_y >= 14) && (_hulk_y <= 18)) {
            _fireball_flag1 = 1;
            if (_fireball_flag2 == 1) { // debug
                _fireball_flag1 = 0;
            }
        } 
        else if ((_hulk_y >= 2) && (_hulk_y <= 13)) {
            _fireball_flag2 = 1;
            if (_fireball_flag1 == 1) { // debug
                _fireball_flag2 = 0;
            }
        }
        
    }
    
    if (_fireball_flag1 == 1) {
    
        _fireball1_x += _fireball_speed;
        lcd.drawSprite(_fireball1_x,_fireball1_y,_fireball_sizey,_fireball_sizex,(int *)fireball);
        
        if (_fireball1_x > WIDTH - _fireball_sizex - 1) { // _fireball1_x > 82
            _fireball_flag1 = 0;
            _fireball1_x = 2 + _width + 1;
            printf("\n --initialize the fireball position-- \n");
        }
         
        printf("\n << BOTTOM fireball >> \n");
        printf(" _fireball1_x = %u \n",_fireball1_x);
        
    }
    
    if (_fireball_flag2 == 1) {
    
        _fireball2_x += _fireball_speed;
        lcd.drawSprite(_fireball2_x,_fireball2_y,_fireball_sizey,_fireball_sizex,(int *)fireball);
        
        if (_fireball2_x > WIDTH - _fireball_sizex - 1) { // _fireball2_x > 82
            _fireball_flag2 = 0;
            _fireball2_x = 2 + _width + 1;
            printf("\n --- initialize the fireball position --- \n");
        }
        
        _fireball_flag1 = 0; // prevent the upper fireball triggers the bottom fireball
        printf("\n << UPPER fireball >> \n");
        printf(" _fireball2_x = %u \n",_fireball2_x);
        
    }
    
}

void Hulk::bubble1_collision_draw(int hulk_x,int hulk_y,N5110 &lcd) {
    lcd.drawSprite(hulk_x,hulk_y,26,28,(int *)hulk_bubble_collision);
}

void Hulk::bubble2_collision_draw(int hulk_x,int hulk_y,N5110 &lcd) {
    lcd.drawSprite(hulk_x,hulk_y,26,28,(int *)hulk_bubble_collision);
}

void Hulk::update(UserInput input){
    
    // update x value depending on direction of movement
    if (input.d == W) { _hulk_x -= _hulk_velocity.x; }
    else if (input.d == E) { _hulk_x += _hulk_velocity.x; }
    
    // automatic bouncing along y-axis
    if (_counter_jumpheight == 8) {
        _hulk_velocity.y = -_hulk_velocity.y;
        _counter_jumpheight = 0;
    }
    
    _counter_jumpheight += 1;
    _hulk_y += _hulk_velocity.y;
        
    // check the x origin to ensure that the hulk doesn't go off screen
    if (_hulk_x < 1) { _hulk_x = 1; }
    if (_hulk_x > WIDTH - _width - 1) { _hulk_x = WIDTH - _width - 1; }
    
}

void Hulk::update_BossTransition_jump(UserInput input){
  
    // update x value depending on direction of movement
    if (input.d == W) { _hulk_x -= _hulk_velocity_walk; }
    else if (input.d == E) { _hulk_x += _hulk_velocity_walk; }
    
    // automatic bouncing along y-axis
    if (_counter_jumpheight_bosstransition == 6) {
        _hulk_bosstransition_jump = -_hulk_bosstransition_jump;
        _counter_jumpheight_bosstransition = 0;
    }
    
    _counter_jumpheight_bosstransition += 1;
    _hulk_y += _hulk_bosstransition_jump;
        
    // check the x origin to ensure that the hulk doesn't go off screen
    if (_hulk_x < 1) { _hulk_x = 1; }
    if (_hulk_x > WIDTH - _width - 1) { _hulk_x = WIDTH - _width - 1; }
    
}

void Hulk::update_BossStage_walk(UserInput input){
    
    // update x value depending on direction of movement
    if (input.d == W) { _hulk_x -= _hulk_velocity_walk; }
    else if (input.d == E) { _hulk_x += _hulk_velocity_walk; }
        
    // check the x origin to ensure that the hulk doesn't go off screen
    if (_hulk_x < 1) { _hulk_x = 1; }
    if (_hulk_x > WIDTH - 23 - _width - 1) { _hulk_x = WIDTH - 23 - _width - 1; }
}

void Hulk::update_BossStage_jump(){
    
    // bounce
    if (_counter_jumpheight_boss == 4) {
        _hulk_velocity_jump = -_hulk_velocity_jump;
        _counter_jumpheight_boss = 0;
    }
        
    _hulk_y += _hulk_velocity_jump;
    _counter_jumpheight_boss += 1;
    
    if (_hulk_x < 1) { _hulk_x = 1; }
    if (_hulk_x > WIDTH - 23 - _width - 1) { _hulk_x = WIDTH - 23 - _width - 1; }
    
    //if (_hulk_y > 18) { _hulk_y = 18; } // prevent bug
    //if (_hulk_y < 1) { _hulk_y = 4; } // prevent bug
    
    //debugging
    printf("\n  | %u | _y = %u |\n\n", _counter_jumpheight_boss, _hulk_y);
}

void Hulk::set_hulk_velocity(Position2D v){
    _hulk_velocity.x = v.x;
    _hulk_velocity.y = v.y;
}

void Hulk::set_hulk_pos(Position2D p) {
    _hulk_x = p.x;
    _hulk_y = p.y;
}

void Hulk::set_fireball1_pos(Position2D p) {
    _fireball1_x = p.x;
    _fireball1_y = p.y;
}

void Hulk::set_fireball2_pos(Position2D p) {
    _fireball2_x = p.x;
    _fireball2_y = p.y;
}


void Hulk::set_counter_jumpheight(int counter) {
    _counter_jumpheight = counter;
}

void Hulk::set_fireball_flag1(int fireball_flag1) {
    _fireball_flag1 = fireball_flag1;
}

void Hulk::set_fireball_flag2(int fireball_flag2) {
    _fireball_flag2 = fireball_flag2;
}

Position2D Hulk::get_hulk_velocity(){ return {_hulk_velocity.x,_hulk_velocity.y}; }

Position2D Hulk::get_hulk_pos() { return {_hulk_x,_hulk_y}; }

Position2D Hulk::get_hulk_size() { return {_width,_height}; }

Position2D Hulk::get_fireball1_pos() { return {_fireball1_x,_fireball1_y}; } // upper

Position2D Hulk::get_fireball2_pos() { return {_fireball2_x,_fireball2_y}; } // bottom

Position2D Hulk::get_fireball_size() { return {_fireball_sizex,_fireball_sizey}; }

int Hulk::get_g_hulkattack_flag() { return _g_hulkattack_flag; }

int Hulk::get_counter_jumpheight() { return _counter_jumpheight; }
