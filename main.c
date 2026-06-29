/*
*    Main Source File.
*    By: Arthur.
*/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION

#include "raygui.h"

#include "SerialRead.h"
#include "WaveGenerator.h"
#include "GUI.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540
#define WINDOW_TITLE "Virtual Oscilloscope"

int main(void)
{    
    SerialPort_t my_com = {0};
    SerialRead_t my_serial = {0};

    my_serial.bytesToRead = 255;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    InitComPort(&my_com, "COM7");
    ConfigComPort(&my_com, 115200, 8, 0, 1);   
  
    GetScreenResolution();
  
    while(!WindowShouldClose())
    {                
        if(IsKeyPressed(KEY_F11))
        {
            TurnFullscreen();                        
        }
        
        ReadComPort(&my_com, &my_serial);
        CheckNewLineCharacter(&my_serial);
        
        BeginDrawing();
       
        ClearBackground(BLACK);
        
        DrawGrid2D();
        
        DrawWave();   
        
        DrawTerminalText(&my_serial);               

        EndDrawing();
    }  
    
    CloseComPort(&my_com);
    
    CloseWindow();
    
    return 0;
}