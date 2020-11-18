/*
 * Exerc5_SaraAndrade.c
 *
 * Created: 06/10/2020 09:59:17
 * Author : saraa
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

// Vari�veis globais
int8_t unid, dezena, pausa; 
int16_t count;
uint8_t LUT[10] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};

ISR(TIMER0_COMPA_vect) //interrup��o do TC0 a cada 1ms = (64*(249+1))/16MHz
{
	count += 1;
	if(count == 1000)
	{
		count = 0;		
		if(pausa != 1)
		{
			PORTC = LUT[dezena];
			PORTB = LUT[unid];
			if(unid < 9)
			{
				unid++;
			}
			else
			{
				unid = 0;
				
				if(dezena < 9)
				{
					dezena++;
				}
				else
				{
					dezena = 0;
				}
			}
		}
	}
}

ISR(INT0_vect) // Interrup��o externa 0, captura o start/pause
{
	if(pausa == 1)
	{
		pausa = 0;
	}
	else
	{
		pausa = 1;
	}
}

ISR(INT1_vect) // Interrup��o externa 1, captura o reset
{
	dezena = 0;
	unid = 0;
	PORTC = LUT[dezena];
	PORTB = LUT[unid];
}

int main(void)
{
	DDRD =	0x00; // Todos os pinos da porta D como entradas
	PORTD = 0b00001100; // Habilita pull-ups dos pinos PD2 e PD3
	
	DDRC =	0xFF; // Habilita Todos os pinos da porta C como sa�da
	PORTC = 0x00; 
	DDRB =	0xFF; // Habilita Todos os pinos da porta B como sa�da
	
	//Configura��o das interrup��es
	EICRA = 0b00001010;// Interrup��es externas INT0 e INT1 na borda de descida
	EIMSK = 0b00000011;// Habilita as interrup��es externas INT0 e INT1
	sei();// Habilita interrup��es globais, ativando o bit I do SREG
	
	//Configura��o dos Timers
	TCCR0A = 0b00000010; // Habilita modo CTC do TC0
	TCCR0B = 0b00000011; // Liga TC0 com prescaler = 64
	OCR0A = 249;		 // Ajusta o comparador para o TC0 contar at� 249
	TIMSK0 = 0b00000010; // Habilita a interrup��o na igualdade de compara��o com OCR0A. A interrup��o ocorre a cada 1ms = (64*(249+1))/16MHz
	
	pausa = 1;
	dezena = 0;
	unid = 0;
	count = 0;

	while (1)
	{ 
	}
}