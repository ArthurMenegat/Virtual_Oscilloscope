/*
    GUI Header File.
    Note: This document contains the Graphical User Interface functions declarations. 
    By: Arthur.
*/

#ifndef APP_STATE_H
#define APP_STATE_H

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

#define MAX_CHANNELS 3

typedef struct screen_t screen_t;

typedef struct
{
    bool is_fullscreen;
    bool toggle_terminal;
    bool toggle_init;
    bool toggle_settings;
    bool is_port_initialized;
    bool edit_drop_box[MAX_CHANNELS];
    bool edit_drop_box_b[MAX_CHANNELS];
    int active_baud[MAX_CHANNELS];
    int active_channel[MAX_CHANNELS];
}app_state_t;

void InitApplication(app_state_t* app_state, screen_t* screen, serial_port_t* my_com, wave_t* my_wave, serial_read_t* my_serial);
void CloseApplication(app_state_t* app_state, serial_port_t* my_com); 
void UpdateHardwareAndState(app_state_t* app_state, serial_port_t* my_com, serial_read_t* my_serial, wave_t* my_wave, screen_t* screen);
void RenderGraphics(app_state_t* app_state, serial_read_t* my_serial, wave_t* my_wave, screen_t* screen, serial_port_t* my_com);

#endif