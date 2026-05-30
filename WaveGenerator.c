/*
*    Plotter Source File
*    Note: This document contains the plotter functions implementations 
*    By: Arthur
*/

#include "raylib.h"
#include "WaveGenerator.h"

void DrawGrid2D(int width, int height)
{ 
    // Horizontal Lines
    for(int i = height; i > 0; i -= 100)
    {
        DrawLine(0, height - i, width, height - i, GRAY);
    }
    
    // Vertical Lines
    for(int i = 0; i < width - 100; i += 100)
    {
        DrawLine(100 + i, 0, 100 + i, height, GRAY);
    }
}