#include "uart.h"

#define UART1_SR   *((uint8_t*)0x005230)
#define UART1_DR   *((uint8_t*)0x005231)
#define UART1_BRR1 *((uint8_t*)0x005232)
#define UART1_BRR2 *((uint8_t*)0x005233)
#define UART1_CR1  *((uint8_t*)0x005234)
#define UART1_CR2  *((uint8_t*)0x005235)
#define UART1_CR3  *((uint8_t*)0x005236)
#define UART1_CR4  *((uint8_t*)0x005237)
#define UART1_CR5  *((uint8_t*)0x005238)
#define UART1_GTR  *((uint8_t*)0x005239)
#define UART1_PSCR *((uint8_t*)0x00523A)
#define UART2_SR   *((uint8_t*)0x005240)
#define UART2_DR   *((uint8_t*)0x005241)
#define UART2_BRR1 *((uint8_t*)0x005242)
#define UART2_BRR2 *((uint8_t*)0x005243)
#define UART2_CR1  *((uint8_t*)0x005244)
#define UART2_CR2  *((uint8_t*)0x005245)
#define UART2_CR3  *((uint8_t*)0x005246)
#define UART2_CR4  *((uint8_t*)0x005247)
#define UART2_CR5  *((uint8_t*)0x005248)
#define UART2_GTR  *((uint8_t*)0x005249)
#define UART2_PSCR *((uint8_t*)0x00524A)

void initializeUART(unsigned long baud_rate, unsigned long f_master) {
	unsigned long brr = f_master/baud_rate;
	UART1_BRR2 = brr & 0x000F;
	UART1_BRR2 |= brr >> 12;
	UART1_BRR1 = (brr >> 4) & 0x00FF;
	UART1_CR1 = 0b00000000;
	UART1_CR2 = 0b00001100;
	UART1_CR3 = 0b00000000;
}

void UARTSend(char* message) {
	char* chr = message;
	while(*chr) {
		while (!(UART1_SR & 0b10000000));
		UART1_DR = *chr;
		chr++;
	}
}