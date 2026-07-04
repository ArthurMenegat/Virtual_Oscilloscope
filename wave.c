/*
*    Wave Source File.
*    Note: This document contains the wave functions implementations. 
*    By: Arthur.
*/

#include "raylib.h"
#include "wave.h"

void AddValueToWave(wave_t* my_wave)
{ 
    for(int i = 0; i < my_wave -> samples - 1; ++i)
    {
        my_wave -> sampling_buffer[i] = my_wave -> sampling_buffer[i + 1]; // Shift buffer.
    }
    
    my_wave -> sampling_buffer[my_wave -> samples - 1] = my_wave -> signal_value;
}

void DrawWave(wave_t* my_wave)
{  
    int amplitude = (my_wave -> y_min) - (my_wave -> y_max);
   
    int y_start = my_wave -> y_min - ((my_wave -> sampling_buffer[0] * amplitude) / my_wave -> adc_max_value);
   
    for(int i = 0; i < my_wave -> samples - 1; ++i)
    {
        int y_end = my_wave -> y_min - ((my_wave -> sampling_buffer[i + 1] * amplitude) / my_wave -> adc_max_value);
        DrawLine(i, y_start, i + 1, y_end, my_wave -> color);
        y_start = y_end;
    }
}

static void CalculateSignalMidpoint(wave_t* my_wave)
{
    unsigned int max_value = 0;
    unsigned int min_value = my_wave -> adc_max_value;
    
    for(int i = 0; i < my_wave -> samples; ++i)
    {
        if(my_wave -> sampling_buffer[i] > max_value)
        {
            max_value = my_wave -> sampling_buffer[i];
        }
        
        if(my_wave -> sampling_buffer[i] < min_value)
        {
            min_value = my_wave -> sampling_buffer[i];
        }
    }
    
    my_wave -> signal_midpoint = (max_value + min_value) / 2;
}

void CalculateSignalPeriod(wave_t* my_wave)
{
    CalculateSignalMidpoint(my_wave);
    
    unsigned int midpoint_index_1 = 0; 
    unsigned int midpoint_index_2 = 0;
    unsigned int rising_edge_counter = 0;
    
    for(int i = 1; i < my_wave -> samples; ++i)
    {
        if(my_wave -> sampling_buffer[i - 1] < my_wave -> signal_midpoint && my_wave -> sampling_buffer[i] >= my_wave -> signal_midpoint) // Finds the rising edge.
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
        my_wave -> period = midpoint_index_2 - midpoint_index_1;
    }
    else
    {
        my_wave -> period = 0;
    }
}