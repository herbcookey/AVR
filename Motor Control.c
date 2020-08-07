#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#define PRESCALER 1024
#define Motor1(s) OCR1C = s
#define Motor2(s) OCR1B = s

uint8_t numbers[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};
uint8_t distance = 0;

int s = 0;
void display_digit(int s);
void Motor();
void Left_wheel(int s);
void Right_wheel(int s);

void UART_init()
{
	UBRR1H = 0x00; // 통신속도 설정 
	UBRR1L = 207; // 9600 보율로 설정

	UCSR1A |= _BV(U2X1); // U2X1만 1 나머지 다 0 
	UCSR1C |= 0x06; // 8 데이터 비트 2개 on

	UCSR1B |= _BV(RXEN1); // UART 수신기능 on
	UCSR1B |= _BV(TXEN1); // UART 송신기능 on
}

uint8_t measure_distance()
{
	PORTB &= ~(1 << PB1);
	_delay_ms(1);
	PORTB |= (1 << PB1);
	_delay_ms(10);
	PORTB &= ~(1 << PB1);
	
	TCNT1 = 0;
	while(!(PINB & 0x01))
		if(TCNT1 > 65000) return 0;
	
	TCNT1 = 0;
	while(PINB & 0x01)
	{
		if (TCNT1 > 65000)
		{	 
			TCNT1 = 0;
			break;
		}
	}

	int pulse_width = 1000000 * TCNT1 * PRESCALER / F_CPU;
		
	return pulse_width / 58; // 초음파가 1cm를 가는데 걸리는 시간으로 나누기 
}

ISR(INT4_vect)
{
	s += 5;
	if(s > 255)
	{
		s = 255;
	}
	
}

ISR(INT5_vect)
{
	s -= 5;
	if(s < 0)
	{
		s = 0;
	}
}


ISR(TIMER0_OVF_vect)
{
	display_digit(s);
}

ISR(TIMER1_OVF_vect)
{
	Motor(s);
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

void Motor(int s)
{
	PORTF &= ~0x03;
	PORTF &= ~0x0C;


	Left_wheel(s);
	Right_wheel(s);
}		


void Left_wheel(int s)
{
	if(s > 5)
	{
		Motor1(s);
	}
	else if(s < 5)
	{
		Motor1(0);
	}
}

void Right_wheel(int s)
{
	if(s > 5)
	{
		Motor2(s);
	}
		
	else if(s < 5)
	{
		Motor2(0);
	} 
}
void Init_PORT()
{
	DDRB |= 0x02; 
	DDRB &= 0xFE;
	DDRB |= (1 << PB7) | (1 << PB6);
	DDRC = 0xFF;
	DDRE = 0x00;
	DDRG = 0x0F;
	DDRF |= (1 << PF0) | (1 << PF1) | (1 << PF2) | (1 << PF3);
	PORTE = 0x30;
}

void Init_PWM()
{
	TCCR1A |= (1 << WGM10) | (0 << WGM11) | (1 << COM1C1) | (1 << COM1B1); //8-bit Fast PWM 
	TCCR1B |=  (1 << WGM12) | (0 << WGM13) | (0 << CS12) | (1 << CS11) |(1 << CS10);; // 8-bit Fast PWM
	OCR1C = 0;
	OCR1B = 0;
}


void Init_INT()
{
	EIMSK |= (1 << INT4) | (1 << INT5); // External Interrupt 4 & 5 Active
	EICRB |= (1 << ISC41) | (1 << ISC51); // Time of occurrence is falling edge
	TCCR0 |= (1 << CS02) | (1 << CS01) | (0 << CS00); // PRESCALER 256
	TIMSK |= (1 << TOIE0) | (1 << TOIE1); // 
	sei();
}


int main()
{
	UART_init();
	Init_PORT();
	Init_PWM();
	Init_INT();

	
	while(1)
	{
		
	}
	
	return 0;
}
