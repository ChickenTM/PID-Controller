#include<xc.h>
#include "i2c.h"

void I2C_init(void) {
    double BRG = 0.5 * PBCLK / I2CSPEED - 1 - 0.5 * PBCLK * 130e-9;
    I2C3CON = 0;
    I2C3CONbits.DISSLW = 0;
    I2C3BRG = (int)BRG;
    I2C3CONbits.ON = 1;
}

void I2C_wait(void) {
    while (I2C3CON & 0x1F) {}
    while (I2C3STATbits.TRSTAT) {}
}

void I2C_start(void) {
    I2C_wait();
    I2C3CONbits.SEN = 1;
    while (I2C3CONbits.SEN == 1) {}
}

void I2C_stop(void) {
    I2C_wait();
    I2C3CONbits.PEN = 1;
    while (I2C3CONbits.PEN == 1) {}
}

void I2C_restart(void) {
    I2C_wait();
    I2C3CONbits.RSEN = 1;
    while (I2C3CONbits.RSEN == 1) {}
}

void ack(void) {
    I2C_wait();
    I2C3CONbits.ACKDT = 0;
    I2C3CONbits.ACKEN = 1;
    while (I2C3CONbits.ACKEN == 1) {}
}

void nack(void) {
    I2C_wait();
    I2C3CONbits.ACKDT = 1;
    I2C3CONbits.ACKEN = 1;
    while (I2C3CONbits.ACKEN == 1) {}
}

void I2C_write(unsigned char data, char ack) {
    I2C_wait();
    I2C3TRN = data;
    while (I2C3STATbits.TBF == 1) {}
    I2C_wait();
    if (ack) while (I2C3STATbits.ACKSTAT == 1) {}
    }

void I2C_read(unsigned char *value, char ack_nack) {
    I2C3CONbits.RCEN = 1;
    while (I2C3CONbits.RCEN == 1) {}
    while (I2C3STATbits.RBF == 0) {}
     *value = I2C3RCV;
    if (!ack_nack) ack();
     else nack();
}

void I2C_sendByte(unsigned char address, unsigned char value){
    I2C_start();
    I2C_write(ADDWRITE, 1);
    I2C_write(address, 1);
    I2C_write(value, 1);
    I2C_stop();
}

void I2C_getByte(unsigned char address, unsigned char *value){
    I2C_start();
    I2C_write(ADDWRITE, 1);
    I2C_write(address, 1);
    I2C_restart();
    I2C_write(ADDREAD, 1);
    I2C_read(value, 1);
    I2C_stop();
}


    