/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <chrono>
#include <cstdint>
#include <cstdio>
//#include <cstdint>
//#include <cstring>
//#include <string>



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
const char* test_msg = "{LED:status}";
const char* tes_msg2 = "{LED:change}";

//TO be used with UWP lab_1
//Thread mythread;
/*
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
*/
int main()
{
    length = snprintf(buffer,BUFF_LENGTH,"\r\nProgram Starting\r\n");
    pc.write(buffer,length); //write the message to the serial port
    //mythread.start(threadMethod); //start running the thread 
    char msg_command[4]= {test_msg[1], test_msg[2] , test_msg[3]};
    char msg_target[7] = {test_msg[5],test_msg[6],test_msg[7],test_msg[8],test_msg[9],test_msg[10]};
    length = snprintf(buffer, BUFF_LENGTH, "%s \n\r",msg_command);
    pc.write(buffer, length);
    length = snprintf(buffer, BUFF_LENGTH, "%s \n\r",msg_target);
    pc.write(buffer, length);

    

    while (true) {

        //length = snprintf(buffer,BUFF_LENGTH,"{Data:{Status:Correct %i}}",number); //create the message using string literal ''

        //length = snprintf(buffer,BUFF_LENGTH,"{Data:{Status:\"Correct\"}}\r\n");
        //pc.write(buffer,length); //write the message to the serial port
        //ThisThread::sleep_for(chrono::seconds(1)); //sleep for 1 second on the serial thread using chrono library
        //number --;

        uint32_t num = pc.read(buf,sizeof(buf));
        if(num > 0){
            //string msg = buf;
            char msg_command[4]= {buf[1], buf[2] , buf[3]};
            char msg_target[7] = {buf[5],buf[6],buf[7],buf[8],buf[9],buf[10]};
            //string message_command = msg.substr(5,6);
            //string message_target = msg.substr(1,3);
            //const char* msg_command = message_command.c_str();
            //const char* msg_target = message_target.c_str();
            if (strcmp(msg_target, "LED")==0){
                if (strcmp(msg_command, "change")==0){
                    led = !led; //turn the led to opposite state that it is in
                    length = snprintf(MSG,MSG_BUFF_LENGTH,"Received Command LED1\r\n");
                    pc.write(MSG,length);
                    
                }else if (strcmp(msg_command, "status")==0) {
                    length = snprintf(MSG,MSG_BUFF_LENGTH,"{status:{LED1:%d}}\r\n",int(led));
                    pc.write(MSG,length);
                } 
            }
            if (strcmp(msg_target,"RNG")){

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
