#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

uint8_t numbers[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};


int s = 0;


ISR(INT4_vect)
{
	s = button();

	if(s == 2500)
	{
		s = 0;
	}
	
}

ISR(INT5_vect)
{
	s = button();

	if(s < 50)
	{
		 s = 0;
	}
}

int button()
{

	if(!(PINE & (1 << 4)))
	{
		s = s + 50;	
		return s;
	}

	if(!(PINE & (1 << 5)))
	{
		s = s - 50;	
		return s;
	}


	return 0;
}






void display_digit(int s)
{
	int digit = s % 10; // 일의 자리 
	int ten = (s % 100)/ 10; // 십의 자리
	int hundred = (s % 1000) / 100; // 백의 자리
	int thousand = s / 1000;
	int arr[] = {digit, ten, hundred, thousand};
	
	

	for (int i = 1; i < 5; i++)
	{
		PORTG = 0x0F;
		PORTG &= (1 << (i - 1));
		PORTC = numbers[arr[i-1]];
		_delay_ms(2);
	}

		
}


void Init_PORT()
{

	DDRC = 0xFF;
	DDRE = 0x00;
	DDRG = 0x0F;
	PORTE = 0x30;
}

void Init_INT4()
{
	EIMSK |= (1 << INT4);
	EICRB |= (1 << ISC41);
	sei();
}

void Init_INT5()
{
	EIMSK |= (1 << INT5);
	EICRB |= (1 << ISC51);
	sei();
}

int main()
{
	Init_PORT();
	Init_INT4();
	Init_INT5();
	
	while(1)
	{
		display_digit(s);
	}
	
	return 0;
}
