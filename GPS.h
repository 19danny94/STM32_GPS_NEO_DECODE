

/*
 *
 * Author:          Daniel Bernhard
 * Version:         v0.1
 */
#ifndef INC_GPS_H_
#define INC_GPS_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>


struct gps_t{
  long int FIXTIME;									//Systicktimer at verrification of checksum
  double lat,lon,hdop,alt,bla;
  short index_begin,index_end,hour,min,sec,millis,fixtype,sats,checksum,checksum_calc;
  char NS_ind,EW_ind;
  bool valid;
  char* message;
  char* message2;

};

struct gps_t GPS;

bool update;



void GPS_p();						//should be called if data was copied to .message variable of GPS_t


#endif /* INC_GPS_H_ */
