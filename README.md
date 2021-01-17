# STM32_GPS_NEO_DECODE
Decode GNGGA Messages from Ublox GPS Modul using Interrupt based receivemode

GNGGA Sample used to Code:
$GNGGA,121017.00,1916.72813,N,01127.64911,E,1,09,0.93,435.5,M,46.6,M,,*4A

How to use existing Code:
V0.1
define your Buffers: Rx_data and printbuffer.

char printbuffer[300];
char Rx_data[100];

create your UART IT Callback function

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  HAL_UART_Receive_IT(&huart1, Rx_data, 100);
  GPS.message=Rx_data;
  update = true;
  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}


start the first receive cycle in main(){

  HAL_UART_Receive_IT (&huart1, Rx_data, 100);
  
  
  while(1){
  
  ....
  
  	  	  if(update){
	  		  update=false;
	  		  GPS_p();
          }
  ....}
  }
  
  
  
  for testing i added:
  
  	  	  long int test;
	      test = HAL_GetTick();                                                        //Hole Zeit von Systicktimer
	      if(!((GPS.FIXTIME+1000) >test)){GPS.valid = false;}                          //Fix verfällt noch 1 sekunde

	    if(GPS.valid&& !printed){
	    	////printf("%d:%d:%d.%d \n %lf  %c\n %lf  %c\n", GPS.hours,GPS.mins,GPS.secs,GPS.millis,GPS.latitude,GPS.NS_ind, GPS.longitude,GPS.EW_ind);
	    	sprintf(printbuffer,"%d:%d:%d.%d \n latitude:%lf NS_ind: %c\n longitude:%lf SE_ind: %c\nHDOP : %lf\nSats : %d\nAltitude:  %lf M\n",      GPS.hour,GPS.min,GPS.sec,GPS.millis,GPS.lat,GPS.NS_ind, GPS.lon,GPS.EW_ind,GPS.hdop,GPS.sats,GPS.alt);
	    	//HAL_UART_Transmit(&huart2,(uint8_t *)GPS.message,strlen(GPS.message),1000);
	    	HAL_UART_Transmit(&huart2,(uint8_t *) printbuffer,strlen(printbuffer),100);
	        sprintf(printbuffer,"indexbegin: %d       indexend:  %d\nChecksum: 0x%x    Berechnete Checksum: 0x%x\n",GPS.index_begin,GPS.index_end,GPS.checksum,GPS.checksum_calc);
	        HAL_UART_Transmit(&huart2,(uint8_t*) printbuffer,strlen(printbuffer),200);
	        printed = true;
	    	switch (GPS.fixtype){
	        	case 0:
	        	////printf("\n no fix\n");
	       	 	break;
	        	case 1:
	        	////printf("\n 2D fix\n");
	        	break;
	        	case 2:
	       		////printf("\n 3D fix\n");
	        	break;
	        default:
	        	////printf ("ERROR\n");
	        	HAL_Delay(100);
	    	}

	    }
	    else{
		    ////printf("NO VALID GPS DATA FOUND");
		}
