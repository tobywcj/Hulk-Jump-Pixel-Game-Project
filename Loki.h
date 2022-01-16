#ifndef LOKI_H
#define LOKI_H

#include "mbed.h"
#include "N5110.h"
#include "Draw.h"
#include "Utils.h"  // for Position


/** Loki Class
@brief Loki in the game 
*/ 

class Loki
{

public:

    Loki();
    void init(int x, int y);
    void draw(N5110 &lcd);
    void bubble_draw(N5110 &lcd,int g_timer_flag1,int g_timer_flag2);
    void fireball_collision_draw(int loki_x,int loki_y,N5110 &lcd);
    void fireball_defended_draw(int loki_x,int loki_y,N5110 &lcd);
    void set_loki_pos(Position2D p);
    void set_bubble1_pos(Position2D p);
    void set_bubble2_pos(Position2D p);
    int return_loki_defence_flag();
    Position2D get_loki_pos();
    Position2D get_loki_size();
    Position2D get_bubble1_pos();
    Position2D get_bubble2_pos();
    Position2D get_bubble_size();

private:
    int _loki_sizex;
    int _loki_sizey;
    int _loki_x;
    int _loki_y;
    int _bubble_sizex;
    int _bubble_sizey;
    int _bubble1_x;
    int _bubble1_y;
    int _bubble2_x;
    int _bubble2_y;
    int _bubble1_speed;
    int _bubble2_speed;
    int _counter_bubble1;
    int _counter_bubble2;
    int _counter_loki;
    int _loki_defence_flag;
    
};
#endif