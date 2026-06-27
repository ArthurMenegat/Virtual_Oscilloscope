/*
    SerialRead Header File.
    Note: This document contains the serial readings functions declarations. 
    By: Arthur.
*/

#ifndef SERIAL_READ_H
#define SERIAL_READ_H

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

typedef struct
{
    char* PortName;
    HANDLE hSerial; // Windows COM handle (struct)
    int Baudrate;
    int Bytesize;
    int Parity;
    int Stopbits;
}SerialPort_t;

int InitCOMPort(SerialPort_t* COMPort, char* PortName); // Inicialize the selected COM port
int ConfigCOMPort(SerialPort_t* COMPort, int baudrate, int bytesize, int parity, int stopbits); // Configure the selected COM port
int ReadCOMPort(SerialPort_t* COMPort, void* buffer, DWORD bytesToRead, DWORD* bytesRead); // Read data in the selected COM Port
void CloseCOMPort(SerialPort_t* COMPort); // Close the selected COM Port
                                         // Note: Always call this function at the end of the program
                                        // or the port could stay blocked for further use
#endif
