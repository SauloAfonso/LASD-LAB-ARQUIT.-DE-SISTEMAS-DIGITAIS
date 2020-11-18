#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>


ISR(TIMER0_COMPA_vect) //interrup��o do TC0 a cada 1ms = (64*(249+1))/16MHz
{
	PORTB ^= 0b00100000; //Inverte o estado do PB5
}

int main(void)
{
	DDRB   = 0b00100000;//habilita PB5 como sa�da e demais pinos como entrada
	PORTB  = 0b11011111;//apaga PB5 e habilita pull-ups nos pinos n�o utilizados

	TCCR0A = 0b00000010; //habilita modo CTC do TC0
	TCCR0B = 0b00000011; //liga TC0 com prescaler = 64
	OCR0A = 249;		 //ajusta o comparador para o TC0 contar at� 249
	TIMSK0 = 0b00000010; //habilita a interrup��o na igualdade de compara��o com OCR0A. A interrup��o ocorre a cada 1ms = (64*(249+1))/16MHz
	sei();
	while(1)
	{
	}
}
