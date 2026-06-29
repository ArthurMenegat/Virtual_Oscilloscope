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

#include "GUI.h"
#include "raylib.h"
#include "raygui.h"

// Screen defines
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540
#define WINDOW_TITLE "Virtual Oscilloscope"

// Terminal button defines
#define T_BUTTON_WIDTH 150
#define T_BUTTON_HEIGHT 50
#define T_BUTTON_Y_POS 25
#define T_BUTTON_COLOR GREEN

#define TERMINAL_TEXT_COLOR DARKGREEN

#define TAB_WIDTH 300
#define TAB_HEIGHT 1080

bool is_fullscreen = false;
bool terminal_toggle = false;
int width = 0;
int height = 0;

bool edit_text_box = false;

Rectangle terminal_button = {20, T_BUTTON_Y_POS, T_BUTTON_WIDTH, T_BUTTON_HEIGHT}; 
Rectangle tab = {1920 - TAB_WIDTH, 0, TAB_WIDTH, TAB_HEIGHT};  

void GetScreenResolution()
{
    int monitor = GetCurrentMonitor();
    
    width = GetMonitorWidth(monitor);
    height = GetMonitorHeight(monitor);
}

void DrawGrid2D()
{ 
    // Horizontal Lines.
    for(int i = height; i > 0; i -= 90)
    {
        DrawLine(0, height - i, width, height - i, GRAY);
    }
    
    // Vertical Lines.
    for(int i = 1; i < width; i += 96)
    {
        DrawLine(96 + i, 0, 96 + i, height, GRAY);
    }
}

void TurnFullscreen()
{   
    is_fullscreen = !is_fullscreen;
    
    if(is_fullscreen)
    {        
        SetWindowSize(width, height);
       
        ToggleFullscreen();
        
        //terminal_button.x = 20;
    }
    else
    {
        ToggleFullscreen();
        SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
     
        //terminal_button.x = 20;
    }
}

void DrawTerminalText(SerialRead_t* serial)
{
    GuiToggle(terminal_button, "Terminal", &terminal_toggle);
    
    if(terminal_toggle)
    {        
        GuiWindowBox(tab, "Terminal");         
    }
    
    //DrawText("------Serial Terminal------", 1500, 120, 20, TERMINAL_TEXT_COLOR);
    //DrawText(TextFormat("Number of Bytes Read: %lu", bytes_read), 1500, 150, 20, TERMINAL_TEXT_COLOR);
    //DrawText(TextFormat("Data received: %s", display_data), 1500, 200, 20, TERMINAL_TEXT_COLOR);
    //DrawText(TextFormat("Period: %u", period), 1500, 250, 20, TERMINAL_TEXT_COLOR);
}