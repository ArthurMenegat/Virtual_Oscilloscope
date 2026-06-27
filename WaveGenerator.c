/*
*    WaveGenerator Source File.
*    Note: This document contains the wave functions implementations. 
*    By: Arthur.
*/

#include "raylib.h"
#include "WaveGenerator.h"

#define SAMPLES 600
#define ADC_MAX_VALUE 1023

unsigned int sampling_buffer[SAMPLES] = {0};

void DrawGrid2D(int width, int height)
{ 
    // Horizontal Lines
    for(int i = height; i > 0; i -= 90)
    {
        DrawLine(0, height - i, width, height - i, GRAY);
    }
    
    // Vertical Lines
    for(int i = 1; i < width; i += 96)
    {
        DrawLine(96 + i, 0, 96 + i, height, GRAY);
    }
}

void AddValueToWave(int signal_value)
{ 
    for(int i = 0; i < SAMPLES - 1; ++i)
    {
        sampling_buffer[i] = sampling_buffer[i + 1]; // Shift buffer
    }
    
    sampling_buffer[SAMPLES - 1] = signal_value;
}

void DrawWave()
{  
   int y_min = 450; 
   int y_max = 180;
   int amplitude = y_min - y_max;
   
   int y_start = y_min - ((sampling_buffer[0] * amplitude) / ADC_MAX_VALUE);
   
   for(int i = 0; i < SAMPLES - 1; ++i)
   {
       int y_start = y_min - ((sampling_buffer[i] * amplitude) / ADC_MAX_VALUE);
       int y_end = y_min - ((sampling_buffer[i + 1] * amplitude) / ADC_MAX_VALUE);
       DrawLine(i, y_start, i + 1, y_end, YELLOW);
       y_start = y_end;
   }
}