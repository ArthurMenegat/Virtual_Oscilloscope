/*
*    SerialRead Source File.
*    Note: This document contains the serial readings functions implementations.
*    By: Arthur.
*/

/*
    Disabling conflicting functions in windows.h
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

#include "raylib.h"
#include "SerialRead.h"
#include "WaveGenerator.h"
#include <stdio.h>

int InitComPort(SerialPort_t* com_port, char* my_port)
{  
    com_port -> PortName = my_port;
    
    com_port -> hSerial = CreateFile(
                                    com_port -> PortName,
                                    GENERIC_READ | GENERIC_WRITE,
                                    0, 
                                    NULL, 
                                    OPEN_EXISTING, 
                                    FILE_ATTRIBUTE_NORMAL, 
                                    NULL
                                    );
    
    if(com_port -> hSerial == INVALID_HANDLE_VALUE)
    {     
        return 1;
    }
    
    COMMTIMEOUTS cto = {0}; 

    cto.ReadIntervalTimeout = MAXDWORD; 
    cto.ReadTotalTimeoutMultiplier = 0;    
    cto.ReadTotalTimeoutConstant = 0;

    SetCommTimeouts(com_port -> hSerial, &cto); // Set the previous configurations
    
    if (!SetCommTimeouts(com_port -> hSerial, &cto)) 
    {
        CloseHandle(com_port -> hSerial); 
        
        return 2; 
    }
    
    return 0; 
}

int ConfigComPort(SerialPort_t* com_port, int baudrate, int bytesize, int parity, int stopbits)
{
    DCB dcb; // DCB (Device Control Block) struct.
    BOOL fSuccess;
    
    com_port -> Baudrate = baudrate;
    com_port -> Bytesize = bytesize;
    com_port -> Parity = parity;
    com_port -> Stopbits = stopbits;
    
    // Inicialize the windows DCB struct.
    SecureZeroMemory(&dcb, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB);
    
    fSuccess = GetCommState(com_port -> hSerial, &dcb);
    
    if(!fSuccess) // Error handler.
    {
        return 2;
    }
    
    // Set configs and the COM state.
    dcb.BaudRate = com_port -> Baudrate;
    dcb.ByteSize = (BYTE)com_port -> Bytesize;    // 
    dcb.Parity = (BYTE)com_port -> Parity;       // DCB struct expects a BYTE.
    dcb.StopBits = (BYTE)com_port -> Stopbits;  //
    
    fSuccess = SetCommState(com_port -> hSerial, &dcb);
    
    if(!fSuccess)
    {
        return 3;
    }
    
    fSuccess = GetCommState(com_port -> hSerial, &dcb); // Get the config again.
    
    if(!fSuccess)
    {
        return 2;
    }
    
    return 0;
}

int ReadComPort(SerialPort_t* com_port, SerialRead_t* serial) 
{       
    BOOL success = ReadFile(
                            com_port -> hSerial,
                            serial -> buffer,
                            serial -> bytesToRead,
                            &(serial -> bytes_read),
                            NULL
                            );
    
    if(!success)
    {
        return 4;
    }

    return 0;
}

void CloseComPort(SerialPort_t* com_port)
{
    CloseHandle(com_port -> hSerial);
}

void CheckNewLineCharacter(SerialRead_t* serial)
{ 
    if(serial -> bytes_read > 0)
    {
        for(DWORD i = 0; i < serial -> bytes_read; ++i)
        {
            char caracter = serial -> buffer[i];
            
            if(caracter == '\n')
            {
                serial -> rx_buffer[serial -> rx_index] = '\0';
                
                strncpy(serial -> display_data, serial -> rx_buffer, sizeof(serial -> display_data) - 1);
                serial -> display_data[sizeof(serial -> display_data) - 1] = '\0'; // Ensures null termination.  
                    
                int signal_value = atoi(serial -> rx_buffer);  // Converts data type.
                AddValueToWave(signal_value);
                    
                serial -> rx_index = 0;
                memset(serial -> rx_buffer, 0, sizeof(serial -> rx_buffer));
            }
            else if(caracter != '\r') 
            {
                if(serial -> rx_index < sizeof(serial -> rx_buffer) - 1)
                {
                    serial -> rx_buffer[serial -> rx_index++] = caracter;
                }
            }
        }                               
    }              
}