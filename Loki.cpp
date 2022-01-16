#include "Loki.h"

// nothing doing in the constructor and destructor
Loki::Loki() {}

void Loki::init(int x, int y) {
    
    _loki_sizex = 23;
    _loki_sizey = 26;
    _loki_x = x;
    _loki_y = y;
    
    // bubbles initialization
    _bubble_sizex = 9;
    _bubble_sizey = 9;
    _bubble1_x = WIDTH - 44;
    _bubble1_y = _loki_y + 15;
    _bubble2_x = WIDTH - 44;
    _bubble2_y = _loki_y - 14;
                
     _bubble1_speed = -3; // bottom bubble// bubbles initialization
     _bubble2_speed = -4; // upper bubble
    
    _counter_loki = 0;
    _counter_bubble1 = 0; // bottom bubble
    _counter_bubble2 = 1; // upper bubble
    _loki_defence_flag = 0;
}

void Loki::draw(N5110 &lcd) {
    
    // loki motion
    if ((_counter_loki >= 0) && (_counter_loki <= 7)) {
        lcd.drawSprite(_loki_x,_loki_y,26,23,(int *)loki); // 58 18
        _loki_defence_flag = 0;
    }
    else if ((_counter_loki >= 8) && (_counter_loki <= 15)) { // 53 2
        if (_counter_loki == 8) {
            _loki_x -= 5;
            _loki_y -= 16;
        }
        lcd.drawSprite(_loki_x,_loki_y,32,29,(int *)loki_fly);
        _loki_defence_flag = 0;
    }
    else if ((_counter_loki >= 16) && (_counter_loki <= 25)) { // 52 13
        if (_counter_loki == 16) {
            _loki_x -= 1;
            _loki_y += 11;
        }
        lcd.drawSprite(_loki_x,_loki_y,31,29,(int *)loki_shield);
        _loki_defence_flag = 1;
    }
    else if ((_counter_loki >= 26) && (_counter_loki <= 31)) { // 53 2
        if (_counter_loki == 26) {
            _loki_x += 1;
            _loki_y -= 11;
        }
        lcd.drawSprite(_loki_x,_loki_y,32,29,(int *)loki_fly);
        _loki_defence_flag = 0;
    }
    else if ((_counter_loki >= 32) && (_counter_loki <= 43)) {
        lcd.drawSprite(_loki_x,_loki_y,32,29,(int *)loki_shield_fly);
        _loki_defence_flag = 1;
    }
    else {
        if (_counter_loki == 44) {
            _loki_x += 5;
            _loki_y += 16;
        }
        lcd.drawSprite(_loki_x,_loki_y,26,23,(int *)loki);
        _loki_defence_flag = 0;
        _counter_loki = 0;
    }
    
    printf("\n loki_x = %u ||| loki_y = %u \n",_loki_x,_loki_y);
    _counter_loki += 1;
    
}

void Loki::fireball_collision_draw(int loki_x,int loki_y,N5110 &lcd) {
    lcd.drawSprite(loki_x,loki_y,26,37,(int *)loki_hurted);
}

void Loki::fireball_defended_draw(int loki_x,int loki_y,N5110 &lcd) {
    lcd.drawSprite(loki_x - 4,loki_y - 5,31,41,(int *)loki_UnderAttacked_shield);
}

void Loki::bubble_draw(N5110 &lcd,int g_timer_flag1,int g_timer_flag2) {

    // bottom bubble motion
    srand(time(NULL));
    
    if ((_bubble1_x < 1) && (_counter_bubble1 == 0)) {
        _bubble1_x = WIDTH - 35;
        _counter_bubble1 += 1;
    }
    if ((_bubble2_x <= rand()%8) && (_counter_bubble1 == 1)) {
        _counter_bubble1 -= 1; 
    }
                    
    if (_counter_bubble1 == 0) { // bottom bubble
        if (g_timer_flag1) {
            g_timer_flag1 = 0;
            lcd.drawSprite(_bubble1_x,_bubble1_y,9,14,(int *)bubble);
            _bubble1_x += _bubble1_speed;
        }
    }
                    
    // upper bubble motion
    if ((_bubble2_x < 1) && (_counter_bubble2 == 0)) {
        _bubble2_x = WIDTH - 35;
        _counter_bubble2 += 1;
    }
    if ((_bubble1_x <= 1) && (_counter_bubble2 == 1)) {
        _counter_bubble2 -= 1; 
    }
    if (_counter_bubble2 == 0) { // upper bubble
        if (g_timer_flag2) { 
            g_timer_flag2 = 0;
            lcd.drawSprite(_bubble2_x,_bubble2_y,9,14,(int *)bubble);
            _bubble2_x += _bubble2_speed;
        }
    }
    
    // debugging
    //printf("| bubble1_pos.x (bottom) = %u | bubble2_pos.x (upper) = %u |",_bubble1_x,_bubble2_x); 
    //printf("\n counter_bubble1 = %u | counter_bubble2 = %u |\n\n",_counter_bubble1,_counter_bubble2);

}

void Loki::set_loki_pos(Position2D p) {
    _loki_x = p.x;
    _loki_y = p.y;
}

void Loki::set_bubble1_pos(Position2D p) {
    _bubble1_x = p.x;
    _bubble1_y = p.y;
}

void Loki::set_bubble2_pos(Position2D p) {
    _bubble2_x = p.x;
    _bubble2_y = p.y;
}

int Loki::return_loki_defence_flag() { return _loki_defence_flag; }

Position2D Loki::get_loki_pos() { return {_loki_x,_loki_y}; }

Position2D Loki::get_loki_size() { return {_loki_sizex,_loki_sizey}; }

Position2D Loki::get_bubble1_pos() { return {_bubble1_x,_bubble1_y}; }

Position2D Loki::get_bubble2_pos() { return {_bubble2_x,_bubble2_y}; }

Position2D Loki::get_bubble_size() { return {_bubble_sizex,_bubble_sizey}; }