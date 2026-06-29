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
#include "SerialRead.h"

void GetScreenResolution();
void DrawGrid2D(); // Draw a 2D grid on the screen.
void TurnFullscreen(); // Resizes screen.
void DrawTerminalText(SerialRead_t* serial); // Draw terminal text. 

#endif