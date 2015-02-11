 
#include "stm32f30x.h"
 
void RCC_Configuration(void)
{
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}
  
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
  /* Connect PA9 to USART1_Tx */
  //GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_7);
 
  /* Connect PA10 to USART1_Rx */
  //GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_7);
 
  /* Configure USART Tx as alternate function push-pull */
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

  //GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
 
  /* Configure USART Rx as alternate function push-pull */
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;

  //GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
 
void USART1_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
 
  /* USART resources configuration (Clock, GPIO pins and USART registers) ----*/
  /* USART configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
  /* USART configuration */
  USART_Init(USART1, &USART_InitStructure);
 
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}
 
int main(void)
{
  RCC_Configuration();
  GPIO_Configuration();
  USART1_Configuration();
  
  while(1)
  {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // Wait for Empty
  
    USART_SendData(USART1, 0x49); // Send 'I'
  }
  
  while(1); // Don't want to exit
}
 
/**************************************************************************************/
 
#ifdef  USE_FULL_ASSERT
 
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
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

