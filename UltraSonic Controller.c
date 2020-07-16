#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define PRESCALER 1024

uint8_t numbers[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};

uint8_t distance = 0;


void UART_init()
{
	UBRR1H = 0x00; // ��żӵ� ���� 
	UBRR1L = 207; // 9600 ������ ����

	UCSR1A |= _BV(U2X1); // U2X1�� 1 ������ �� 0 
	UCSR1C |= 0x06; // 8 ������ ��Ʈ 2�� on

	UCSR1B |= _BV(RXEN1); // UART ���ű�� on
	UCSR1B |= _BV(TXEN1); // UART �۽ű�� on
}

void Timer_init()
{
	TCCR1B |= (1 << CS12) | (1 << CS10);
}



void display_digit(int distance)
{
	int digit = distance % 10; // ���� �ڸ� 
	int ten = (distance % 100)/ 10; // ���� �ڸ�
	int hundred = distance / 100;
	int arr[] = {digit, ten, hundred, 0};
	
	

	for (int i = 1; i < 5; i++)
	{
		PORTG = 0x0F;
		PORTG &= (1 << (i - 1));
		PORTC = numbers[arr[i-1]];
		_delay_ms(2);
	}

		
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
		
	return pulse_width / 58; // �����İ� 1cm�� ���µ� �ɸ��� �ð����� ������ 
}

ISR(TIMER0_COMP_vect)
{

	distance = measure_distance();
	display_digit(distance);
}

int main()
{

	UART_init();
	
	DDRA = 0xFF;
	DDRB |= 0x02;
	DDRB &= 0xFE;
	DDRC = 0xFF;
	DDRG = 0x0F;
	
	TCCR0 |= (1<< CS02);	//256 PRESCALER
	TCCR0 |= (1<< WGM01);	
	OCR0 = 65;	


	TIMSK |= (1<<OCIE0);	
	sei();			
	Timer_init();

	

	while(1)
	{

		if(distance > 100)
		{
			PORTA = 0xF0;
			
		}

		else 
		{
			PORTA = 0x0F;
			
		
		}
	}
	return 0;
}
