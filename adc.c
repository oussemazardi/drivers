#include "stm32f4xx.h"
#include "adc.h"

#define GPIOAEN		    (1U<<0)
#define ADC1EN			(1U<<8)
#define ADC_CH1			(1U<<0)
#define ADC_SEQ_LEN_1	 0x00
#define CR2_AD0N		(1U<<0)
#define CR2_SWSTART		(1U<<30)
#define SR_EOC			(1U<<1)
#define CR2_CONT		(1U<<1)



void pa1_adc_init(void)
{


	RCC->AHB1ENR |= GPIOAEN;

	
	GPIOA->MODER |=(1U<<2);
	GPIOA->MODER |=(1U<<3);
	
	
	RCC->APB2ENR |= ADC1EN;

	
	ADC1->SQR3 = ADC_CH1;

	
	ADC1->SQR1 = ADC_SEQ_LEN_1;

	
	ADC1->CR2 |= CR2_AD0N;
}


void start_converstion(void)
{
	
	ADC1->CR2 |= CR2_CONT;
	
	ADC1->CR2 |= CR2_SWSTART;
}



uint32_t adc_read(void)
{
	
	while(!(ADC1->SR & SR_EOC)){}

	
	return (ADC1->DR);
}
