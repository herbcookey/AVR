/*
 * GccApplication1.c
 *
 * Created: 2021-01-04 오후 12:38:09
 * Author : 21530011
 */ 

#define F_CPU 16000000L
#define MAX 10
#define PWD 123
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART1.h"

FILE OUTPUT
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

void ADC_init(unsigned char channel)
{
	ADMUX |= (1 << REFS0);
	ADCSRA |= 0x07;
	ADCSRA |= (1 << ADEN);
	ADCSRA &= 0b11011111; //단일변환
	//ADCSRA |= (0 << ADFR);
	
	ADMUX |= ((ADMUX & 0xE0) | channel);
	ADCSRA |= (1 << ADSC);
}
int read_ADC(void)
{
	while(!(ADCSRA & (1 << ADIF)));
	return ADC;
}

int main(void)
{
	unsigned char CNT = 0;
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();
	ADC_init(0);
	
	//UART1_transmit('a');
	
	int array[100][8];
	int i;
	int pwd = 0;
	int zero = 0 ;
	int one = 0 ;
	int two = 0 ;
	int three = 0 ;
	int four = 0;
	int five = 0;
	int six = 0;
	int seven = 0;

	//printf("HELLO");
	while(1)
	{
		for (i=0;i<30; i++)
		for (CNT = 0 ; CNT< 8; CNT++)
		{
			ADMUX = (ADMUX & 0xF8 | CNT);
			_delay_ms(10);
			ADCSRA |= (1 << ADSC);
			array[i][CNT] = read_ADC();
			
			if(array[i][CNT] >= 920)
			{
				array[i][CNT] = 1;
			}
			else
			{
				array[i][CNT] = 0;
			}
			
			
		}
		
		for (i=0;i<30; i++)
		{
			for (CNT = 0 ; CNT< 8; CNT++)
			printf("%d\t",array[i][CNT]);
			printf("pwd : %d\n", pwd);
			
			zero = zero + array[i][0];
			one = one + array[i][1];
			two = two + array[i][2];
			three = three + array[i][3];
			four = four + array[i][4];
			five = five + array[i][5];
			six = six + array[i][6];
			seven = seven + array[i][7];
			if(zero == MAX)
			{
				zero = 0;
				pwd = 10*pwd + 1;
				if(pwd == PWD)
				{
					printf("pwd : %d\n", pwd);
					break;
				}
				else if(pwd > PWD)
				{
					pwd = 0;
				}
			}
			if(one == MAX)
			{
				one = 0;
				pwd = 10*pwd + 2;
				if(pwd == PWD)
				{
					printf("pwd : %d\n", pwd);
					break;
				}
				else if(pwd > PWD)
				{
					pwd = 0;
				}
			}
			if(two == MAX)
			{
				two = 0;
				pwd = 10*pwd + 3;
				if(pwd == PWD)
				{
					printf("pwd : %d\n", pwd);
					break;
				}
				else if(pwd > PWD)
				{
					pwd = 0;
				}
			}
			if(three == MAX)
			{
				three = 0;
				pwd = 10*pwd + 4;
				if(pwd == PWD)
				{
					printf("pwd : %d\n", pwd);
					break;
				}
				else if(pwd > PWD)
				{
					pwd = 0;
				}
			}
			if(four == MAX)
			{
				four = 0;
				pwd = 10*pwd + 5;
				if(pwd == PWD)
				{
					printf("pwd : %d\n", pwd);
					break;
				}
				else if(pwd > PWD)
				{
					pwd = 0;
				}
			}
			if(five == MAX)
			{
				five = 0;
				pwd = 10*pwd + 6;
				if(pwd == PWD)
				{
					printf("pwd : %d\n", pwd);
					break;
				}
				else if(pwd > PWD)
				{
					pwd = 0;
				}
			}
			if(six == MAX)
			{
				six = 0;
				pwd = 10*pwd + 7;
				if(pwd == PWD)
				{
					printf("pwd : %d\n", pwd);	
					break;
				}
				else if(pwd > PWD)
				{
					pwd = 0;
				}
			}
			if(seven == MAX)
			{
				seven = 0;
				pwd = 10*pwd + 8;
				if(pwd == PWD)
				{
					printf("pwd : %d\n", pwd);
					break;
				}
				else if(pwd > PWD)
				{
					pwd = 0;
				}
			}
			
		}
		if(pwd == PWD)
		{
			printf("OPEN");
			break;
		}
	}
	
}
