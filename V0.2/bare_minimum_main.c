/****************************************************************************
 *                                                                          *
 * Filename: Bare Minimum                                                       *
 *                                                                          *
 * Purpose : Standard C sample                    *
 *                                                                          *
 * Created by: Daniel Bernhard 2021                                             *
 *                                            *
 *                                                                          *
 ****************************************************************************/

#include <stdio.h>
#include "main.h"
#include "GPS.h"
/* entry point */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  HAL_UART_Receive_IT(&huart1, Rx_data, RX_BUFFER_SIZE);
  Position.message=Rx_data;
  update = true;
  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

int main(void)
{
		HAL_Init();
	    SystemClock_Config();
  		MX_GPIO_Init();
  		MX_USART1_UART_Init();
 	 	HAL_UART_Receive_IT (&huart1, Rx_data, RX_BUFFER_SIZE);

		//Put GPS Modul to 115200 baud at 10Hz
		uint8_t ubxcfgrate[] = { // UBX-CFG-RATE 10 Hz Measurement/Navigation

  		0xB5,0x62,0x06,0x08,0x06,0x00,

  		0x64,0x00,0x01,0x00,0x01,0x00, // Payload

  		0x7A,0x12 }; // Checksum
  		HAL_UART_Transmit(&huart1,(uint8_t *)ubxcfgrate,14,200);
		 while (1)
  		{
			if(update){
	  	 	update=false;
	  		GPS_p(&Position);
			}
		if(!((Position.FIXTIME+1000) >test)){Position.valid = false;} 

		}


}