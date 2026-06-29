/*
    WaveGenerator Header File.
    Note: This document contains the wave functions declarations. 
    By: Arthur.
*/

#ifndef WAVE_GENERATOR_H
#define WAVE_GENERATOR_H

void AddValueToWave(int signal_value); // Insert new values in the wave buffer shifting the positions to the left.
void DrawWave(void); // Draw the wave and calculates the position and amplitude relative to the screen resolution.
unsigned int CalculateSignalPeriod(void); // Find the signal period by calculating the samples between two points that cross the   midpoint (rising edge).

#endif