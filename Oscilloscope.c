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

// Port button defines
#define P_BUTTON_X_POS 180
#define P_BUTTON_Y_POS 50
#define P_BUTTON_WIDTH 150
#define P_BUTTON_HEIGHT 50
#define P_BUTTON_COLOR GREEN

bool ToggleButton(Vector2 mousePos, Rectangle button, Color buttonColor, bool* toggleState);
void DrawButtonText(Rectangle button, const char* text);
DWORD ReceiveData(SerialPort_t* mycom, char buffer[], char dadosExibicao[], DWORD bytesRead);

int main(void)
{
    SerialPort_t mycom;
    
    Rectangle terminalButton = {T_BUTTON_X_POS, T_BUTTON_Y_POS, T_BUTTON_WIDTH, T_BUTTON_HEIGHT};
    Rectangle portButton = {P_BUTTON_X_POS, P_BUTTON_Y_POS, P_BUTTON_WIDTH, P_BUTTON_HEIGHT};
    
    bool terminalToggle = false;
    bool inicializeToggle = false;
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
    
    InitCOMPort(&mycom, "COM7");
    ConfigCOMPort(&mycom, 9600, 8, 0, 1);
    
    char buffer[256] = {0};
    char dadosExibicao[256] = "Waiting data to arrive...";
    DWORD bytesRead = 0;
    
    while(!WindowShouldClose())
    { 
        Vector2 mousePos = GetMousePosition(); 
       
        bytesRead = ReceiveData(&mycom, buffer, dadosExibicao, bytesRead);
       
        BeginDrawing();
       
        ClearBackground(BLACK);
        // DrawGrid2D(WINDOW_WIDTH, WINDOW_HEIGHT);
        
        if(ToggleButton(mousePos, terminalButton, T_BUTTON_COLOR, &terminalToggle))
        {    
            DrawText("------Serial Terminal------", 20, 120, 20, DARKGREEN);
            DrawText(TextFormat("Number of Bytes Read: %lu", bytesRead), 20, 150, 20, DARKGREEN);
            DrawText(TextFormat("Data received: %s", dadosExibicao), 20, 200, 20, DARKGREEN);
        } 

        DrawButtonText(terminalButton, "Terminal");

        if(ToggleButton(mousePos, portButton, T_BUTTON_COLOR, &inicializeToggle))
        {    
            
        }  
           
        DrawButtonText(portButton, "Inicialize");   
           
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

void DrawButtonText(Rectangle button, const char* text)
{
    int fontSize = 20;
    int textLength = MeasureText(text, fontSize);
    
    // Calculate the text position relative to the button and text dimensions 
    int textXPos = button.x + (button.width - textLength) / 2;
    int textYPos = button.y + (button.height - fontSize) / 2;
   
    DrawText(text, textXPos, textYPos, fontSize, DARKGREEN);       
}

DWORD ReceiveData(SerialPort_t* mycom, char buffer[], char dadosExibicao[], DWORD bytesRead)
{  
    memset(buffer, 0, sizeof(*buffer));
        
    ReadCOMPort(mycom, buffer, sizeof(*buffer) - 1, &bytesRead);
       
    if (bytesRead > 0)       
    {
        strncpy(dadosExibicao, buffer, sizeof(*dadosExibicao) - 1);
    }
    
    return bytesRead;
}