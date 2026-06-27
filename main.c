/*
*    Oscilloscope Source File.
*    By: Arthur.
*/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION

#include "raygui.h"

#include "SerialRead.h"
#include "WaveGenerator.h"
#include <stdio.h>
#include <string.h>

// Screen defines
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540
#define WINDOW_TITLE "Virtual Oscilloscope"

// Terminal button defines
#define T_BUTTON_WIDTH 150
#define T_BUTTON_HEIGHT 50
#define T_BUTTON_Y_POS 25
#define T_BUTTON_COLOR GREEN

#define BUFFER_SIZE 256

#define TERMINAL_TEXT_COLOR DARKGREEN

bool ToggleButton(Vector2 mousePos, Rectangle button, Color buttonColor, bool* toggleState);
void DrawButtonText(Rectangle button, const char* text, Color textColor);

int main(void)
{
    SerialPort_t mycom;
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
    
    InitCOMPort(&mycom, "COM7");
    ConfigCOMPort(&mycom, 9600, 8, 0, 1);
    
    bool terminal_toggle = false;
    bool is_fullscreen = false;
    
    char buffer[BUFFER_SIZE] = {0};
    char display_data[BUFFER_SIZE] = "Waiting data to arrive...";
    DWORD bytes_read = 0;
    
    char rx_buffer[BUFFER_SIZE] = {0};
    int rx_index = 0;
    
    Rectangle terminal_button = {0, T_BUTTON_Y_POS,
                                T_BUTTON_WIDTH, T_BUTTON_HEIGHT};
                                
    while(!WindowShouldClose())
    { 
        int monitor = GetCurrentMonitor();
        Vector2 mouse_pos = GetMousePosition();
        
        memset(buffer, 0, sizeof(buffer)); // Cleans buffer

        if(IsKeyPressed(KEY_F))
        {
            is_fullscreen = !is_fullscreen;
            
            if(is_fullscreen)
            {        
                SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
                                
                ToggleFullscreen();
                
                terminal_button.x = 20;
            }
            else
            {
                ToggleFullscreen();
                SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
                
                terminal_button.x = 20;
            }
        }
        
        ReadCOMPort(&mycom, buffer, sizeof(buffer) - 1, &bytes_read);
        
        // Checks for new line character
        if(bytes_read > 0)
        {
            for(int i = 0; i < bytes_read; ++i)
            {
                char caracter = buffer[i];
            
                if(caracter == '\n')
                {
                    rx_buffer[rx_index] = '\0';
                    strncpy(display_data, rx_buffer, sizeof(display_data) - 1);
                    
                    int signal_value = atoi(rx_buffer);  // Converts data type
                    AddValueToWave(signal_value);
                    
                    rx_index = 0;
                    memset(rx_buffer, 0, sizeof(rx_buffer));
                }
                else if(caracter != '\r') 
                {
                    if(rx_index < sizeof(rx_buffer) - 1)
                    {
                        rx_buffer[rx_index++] = caracter;
                    }
                }
            }                               
        }                                         
         
        BeginDrawing();
       
        ClearBackground(BLACK);
        DrawGrid2D(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        DrawWave();

        if(ToggleButton(mouse_pos, terminal_button, T_BUTTON_COLOR, &terminal_toggle))
        {    
            DrawText("------Serial Terminal------", 1030, 120, 20, TERMINAL_TEXT_COLOR);
            DrawText(TextFormat("Number of Bytes Read: %lu", bytes_read), 1030, 150, 20, TERMINAL_TEXT_COLOR);
            DrawText(TextFormat("Data received: %s", display_data), 1030, 200, 20, TERMINAL_TEXT_COLOR);
        } 

        DrawButtonText(terminal_button, "Terminal", BLACK);
           
        EndDrawing();
    }  
    
    CloseCOMPort(&mycom);
    CloseWindow();
    
    return 0;
}

bool ToggleButton(Vector2 mouse_pos, Rectangle button, Color button_color, bool* toggle_state)
{
    bool is_hovered = CheckCollisionPointRec(mouse_pos, button);
    
    if(is_hovered)   
    {
        DrawRectangleRec(button, LIGHTGRAY);
           
        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            *toggle_state = !(*toggle_state); // Assign the inverse value in the toggleState address       
        }
    }
    else
    {
        DrawRectangleRec(button, button_color);
    }
   
    return *toggle_state;
}

void DrawButtonText(Rectangle button, const char* text, Color text_color)
{
    int font_size = 20;
    int text_length = MeasureText(text, font_size);
    
    // Calculate the text position relative to the button and text dimensions 
    int text_X_Pos = button.x + (button.width - text_length) / 2;
    int text_Y_Pos = button.y + (button.height - font_size) / 2;
   
    DrawText(text, text_X_Pos, text_Y_Pos, font_size, text_color);       
}