#include "spi.h"



#define SPI1EN			(1U<<12)
#define GPIOAEN			(1U<<0)

#define SR_TXE			(1U<<1)
#define SR_RXNE			(1U<<0)

#define SR_BSY			(1U<<7)


//PA5 -> CLK
//PA6 -> MISO
//PA7 -> MOSI

//PA9 -> Slave Select


void spi_gpio_init(void)
{
	
	RCC->AHB1ENR |= GPIOAEN;

	

	
	GPIOA->MODER &=~(1U<<10);
	GPIOA->MODER |=(1U<<11);

	
	GPIOA->MODER &=~(1U<<12);
	GPIOA->MODER |=(1U<<13);

	
	GPIOA->MODER &=~(1U<<14);
	GPIOA->MODER |=(1U<<15);


	
	GPIOA->MODER |=(1U<<18);
	GPIOA->MODER &=~(1U<<19);

	
	
	GPIOA->AFR[0] |=(1U<<20);
	GPIOA->AFR[0] &= ~(1U<<21);
	GPIOA->AFR[0] |=(1U<<22);
	GPIOA->AFR[0] &= ~(1U<<23);

	
	GPIOA->AFR[0] |=(1U<<24);
	GPIOA->AFR[0] &= ~(1U<<25);
	GPIOA->AFR[0] |=(1U<<26);
	GPIOA->AFR[0] &= ~(1U<<27);

	
	GPIOA->AFR[0] |=(1U<<28);
	GPIOA->AFR[0] &= ~(1U<<29);
	GPIOA->AFR[0] |=(1U<<30);
	GPIOA->AFR[0] &= ~(1U<<31);


}

void spi1_config(void)
{
	
	RCC->APB2ENR |= SPI1EN;

	
	SPI1->CR1 |=(1U<<3);
	SPI1->CR1 &=~(1U<<4);
	SPI1->CR1 &=~(1U<<5);

	
	SPI1->CR1 |=(1U<<0);
	SPI1->CR1 |=(1U<<1);

	
	SPI1->CR1 &=~(1U<<10);

	
	SPI1->CR1 &= ~(1U<<7);

	
	SPI1->CR1 |= (1U<<2);

	
	SPI1->CR1 &= ~(1U<<11);

	
	SPI1->CR1 |= (1<<8);
	SPI1->CR1 |= (1<<9);

	
	SPI1->CR1 |= (1<<6);

}

void spi1_transmit(uint8_t *data,uint32_t size)
{
	uint32_t i=0;
	uint8_t temp;

	while(i<size)
	{
		
		while(!(SPI1->SR & (SR_TXE))){}

		
		SPI1->DR = data[i];
		i++;
	}
	
	while(!(SPI1->SR & (SR_TXE))){}

	
	while((SPI1->SR & (SR_BSY))){}

	
	temp = SPI1->DR;
	temp = SPI1->SR;
}

void spi1_receive(uint8_t *data,uint32_t size)
{
	while(size)
	{
		
		SPI1->DR =0;

		
		while(!(SPI1->SR & (SR_RXNE))){}

		
		*data++ = (SPI1->DR);
		size--;
	}
}


void cs_enable(void)
{
	GPIOA->ODR &=~(1U<<9);

}


void cs_disable(void)
{
	GPIOA->ODR |=(1U<<9);
}
