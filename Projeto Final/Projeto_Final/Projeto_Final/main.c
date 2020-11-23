#include <atmel_start.h>

// Vari�veis Globais
int8_t seg, min, hrs;
int16_t mili;
char tarefas, tarefa_atual;

// Interrup��o externa 0, captura o Ligar/Desligar
ISR(INT0_vect)
{
	
}

// Interrup��o externa 1, captura o Play/Pause
ISR(INT1_vect)
{
	
}

// Interrup��o 0 por mudan�a de pino, captura o Finalizar Tarefa
ISR(PCINT0_vect)
{
	if (tarefa_atual == tarefas)
	{
		// Finaliza contagem
		atualizaDisplay('x');
	}
	else
	{
		// Salvar hrs e min na mem�ria flash
		atualizaDisplay('f');
		tarefa_atual ++;
		_delay_ms(1000);
		atualizaDisplay(tarefa_atual);
	}
}	

// Interrup��o do TC0 a cada 1ms = (64*(249+1))/16MHz
ISR(TIMER0_COMPA_vect)
{
	mili ++;
	
	if (hrs == 0)
	{
		S0 = S1 = S2 = 0;
	}
	
	if (mili == 1000)
	{
		mili = 0;
		seg ++;
		
		if (seg == 60)
		{
			seg = 0;
			min ++;
			
			if (min == 5)
			{
				OCR0A = 0.05*256;
			}
			else if (min == 15)
			{
				OCR0A = 0.25*256;
			}
			else if (min == 30)
			{
				OCR0A = 0.50*256;
			}
			else if (min ==45)
			{
				OCR0A = 0.75*256;
			}
			else if (min ==60)
			{
				OCR0A = 0.98*256;
				min = 0;
				hrs ++;
				
				if (hrs == 1)
				{
					S0 = 1;
					S1 = S2 = 0;
					led1 = 1;
				}
				if (hrs == 2)
				{
					S1 = 1;
					S0 = S2 = 0;
					led2 = 1;
				}
				if (hrs == 3)
				{
					S2 = 0;
					S0 = S1 = 1;
					led3 = 1;
				}
				if (hrs == 4)
				{
					S2 = 1;
					S0 = S1 = 0;
					led4 = 1;
				}
				if (hrs == 5)
				{
					S1 = 0;
					S0 = S2 = 1;
					led5 = 1;
				}
				if (hrs == 6)
				{
					S0 = 0;
					S1 = S2 = 1;
					led6 = 1;
					
					// Finalizar
				}
			} 
		}
	}
}

// Fun��o de tratamento de interrup��o � Recep��o USART
ISR(USART_RX_vect)
{
	tarefas = UDR0; // UDR0 cont�m o dado recebido via USART
	tarefa_atual = 1;
	
	nokia_lcd_clear();
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_set_cursor(0, 10);
	nokia_lcd_write_string("Ser�o realizadas", 1);
	nokia_lcd_set_cursor(0, 20);
	nokia_lcd_write_string(tarefas, 1);
	nokia_lcd_set_cursor(0, 30);
	nokia_lcd_write_string("Atividades", 1);
	nokia_lcd_set_cursor(0, 40);
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_render();
	
	atualizaDisplay(tarefa_atual);
	
	USART_Transmit(tarefas);
}

int main(void)-
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	mili = 0;
	seg = 0;
	min = 0;
	hrs = 0;
	
	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0, 10);
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_set_cursor(0, 20);
	nokia_lcd_write_string("Quantas atividades", 1);
	nokia_lcd_set_cursor(0, 30);
	nokia_lcd_write_string("deseja realizar?", 1);
	nokia_lcd_set_cursor(0, 40);
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_render();
	
	while (1) 
	{
	}
}

// Fun��o para atualiza��o do display PCD8544-7
void atualizaDisplay(char entrada){
	
	if (entrada == 'f')
	{
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 10);
		nokia_lcd_write_string("Atividade", 1);
		nokia_lcd_set_cursor(0, 20);
		nokia_lcd_write_string(tarefa_atual, 1);
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string("Finalizada!", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
	}
	else if (entrada == 'x')
	{
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		// Pegar informa��es da mem�ria flash
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
		
	}
	else
	{
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 10);
		nokia_lcd_write_string("Atividade", 1);
		nokia_lcd_set_cursor(0, 20);
		nokia_lcd_write_string(entrada, 1);
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string("Em Andamento...", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
	}
}
