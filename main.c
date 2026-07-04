/*
*    Main Source File.
*    By: Arthur.
*/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION

#include "raygui.h"

#include "serialread.h"
#include "wave.h"
#include "gui.h"
#include "appstate.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540
#define WINDOW_TITLE "Virtual Oscilloscope"

#define MENU_BAR_X_POS 0
#define MENU_BAR_Y_POS 0
#define MENU_BAR_WIDTH 1920
#define MENU_BAR_HEIGHT 90
#define MENU_BAR_COLOR DARKGRAY

#define MAX_CHANNELS 3

int main(void)
{
    app_state_t app_state = {0};
    screen_t screen = {0};
    serial_port_t my_com[MAX_CHANNELS] = {0}; // Port instance.
    serial_read_t my_serial[MAX_CHANNELS] = {0}; // Serial reading instance.
    wave_t wave[MAX_CHANNELS] = {0}; 

    bar_t my_menu_bar = {.x = MENU_BAR_X_POS, 
                         .y = MENU_BAR_Y_POS,
                         .width = MENU_BAR_WIDTH,
                         .height = MENU_BAR_HEIGHT,
                         .color = MENU_BAR_COLOR};   

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60); 

    InitApplication(&app_state, &screen, my_com, wave, my_serial);

    while(!WindowShouldClose())
    {          
        UpdateHardwareAndState(&app_state, my_com, my_serial, wave, &screen);
        RenderGraphics(&app_state, my_serial, wave, &screen, my_com);
        
        // CalculateSignalPeriod(&wave[1]);
 
        ShowWaveInfo(&wave[0]);
    }  
    
    CloseApplication(&app_state, my_com);
    
    CloseWindow();
    
    return 0;
}