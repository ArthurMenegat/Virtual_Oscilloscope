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

#include "wave.h"

#define BUFFER_SIZE 256

typedef struct
{
    const char* PortName;
    HANDLE hSerial; // Windows COM handle (struct).
    int Baudrate;
    int Bytesize;
    int Parity;
    int Stopbits;
}serial_port_t;

typedef struct
{
    DWORD bytes_read; // Number of bytes read. 
    char buffer[BUFFER_SIZE]; 
    char display_data[BUFFER_SIZE]; 
    char rx_buffer[BUFFER_SIZE];       
    int bytesToRead;
    int rx_index;
}serial_read_t;

int InitComPort(serial_port_t* com_port, const char* my_port); // Inicialize the selected COM port.
int ConfigComPort(serial_port_t* com_port, int baudrate, int bytesize, int parity, int stopbits); // Configure the selected COM port.
int ReadComPort(serial_port_t* com_port, serial_read_t* serial); // Read data in the selected COM Port.
void CloseComPort(serial_port_t* com_port); // Close the selected COM Port.
                                         // Note: Always call this function at the end of the program
                                        // or the port could stay blocked for further use.
void CheckNewLineCharacter(serial_read_t* serial, wave_t* my_wave); // Checks for new line character.
                                                 // Note: Always put a '\n' at the end of the package before sending it to your PC.
#endif
