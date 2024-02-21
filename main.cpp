/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <chrono>
#include <cstdint>
#include <cstring>


// Blinking rate in milliseconds
#define BLINKING_RATE     10ms

#define BUFF_LENGTH       64
#define MSG_BUFF_LENGTH 32

int length;
char buffer[BUFF_LENGTH] = {};

UnbufferedSerial pc(USBTX, USBRX, 115200);
char buf[32];
char MSG[32];
char rec[32];

//BufferedSerial pc(USBTX, USBRX, 115200);

//BufferedSerial pc(PA_11, PA_12, 115200);

// use com 5 for conenction in PUTTY
int  number = 0;
DigitalOut led(LED1);
//DigitalOut led1(D2);


//TO be used with UWP lab_1
Thread mythread;

void threadMethod()
{
    // Initialise the digital pin LED1 as an output
    //DigitalOut led(LED1);
    while(true)
    {
        if(buf = pc.read(rec,sizeof(rec))){
            if (strcmp(rec, "{change:\"LED\"}")==0){
                led = !led; //turn the led to opposite state that it is in
                length = snprintf(MSG,MSG_BUFF_LENGTH,"Received Command LED1\r\n");
                pc.write(MSG,length);
                
            }else if (strcmp(rec, "{get_status:\"LED\"}")==0) {
                length = snprintf(MSG,MSG_BUFF_LENGTH,"{status:{LED1:%d}}\r\n",int(led));
                pc.write(MSG,length);
            } 
        }
        ThisThread::sleep_for(BLINKING_RATE); //sleep the thread for 500ms
    }
}

int main()
{
    length = snprintf(buffer,BUFF_LENGTH,"\r\nProgram Starting\r\n");
    pc.write(buffer,length); //write the message to the serial port
    //mythread.start(threadMethod); //start running the thread 

    while (true) {

        //length = snprintf(buffer,BUFF_LENGTH,"{Data:{Status:Correct %i}}",number); //create the message using string literal ''

        //length = snprintf(buffer,BUFF_LENGTH,"{Data:{Status:\"Correct\"}}\r\n");
        //pc.write(buffer,length); //write the message to the serial port
        //ThisThread::sleep_for(chrono::seconds(1)); //sleep for 1 second on the serial thread using chrono library
        //number --;
        if(uint32_t num = pc.read(buf,sizeof(buf))){
            if (strcmp(buf, "{change:\"LED\"}")==0){
                led = !led; //turn the led to opposite state that it is in
                length = snprintf(MSG,MSG_BUFF_LENGTH,"Received Command LED1\r\n");
                pc.write(MSG,length);
                
            }else if (strcmp(buf, "{get_status:\"LED\"}")==0) {
                length = snprintf(MSG,MSG_BUFF_LENGTH,"{status:{LED1:%d}}\r\n",int(led));
                pc.write(MSG,length);
            } 
        }
        ThisThread::sleep_for(BLINKING_RATE); //sleep the thread for 500ms
        /*
        length = snprintf(buffer,BUFF_LENGTH,"{Data:{Status:\"Correct\"}}\r\n");
        pc.write(buffer,length); //write the message to the serial port
        ThisThread::sleep_for(chrono::seconds(1)); //sleep for 1 second on the serial thread using chrono library
        number --;
        */

    }
    //mythread.join(); //join the thread to close it at the end of the program
}
