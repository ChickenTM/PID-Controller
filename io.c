#include<xc.h>
#include "io.h"

void IO_init (void)
{
    TRISCbits.TRISC13 = 0;
    TRISCbits.TRISC8 = 1;
    ANSELCbits.ANSC8 = 1;

    ANSELBbits.ANSB13 = 0;
    TRISBbits.TRISB13 = 0;
    TRISBbits.TRISB7 = 0;

    TRISDbits.TRISD1 = 0;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC15 = 0;
}