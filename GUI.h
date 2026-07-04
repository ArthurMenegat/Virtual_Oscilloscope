/*
    GUI Header File.
    Note: This document contains the Graphical User Interface functions declarations. 
    By: Arthur.
*/

#ifndef GUI_H
#define GUI_H

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

#include "raylib.h"
#include "serialread.h"
#include "appstate.h"

typedef struct screen_t
{
    int width;
    int height;
    int monitor;
}screen_t;

typedef struct 
{
    int x; 
    int y;
    int width;
    int height;
    Color color;
}bar_t;

void GetScreenResolution(screen_t* screen);
void DrawGrid2D(screen_t* screen); // Draw a 2D grid on the screen.
void TurnFullscreen(app_state_t* app_state, screen_t* screen); // Resizes screen. 
void DrawTerminal(Rectangle* my_button, serial_read_t* my_serial);
void ShowWaveInfo(wave_t* my_wave);
void ShowBar(bar_t* my_bar);

#endif