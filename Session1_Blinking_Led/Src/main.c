 /******************************************************************************
 * @file           : main.c
 * @author         : Eng : Mostafa Elshiekh -- Learn In Depth
 * @brief          : Hello Blinking Led With Free RTOS
 ******************************************************************************/


#include "FreeRTOS.h"
#include "STM32F103C6_GPIO_DRIVERS.h"
#include "task.h"


TaskHandle_t vHelloBlinkingLed = NULL;

/***Task prototype****/
void vHelloBlinkingLedHandler(void *parms);


void HW_Init(void)
{
	GPIO_Pin_Configure_t PIN13CFG;
	PIN13CFG.GPIO_Pin_Number = GPIO_PIN_13;
	PIN13CFG.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PIN13CFG.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M;

	MCAL_GPIO_Init(GPIOC, &PIN13CFG);
}

int main(void)
{
	RCC_GPIOC_CLOCK_EN();
	HW_Init();


	/******tasks creation******/
	xTaskCreate(vHelloBlinkingLedHandler, "BlinkingLed", 128, NULL, 1, vHelloBlinkingLed);


	vTaskStartScheduler();



	while(1)
	{

	}

	return 0;

}

void vHelloBlinkingLedHandler(void *parms)
{

	while(1)
	{
		MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
		vTaskDelay(50);
		MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
		vTaskDelay(50);
	}

}
