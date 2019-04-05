/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 9.3.0   2019-04-05

The MIT License (MIT)
Copyright (c) 2019 STMicroelectronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************
*/

/* Includes */
#include "main.h"

/* Private typedef */
/* Private define  */
extern void SysTick_DelayUs(uint32_t nTime);
extern void SysTick_DelayMs(uint32_t nTime);
/* Private macro */
/* Private variables */
uint16_t uart_count=0;
uint8_t uart_data[1000];
/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{

  /**
  *  IMPORTANT NOTE!
  *  The symbol VECT_TAB_SRAM needs to be defined when building the project
  *  if code has been located to RAM and interrupts are used. 
  *  Otherwise the interrupt table located in flash will be used.
  *  See also the <system_*.c> file and how the SystemInit() function updates 
  *  SCB->VTOR register.  
  *  E.g.  SCB->VTOR = 0x20000000;  
  */

  /* TODO - Add your application code here */
	RCC_Configuration();
	SysTick_Configuration();
	GPIO_Configuration();

	UART_Configuration();
	NVIC_Configuration();
	TIM_Configuration();

	ClearLED();
	TestLED_ALL(0);
  /* Infinite loop */
  while (1)
  {
	msDelay(2);
	ClearLED();
	uartAllLED(1);
  }

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}

#ifdef USE_SEE
#ifndef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval sEE_FAIL.
  */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  /* Return with error code */
  return sEE_FAIL;
}
#endif
#endif /* USE_SEE */


/*Systick*/
uint32_t System_GetTick(void){
	return systick.Systick_Counter;
}

/*Config*/
void RCC_Configuration(void){
	/*Use PB13, PA9 PA10 UART1*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	/*Use Timer 2 as tim base*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	/*UART1*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	/*UART2*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
}
void GPIO_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	/*PB13*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*PA5 and PA7*/
	GPIO_InitStructure.GPIO_Pin = DI | DCKI;
	GPIO_Init(PORT_LED,&GPIO_InitStructure);

	/*PA9-Rx PA10-Tx*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/*PA3 as Rx*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
void SysTick_Configuration(void){
  if (SysTick_Config(SystemCoreClock/1000) ) //1000000:us 1000:ms
  {
		while(1);
  }
}
void TIM_Configuration(void){
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	/*
	TIM2 freq is 1Hz
	Freq = 1Hz = 1s = 1000ms => choose TIM_Periodd = 1000 -1;
	*/
	TIM_DeInit(TIM2);
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_InitStructure.TIM_Prescaler	=63999;
	TIM_InitStructure.TIM_Period =999;
	TIM_InitStructure.TIM_ClockDivision = 0;
	TIM_InitStructure.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE); /*Tim IT Enable*/

	/*TIM3 freq = 1000 Hz*/
	TIM_DeInit(TIM3);
	TIM_InitStructure.TIM_Prescaler = 63;
	TIM_InitStructure.TIM_Period = 1000;
	TIM_TimeBaseInit(TIM3, &TIM_InitStructure);
	TIM_ITConfig(TIM3,TIM_IT_CC2,ENABLE);

}
void NVIC_Configuration(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	/*Enable TIM2 interrupt*/
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM3_IRQn);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void UART_Configuration(void){
	USART_InitTypeDef USART_InitStructure;

	/*UART1*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);

	/*UART2*/
	USART_InitStructure.USART_BaudRate = 250000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_Init(USART2,&USART_InitStructure);

	/*Clear Receive Flag*/
	USART_ClearFlag(USART2,USART_IT_RXNE);

	/*Enable interrupt when receive or Error*/
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	/*Enable UART*/
	USART_Cmd(USART1,ENABLE); /*printf*/
	USART_Cmd(USART2,ENABLE);
}

/*Delay*/
void usDelay(uint32_t nTime){
	uint16_t counter = nTime & 0xffff;
	TIM_Cmd(TIM3, ENABLE);
	TIM_SetCounter(TIM3,counter);
	while (counter > 1)
	{
		counter = TIM_GetCounter(TIM3);
	}
	TIM_Cmd(TIM3, DISABLE);
}
void msDelay(uint32_t nTime){ /*function to delay nTime ms with TIM2 set as ms*/
	uint16_t counter = nTime & 0xffff;
	TIM_Cmd(TIM2,ENABLE);
	TIM_SetCounter(TIM2, counter);
	while (counter > 1)
	{
		counter = TIM_GetCounter(TIM2);
	}
	TIM_Cmd(TIM2,DISABLE);
}

