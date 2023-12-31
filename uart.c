#include<xc.h>
#include "uart.h"

void UART_init(void) {
    ANSELAbits.ANSA6 = 0;
    TRISAbits.TRISA6 = 1;
    TRISCbits.TRISC12 = 0;
    
    U1BRG = 51;
    U1STA = 0;
    U1MODE = 0x8000;
    U1STA = 0x1400;
}

char UART_readChar(void) {
    while (!U1STAbits.URXDA) {}
    return U1RXREG;
}

void UART_sendChar(char c) {
    while (U1STAbits.UTXBF) {}
    U1TXREG = c;
}

void UART_sendString(char *str) {
    for (; *str; str++) UART_sendChar(*str);    
}