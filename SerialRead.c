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
#include <stdio.h>

int InitCOMPort(SerialPort_t* COMPort, char* myPort)
{  
    COMPort -> PortName = myPort;
    
    COMPort -> hSerial = CreateFile(
                                    COMPort -> PortName,
                                    GENERIC_READ | GENERIC_WRITE,
                                    0, 
                                    NULL, 
                                    OPEN_EXISTING, 
                                    FILE_ATTRIBUTE_NORMAL, 
                                    NULL
                                    );
    
    if(COMPort -> hSerial == INVALID_HANDLE_VALUE)
    {     
        return 1;
    }
    
    return 0; 
}

int ConfigCOMPort(SerialPort_t* COMPort, int baudrate, int bytesize, int parity, int stopbits)
{
    DCB dcb; // DCB (Device Control Block) struct
    BOOL fSuccess;
    
    COMPort -> Baudrate = baudrate;
    COMPort -> Bytesize = bytesize;
    COMPort -> Parity = parity;
    COMPort -> Stopbits = stopbits;
    
    // Inicialize the windows DCB struct
    SecureZeroMemory(&dcb, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB);
    
    fSuccess = GetCommState(COMPort -> hSerial, &dcb);
    
    if(!fSuccess) // Error handler
    {
        return 2;
    }
    
    // Set configs and the COM state
    dcb.BaudRate = COMPort -> Baudrate;
    dcb.ByteSize = (BYTE)COMPort -> Bytesize;    // 
    dcb.Parity = (BYTE)COMPort -> Parity;       // DCB struct expects a BYTE
    dcb.StopBits = (BYTE)COMPort -> Stopbits;  //
    
    fSuccess = SetCommState(COMPort -> hSerial, &dcb);
    
    if(!fSuccess)
    {
        return 3;
    }
    
    fSuccess = GetCommState(COMPort -> hSerial, &dcb); // Get the config again
    
    if(!fSuccess)
    {
        return 2;
    }
    
    return 0;
}

int ReadCOMPort(SerialPort_t* COMPort, void* buffer, DWORD bytesToRead, DWORD* bytesRead) 
{       
    COMMTIMEOUTS cto = {0}; 

    cto.ReadIntervalTimeout = MAXDWORD; 
    cto.ReadTotalTimeoutMultiplier = 0;    
    cto.ReadTotalTimeoutConstant = 0;

    SetCommTimeouts(COMPort -> hSerial, &cto); // Set the previous configurations

    BOOL success = ReadFile(
                            COMPort -> hSerial,
                            buffer,
                            bytesToRead,
                            bytesRead,
                            NULL
                            );
    
    if(!success)
    {
        return 4;
    }

    return 0;
}

void CloseCOMPort(SerialPort_t* COMPort)
{
    CloseHandle(COMPort -> hSerial);
}