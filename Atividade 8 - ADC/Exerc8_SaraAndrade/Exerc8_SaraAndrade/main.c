/*
 * Exerc8_SaraAndrade.c
 *
 * Created: 25/10/2020 12:28:18
 * Author : saraa
 */ 

#define F_CPU 16000000UL // Frequ�ncia de trabalho da CPU
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "nokia5110.h"

#define tam_vetor 2 
unsigned char leitura_ADC_string[tam_vetor];
int8_t count = 0;

ISR(ADC_vect)
{
	OCR0A  = ADC / 4;
	count = (OCR0A * 100) / 256;
}

//------------------------------------------------------------------------------------
// Convers�o de inteiro para string
//------------------------------------------------------------------------------------
void int2string(unsigned int valor, unsigned char *disp)
{
	for(uint8_t n=0; n<tam_vetor; n++)
	disp[n] = 0 + 48; // Limpa vetor para armazenagem dos digitos
	disp += (tam_vetor-1);
	do
	{
		*disp = (valor%10) + 48; // Pega o resto da divis�o por 10
		valor /=10; // Pega o inteiro da divis�o por 10
		disp--;
	}while (valor!=0);
}

int main()
{
	DDRB = 0xFF; // Portas B como sa�da
	DDRD = 0xFF; // Portas D como sa�da
	DDRC = 0x00; // Portas C como entrada
	PORTC = 0xFE; // Desabilita o pullup do PC0
	
	// Fast PWM, TOP = 0xFF, OC0A habilitado
	TCCR0A = 0b10000011; // PWM n�o invertido no pino OC0A
	TCCR0B = 0b00000101; // Liga TC0, prescaler = 1024, fpwm = f0sc/(256*prescaler) = 16MHz/(256*1024) = 61Hz
	OCR0A = 0; // Controle do ciclo ativo do PWM 0C0A

	// Configura��es ADC
	ADMUX = 0b01000000; // Tens�o AVCC, canal 0
	ADCSRA = 0b11101111; // Habilita o AD, habilita interrup��o, modo de convers�o cont�nua, prescaler = 128
	ADCSRB = 0x00; // Mdo de convers�o cont�nua
	DIDR0 = 0b00111110; // Habilita pino PC0 como entrada do ADC0
	
	sei(); // Habilita interrup��es globais
	nokia_lcd_init(); // Inicia o LCD
	
	while(1)
	{
		nokia_lcd_clear(); // Limpa o LCD
		int2string(count, leitura_ADC_string); // Converte a leitura do ADC em string
		nokia_lcd_set_cursor(20, 10);
		nokia_lcd_write_string(leitura_ADC_string, 4); // Escreve a leitura no buffer do LCD
		nokia_lcd_render(); // Atualiza a tela do display com o conte�do do buffer
		_delay_ms(1000);
	}
}