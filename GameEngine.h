#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "mbed.h"
#include "N5110.h"
#include "Hulk.h"
#include "Utils.h"
#include "Platform.h"
#include "Draw.h"
#include "Tone.h"
#include "Loki.h"

class GameEngine {
    public:
        GameEngine();  // pass in the lcd object from the main file
        void reset_game(int floor,int hulk_lives,int loki_lives);
        void hulk_init();
        void init(int hulk_x,int hulk_y,int platform_height,int platform_width,int loki_x,int loki_y);
        void tutorial2_draw(N5110 &lcd,int &g_hulkattack_flag);
        void tutorial3_draw(N5110 &lcd);
        void tutorial2_update(UserInput input);
        void tutorial3_update();
        int update(UserInput input);
        int BossStage_update(UserInput input);
        void BossStage_update_jump();
        void BossStage_AfterCollision_update(int g_JumpStop_flag);
        void draw(N5110 &lcd,int &g_hulkattack_flag);
        void BossTransition_draw(N5110 &lcd,int &g_hulkattack_flag);
        void BossStage_draw(N5110 &lcd,int g_timer_flag1,int g_timer_flag2,int &g_hulkattack_flag);
        int BossStage_flag_feedback();
        int BossStage_JumpStop_flag();
        int return_loki_lives();
        bool detect_collision(Position2D player_location, Position2D goal_location, int range);
        void refresh_scene();
        void WinBoss_background_draw(N5110 &lcd);
        void WinBoss_draw(N5110 &lcd,int WinBoss_flag);
        int return_WinBoss_flag();
        
    private:
        void check_platform_collision();
        void check_PlatformStage_hulk_bubble_collision();
        void check_BossStage_hulk_bubble_collision();
        void check_loki_fireball_collision();
        int _floor;
        int _hulk_lives;
        int _loki_lives;
        bool _bubble1_collision;
        bool _bubble2_collision;
        bool _fireball1_collision;
        bool _fireball2_collision;
        int _counter_door;
        int _counter_WinBoss_walk;
        int _counter_WinBoss_hit;
        int _counter_WinBoss_hit_times;
        int _g_hulkattack_flag_feedback;
        int _JumpStop_flag;
        int _loki_defence_flag;
        int _set_hulk_x;
        int _set_hulk_y;
        int _WinBoss_HulkWalk;
        int _WinBoss_flag;
        int _bubble_x;
        int _bubble_y;
        Hulk _hulk;
        Platform _platform;
        Loki _loki;
};

#endif