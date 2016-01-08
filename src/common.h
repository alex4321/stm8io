#ifndef COMMON_H
#define COMMON_H

#define SETBIT(register, bit, value) if(value) \
	{register |= 1 << bit;} \
	else {register &= ~(1 << bit);}
#define GETBIT(register, bit) ((register & (1 << bit)) ? 1 : 0)

#endif