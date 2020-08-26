#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void LED()
{
	PORTA = 0x0F;
	
	if ((PINE & 0x10) == 0)
	{
		PORTA = 0x00;
	}
}

ISR(TIMER0_OVF_vect)
{
	LED();
}

void Init_Port()
{
	DDRA |= 0xFF;
	DDRF &= ~0x10;
}

void Init_Timer()
{
	TCCR0 |= (1 << CS02) | (1 << CS01) | (0 << CS00);
	TIMSK |= (1 << TOIE0);
	sei();
}
	

int main()
{
	Init_Port();
	Init_Timer();

	while(1)
	{
	}

	return 0;
}
