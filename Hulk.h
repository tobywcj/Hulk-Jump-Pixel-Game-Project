#ifndef HULK_H
#define HULK_H

#include "mbed.h"
#include "N5110.h"
#include "Utils.h"  // for Position
#include "Draw.h"
#include <stdlib.h>


/** Hulk Class
@brief Controls the hulk in the game 
*/ 

class Hulk
{

public:

    Hulk();
    void init(int x, int y);
    void draw(N5110 &lcd,int &g_hulkattack_flag);
    void fireball_draw(N5110 &lcd,int &g_hulkattack_flag);
    void bubble1_collision_draw(int hulk_x,int hulk_y,N5110 &lcd);
    void bubble2_collision_draw(int hulk_x,int hulk_y,N5110 &lcd);
    void update(UserInput input);
    void update_BossTransition_jump(UserInput input);
    void update_BossStage_walk(UserInput input);
    void update_BossStage_jump();
    // accessors and mutators
    void set_hulk_velocity(Position2D v);
    void set_hulk_pos(Position2D p);
    void set_fireball1_pos(Position2D p);
    void set_fireball2_pos(Position2D p);
    void set_counter_jumpheight(int counter);
    void set_fireball_flag1(int fireball_flag1);
    void set_fireball_flag2(int fireball_flag2);
    int get_counter_jumpheight();
    int get_g_hulkattack_flag();
    Position2D get_hulk_velocity();
    Position2D get_hulk_pos();
    Position2D get_hulk_size();
    Position2D get_fireball1_pos();
    Position2D get_fireball2_pos();
    Position2D get_fireball_size();
    
private:
    int _height;
    int _width;
    int _hulk_x;
    int _hulk_y;
    int _fireball_sizex;
    int _fireball_sizey;
    int _fireball1_x;
    int _fireball1_y;
    int _fireball2_x;
    int _fireball2_y;
    int _fireball_speed;
    int _counter_jumpheight;
    int _counter_jumpheight_bosstransition;
    int _counter_jumpheight_boss;
    int _counter_hulkattack;
    int _hulk_velocity_jump;
    int _hulk_velocity_walk;
    int _g_hulkattack_flag;
    int _hulk_bosstransition_jump;
    int _fireball_flag1;
    int _fireball_flag2;
    Position2D _hulk_velocity;
    
};
#endif