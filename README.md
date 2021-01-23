# STM32_GPS_NEO_DECODE
Decode GNGGA Messages from Ublox GPS Modul using Interrupt based receivemode

GNGGA Sample used to Code:
$GNGGA,121017.00,1916.72813,N,01127.64911,E,1,09,0.93,435.5,M,46.6,M,,*4A

How to use existing Code:
Please make sure you selected "Generate peripheral initialization as a pair of .c/.h" at your "project manager Tab" in Cube IDE
include the GPS.h into you Project

ad your Callback Function for the reception interrupt like this:

	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{
 		HAL_UART_Receive_IT(&huart1, Rx_data, RX_BUFFER_SIZE);
  		Position.message=Rx_data;
		update = true;
	  	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	}
	
	
Call GPS_p function is there is a new Dataset in the Buffer. You have to make sure that your.message variable in your GPS_t struct contains the new Data

		 if(update){
	  		  update=false;
	  		  GPS_p(&Position);
	  		  printed=false;
	  	  }


Now you can work with the Data. i added an:
	if(Position.valid){
	}
loop to my Code
