
#include <stdint.h>
#include <stdbool.h>

#include <stm32f30x.h>

extern void delay_us(uint32_t us);

void toggleLEDs(GPIO_TypeDef *P);
/**
 * Stub required by newlibc.
 * 
 * E.g. for malloc()
 */
void
_sbrk(void)
{
}

/**
 * Stub required by newlibc.
 *
 * Used for static constructors in C++
 */
void
_init(void)
{
}

int
main (void)
{	
	// Struct of PIO Port A
	GPIO_TypeDef *PA = GPIOA;	

	// Struct of PIO Port E
	GPIO_TypeDef *PE = GPIOE;
	
	//setzen den Button auf Input
	PA->MODER |= 0;
	//setzen den Button auf pulldown
	PA->PUPDR = 0b10; 

	// Red LED is PE9
	// set mode to 01 -> output
	PE->MODER = ((1 << (8*2)) | (1 << (9*2)) | (1 << (10*2)) | (1 << (11*2)) | (1 << (12*2)) | (1 << (13*2)) | (1 << (14*2)) | (1 << (15*2)));

	// switch LED on (is connected between IO and GND)
	PE->ODR = ((1 << 9) | (1 << 11) | (1 << 13) | (1 << 15));

	int lastStatus = (PA->IDR & 1);
	int curStatus;

	while(1) {
		curStatus = (PA->IDR & 1);
		//Wenn der Button gedrückt wird, wird es ausgeführt
		if (curStatus != lastStatus) {
			lastStatus = curStatus;
			toggleLEDs(PE);		
		}
	}
}

//Toggle Funktion die die LEDs wechselt
void toggleLEDs(GPIO_TypeDef *p) {
	p->ODR = ~(p->ODR);
}



