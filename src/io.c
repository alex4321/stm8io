#include "io.h"
#include "common.h"
char display_buffer[DISPLAY_DIGIT_COUNT + 1] = {' ', ' ', ' ', 0};

void initializeIO() {
	// I/O port modes
	SETBIT(GPIOA->DDR, 1, 1); //PA1
	SETBIT(GPIOA->CR1, 1, 1);
	SETBIT(GPIOA->DDR, 2, 1); //PA2
	SETBIT(GPIOA->CR1, 2, 1);
	SETBIT(GPIOA->DDR, 3, 1); //PA3
	SETBIT(GPIOA->CR1, 3, 1);
	SETBIT(GPIOB->DDR, 4, 1); //PB4
	SETBIT(GPIOB->DDR, 5, 1); //PB5
	SETBIT(GPIOD->DDR, 1, 1); //PD1
	SETBIT(GPIOD->CR1, 1, 1);
	SETBIT(GPIOD->DDR, 2, 1); //PD2
	SETBIT(GPIOD->CR1, 2, 1);
	SETBIT(GPIOD->DDR, 3, 1); //PD3
	SETBIT(GPIOD->CR1, 3, 1);
	SETBIT(GPIOD->DDR, 4, 1); //PD4
	SETBIT(GPIOD->CR1, 4, 1);
	SETBIT(GPIOC->DDR, 3, 1); //PC3
	SETBIT(GPIOC->CR1, 3, 1);
	SETBIT(GPIOC->DDR, 4, 0); //PC4
	SETBIT(GPIOC->CR1, 4, 1);
	SETBIT(GPIOC->DDR, 5, 0); //PC5
	SETBIT(GPIOC->CR1, 5, 1);
	SETBIT(GPIOC->DDR, 6, 0); //PC6
	SETBIT(GPIOC->CR1, 6, 1);
	SETBIT(GPIOC->DDR, 7, 0); //PC6
	SETBIT(GPIOC->CR1, 7, 1);
}

void useLine(uint8_t line) {
	switch(line) {
	case 1:
		SETBIT(GPIOD->ODR, 1, 1);
		SETBIT(GPIOD->ODR, 2, 1);
		SETBIT(GPIOD->ODR, 3, 0);
		break;
	case 2:
		SETBIT(GPIOD->ODR, 1, 1);
		SETBIT(GPIOD->ODR, 2, 0);
		SETBIT(GPIOD->ODR, 3, 1);
		break;
	case 3:
		SETBIT(GPIOD->ODR, 1, 0);
		SETBIT(GPIOD->ODR, 2, 1);
		SETBIT(GPIOD->ODR, 3, 1);
		break;
	default:
		SETBIT(GPIOD->ODR, 1, 1);
		SETBIT(GPIOD->ODR, 2, 1);
		SETBIT(GPIOD->ODR, 3, 1);
	}
}

void showCodedCharacter(uint8_t code) {
	SETBIT(GPIOD->ODR, 4, code & 0b00000001);
	SETBIT(GPIOA->ODR, 1, code & 0b00000010);
	SETBIT(GPIOA->ODR, 2, code & 0b00000100);
	SETBIT(GPIOA->ODR, 3, code & 0b00001000);
	SETBIT(GPIOB->ODR, 4, code & 0b00010000);
	SETBIT(GPIOB->ODR, 5, code & 0b00100000);
	SETBIT(GPIOC->ODR, 3, code & 0b01000000);
}

void showCharacter(char character) {
	uint8_t code = 0;
	switch(character) {
		case ' ': code = 0b00000000; break;
		case '1': code = 0b00010001; break;
		case '2': code = 0b01101101; break;
		case '3': code = 0b01110101; break;
		case '4': code = 0b01010011; break;
		case '5': code = 0b01110110; break;
		case '6': code = 0b01111110; break;
		case '7': code = 0b00010101; break;
		case '8': code = 0b01111111; break;
		case '9': code = 0b01110111; break;
		case '0': code = 0b00111111; break;
		default:  code = 0b00000000; break;
	}
	showCodedCharacter(code);
}

int keyboardLineState() {
	return (!GETBIT(GPIOC->IDR, 4)) |
		((!GETBIT(GPIOC->IDR, 5)) << 1) |
		((!GETBIT(GPIOC->IDR, 6)) << 2) |
		((!GETBIT(GPIOC->IDR, 7)) << 3);
}

int stage() {
	int i;
	int keyboard_state = 0;
	int line_state;
	int line_mask;
	for (i=1; i<=DISPLAY_DIGIT_COUNT; i++) {
		useLine(0);
		showCharacter(display_buffer[i-1]);
		useLine(i);
		line_state = keyboardLineState();
		if (line_state) {
			line_mask = 1 << (1 + DISPLAY_DIGIT_COUNT + i);
			keyboard_state |= line_state | line_mask;
		}
	}
	return keyboard_state;
}

int stageProtected() {
	int i;
	int kbd = stage();
	int result = kbd;
	int current_kbd;
	for (i=1; i<KEY_READ_COUNT; i++) {
		current_kbd = stage();
		if (current_kbd != kbd) {
			result = 0;
		}
	}
	return result;
}

char keyChar(int code) {
	switch(code) {
	case 136: return '1';
	case  72: return '2';
	case  40: return '3';
	case 132: return '4';
	case  68: return '5';
	case  36: return '6';
	case 130: return '7';
	case  66: return '8';
	case  34: return '9';
	case  65: return '0';
	case 129: return 'b';
	case  33: return 'e';
	default: return 0;
	};
}

void print(char* str) {
	int i;
	for (i=0; i<DISPLAY_DIGIT_COUNT; i++) {
		if (str[i] != 0) {
			display_buffer[i] = str[i];
		} else {
			display_buffer[i] = ' ';
		}
	}
}