#ifndef _I2C_H
#define _12C_H

#define I2CSPEED 100000
#define PBCLK 8000000
#define ADDREAD 0xD1
#define ADDWRITE 0xD0
#define WHO_AM_I 0x75

#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_CONFIG2 0x1D
#define ACCEL_INT_CTRL 0x69
#define USER_CTRL 0x6A
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42


//I2C related functions declared
void I2C_init();
void I2C_wait(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_restart (void);
void ack(void); 
void nack(void);
void I2C_write(unsigned char, char); 
void I2C_read(unsigned char*,char);
void I2C_sendByte(unsigned char, unsigned char ); 
void I2C_getByte(unsigned char, unsigned char *);

#endif /* _I2C_H */