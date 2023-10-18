 /******************************************************************************
 * @file           : main.c
 * @author         : Eng : Mostafa Elshiekh -- Learn In Depth
 * @brief          :RTOS Issues  -APP(Application about Deadlock) Session 4
 ******************************************************************************/

#include "STM32_F103C6.h"
#include "STM32F103C6_GPIO_DRIVERS.h"
#include "STM32F103C6_USART_DRIVERS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "string.h"
#include "semphr.h"


/*Global Vars*/
char UART_MESSAGE[50];
#define taken 0
#define given 1


/*=================Fanctions Prototype=========*/
void FunUARTConfig(void);
void FunGPIOConfig(void);
void FunPrintSemaphoreState(char tasknum,char semphrNum ,char semphrstate);



/*================TASKS HANDELS===============*/
TaskHandle_t xTask1_Handle = NULL;
TaskHandle_t xTask2_Handle = NULL;
TaskHandle_t xTask3_Handle = NULL;
SemaphoreHandle_t xSemaphore1 = NULL;
SemaphoreHandle_t xSemaphore2 = NULL;



/*===============Tasks Prototype==============*/
void vTask1_Handler(void* params);
void vTask2_Handler(void* params);
void vTask3_Handler(void* params);



int main(void)
{

	FunGPIOConfig();
	FunUARTConfig();


    /*===============Tasks Creation===============*/
	xTaskCreate(vTask1_Handler, "task1", 128, NULL, 1, xTask1_Handle);

	xTaskCreate(vTask2_Handler, "task2", 128, NULL, 2, xTask2_Handle);

	xTaskCreate(vTask3_Handler, "task3", 128, NULL, 3, xTask3_Handle);


	//Semaphore Create
	xSemaphore1 = xSemaphoreCreateBinary();
	xSemaphore2 = xSemaphoreCreateBinary();

	xSemaphoreGive(xSemaphore1);
	xSemaphoreGive(xSemaphore2);

	//Scheduler Start
	vTaskStartScheduler();

	while(1)
	{

	}

	return 0;
}


void FunUARTConfig(void)
{
	//==========================UART2 INIT=============================//
		/* PA2  ->TX
		 * PA3 ->RX
		 * PA0 ->CTS
		 * PA1 ->RTS*/
		USART_Config uartCFG;
		uartCFG.BaudRate = USART_BaudRate_115200;
		uartCFG.HW_FlowCtrl = USART_HW_FlowCtrl_NONE;
		uartCFG.IRQ_Enable = USART_IRQ_Enable_RXNE;
		uartCFG.Parity = USART_Parity_NONE;
		uartCFG.PayLoad_Length = USART_PayLoad_Length_8B;
		uartCFG.StopBits = USART_StopBits_1;
		uartCFG.USART_MODE = USART_MODE_RX_AND_TX;
		MCAL_UART_INIT(USART2, &uartCFG);
		MCAL_UART_GPIO_Set_Pins(USART2);
		//================================================================//
}

void FunGPIOConfig(void)
{
	   RCC_GPIOC_CLOCK_EN();
		RCC_GPIOA_CLOCK_EN();

		GPIO_Pin_Configure_t PIn13_CFG;
		PIn13_CFG.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
		PIn13_CFG.GPIO_OUTPUT_SPEED=GPIO_SPEED_10M;
		PIn13_CFG.GPIO_Pin_Number=GPIO_PIN_13;
		MCAL_GPIO_Init(GPIOC,&PIn13_CFG);
		MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);

		GPIO_Pin_Configure_t PIn12_CFG;
		PIn12_CFG.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
		PIn12_CFG.GPIO_OUTPUT_SPEED=GPIO_SPEED_10M;
		PIn12_CFG.GPIO_Pin_Number=GPIO_PIN_12;
		MCAL_GPIO_Init(GPIOA,&PIn12_CFG);
		MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);

		GPIO_Pin_Configure_t PIn11_CFG;
		PIn11_CFG.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
		PIn11_CFG.GPIO_OUTPUT_SPEED=GPIO_SPEED_10M;
		PIn11_CFG.GPIO_Pin_Number=GPIO_PIN_11;
		MCAL_GPIO_Init(GPIOA,&PIn11_CFG);
		MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);

		GPIO_Pin_Configure_t PIn10_CFG_For_Tick;
		PIn10_CFG_For_Tick.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
		PIn10_CFG_For_Tick.GPIO_OUTPUT_SPEED=GPIO_SPEED_10M;
		PIn10_CFG_For_Tick.GPIO_Pin_Number=GPIO_PIN_10;
		MCAL_GPIO_Init(GPIOA,&PIn10_CFG_For_Tick);
		MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
}

void FunPrintSemaphoreState(char tasknum,char semphrNum ,char semphrstate)
{
	int counter=0;

        if(semphrstate == taken)
        {
			strcpy(UART_MESSAGE ,"\nTASK :The Semaphore  Was Taken By Me<-\n");

			UART_MESSAGE[4]=tasknum;
			UART_MESSAGE[20]=semphrNum;
			for(counter=0;counter<strlen(UART_MESSAGE);counter++)
			{
				MCAL_UART_SEND_DATA(USART2, (UART_MESSAGE+counter), enable);
			}
        }
        else if(semphrstate == given)
        {
        	strcpy(UART_MESSAGE ,"\nTASK :The Semaphore  Was Given By Me->\n");
        	UART_MESSAGE[4]=tasknum;
        	UART_MESSAGE[20]=semphrNum;
			for(counter=0;counter<strlen(UART_MESSAGE);counter++)
			{
				MCAL_UART_SEND_DATA(USART2, (UART_MESSAGE+counter), enable);
			}
        }
}



/*======================System Tasks========================*/
void vTask1_Handler(void* params)
{
   int i =0, j=0;
   int xLastWakeTime = xTaskGetTickCount();
		while(1)
		{
			if(xSemaphoreTake(xSemaphore1,portMAX_DELAY) == 1 )
			{
				// if Semaphore1 has been successfully taken .. then print it
				FunPrintSemaphoreState('1','1',taken);
			}

			j=0;
			//trying taking xSemaphore2 after half time left
			for(i=0;i<300000;i++)
			{
				if(i>200000 &&j==0)
				{
					j=1;
					if(xSemaphoreTake(xSemaphore2,portMAX_DELAY) == 1 )
					{
						// if Semaphore2 has been successfully taken .. then print it
						FunPrintSemaphoreState('1','2',taken);
					}
				}
			}

			MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
			vTaskDelay(10);
			MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
			vTaskDelay(10);


			if(xSemaphoreGive(xSemaphore1)==1);
			{
				// if Semaphore1 has been successfully giving .. then print it
				FunPrintSemaphoreState('1','1',given);
			}
			if(xSemaphoreGive(xSemaphore2)==1);
			{
				// if Semaphore2 has been successfully giving .. then print it
				FunPrintSemaphoreState('1','2',given);
			}
			vTaskDelayUntil(&xLastWakeTime,120);
		}
}

void vTask2_Handler(void* params)
{
	   int xLastWakeTime = xTaskGetTickCount();
			while(1)
			{
				MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
				vTaskDelay(10);
				MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
				vTaskDelay(10);
				vTaskDelayUntil(&xLastWakeTime,120);
			}
}


void vTask3_Handler(void* params)
{
	   int xLastWakeTime = xTaskGetTickCount();
			while(1)
			{
				if(xSemaphoreTake(xSemaphore2,portMAX_DELAY) == 1 )
				{
					// if Semaphore2 has been successfully taken .. then print it
					FunPrintSemaphoreState('3','2',taken);
				}

				if(xSemaphoreTake(xSemaphore1,portMAX_DELAY)==1)
				{
					// if Semaphore1 has been successfully taken .. then print it
					FunPrintSemaphoreState('3','1',taken);

				}

				MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
				vTaskDelay(10);
				MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
				vTaskDelay(10);


				/*Giving The xSemaphores*/
				if(xSemaphoreGive(xSemaphore2)==1);
				{
					// if Semaphore2 has been successfully giving .. then print it
					FunPrintSemaphoreState('3','2',given);
				}
				if(xSemaphoreGive(xSemaphore1)==1);
				{
					// if Semaphore1 has been successfully giving .. then print it
					FunPrintSemaphoreState('3','1',given);
				}

				vTaskDelayUntil(&xLastWakeTime,120);

			}
}

void vApplicationTickHook( void )
{
	MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
}




















