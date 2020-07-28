#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

char pattern = 0x01;

char shift(char pattern)
{
	return ((pattern << 7) | (pattern >> 1));
}

ISR(INT4_vect)
{
	pattern = shift(pattern);
	PORTA = pattern;
}

void Init_PORT()
{
	DDRA = 0xFF;
	PORTA = 0x00;

	DDRE = 0x00;
	PORTE = 0x10;
}

void Init_INT4()
{
	EIMSK |= (1 << INT4);
	EICRB |= (1 << ISC41);
	sei();
}

int main()
{
	Init_PORT();
	Init_INT4();
	
	while(1)
	{
	}
	
	return 0;
}
