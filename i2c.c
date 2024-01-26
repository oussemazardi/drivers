#include "stm32f4xx.h"



#define 	GPIOBEN			(1U<<1)
#define 	I2C1EN			(1U<<21)

#define 	I2C_100KHZ						80		//0B 0101 0000 = Decimal = 80
#define 	SD_MODE_MAX_RISE_TIME			17
#define		CR1_PE			(1U<<0)

#define		SR2_BUSY		(1U<<1)
#define     CR1_START		(1U<<8)
#define		SR1_SB			(1U<<0)
#define		SR1_ADDR		(1U<<1)
#define		SR1_TXE			(1U<<7)
#define		CR1_ACK			(1U<<10)
#define     CR1_STOP		(1U<<9)
#define		SR1_RXNE		(1U<<6)
#define	    SR1_BTF			(1U<<2)

/*
 * PB8 ---- SCL
 * PB9 ----- SDA
 * */

void I2C1_init(void)
{
	
	 RCC->AHB1ENR |=GPIOBEN;

	
	GPIOB->MODER &=~(1U<<16);
	GPIOB->MODER |=(1U<<17);

	GPIOB->MODER &=~(1U<<18);
	GPIOB->MODER |=(1U<<19);

	
	GPIOB->OTYPER |=(1U<<8);
	GPIOB->OTYPER |=(1U<<9);

	
	GPIOB->PUPDR |=(1U<<16);
	GPIOB->PUPDR &=~(1U<<17);

	GPIOB->PUPDR |=(1U<<18);
	GPIOB->PUPDR &=~(1U<<19);

	
	GPIOB->AFR[1] &=~(1U<<0);
	GPIOB->AFR[1] &=~(1U<<1);
	GPIOB->AFR[1] |=(1U<<2);
	GPIOB->AFR[1] &=~(1U<<3);

	GPIOB->AFR[1] &=~(1U<<4);
	GPIOB->AFR[1] &=~(1U<<5);
	GPIOB->AFR[1] |=(1U<<6);
	GPIOB->AFR[1] &=~(1U<<7);


	
	 RCC->APB1ENR |= I2C1EN;

	
	I2C1->CR1 |= (1U<<15);

	
	I2C1->CR1 &=~(1U<<15);

	
	I2C1->CR2 = (1U<<4);   //16 Mhz

	
	I2C1->CCR = I2C_100KHZ;

	
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	
	I2C1->CR1 |= CR1_PE;


}

void I2C1_byteRead(char saddr, char maddr, char* data) {

      volatile int tmp;

	  
	  while (I2C1->SR2 & (SR2_BUSY)){}

	
	  I2C1->CR1 |= CR1_START;

	 
	  while (!(I2C1->SR1 & (SR1_SB))){}

	 
	   I2C1->DR = saddr << 1;

	  
	   while (!(I2C1->SR1 & (SR1_ADDR))){}

	 
	  tmp = I2C1->SR2;

     
      I2C1->DR = maddr;

     
     while (!(I2C1->SR1 & SR1_TXE)){}

   
      I2C1->CR1 |= CR1_START;

     
     while (!(I2C1->SR1 & SR1_SB)){}

     
     I2C1->DR = saddr << 1 | 1;

     
    while (!(I2C1->SR1 & (SR1_ADDR))){}

   
    I2C1->CR1 &= ~CR1_ACK;

   
    tmp = I2C1->SR2;

   
     I2C1->CR1 |= CR1_STOP;

   
    while (!(I2C1->SR1 & SR1_RXNE)){}

  
      *data++ = I2C1->DR;
}

void I2C1_burstRead(char saddr, char maddr, int n, char* data) {

	volatile int tmp;

	 
	  while (I2C1->SR2 & (SR2_BUSY)){}

     
    I2C1->CR1 |= CR1_START;

   
     while (!(I2C1->SR1 & SR1_SB)){}

    
     I2C1->DR = saddr << 1;

    
    while (!(I2C1->SR1 & SR1_ADDR)){}

   
     tmp = I2C1->SR2;

    
    while (!(I2C1->SR1 & SR1_TXE)){}

   
    I2C1->DR = maddr;

    
    while (!(I2C1->SR1 & SR1_TXE)){}

   
    I2C1->CR1 |= CR1_START;

    
    while (!(I2C1->SR1 & SR1_SB)){}

    
    I2C1->DR = saddr << 1 | 1;

   
    while (!(I2C1->SR1 & (SR1_ADDR))){}

   
    tmp = I2C1->SR2;

   
      I2C1->CR1 |=  CR1_ACK;

    while(n > 0U)
    {
    	/*if one byte*/
    	if(n == 1U)
    	{
    		/* Disable Acknowledge */
    	    I2C1->CR1 &= ~CR1_ACK;

    	    /* Generate Stop */
    	    I2C1->CR1 |= CR1_STOP;

 			/* Wait for RXNE flag set */
            while (!(I2C1->SR1 & SR1_RXNE)){}

            /* Read data from DR */
            *data++ = I2C1->DR;
            break;
    	}
    	else
    	{
       	 /* Wait until RXNE flag is set */
           while (!(I2C1->SR1 & SR1_RXNE)){}

           /* Read data from DR */
           (*data++) = I2C1->DR;

           n--;
    	}
    }

}


void I2C1_burstWrite(char saddr, char maddr, int n, char* data) {

	volatile int tmp;

	
	 while (I2C1->SR2 & (SR2_BUSY)){}

   
    I2C1->CR1 |= CR1_START;

   
    while (!(I2C1->SR1 & (SR1_SB))){}

  
    I2C1->DR = saddr << 1;

    /* Wait until addr flag is set */
    while (!(I2C1->SR1 & (SR1_ADDR))){}

    /* Clear addr flag */
    tmp = I2C1->SR2;

   
    while (!(I2C1->SR1 & (SR1_TXE))){}

 
    I2C1->DR = maddr;

    for (int i = 0; i < n; i++) {

        while (!(I2C1->SR1 & (SR1_TXE))){}

      /* Transmit memory address */
      I2C1->DR = *data++;
    }

   
    while (!(I2C1->SR1 & (SR1_BTF))){}

    
   I2C1->CR1 |= CR1_STOP;
}





