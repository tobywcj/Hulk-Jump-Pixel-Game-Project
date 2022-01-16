#include "Platform.h"

// nothing doing in the constructor and destructor
Platform::Platform() {}

void Platform::init(int height,int width) {
    srand(time(NULL));
    _p1x = rand()%48; 
    _p2x = rand()%48;
    _p3x = rand()%48;
    _p4x = rand()%48;
    _p1y = 35 + rand()%4; 
    _p2y = 25 + rand()%4;
    _p3y = 15 + rand()%4;
    _p4y = 10;
    _height = height;
    _width = width;
    _p1_speed = 2;  // default speed
    _p2_speed = 2;
    _p3_speed = 2;
    _p4_speed = 2;
    
}

void Platform::draw(N5110 &lcd) { 
    lcd.drawRect(_p1x,37,_width,_height,FILL_BLACK); 
    lcd.drawRect(_p2x,27,_width,_height,FILL_BLACK); 
    lcd.drawRect(_p3x,17,_width,_height,FILL_BLACK); 
    lcd.drawRect(_p4x,7,_width,_height,FILL_BLACK); 
}

void Platform::update() {
    
    _p1x += _p1_speed;
    _p2x -= _p2_speed;
    _p3x += _p3_speed;
    _p4x -= _p4_speed;
    
    // check the x origin to ensure that the platforms don't go off screen
    // platform 1
    if (_p1x < 1) {
        _p1x = 1;
        _p1_speed = 2; 
    }
    if (_p1x > WIDTH - _width - 1) {
        _p1x = WIDTH - _width - 1; 
        _p1_speed = -2;
    }
    
    // platform 2
    if (_p2x < 1) {
        _p2x = 1;
        _p2_speed = -2; 
    }
    if (_p2x > WIDTH - _width - 1) {
        _p2x = WIDTH - _width - 1; 
        _p2_speed = 2;
    }
    
    // platform 3
    if (_p3x < 1) {
        _p3x = 1;
        _p3_speed = 2; 
    }
    if (_p3x > WIDTH - _width - 1) {
        _p3x = WIDTH - _width - 1; 
        _p3_speed = -2;
    }
    
    // platform 4
    if (_p4x < 1) {
        _p4x = 1;
        _p4_speed = -2; 
    }
    if (_p4x > WIDTH - _width - 1) {
        _p4x = WIDTH - _width - 1; 
        _p4_speed = 2;
    } 
    
}

Position2D Platform::get_pos1() { return {_p1x,_p1y}; }
Position2D Platform::get_pos2() { return {_p2x,_p2y}; }
Position2D Platform::get_pos3() { return {_p3x,_p3y}; }
Position2D Platform::get_pos4() { return {_p4x,_p4y}; }

int Platform::get_speed1() { return _p1_speed; }
int Platform::get_speed2() { return _p2_speed; }
int Platform::get_speed3() { return _p3_speed; }
int Platform::get_speed4() { return _p4_speed; }

Position2D Platform::get_size() { return {_width,_height}; }