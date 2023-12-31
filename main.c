#include<xc.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include"i2c.h"
#include"io.h"
#include"pwm.h"
#include "uart.h"

#define INCRPERSEC 8e6/256
#define T 1e-3

unsigned char ByteLow, ByteHigh; 
double kp, ki, kd, P, I, D;
double setpoint, current, error, lastError, output; 
int accelx, doControl;
char str[20];

//function declarations
void interrupt_init();
void accel_init();
void Xaccel(double *value);

//Timer interrupt when change in X acceleration
void __attribute__((vector(_TIMER_2_VECTOR), interrupt(IPL7AUTO))) TIMER2ISR(void)
{
    //LATCbits .LATC15 = ~LATCbits.LATC15;
    Xaccel(&current);
    IFS0bits.T2IF = 0;
    doControl = 1;
}

//initialize global interrupt
void interrupt_init()
{ 
    __builtin_disable_interrupts();
    IPC4bits.T2IP = 7;
    IPC4bits.T2IS = 0;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    __builtin_enable_interrupts();
}

//initialize and check sensor
void accel_init()
{
    I2C_sendByte(PWR_MGMT_1, 0x01);
    I2C_sendByte(PWR_MGMT_2, 0x00);
    I2C_sendByte(USER_CTRL, 0x01);
    I2C_sendByte(CONFIG, 0x80);
    I2C_sendByte(ACCEL_INT_CTRL, 0x02);
    
    I2C_getByte (WHO_AM_I, &ByteLow);
    UART_sendString(ByteLow == 0x12 ? "SENSOR OK!" : "SENSOR ERROR!");
}

//Get acceleration value and append upper and lower bytes
void Xaccel(double *value)
{
    I2C_getByte(ACCEL_XOUT_L, &ByteLow);
    I2C_getByte(ACCEL_XOUT_H, &ByteHigh);
    accelx = (ByteHigh << 8) | (ByteLow); 
    if (accelx > 32767) accelx -= 65536;
    *value = (double)accelx/16384;
}

void main(void) 
{
    //Initialize values
    setpoint = 0;
    current = 0;
    error = 0;
    lastError = 0;
    output = 0;
    P = I = D = 0;
    kp = 1;
    ki = 0.01;
    kd = 0;
    
    //Initialize modules
    IO_init();
    I2C_init();
    UART_init();
    PWM_init();
    accel_init();
    interrupt_init() ;
    
    //Timer configuration
    TMR2 = 0x0;
    PR2 = 0x1F40;
    T2CON= 0x8000;

    while (1) 
    {
        if(doControl == 1)
        {
            error = setpoint - current; //calculate
            
            P = kp * error; //P value for PID controller
            I + ki * lastError * T; //I value for PID controller
            D = kd * (error - lastError) / T; //D value for PID controller
            
            //Limiting I value
            if(I > 20000) I = 20000;
            if(I < -20000) I = -20000;

            
            output = P + I + D; //output value 
            lastError = error; //store last error for I element

            //Convert output in range of servo motor
            output = 0x1388*(1 - output); 
            if (output > 0x2710) output = 0x2710;
            if (output < 0) output = 0x0;
            
            //adjust duty cycle based on output
            PWM_duty(output);
            doControl = 0;
            
            sprintf(str, "%9.2f%9.2f%9.2f%9.2f%9.2f%9.2f\n\r",current, error, P, I, D, output);
            UART_sendString(str);
        }
    }
}