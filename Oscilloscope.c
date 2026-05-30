/*
*    Oscilloscope Source File 
*    By: Arthur
*/

#include "raylib.h"
#include "SerialRead.h"
#include "WaveGenerator.h"
#include <stdio.h>
#include <string.h>

// Screen defines
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "Oscilloscope Simulator"

// Button defines
#define BUTTON_X_POS 20
#define BUTTON_Y_POS 50
#define BUTTON_WIDTH 150
#define BUTTON_HEIGHT 50
#define BUTTON_COLOR LIGHTGRAY

int main(void)
{
    SerialPort_t mycom;
    
    Rectangle button = {BUTTON_X_POS, BUTTON_Y_POS, BUTTON_WIDTH, BUTTON_HEIGHT};
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
    
    InitCOMPort(&mycom, "COM7");
    ConfigCOMPort(&mycom, 9600, 8, 0, 1);
    
    char buffer[256] = {0};
    char dadosExibicao[256] = "Waiting data to arrive...";
    DWORD bytesRead = 0;
    bool exibirDados = false;
    
    while(!WindowShouldClose())
    { 
       Vector2 mousePos = GetMousePosition(); 

       memset(buffer, 0, sizeof(buffer));
        
       ReadCOMPort(&mycom, buffer, sizeof(buffer) - 1, &bytesRead);
       
       if (bytesRead > 0) 
       {
            strncpy(dadosExibicao, buffer, sizeof(dadosExibicao) - 1);
       }
       
       BeginDrawing();
       
       ClearBackground(BLACK);
       // DrawGrid2D(WINDOW_WIDTH, WINDOW_HEIGHT);
              
       if(CheckCollisionPointRec(mousePos, button))
       {
           DrawRectangleRec(button, BUTTON_COLOR);
           
           if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
           {
               exibirDados = !exibirDados;
           }
       }
       else
       {
           DrawRectangleRec(button, GREEN);
       }
       
       DrawText("Terminal", 25, 65, 20, DARKGREEN);
       
       if(exibirDados)
       {    
           DrawText("------Serial Terminal------", 20, 120, 20, DARKGREEN);
           DrawText(TextFormat("Number of Bytes Read: %lu", bytesRead), 20, 150, 20, DARKGREEN);
           DrawText(TextFormat("Data received: %s", dadosExibicao), 20, 200, 20, DARKGREEN);
       }      
                
       EndDrawing();
    }  
    
    CloseCOMPort(&mycom);
    CloseWindow();
    
    return 0;
}