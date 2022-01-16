#ifndef PLATFORM_H
#define PLATFORM_H

#include "mbed.h"
#include "N5110.h"
#include "Utils.h"

class Platform
{
public:

    Platform();
    void init(int height, int width);
    void draw(N5110 &lcd);
    void update();
    Position2D get_pos1();
    Position2D get_pos2();
    Position2D get_pos3();
    Position2D get_pos4();
    int get_speed1();
    int get_speed2();
    int get_speed3();
    int get_speed4();
    Position2D get_size();

private:

    int _height;
    int _width;
    int _p1x;
    int _p2x;
    int _p3x;
    int _p4x;
    int _p1y;
    int _p2y;
    int _p3y;
    int _p4y;
    int _p1_speed;
    int _p2_speed;
    int _p3_speed;
    int _p4_speed;
    int _score;

};
#endif