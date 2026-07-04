/*
    Wave Header File.
    Note: This document contains the wave functions declarations. 
    By: Arthur.
*/

#ifndef WAVE_H
#define WAVE_H

#include "raylib.h"

#define SAMPLES 1920

typedef struct
{
    unsigned int signal_midpoint;
    unsigned int signal_value;
    unsigned int samples;
    unsigned int sampling_buffer[SAMPLES];
    unsigned int adc_max_value;
    unsigned int period;
    int y_min; 
    int y_max; 
    Color color;
}wave_t;

void AddValueToWave(wave_t* my_wave); // Insert new values in the wave buffer shifting the positions to the left.
void DrawWave(wave_t* my_wave); // Draw the wave and calculates the position and amplitude relative to the screen resolution.
void CalculateSignalPeriod(wave_t* my_wave); // Find the signal period by calculating the samples between two points that cross the midpoint (rising edge).

#endif