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

// Terminal button defines
#define T_BUTTON_X_POS 20
#define T_BUTTON_Y_POS 50
#define T_BUTTON_WIDTH 150
#define T_BUTTON_HEIGHT 50
#define T_BUTTON_COLOR GREEN

#define BUFFER_SIZE 256

#define TERMINAL_TEXT_COLOR DARKGREEN

bool ToggleButton(Vector2 mousePos, Rectangle button, Color buttonColor, bool* toggleState);
void DrawButtonText(Rectangle button, const char* text, Color textColor);

int main(void)
{
    SerialPort_t mycom;
    
    Rectangle terminalButton = {T_BUTTON_X_POS, T_BUTTON_Y_POS, T_BUTTON_WIDTH, T_BUTTON_HEIGHT};
    
    bool terminalToggle = false;
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
    
    InitCOMPort(&mycom, "COM7");
    ConfigCOMPort(&mycom, 9600, 8, 0, 1);
    
    char buffer[BUFFER_SIZE] = {0};
    char dadosExibicao[BUFFER_SIZE] = "Waiting data to arrive...";
    DWORD bytesRead = 0;
    
    char rxBuffer[BUFFER_SIZE] = {0};
    int rxIndex = 0;
    int intBuf[sizeof(rxBuffer)]= {0};
    
    while(!WindowShouldClose())
    { 
        Vector2 mousePos = GetMousePosition(); 
       
        memset(buffer, 0, sizeof(buffer));
        
        ReadCOMPort(&mycom, buffer, sizeof(buffer) - 1, &bytesRead);
       
        // Checks for new line character
        if(bytesRead > 0)
        {
            for(int i = 0; i < bytesRead; i++)
            {
                char caracter = buffer[i];
            
                if(caracter == '\n')
                {
                    rxBuffer[rxIndex] = '\0';
                    strncpy(dadosExibicao, rxBuffer, sizeof(dadosExibicao) - 1);
                    
                    rxIndex = 0;
                    memset(rxBuffer, 0, sizeof(rxBuffer));
                }
                else if(caracter != '\r') // '\r' indicates the end of the string
                {
                    if(rxIndex < sizeof(rxBuffer) - 1)
                    {
                        rxBuffer[rxIndex++] = caracter;
                    }
                }
            }                               
        }                                   
         
        int num = atoi(dadosExibicao);
         
        BeginDrawing();
       
        ClearBackground(BLACK);
        DrawGrid2D(WINDOW_WIDTH, WINDOW_HEIGHT);
        
        // Draw the wave of the target signal
        DrawLine(0, num, 1200, num, YELLOW);
        
        if(ToggleButton(mousePos, terminalButton, T_BUTTON_COLOR, &terminalToggle))
        {    
            DrawText("------Serial Terminal------", 20, 120, 20, TERMINAL_TEXT_COLOR);
            DrawText(TextFormat("Number of Bytes Read: %lu", bytesRead), 20, 150, 20, TERMINAL_TEXT_COLOR);
            DrawText(TextFormat("Data received: %s", dadosExibicao), 20, 200, 20, TERMINAL_TEXT_COLOR);
        } 

        DrawButtonText(terminalButton, "Terminal", BLACK);
           
        EndDrawing();
    }  
    
    CloseCOMPort(&mycom);
    CloseWindow();
    
    return 0;
}

bool ToggleButton(Vector2 mousePos, Rectangle button, Color buttonColor, bool* toggleState)
{
    bool isHovered = CheckCollisionPointRec(mousePos, button);
    
    if(isHovered)   
    {
        DrawRectangleRec(button, LIGHTGRAY);
           
        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            *toggleState = !(*toggleState); // Assign the inverse value in the toggleState address       
        }
    }
    else
    {
        DrawRectangleRec(button, buttonColor);
    }
   
    return *toggleState;
}

void DrawButtonText(Rectangle button, const char* text, Color textColor)
{
    int fontSize = 20;
    int textLength = MeasureText(text, fontSize);
    
    // Calculate the text position relative to the button and text dimensions 
    int textXPos = button.x + (button.width - textLength) / 2;
    int textYPos = button.y + (button.height - fontSize) / 2;
   
    DrawText(text, textXPos, textYPos, fontSize, textColor);       
}