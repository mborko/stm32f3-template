/*#############################################################
Driver name	    : VCP_F3.c
Author 					: Grant Phillips
Date Modified   : 10/04/2014
Compiler        : Keil ARM-MDK (uVision V4.70.0.0)
Tested On       : STM32F3-Discovery

Description			: Provides a library to use the USER USB port
									on the STM32F3-Discovery to establish a serial
									communication with a remote device (e.g. PC)
									via a Virtual COM Port.  Please study the
									documentation on the example website to fully
									understand how to install and implement this.

Requirements    : * STM32F3-Discovery Board

Functions				: VCP_Init
									VCP_ResetPort
									VCP_PutStr
									VCP_GetStr
													  
Special Note(s) : NONE
##############################################################*/

#include <stdio.h>
#include <string.h>

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_prop.h"



extern __IO uint32_t packet_sent;
extern __IO uint8_t Send_Buffer[VIRTUAL_COM_PORT_DATA_SIZE] ;
extern __IO  uint32_t packet_receive;
extern __IO uint8_t Receive_length;
extern __IO uint8_t Receive_Buffer[64];



void VCP_Init(void);
void VCP_ResetPort(void);
void VCP_PutStr(char *str);
void VCP_GetStr(char str[]);



/*********************************************************************************************
Function name   : VCP_Init
Author 					: Grant Phillips
Date Modified   : 10/04/2014
Compiler        : Keil ARM-MDK (uVision V4.70.0.0)

Description			: Initializes the Virtual COM Port module

Special Note(s) : NONE

Parameters			: NONE
Return value		: NONE
*********************************************************************************************/
void VCP_Init(void)
{
  Set_System();
  Set_USBClock();
  USB_Interrupts_Config();		
  USB_Init();
	
	packet_sent=1;
}



/*********************************************************************************************
Function name   : VCP_ResetPort
Author 					: Grant Phillips
Date Modified   : 10/04/2014
Compiler        : Keil ARM-MDK (uVision V4.70.0.0)

Description			: Resets the Virtual COM Port so that the user is not required to unplug the
									USB port and plug it back in again.

Special Note(s) : NONE

Parameters			: NONE

Return value		: NONE
*********************************************************************************************/
void VCP_ResetPort(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);						//enable the AHB bus to use GPIOA
	
	/* Configure PA12 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;										//which pins to setup, seperated by |
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;									//setup for output mode
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//push-pull mode; also available is GPIO_OType_OD (open drain)
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//update speed is 50Mhz; also available is GPIO_Speed_10MHz and GPIO_Speed_2MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;							//output not pulled up or down
  GPIO_Init(GPIOA, &GPIO_InitStructure);												//initialize GPIOA with the above structure
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_RESET);									//pull the USB BUS+ low to reset the bus
}



/*********************************************************************************************
Function name   : VCP_PutStr
Author 					: Grant Phillips
Date Modified   : 10/04/2014
Compiler        : Keil ARM-MDK (uVision V4.70.0.0)

Description			: Prints a string to the Virtual COM Port

Special Note(s) : NONE

Parameters			: str			-	string (char array) to print

Return value		: NONE
*********************************************************************************************/
void VCP_PutStr(char *str)
{
	/*
	if (packet_sent == 1)																	//make sure previous data has been sent
		CDC_Send_DATA((unsigned char*)str, strlen(str));
	*/
	
	while(packet_sent != 1)																//make sure previous data has been sent
	{}
	CDC_Send_DATA((unsigned char*)str, strlen(str));
}



/*********************************************************************************************
Function name   : VCP_GetStr
Author 					: Grant Phillips
Date Modified   : 10/04/2014
Compiler        : Keil ARM-MDK (uVision V4.70.0.0)

Description			: Waits for a string from the Virtual COM Port terminated by \n or \r

Special Note(s) : NONE

Parameters			: str			-	string (char array) to print

Return value		: NONE
*********************************************************************************************/
void VCP_GetStr(char str[])
{
	uint8_t i;
	
	CDC_Receive_DATA();																		//start receiving data from Virtual COM Port

	//wait for the \n or \r terminating characters
	while((Receive_Buffer[Receive_length-1] != '\n') && (Receive_Buffer[Receive_length-1] != '\r'));			
	
	for(i=0; i<Receive_length; i++)
	{
		str[i] = Receive_Buffer[i];
	}
	str[i] = '\0';
	Receive_length = 0;
}





