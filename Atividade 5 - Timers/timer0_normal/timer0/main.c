//================================================================================ //
// HABILITANDO A INTERRUP��O POR ESTOURO DO TC0 //
//================================================================================ //
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
//----------------------------------------------------------------------------------
ISR(TIMER0_OVF_vect) //interrup��o do TC0
{
	PORTB ^= 0b00100000; //Inverte o estado do PB5
}
//----------------------------------------------------------------------------------
int main()
{
	DDRB   = 0b00100000;//habilita PB5 como sa�da e demais pinos como entrada
	PORTB  = 0b11011111;//apaga PB5 e habilita pull-ups nos pinos n�o utilizados
	TCCR0B = 0b00000001;//TC0 sem prescaler. Overflow a cada 16us = 256/16MHz
	TIMSK0 = 0b00000001;//habilita a interrup��o por estouro do TC0
	sei(); //habilita a chave de interrup��o global
	while(1)
	{
		//a cada estouro do TC0 o programa desvia para ISR(TIMER0_OVF_vect)
	}
}
//===================================================================================
