#include <stm8s.h>
#include "common.h"
#include "uart.h"
#include "io.h"

#define BACKSPACE_KEY 'b'
#define ENTER_KEY 'e'

void keyHandler(int code) {
	static char input_buffer[DISPLAY_DIGIT_COUNT + 1] = {' ', ' ', ' ', 0};
	static int previous_key = -1;
	int i;
	char chr;
	if (previous_key == code) {
		return;
	}
	chr = keyChar(code);
	previous_key = code;
	if (chr >= '0' && chr <= '9') {
		for (i=0; i<DISPLAY_DIGIT_COUNT-1; i++) {
			input_buffer[i] = input_buffer[i + 1];
		}
		input_buffer[DISPLAY_DIGIT_COUNT - 1] = chr;
	} else if (chr == BACKSPACE_KEY) {
		for (i=0; i<DISPLAY_DIGIT_COUNT; i++) {
			input_buffer[i] = ' ';
		}
	} else if (chr == ENTER_KEY) {
		UARTSend(input_buffer);
		for (i=0; i<DISPLAY_DIGIT_COUNT; i++) {
			input_buffer[i] = '0';
		}
	}
	print(input_buffer);
}

void main(void) {
	disableInterrupts();
	initializeUART(9600UL, 2000000UL);
	initializeIO();
	enableInterrupts();
	while(1) {
		keyHandler(stageProtected());
	}
}