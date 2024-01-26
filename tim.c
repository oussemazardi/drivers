#include "stm32f4xx.h"



#define TIM2EN		(1U<<0)
#define CR1_CEN		(1U<<0)

void tim2_1hz_init(void)
{
	
	RCC->APB1ENR |=TIM2EN;

	
	TIM2->PSC =  1600 - 1 ;  //  16 000 000 / 1 600 = 10 000
	
	TIM2->ARR =  10000 - 1;  // 10 000 / 10 000 = 1
	
	TIM2->CNT = 0;

	
	TIM2->CR1 = CR1_CEN;
}
