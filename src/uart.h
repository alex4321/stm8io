#ifndef UART_H
#define UART_H
#include <stm8s.h>

void initializeUART(unsigned long baud_rate, unsigned long f_master);
void UARTSend(char* message);

#endif