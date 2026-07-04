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

#include "gui.h"
#include "raylib.h"
#include "raygui.h"
#include "serialread.h"
#include "appstate.h"

#define BACKGROUND_COLOR BLACK

#define MAX_CHANNELS 3
#define SAMPLES 1920
#define ADC_MAX_VALUE 1023
#define BYTES_TO_READ 255

#define CHANNEL1_WAVE_Y_MIN 450
#define CHANNEL1_WAVE_Y_MAX 270
#define CHANNEL1_WAVE_COLOR YELLOW

#define CHANNEL2_WAVE_Y_MIN 720
#define CHANNEL2_WAVE_Y_MAX 540
#define CHANNEL2_WAVE_COLOR RED

#define CHANNEL3_WAVE_Y_MIN 990
#define CHANNEL3_WAVE_Y_MAX 810
#define CHANNEL3_WAVE_COLOR GREEN

#define TERMINAL_BUTTON_WIDTH 150
#define TERMINAL_BUTTON_HEIGHT 50
#define TERMINAL_BUTTON_X_POS 20
#define TERMINAL_BUTTON_Y_POS 20
#define TERMINAL_BUTTON_TITLE "Terminal"

#define INIT_BUTTON_WIDTH 150
#define INIT_BUTTON_HEIGHT 50
#define INIT_BUTTON_X_POS 180
#define INIT_BUTTON_Y_POS 20
#define INIT_BUTTON_TITLE "Inicialize"

#define SETTINGS_BUTTON_WIDTH 150
#define SETTINGS_BUTTON_HEIGHT 50
#define SETTINGS_BUTTON_X_POS 340
#define SETTINGS_BUTTON_Y_POS 20
#define SETTINGS_BUTTON_TITLE "Settings"

#define WINDOW_BOX_X_POS 200
#define WINDOW_BOX_Y_POS 100
#define WINDOW_BOX_WIDTH 400
#define WINDOW_BOX_HEIGHT 400
#define WINDOW_BOX_TEXT "Settings"

#define DROP_BOX_X_POS WINDOW_BOX_X_POS + 20
#define DROP_BOX_Y_POS WINDOW_BOX_Y_POS + 30
#define DROP_BOX_WIDTH 150
#define DROP_BOX_HEIGHT 20

void InitApplication(app_state_t* app_state, screen_t* screen, serial_port_t* my_com, wave_t* my_wave, serial_read_t* my_serial)
{    
    static const int waves_positions_min[MAX_CHANNELS] = {CHANNEL1_WAVE_Y_MIN, CHANNEL2_WAVE_Y_MIN, CHANNEL3_WAVE_Y_MIN};
    static const int waves_positions_max[MAX_CHANNELS] = {CHANNEL1_WAVE_Y_MAX, CHANNEL2_WAVE_Y_MAX, CHANNEL3_WAVE_Y_MAX};
    static const Color waves_colors[MAX_CHANNELS] = {CHANNEL1_WAVE_COLOR, CHANNEL2_WAVE_COLOR, CHANNEL3_WAVE_COLOR};

    GetScreenResolution(screen);
    
    for(int i = 0; i < MAX_CHANNELS; ++i)
    {
        my_wave[i].adc_max_value = ADC_MAX_VALUE;
        my_wave[i].samples = SAMPLES;
        
        my_wave[i].y_min = waves_positions_min[i];
        my_wave[i].y_max = waves_positions_max[i];
        my_wave[i].color = waves_colors[i];
        
        my_serial[i].bytesToRead = BYTES_TO_READ;
    }
}

void CloseApplication(app_state_t* app_state, serial_port_t* my_com)
{
    if(app_state -> is_port_initialized)
    {
        for(int i = 0; i < MAX_CHANNELS; i++)
        {
            CloseComPort(&my_com[i]);
        }
           
        app_state -> is_port_initialized = false;
    }
}

void UpdateHardwareAndState(app_state_t* app_state, serial_port_t* my_com, serial_read_t* my_serial, wave_t* my_wave, screen_t* screen)
{
    // Fullscreen toggle.
    if(IsKeyPressed(KEY_F11))
    {
        app_state -> is_fullscreen = !(app_state -> is_fullscreen);
        
        TurnFullscreen(app_state, screen);                        
    }
    
    // Initialize or close COM port relative to init_button state. 
    if(app_state -> toggle_init) 
    {
        if(!(app_state -> is_port_initialized)) 
        {
            for(int i = 0; i < MAX_CHANNELS; ++i)
            {
                InitComPort(&my_com[i], my_com[i].PortName);
                ConfigComPort(&my_com[i], my_com[i].Baudrate, 8, 0, 1);
            }
                
            app_state -> is_port_initialized = true;
        }
    }
    else
    {    
        if(app_state -> is_port_initialized)
        {
            for(int i = 0; i < MAX_CHANNELS; ++i)
            {
                CloseComPort(&my_com[i]);
            }
                
            app_state -> is_port_initialized = false;
        }
    }
    
    // Inicialize port readings.
    if(app_state -> is_port_initialized)
    {
        for(int i = 0; i < MAX_CHANNELS; ++i)
        {
            ReadComPort(&my_com[i], &my_serial[i]);
            CheckNewLineCharacter(&my_serial[i], &my_wave[i]);
        }
    }
}

void RenderGraphics(app_state_t* app_state, serial_read_t* my_serial, wave_t* my_wave, screen_t* screen, serial_port_t* my_com)
{    
    BeginDrawing();
    
    ClearBackground(BACKGROUND_COLOR);
   
    DrawGrid2D(screen);
   
    for(int i = 0; i < MAX_CHANNELS; ++i)
    {
        DrawWave(&my_wave[i]);   
    }
 
    // Terminal button toggle
    Rectangle terminal_button = {.x = TERMINAL_BUTTON_X_POS, 
                                 .y = TERMINAL_BUTTON_Y_POS,
                                 .width = TERMINAL_BUTTON_WIDTH,
                                 .height = TERMINAL_BUTTON_HEIGHT};
   
    GuiToggle(terminal_button, TERMINAL_BUTTON_TITLE, &(app_state -> toggle_terminal));
   
    if(app_state -> toggle_terminal)
    {                                   
        DrawTerminal(&terminal_button, my_serial);  
    }
    
    // Init button toggle
    Rectangle init_button = {.x = INIT_BUTTON_X_POS, 
                             .y = INIT_BUTTON_Y_POS,
                             .width = INIT_BUTTON_WIDTH,
                             .height = INIT_BUTTON_HEIGHT};
   
    GuiToggle(init_button, INIT_BUTTON_TITLE, &(app_state -> toggle_init));
   
    // Settings button toggle.
    Rectangle settings_button = {.x = SETTINGS_BUTTON_X_POS, 
                                 .y = SETTINGS_BUTTON_Y_POS,
                                 .width = SETTINGS_BUTTON_WIDTH,
                                 .height = SETTINGS_BUTTON_HEIGHT}; 
   
    GuiToggle(settings_button, SETTINGS_BUTTON_TITLE, &(app_state -> toggle_settings));
   
    // Settings window toggle
    if(app_state -> toggle_settings)
    {
        const char* ports[] = {"\\\\.\\COM3", "\\\\.\\COM4", "\\\\.\\COM7"}; 
        
        const int baudrates[] = {300, 600, 750, 1200, 2400, 4800,           
                                 9600, 19200, 31250, 38400, 57600, 
                                 74880, 115200};
        
        Rectangle settings = {.x = WINDOW_BOX_X_POS, 
                              .y = WINDOW_BOX_Y_POS,
                              .width = WINDOW_BOX_WIDTH,
                              .height = WINDOW_BOX_HEIGHT};
                              
        if(GuiWindowBox(settings, WINDOW_BOX_TEXT))
        {           
            app_state -> toggle_settings = false; 
        }
        
        // Drop box instances.
        Rectangle baudrate_channel1_box = {.x = DROP_BOX_X_POS,
                                           .y = DROP_BOX_Y_POS + 100,
                                           .width = DROP_BOX_WIDTH,
                                           .height = DROP_BOX_HEIGHT};
        
        Rectangle baudrate_channel2_box = {.x = DROP_BOX_X_POS + 200,
                                           .y = DROP_BOX_Y_POS + 100,
                                           .width = DROP_BOX_WIDTH,
                                           .height = DROP_BOX_HEIGHT};
                                           
        Rectangle baudrate_channel3_box = {.x = DROP_BOX_X_POS,
                                           .y = DROP_BOX_Y_POS + 300,
                                           .width = DROP_BOX_WIDTH,
                                           .height = DROP_BOX_HEIGHT};
        
        Rectangle channel1_box = {.x = DROP_BOX_X_POS,
                                  .y = DROP_BOX_Y_POS,
                                  .width = DROP_BOX_WIDTH,
                                  .height = DROP_BOX_HEIGHT};
                                  
        Rectangle channel2_box = {.x = DROP_BOX_X_POS + 200,
                                  .y = DROP_BOX_Y_POS,
                                  .width = DROP_BOX_WIDTH,
                                  .height = DROP_BOX_HEIGHT};
                          
        Rectangle channel3_box = {.x = DROP_BOX_X_POS,
                                  .y = DROP_BOX_Y_POS + 200,
                                  .width = DROP_BOX_WIDTH,
                                  .height = DROP_BOX_HEIGHT};
                                  
        Rectangle channels_vector[MAX_CHANNELS] = {channel1_box, channel2_box, channel3_box};
        Rectangle baudrates_vector[MAX_CHANNELS] = {baudrate_channel1_box, baudrate_channel2_box, baudrate_channel3_box};
        
        for(int i = 0; i < MAX_CHANNELS; ++i)
        {
            if(GuiDropdownBox(channels_vector[i], "COM3;COM4;COM7", &app_state -> active_channel[i], app_state -> edit_drop_box[i]))
            {                
                app_state -> edit_drop_box[i] = !app_state -> edit_drop_box[i];                                        
               
                my_com[i].PortName = ports[app_state -> active_channel[i]];
            }
            
            if(GuiDropdownBox(baudrates_vector[i], "300bps;600bps;750bps;1200bps;2400bps;4800bps;9600bps;19200bps;31250bps;38400bps;57600bps;74880bps;115200bps", &app_state -> active_baud[i], app_state -> edit_drop_box_b[i]))
            {                
                app_state -> edit_drop_box_b[i] = !app_state -> edit_drop_box_b[i];

                my_com[i].Baudrate = baudrates[app_state -> active_baud[i]];
            }
        }    
    }
  
    EndDrawing();
}