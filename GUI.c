/*
    GUI Source File.
    Note: This document contains the Graphical User Interface functions implementations. 
    By: Arthur.
*/

#if defined(_WIN32)           
    #define NOGDI  
    #define NOUSER
    #define WIN32_LEAN_AND_MEAN  
#endif

#include <windows.h>         

#if defined(_WIN32)
    #undef CloseWindow
    #undef ShowCursor
    #undef DrawText
    #undef Rectangle
    #undef PlaySound
#endif

#include "gui.h"
#include "raylib.h"
#include "raygui.h"

// Screen defines
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

#define TERMINAL_TEXT_COLOR DARKGREEN

void GetScreenResolution(screen_t* screen)
{    
    screen -> monitor = GetCurrentMonitor();
    screen -> height = GetMonitorHeight(screen -> monitor);
    screen -> width = GetMonitorWidth(screen -> monitor);
}

void DrawGrid2D(screen_t* screen)
{ 
    // Horizontal Lines.
    for(int i = screen -> height; i > 0; i -= 90)
    {
        DrawLine(0, screen -> height - i, screen -> width, screen -> height - i, GRAY);
    }
    
    // Vertical Lines.
    for(int i = 1; i < screen -> width; i += 96)
    {
        DrawLine(96 + i, 90, 96 + i, screen -> height, GRAY);
    }
}

void TurnFullscreen(app_state_t* app_state, screen_t* screen)
{       
    if(app_state -> is_fullscreen)
    {        
        SetWindowSize(screen -> width, screen -> height);
       
        ToggleFullscreen();
    }
    else
    {
        ToggleFullscreen();
        SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    }
}

void DrawTerminal(Rectangle* my_button, serial_read_t* my_serial)
{
    DrawRectangle(1500, 0, 420, 1080, BLACK);
    DrawText("------Serial Terminal------", 1570, 0, 20, TERMINAL_TEXT_COLOR);
    DrawText(TextFormat("Number of Bytes Read: %lu", my_serial -> bytes_read), 1500, 150, 20, TERMINAL_TEXT_COLOR);
    DrawText(TextFormat("Data received: %s\n", my_serial -> display_data), 1500, 200, 20, TERMINAL_TEXT_COLOR);
}

void ShowWaveInfo(wave_t* my_wave)
{
    DrawText(TextFormat("Period: %u", my_wave -> period), 20, 90, 20, TERMINAL_TEXT_COLOR);
}

void ShowBar(bar_t* my_bar)
{
    Rectangle bar = {bar.x = my_bar -> x,
                     bar.y = my_bar -> y,
                     bar.width = my_bar -> width,
                     bar.height = my_bar -> height};
    
    DrawRectangleRec(bar, my_bar -> color);
}