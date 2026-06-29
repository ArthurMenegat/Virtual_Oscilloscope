/*
*    WaveGenerator Source File.
*    Note: This document contains the wave functions implementations. 
*    By: Arthur.
*/

#include "raylib.h"
#include "WaveGenerator.h"

#define SAMPLES 1920
#define ADC_MAX_VALUE 1023

unsigned int sampling_buffer[SAMPLES] = {0};
unsigned int signal_midpoint = 0;

void AddValueToWave(int signal_value)
{ 
    for(int i = 0; i < SAMPLES - 1; ++i)
    {
        sampling_buffer[i] = sampling_buffer[i + 1]; // Shift buffer.
    }
    
    sampling_buffer[SAMPLES - 1] = signal_value;
}

void DrawWave()
{  
   int y_min = 450; 
   int y_max = 270;
   int amplitude = y_min - y_max;
   
   int y_start = y_min - ((sampling_buffer[0] * amplitude) / ADC_MAX_VALUE);
   
   for(int i = 0; i < SAMPLES - 1; ++i)
   {
       int y_end = y_min - ((sampling_buffer[i + 1] * amplitude) / ADC_MAX_VALUE);
       DrawLine(i, y_start, i + 1, y_end, YELLOW);
       y_start = y_end;
   }
}

static void CalculateSignalMidpoint()
{
    unsigned int max_value = 0;
    unsigned int min_value = ADC_MAX_VALUE;
    
    for(int i = 0; i < SAMPLES; ++i)
    {
        if(sampling_buffer[i] > max_value)
        {
            max_value = sampling_buffer[i];
        }
        
        if(sampling_buffer[i] < min_value)
        {
            min_value = sampling_buffer[i];
        }
    }
    
    signal_midpoint = (max_value + min_value) / 2;
}

unsigned int CalculateSignalPeriod()
{
    CalculateSignalMidpoint();
    
    unsigned int midpoint_index_1 = 0; 
    unsigned int midpoint_index_2 = 0;
    unsigned int rising_edge_counter = 0;
    unsigned int period = 0;
    
    for(int i = 1; i < SAMPLES; ++i)
    {
        if(sampling_buffer[i - 1] < signal_midpoint && sampling_buffer[i] >= signal_midpoint) // Finds the rising edge.
        {
            rising_edge_counter++;
            
            if(rising_edge_counter == 1)
            {
                midpoint_index_1 = i;
            }
            
            if(rising_edge_counter == 2)
            {
                midpoint_index_2 = i;
                break;
            }
        }
    }
    
    if(rising_edge_counter >= 2)
    {        
        period = midpoint_index_2 - midpoint_index_1;
    }
    else
    {
        period = 0;
    }
    
    return period;
}