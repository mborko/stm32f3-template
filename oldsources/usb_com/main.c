/**
 ******************************************************************************
 *
 * File Name       : main.c
 * Author          : Grant Phillips
 * Date Modified   : 10/04/2014
 * Compiler        : Keil ARM-MDK (uVision V4.70.0.0)
 * Tested On       : STM32F3-Discovery
 *
 * Description     : Example program that uses the STM32F3-Discovery 
 * board as a Virtual COM Port (VCP) module to 
 * write "Hello World!" serially to a PC's COM 
 * port and then display a count from 0 to 255 
 * repeatedly.  Run a program called Termite 
 * (http://www.compuphase.com/software_termite.htm) 
 * to use as a RS-232 terminal on the PC to see 
 * what messages are sent to the PC, and to send 
 * messages to the STM32F3-Discovery.
 *
 * Requirements    : * STM32F3-Discovery Board
 * Make sure the VCP_F3.c file is in the same folder than the project
 * Circuit         : * Mini-USB cable from the User USB port on the
 * STM32F3-Discovery to a USB port on a PC
 *
 * See the STM32F3-Discovery User Manual (UM1570) for the block 
 * diagram of the STM32F303VCT6 processor (p13), a summary of
 * the GPIO connections (p21-29) and the schematic diagram (p31-34)
 *
 ******************************************************************************
 */

/* #includes -------------------------------------------------*/
#include "main.h"
#include "stm32f3_discovery.h"

/* #defines --------------------------------------------------*/

/* #function prototypes --------------------------------------*/
void Delay(__IO uint32_t nTime);

/* #global variables -----------------------------------------*/
//structure used for setting up a GPIO port
GPIO_InitTypeDef GPIO_InitStructure;
//structure used for setting up the SysTick Interrupt
RCC_ClocksTypeDef RCC_Clocks;

// Unused global variables that have to be included to ensure correct compiling */
// ===============================================================================
__IO uint32_t TimingDelay = 0;																	//used with the Delay function
__IO uint8_t DataReady = 0;
__IO uint32_t USBConnectTimeOut = 100;
__IO uint32_t UserButtonPressed = 0;
__IO uint8_t PrevXferComplete = 1;
// ===============================================================================

int main(void)
{
	uint8_t count=0;
	char buf[40];

	/* Set the SysTick Interrupt to occur every 1ms) */
	RCC_GetClocksFreq(&RCC_Clocks);
	if (SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000))
		//will end up in this infinite loop if there was an error with Systick_Config
		while(1);

	//pull the USB D+ line low to disconnect the USB device from the PC
	VCP_ResetPort();
	//allow a few milliseconds before reconnecting
	Delay(500);
	//initialize the STM32F3-Discovery as a Virtual COM Port - device gets re-detected by the PC
	VCP_Init();

	/* Wait for the user to press the BLUE USER button
	   to start the program.  This gives the user chance
	   to first connect to the Virtual COM Port from an
	   application like Termite */
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI); 
	//wait until the button is pressed
	while(UserButtonPressed != 1);

	//print characetrs to the buf string
	sprintf(buf, "Hello World!\n");
	//write a string variable to the VCP
	VCP_PutStr(buf);

	/* Main program loop */
	while (1)
	{
		//print a newline and integer value (in HEX) to the buf string
		sprintf(buf, "\n%2X", count);
		//write a string variable to the VCP
		VCP_PutStr(buf);
		count = count + 1;
		Delay(250);
	}	
}

// Function to insert a timing delay of nTime
void Delay(__IO uint32_t nTime)
{
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

// Function to Decrement the TimingDelay variable.
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

// Unused functions that have to be included to ensure correct compiling
uint32_t L3GD20_TIMEOUT_UserCallback(void)
{
	return 0;
}

uint32_t LSM303DLHC_TIMEOUT_UserCallback(void)
{
	return 0;
}

