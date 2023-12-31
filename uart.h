#ifndef _EXAMPLE_FILE_NAME_H
#define _EXAMPLE_FILE_NAME_H

#define EXAMPLE_CONSTANT 0

//UART function declaration
void UART_init(void);
char UART_readChar(void);
void UART_sendChar(char c);
void UART_sendString(char *str);

#endif