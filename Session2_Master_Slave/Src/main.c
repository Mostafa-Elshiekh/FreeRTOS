 /******************************************************************************
 * @file           : main.c
 * @author         : Eng : Mostafa Elshiekh -- Learn In Depth
 * @brief          :Inter-Process communication in RTOS -APP2(Master/Slave) Session 2
 ******************************************************************************/


#include "FreeRTOS.h"
#include "STM32F103C6_GPIO_DRIVERS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"



//Handles
TaskHandle_t vTaskMasterHandel = NULL;
TaskHandle_t vTaskSlaveHandel = NULL;
QueueHandle_t xQueueHandel = NULL;


//Tasks prototype
void vTaskMasterHandler(void *par);
void vTaskSlaveHandler(void *par);


void HW_Init(void)
{
	//led pin init as output pin
	GPIO_Pin_Configure_t PINC13CFG;
	PINC13CFG.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PINC13CFG.GPIO_Pin_Number = GPIO_PIN_13;
	PINC13CFG.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M;
	MCAL_GPIO_Init(GPIOC, &PINC13CFG);
}


int main(void)
{
	RCC_GPIOC_CLOCK_EN();
	HW_Init();

	//Tasks creation
	xTaskCreate(vTaskMasterHandler, "Master generates commands", 128, NULL, 1, vTaskMasterHandel);
	xTaskCreate(vTaskSlaveHandler, "Slave execute commands", 128, NULL, 2, vTaskSlaveHandel);

	//Queue Creation
	xQueueHandel = xQueueCreate(10 , sizeof(uint8_t));

	//Scheduler Start
	vTaskStartScheduler();

	while(1)
	{

	}
	return 0;

}



void vTaskMasterHandler(void *par)
{

	uint8_t OffCommand =0;
	uint8_t ONCommand =1;

	while(1)
	{
		xQueueSend(xQueueHandel ,&OffCommand ,(TickType_t)5); //Send OFF Command To the Queue
		vTaskDelay(10);
		xQueueSend(xQueueHandel ,&ONCommand ,(TickType_t)5);//Send ON Command To the Queue
		vTaskDelay(10);
	}

}

void vTaskSlaveHandler(void *par)
{

   uint8_t CommandRecv=0;

   while(1)
   {
	   xQueueReceive(xQueueHandel, &CommandRecv, (TickType_t)5);

	   if(CommandRecv == 0)
	   {
		   MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);//Turn Off Led When Read OFF command from Queue
	   }
	   else if(CommandRecv == 1)
	   {
		   MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);//Turn On Led When Read OFF command from Queue
	   }
	   vTaskDelay(10);
   }

}

















