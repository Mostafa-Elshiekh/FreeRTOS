//app one

 /******************************************************************************
 * @file           : main.c
 * @author         : Eng : Mostafa Elshiekh -- Learn In Depth
 * @brief          :Inter-Process communication in RTOS -APP1(Led/Button) Session 2
 ******************************************************************************/


#include "FreeRTOS.h"
#include "STM32F103C6_GPIO_DRIVERS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"

//Handles
TaskHandle_t vTaskButtonHandel = NULL;
TaskHandle_t vTaskLedHandel = NULL;
SemaphoreHandle_t xSemaphore = NULL;

//Tasks prototype
void vTaskButtonHandler(void *par);
void vTaskLedHandler(void *par);

//Global Variables
uint8_t Button_Curr_State = 0;
uint8_t Button_Prev_State=0;


void HW_Init(void)
{

	//led pin init as output pin
	GPIO_Pin_Configure_t PINC13CFG;
	PINC13CFG.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PINC13CFG.GPIO_Pin_Number = GPIO_PIN_13;
	PINC13CFG.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M;
	MCAL_GPIO_Init(GPIOC, &PINC13CFG);

	//Button pin init as input pin
	GPIO_Pin_Configure_t PIN15CFG;
	PIN15CFG.GPIO_MODE = GPIO_MODE_INPUT_PD;
	PIN15CFG.GPIO_Pin_Number=GPIO_PIN_15;
	MCAL_GPIO_Init(GPIOC, &PIN15CFG);

}


int main(void)
{

	//HW init
	RCC_GPIOC_CLOCK_EN();
	HW_Init();


	//Tasks creation
	xTaskCreate(vTaskButtonHandler, "Monitor Button", 128, NULL, 1, vTaskButtonHandel);
	xTaskCreate(vTaskLedHandler, "Led control", 128, NULL, 2, vTaskLedHandel);

	//Semaphore Create
	xSemaphore = xSemaphoreCreateBinary();

	//Scheduler Start
	vTaskStartScheduler();

	while(1)
	{

	}

	return 0;
}


void vTaskButtonHandler(void *par)
{

	while(1)
	{
		Button_Curr_State = MCAL_GPIO_Read_Pin(GPIOC, GPIO_PIN_15); //Read Current Button State

		if(Button_Curr_State != Button_Prev_State) //Check if state changing occur
		{
			xSemaphoreGive(xSemaphore); //Give the semaphore when the button state is changed
		}

		Button_Prev_State = Button_Curr_State; //Update the prev state

		vTaskDelay(25);
	}

}



void vTaskLedHandler(void *par)
{

	while(1)
	{
		if(xSemaphoreTake(xSemaphore , (TickType_t)5)==pdTRUE) //Try to take the Semaphore
		{
			if(Button_Curr_State == 1)
			{
				MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);//Led On When The Button Is Pressed
			}
			else
			{
				MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);//Led Off When The Button Is UnPressed
			}
		}
		vTaskDelay(30);
	}

}
