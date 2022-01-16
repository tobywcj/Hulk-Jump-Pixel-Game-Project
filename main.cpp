///////////// includes /////////////////////
#include "mbed.h"
#include "platform/mbed_thread.h"
#include "Joystick.h"
#include "N5110.h"
#include "ShiftReg.h"  
#include "GameEngine.h"
#include "Bitmap.h"
#include "Utils.h"
#include "Draw.h"
#include "Tone.h"
#include "Music.h"
#include "Loki.h"

///////////// defines /////////////////////
#define PLATFORM_WIDTH 22
#define PLATFORM_HEIGHT 3
#define HULK_WIDTH 20
#define HULK_HEIGHT 24

///////////// objects ///////////////////
N5110 lcd(p14,p8,p9,p10,p11,p13,p21);
Joystick joystick(p20,p19);
AnalogIn joy_button(p17);
DigitalIn buttonA(p29);
DigitalIn buttonB(p28);
DigitalIn buttonC(p27);
DigitalIn buttonD(p26);
BusOut leds(LED4,LED3,LED2,LED1);
ShiftReg seven_seg;
GameEngine game;
Loki loki_bubble;
Hulk hulk_motion;
Ticker ticker;
AnalogIn pot(p17); // display lives
Tone dac(p18);

///////////// function prototypes ///////////////
void init();
void render(int &g_hulkattack_flag);
void welcome();
void display_floor(int floor);
void display_hulk_lives(int lives);
void display_loki_lives(int lives);
void print_menu();
void tutorial_1();
void tutorial_2(N5110 &lcd,int &g_hulkattack_flag);
void tutorial_3(N5110 &lcd);
void tutorial_4();
void tutorial_5();
void tutorial_6();
void boss_transition_background();
void boss_stage_background();
void game_over();
void win();
void timer_isr();

volatile int g_timer_flag1 = 0; // flag - must be volatile as changes within ISR
volatile int g_timer_flag2 = 0;
volatile int g_JumpStop_flag = 0;
int g_hulkattack_flag = 0;

////////////////////////////////////////

int main() {
    
    init();         // initialise devices and objects
    welcome();      // waiting for the user to start 
    int fps = 70;
    
    //int n = sizeof(SpiderMan_note_array)/sizeof(int);
    // tell it the number of notes, arrays, BPM and whether to repeat
    //dac.play_melody(n,SpiderMan_note_array,SpiderMan_duration_array,60,true);
    
    while (1) {
        
        print_menu();
        
        // if any buttons pressed then jump to appropriate menu function
        // START THE GAME
        if (buttonA.read() == 1) {
            
            game.init(2,18,3,22,0,0);       // hulk_x, hulk_y, platform_height, platform_width, loki_x, loki_y
            render(g_hulkattack_flag);      // first draw the initial frame 
            thread_sleep_for(1000/fps);     // and wait for one frame period - millseconds
            
            int floor = 1;   // display floors on LEDs
            
            while ((floor > 0) && (floor < 6)) {
                // read the joystick input and store in a struct
                display_floor(floor);
                UserInput input = {joystick.get_direction(),joystick.get_mag()};
                floor = game.update(input);          // update the game engine based on input
                game.update(input);                  // update the game engine based on input
                render(g_hulkattack_flag);               // draw frame on screen
                thread_sleep_for(1000/fps);              // and wait for one frame period - ms
            }
            
            if (floor == 0) {
                game_over();
            } 
            
            // boss transition stage
            else if (floor == 6) {
                game.init(61,15,0,0,0,0);
                
                // pass through the boss door to enter the next stage
                while (buttonA.read() == 0) {
                    display_floor(floor);
                    UserInput input = {joystick.get_direction(),joystick.get_mag()};
                    floor = game.update(input);    // update the game engine based on input
                    game.update(input);            // update the game engine based on input
                    boss_transition_background();
                    game.BossTransition_draw(lcd,g_hulkattack_flag);
                    lcd.refresh();
                    thread_sleep_for(1000/fps);        // and wait for one frame period - ms
                }
                
                floor++;
                printf("\n <<<<<<<<<<<<<<<<<<<< floor = %u >>>>>>>>>>>>>>>>>>>>>>>> \n",floor);
            }
            
            // Boss Stage
            if (floor == 7) {
                
                init();
                game.init(2,18,0,0,WIDTH - 23 - 2,18); // hulk_x, hulk_y, platform_height, platform_width, loki_x, loki_y
                int hulk_lives = 4;   // display hulk lives on LEDs
                int loki_lives = 4;   // display loki lives on LEDs
                int counter_jump = 0;
                int counter_jump_debug = 0;
                
                // set-up the ticker so that the ISR it is called every 0.05 seconds
                ticker.attach(&timer_isr,500ms);
                
                // boss stage
                while ((hulk_lives > 0) && (loki_lives > 0)) {
                    boss_stage_background();
                    UserInput input = {joystick.get_direction(),joystick.get_mag()};
                    hulk_lives = game.BossStage_update(input);
                    loki_lives = game.return_loki_lives();
                    display_hulk_lives(hulk_lives);
                    display_loki_lives(loki_lives);
                    
                    // control of the hulk
                    if (counter_jump == 0) {
                        game.BossStage_update(input); 
                    }
                    
                    if (buttonC.read()) { // hulk attack
                        g_hulkattack_flag = 1;
                    }
                    
                    if ((input.d == N) && (counter_jump_debug == 0)) { // hulk jump
                        counter_jump_debug += 1;
                        counter_jump = 1;
                        game.hulk_init(); // prevent hulk jumping bug
                    } 
                    
                    if ((counter_jump >= 1) && (counter_jump <= 8)) {
                        game.BossStage_update_jump();
                        counter_jump += 1;
                        g_JumpStop_flag = game.BossStage_JumpStop_flag();
                        if (g_JumpStop_flag == 1) {
                            printf("\n JumpStop_flag = %u \n",g_JumpStop_flag); // debug
                            g_JumpStop_flag = 0;
                            counter_jump = 0;
                            counter_jump_debug = 0;
                            game.BossStage_AfterCollision_update(g_JumpStop_flag);
                        }
                    } 
                    else {
                        counter_jump = 0;
                        counter_jump_debug = 0;
                        game.BossStage_update(input);
                    }
                    
                    game.BossStage_draw(lcd,g_timer_flag1,g_timer_flag2,g_hulkattack_flag);
                    g_hulkattack_flag = game.BossStage_flag_feedback();
                    lcd.refresh();
                    thread_sleep_for(1000/fps);
                    printf("\n hulk lives = %u\n",hulk_lives);
                    printf(" loki lives = %u\n",loki_lives);
                    printf("\n |||||||||||| \n");
                }
                
                if (hulk_lives == 0) {
                    thread_sleep_for(100);
                    game_over();
                } 
                else if (loki_lives == 0) {
                    floor++;
                }
                else if ((hulk_lives == 0) && (loki_lives == 0)) {
                    thread_sleep_for(100);
                    game_over();
                }
                
            }
            
            // a pixel animation video
            if (floor == 8) { 
                
                seven_seg.write(0x00);
                leds = 0b0000;
                display_floor(0);
                int WinBoss_flag = 1;
                
                while (WinBoss_flag == 1) {
                    lcd.clear();
                    game.WinBoss_background_draw(lcd);
                    game.WinBoss_draw(lcd,WinBoss_flag);
                    lcd.refresh();
                    thread_sleep_for(1000/fps);
                    WinBoss_flag = game.return_WinBoss_flag();
                }
                
                win();
                
            }
            
            init();
        }
        
        // TUTORIAL
        if (buttonC.read() == 1) {
            
            int counter_frame = 0;
            
            while (counter_frame <= 55) {
                tutorial_1();
                thread_sleep_for(1000/fps);
                counter_frame += 1;
            }
            counter_frame = 0;
            
            while (counter_frame <= 60) {
                UserInput input = {joystick.get_direction(),joystick.get_mag()};
                game.tutorial2_update(input);
                tutorial_2(lcd,g_hulkattack_flag);
                thread_sleep_for(1000/fps);
                counter_frame += 1;
            }  
            counter_frame = 0;
            
            while (counter_frame <= 55) {
                game.tutorial3_update();
                tutorial_3(lcd);
                thread_sleep_for(1000/fps);
                counter_frame += 1;
            }
            counter_frame = 0;
            
            while (counter_frame <= 55) {
                tutorial_4();
                thread_sleep_for(1000/fps);
                counter_frame += 1;
            }
            counter_frame = 0;
            
            while (counter_frame <= 55) {
                tutorial_5();
                thread_sleep_for(1000/fps);
                counter_frame += 1;
            }
            counter_frame = 0;
            
            while (buttonC.read() == 0) {
                tutorial_6();
            }
                    
            thread_sleep_for(100);
            lcd.clear();
            lcd.refresh();
            
        }
    
    }
}

void init() {
    seven_seg.write(0x00);
    leds = 0b0000;
    lcd.init();
    lcd.setContrast(0.35);
    joystick.init();
    game.init(WIDTH/2 - 10,23,3,22,WIDTH - 23 - 8,18); // hulk_x, hulk_y, platform_height, platform_width, loki_x, loki_y
    // dac.init();
    
    // PCB has external pull-down resistors so turn the internal ones off
    // (default for DigitalIn)
    buttonA.mode(PullNone);
    buttonB.mode(PullNone);
    buttonC.mode(PullNone);
    buttonD.mode(PullNone);
}

void render(int &g_hulkattack_flag) {  // clear screen, re-draw and refresh
    lcd.clear();  
    game.draw(lcd,g_hulkattack_flag);
    lcd.refresh();
}

void welcome() { // splash screen
    lcd.printString("Ready or Not?",0,1);  
    lcd.printString("Press D",0,4);
    lcd.refresh();
     
    // wait flashing LEDs until button A is pressed 
    while ( buttonD.read() == 0) {
        leds = 0b1111;
        thread_sleep_for(100);
        leds = 0b0000;
        thread_sleep_for(100);   
    }
}

void display_floor(int floor) {
    if (floor == 1) {
        seven_seg.write(0x06);
    } else if (floor == 2) {
        seven_seg.write(0x5B);
    } else if (floor == 3) {
        seven_seg.write(0x4F);
    } else if (floor == 4) {
        seven_seg.write(0x66);
    } else if (floor == 5) {
        seven_seg.write(0x6D);
    } else if (floor == 6) {
        seven_seg.write(0x7D);
    } else if (floor == 7) {
        seven_seg.write(0x07);
    } else if (floor == 8) {
        seven_seg.write(0x7F);
    } else {
        seven_seg.write(0x3F);
    }
}

void display_hulk_lives(int lives) {
    if (lives == 4) {
        seven_seg.write(0x66);
    } else if (lives == 3) {
        seven_seg.write(0x4F);
    } else if (lives == 2) {
        seven_seg.write(0x5B);
    } else if (lives == 1) {
        seven_seg.write(0x06);
    } else {
        seven_seg.write(0x3F);
    }
}

void display_loki_lives(int lives) {
    if (lives == 4) {
        leds = 0b1111;
    } else if (lives == 3) {
        leds = 0b1110;
    } else if (lives == 2) {
        leds = 0b1100;
    } else if (lives == 1) {
        leds = 0b1000;
    } else {
        leds = 0b0000;
    }
}

void print_menu()
{
    srand(time(NULL));
    lcd.clear();
    lcd.printString("HULK",6,1);
    lcd.printString("JUMP",11,2);
    lcd.drawSprite(45,3,26,27,(int *)logo);
    lcd.drawSprite(3 + rand()%3,3,3,3,(int *)star);
    lcd.drawSprite(39,11 + rand()%3,3,3,(int *)star);
    lcd.drawSprite(3 + rand()%3,26,3,3,(int *)star);
    lcd.drawSprite(79,27 + rand()%3,3,3,(int *)star);
    lcd.drawSprite(27 + rand()%3,24,3,3,(int *)star);
    lcd.drawSprite(71 + rand()%3,12,3,3,(int *)star);
    lcd.drawSprite(35,rand()%3,3,3,(int *)star);
    lcd.drawSprite(78 + rand()%3,2,3,3,(int *)star);
    lcd.printString("Start-hit A",0,4);
    lcd.printString("Tutorial-hit C",0,5);
    lcd.refresh();
}

void tutorial_1()
{
    srand(time(NULL));
    lcd.clear();
    lcd.printString("Welcome Player",0,0);
    lcd.printString("Hulk Control:",0,1);
    lcd.printString("Joystick &",0,2);
    lcd.printString("buttons",0,3);
    lcd.drawSprite(42 + rand()%24,27,22,16,(int *)iron_man);
    lcd.refresh();
}

void tutorial_2(N5110 &lcd,int &g_hulkattack_flag)
{
    lcd.clear();
    lcd.drawSprite(3 + rand()%67,47 - 1 - 15,15,13,(int *)fuel_can);
    game.tutorial2_draw(lcd,g_hulkattack_flag);
    lcd.printString(" Hulk Control",0,1);
    lcd.refresh();
}

void tutorial_3(N5110 &lcd)
{
    lcd.clear();
    game.tutorial3_draw(lcd);
    lcd.printString("Platform Stage",0,0);
    lcd.refresh();
}

void tutorial_4()
{
    lcd.clear();
    lcd.printString("If hulk sinks",0,0);
    lcd.printString("to the sea",0,1);
    lcd.printString("on floor 1...",0,2);
    lcd.printString("    lose !",0,4);
    lcd.refresh();
}

void tutorial_5()
{
    lcd.clear();
    lcd.printString("Boss Stage",0,0);
    lcd.printString(" Hulk   Boss?",0,1);
    lcd.drawSprite(6,15,32,24,(int *)big_hulk);
    lcd.drawSprite(50,21,26,23,(int *)loki_black);
    lcd.refresh();
}

void tutorial_6()
{
    lcd.clear();
    lcd.printString("Use buttonC",0,0);
    lcd.printString("to attack the",0,1);
    lcd.printString("boss !",0,2);
    lcd.printString("Press C",0,4);
    lcd.printString("to return menu",0,5);
    lcd.refresh();
}

    
void boss_transition_background()
{
    srand(time(NULL));
    lcd.clear();
    lcd.drawSprite(60,12 + rand()%4,3,3,(int *)star);
    lcd.drawSprite(25 + rand()%4,25,3,3,(int *)star);
    lcd.drawSprite(52 + rand()%3,34,3,3,(int *)star);
    lcd.drawSprite(68,28 + rand()%3,3,3,(int *)star);
    lcd.drawSprite(34 + rand()%4,36,3,3,(int *)star);
    lcd.drawSprite(27 + rand()%3,15,3,3,(int *)star);
    lcd.drawSprite(47,20 + rand()%4,3,3,(int *)star);
    lcd.drawSprite(73 + rand()%4,20,3,3,(int *)star);
    lcd.plotSpecificArray(84,40); // floating island
    lcd.drawLine(0,41,WIDTH,41,1);
    lcd.plotSpecificArray(84,42);
    lcd.drawLine(10,43,77,43,1);
    lcd.drawLine(15,44,68,44,1);
    lcd.drawLine(18,45,70,45,1);
    lcd.drawLine(25,46,65,46,1);
}

void boss_stage_background()
{
    lcd.clear();
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
    lcd.plotSpecificArray(84,43); // floating island
    lcd.drawLine(0,44,WIDTH,44,1);
    lcd.drawLine(12,45,75,45,1);
    lcd.drawLine(22,46,68,46,1);
}

void game_over() { // splash screen 
    while (buttonB.read() == 0) { // not yet press so remain low level
        lcd.clear();
        lcd.printString(" --Game Over-- ",0,1);  
        lcd.printString("Press B",0,3);
        lcd.printString("to return menu",0,4);
        lcd.refresh();
        leds = 0b1111;
        thread_sleep_for(50);
        lcd.clear();
        lcd.refresh();
        leds = 0b0000;
        thread_sleep_for(50);   
    }
    game.reset_game(1,4,4); // reset floor, hulk_lives, loki_lives
}

void win() {
    while (buttonB.read() == 0) {
        lcd.clear();
        lcd.printString("  -- Win! --   ",0,1);  
        lcd.printString("Press B",0,3);
        lcd.printString("to return menu",0,4);
        lcd.refresh();
        leds = 0b1111;
        thread_sleep_for(50);
        lcd.clear();
        lcd.refresh();
        leds = 0b0000;
        thread_sleep_for(50);   
    }
    game.reset_game(1,4,4); // reset floor, hulk_lives, loki_lives
}

// time-triggered interrupt
void timer_isr()
{
        g_timer_flag1 = 1;   // set flag in ISR
        g_timer_flag2 = 1;
}