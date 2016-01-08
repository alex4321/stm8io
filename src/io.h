#ifndef IO_H
#define IO_H

#include <stm8s.h>
#define DISPLAY_DIGIT_COUNT 3
#define KEY_READ_COUNT 30

void initializeIO();
void useLine(uint8_t line);
void showCodedCharacter(uint8_t code);
void showCharacter(char character);
int keyboardLineState();
int stage();
int stageProtected();
char keyChar(int code);
void print(char* str);

#endif