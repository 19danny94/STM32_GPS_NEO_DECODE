#include "GPS.h"







/*Function to filter an Process Raw input strings of Ublox NEMA Modul
 * you have to give an Pointer to your struct wich contains the GPS data to this function
 */
void GPS_p(struct gps_t* GPS){
    (*GPS).valid = false;
    (*GPS).lat,(*GPS).lon,(*GPS).hdop,(*GPS).alt,(*GPS).bla,(*GPS).index_end,(*GPS).hour,(*GPS).min,(*GPS).sec,(*GPS).millis,(*GPS).sats,(*GPS).checksum,(*GPS).checksum_calc = 0;

    if (strstr((*GPS).message, "$GNGGA")){
        //Valid message in stream
        (*GPS).message2 = strstr((*GPS).message,"$GNGGA");         //pointer auf begin des GNGGA satzes setzen
        sscanf((*GPS).message2, "$GNGGA,%2hd%2hd%2hd.%2hd,%lf,%c,%lf,%c,%hd,%hd,%lf,%lf,M,%lf,M,,*%x",&(*GPS).hour,&(*GPS).min,&(*GPS).sec,&(*GPS).millis,&(*GPS).lat,&(*GPS).NS_ind,&(*GPS).lon,&(*GPS).EW_ind,&(*GPS).fixtype,&(*GPS).sats,&(*GPS).hdop,&(*GPS).alt,&(*GPS).bla,&(*GPS).checksum);

        //Reset for Checksum calculation
        (*GPS).checksum_calc=0;
        (*GPS).index_end=1;
        char x = 'A';

        //while no '+' found go on and search
        while(x!= '*'){
            (*GPS).index_end++;
            x = (*GPS).message2[(*GPS).index_end];
        }

        (*GPS).index_end -=2;

        //Exclusiv OR to build Checksum out of Data
        for (int i=1; i<(*GPS).index_end;i++){
            (*GPS).checksum_calc=(*GPS).checksum_calc^(*GPS).message2[i];

        }

        //If Data was valid...
            if((*GPS).checksum_calc==(*GPS).checksum){
                (*GPS).valid = true;
                (*GPS).FIXTIME = (HAL_GetTick());
            }
    }


}



/*Convert the raw Data into degrees
 * you must give the float Value and the Indicator as char to this Function
 */

double min_to_degree(double lonlat, char indicator){
	char temp_str[20];															//create string out of Data
	sprintf(temp_str,"%lf", lonlat);											//
	char * str_point = strchr(temp_str, '.');									//search for the '.'
	double mins;																//
	sscanf(str_point-2, "%lf", &mins);											//put string into new variable wich contains minutes
	unsigned short int degrees;

	sscanf(str_point-5, "%3hd", &degrees);										//Copy degrees in another string
	if (degrees == 0){
		sscanf(str_point-4, "%2hd", &degrees);									//degree <100°----> copy
	}
	double end_degrees = degrees+(mins/60);										//devide mins by 60
	if(indicator == 'S'||indicator == 'W'){										//If we are soutch or West the degrees go negativ
		end_degrees =end_degrees*-1;
	}
	return(end_degrees);														//Return the degree value
}


/*Give you Data out of your struct for simple Access
 * You have to give an Pointer to your GPS Data Struct
 */
double get_lat_degree(struct gps_t* GPS){
	double degrees = min_to_degree((*GPS).lat, (*GPS).NS_ind);
	return (degrees);
}

double get_lon_degree(struct gps_t *GPS){
	double degrees = min_to_degree((*GPS).lon, (*GPS).EW_ind);
	return (degrees);
}

double get_alt(struct gps_t *GPS){
	return (*GPS).alt;
}

char get_GPS_time(struct gps_t *GPS){
	char temp_str[14];
	sprintf(temp_str, "%d:%d:%d.%d", (*GPS).hour,(*GPS).min,(*GPS).sec,(*GPS).millis);
	return (*temp_str);
}

