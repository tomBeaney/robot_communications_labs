/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>



// Blinking rate in milliseconds
#define BLINKING_RATE     10ms

#define BUFF_LENGTH       64
#define MSG_BUFF_LENGTH 64

int length;
char buffer[BUFF_LENGTH] = {0};

UnbufferedSerial pc(USBTX, USBRX, 115200);
char buf[64];
char MSG[64];
char rec[64];
int rec_pointer = 0;
//BufferedSerial pc(USBTX, USBRX, 115200);

//BufferedSerial pc(PA_11, PA_12, 115200);

int  number = 0;
DigitalOut led(LED1);
//DigitalOut led1(D2);
const char* test_msg = "{LED:status}"; //test messages to check if string manipulation works
const char* tes_msg2 = "{LED:change}";
int commandNumber = 0;

//TO be used with UWP lab_1

int main()
{
    length = snprintf(buffer,BUFF_LENGTH,"\r\nProgram Starting\r\n");
    pc.write(buffer,length); //write the message to the serial port

    //pc.set_flow_control(mbed::SerialBase::RTSCTS); results in error message code 0x80FF0144 mbedOS fatal error

    while (true) {
        uint32_t num = pc.read(buf,sizeof(buf)); //reads only 1 character at the time so have to send messages as broken chars
        length = snprintf(MSG,MSG_BUFF_LENGTH,"whats in the buffer: %s %d \n\r",buf,num);
        pc.write(MSG,length);
        if(num > 0){
            //string msg = buf;
            //builds own circular buffer that stores last chars received until terminator value is called (0)
            rec[rec_pointer] = buf[0];
            rec_pointer ++;
            length = snprintf(MSG,MSG_BUFF_LENGTH,"rec : %s \n\r rec pointer: %d \n\r",rec,rec_pointer); //see what character we got and whats in the buffer
            pc.write(MSG,length);
            if (rec_pointer > sizeof(rec)-1){ //stop buffer overflow
                rec_pointer = 0;
            }
            if (strcmp(buf, "0")==0){
                length = snprintf(MSG,MSG_BUFF_LENGTH,"end of message\n\r");//create the message using string literal ''
                pc.write(MSG,length);
                char msg_command[4]= {rec[rec_pointer-12], rec[rec_pointer-11] , rec[rec_pointer-10]};
                char msg_target[7] = {rec[rec_pointer-8],rec[rec_pointer-7],rec[rec_pointer-6],rec[rec_pointer-5],rec[rec_pointer-4],rec[rec_pointer-3]};
                //string manipulation to read contense of message in format "{command:thing to do}"
                if (strcmp(msg_command, "LED")==0){
                    if (strcmp(msg_target, "change")==0){
                        led = !led; //turn the led to opposite state that it is in
                        length = snprintf(MSG,MSG_BUFF_LENGTH,"Received Command LED1\r\n");
                        pc.write(MSG,length);
                        commandNumber ++;
                        
                    }else if (strcmp(msg_target, "status")==0) {
                        length = snprintf(MSG,MSG_BUFF_LENGTH,"{status:{LED1:%d}}\r\n",int(led));//return what state the LED is currently in 
                        pc.write(MSG,length);
                    } 
                }
                else if (strcmp(msg_command,"CMD")==0){
                    if (strcmp(msg_target, "status")==0){
                        length = snprintf(MSG,MSG_BUFF_LENGTH,"{status:{CMD:%d}}\r\n",commandNumber); //returns the number of times LED has been changed
                        pc.write(MSG,length);
                    }
                }
                else{//error checking the received message
                    length = snprintf(MSG,MSG_BUFF_LENGTH,"whats the command: %s\n\r",msg_command);
                    pc.write(MSG,length);
                    length = snprintf(MSG,MSG_BUFF_LENGTH,"whats the target: %s\n\r",msg_target);
                    pc.write(MSG,length);
                }
                rec_pointer = 0; //reset the buffer when we have got a successful message
            }
        }
        ThisThread::sleep_for(1ms); //sleep the thread for 1ms

    }
}
