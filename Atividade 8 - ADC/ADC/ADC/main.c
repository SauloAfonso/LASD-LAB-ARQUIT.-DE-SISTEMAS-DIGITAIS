//================================================================================ //
// Teste do ADC
//================================================================================ //

#define F_CPU 16000000UL //Frequ�ncia de trabalho da CPU
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "nokia5110.h"

#define tam_vetor 4
unsigned char leitura_ADC_string[tam_vetor];
uint16_t leitura_ADC = 0;

ISR(ADC_vect)
{
	leitura_ADC = ADC;
}

//------------------------------------------------------------------------------------
//Convers�o de inteiro para string
//------------------------------------------------------------------------------------
void int2string(unsigned int valor, unsigned char *disp)
{
	for(uint8_t n=0; n<tam_vetor; n++)
		disp[n] = 0 + 48; //limpa vetor para armazenagem dos digitos
	disp += (tam_vetor-1);
	do
	{
		*disp = (valor%10) + 48; //pega o resto da divis�o por 10
		valor /=10; //pega o inteiro da divis�o por 10
		disp--;
	}while (valor!=0);
}

int main()
{
	//GPIO
	DDRB = 0xFF; //Porta B como sa�da
	DDRC = 0x00; //Porta C como entrada
	PORTC = 0xFE; //Desabilita o pullup do PC0

	//Configura ADC
	ADMUX = 0b00000000; //Tens�o interna de ref (1.1V), canal 0
	ADCSRA = 0b11101111; //habilita o AD, habilita interrup��o, modo de convers�o cont�nua, prescaler = 128
	ADCSRB = 0x00; //modo de convers�o cont�nua
	DIDR0 = 0b00111110; //habilita pino PC0 como entrada do ADC0
	
	sei(); //Habilita interrup��es globais
	nokia_lcd_init(); //Inicia o LCD
	
	while(1)
	{
		nokia_lcd_clear(); //Limpa o LCD
		int2string(leitura_ADC, leitura_ADC_string); //converte a leitura do ADC em string
		nokia_lcd_write_string(leitura_ADC_string,1); //Escreve a leitura no buffer do LCD
		nokia_lcd_render(); //Atualiza a tela do display com o conte�do do buffer
		_delay_ms(1000);
	}
}
