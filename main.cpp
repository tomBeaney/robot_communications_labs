/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <chrono>
#include <cstdio>


// Blinking rate in milliseconds
#define BLINKING_RATE     500ms

#define BUFF_LENGTH       64

int length;
char buffer[BUFF_LENGTH] = {};
BufferedSerial pc(USBTX, USBRX, 115200);
// use com 5 for conenction in PUTTY
int number = 0;


int main()
{
    // Initialise the digital pin LED1 as an output
    DigitalOut led(LED1);

    while (true) {
        led = !led; //turn the led to opposite state that it is in
        ThisThread::sleep_for(BLINKING_RATE); //sleep the thread for 500ms
        length = snprintf(buffer,BUFF_LENGTH,"\r\nHello this is a test"); //create the message using string literal ''
        pc.write(buffer,length); //write the message to the serial port
        ThisThread::sleep_for(chrono::seconds(1)); //sleep for 1 second on the serial thread using chrono library
    }
}
