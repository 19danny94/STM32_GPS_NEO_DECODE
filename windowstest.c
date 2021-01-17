
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


//Variablen
struct gps_t{
  long int FIXTIME;
  double lat,lon,hdop,alt,bla;
  short index_begin,index_end,hour,min,sec,millis,fixtype,sats,checksum,checksum_calc;
  char NS_ind,EW_ind;
  bool valid;
  char* message;
  char* message2;

};
struct gps_t GPS;
long int tick = 29380;

int HAL_gettick(){                  //Simuliere Systicktimer
    tick+=1700;
    return(tick);
}

void GPS_p(){
    //Reset aller variablen, falls es was schief geht steht überall null
    GPS.valid = false;
    GPS.lat,GPS.lon,GPS.hdop,GPS.alt,GPS.bla,GPS.index_end,GPS.hour,GPS.min,GPS.sec,GPS.millis,GPS.sats,GPS.checksum,GPS.checksum_calc = 0;   
    
    if (strstr(GPS.message, "$GNGGA")){
        //Valid message in stream
        //Beispiel von Position zuhause $GNGGA,151017.00,4916.72813,N,01127.64911,E,1,09,0.93,435.5,M,46.6,M,,*4A
        GPS.message2 = strstr(GPS.message,"$GNGGA");         //pointer auf begin des GNGGA satzes setzen
        sscanf(GPS.message2, "$GNGGA,%2hd%2hd%2hd.%2hd,%lf,%c,%lf,%c,%hd,%hd,%lf,%lf,M,%lf,M,,*%x",&GPS.hour,&GPS.min,&GPS.sec,&GPS.millis,&GPS.lat,&GPS.NS_ind,&GPS.lon,&GPS.EW_ind,&GPS.fixtype,&GPS.sats,&GPS.hdop,&GPS.alt,&GPS.bla,&GPS.checksum);
        
        printf("position %f %c , %f %c @ %f M\nfix_info:\nSATS: %d\nFIX: %d\nHDOP: %f\nchecksum: 0x%2hx\r\n",GPS.lat,GPS.NS_ind,GPS.lon,GPS.EW_ind,GPS.alt,GPS.sats,GPS.fixtype,GPS.hdop,GPS.checksum);
        
        //Resette alles für Checksumme
        GPS.checksum_calc=0;
        GPS.index_end=1;
        char x = 'A';
        
        //solange das zeichen kein * ist weiter prüfen und den index mit hoch zählen
        while(x!= '*'){                                                 
            GPS.index_end++;
            x = GPS.message2[GPS.index_end];
        }
        
        //warum -2 erklärt sich mir nicht ganz 
        GPS.index_end -=2;    
        
        //Exclusiv ODER verknüpfung aller empfangenen Zeichen im Satz bildet die Checksumme
        for (int i=1; i<GPS.index_end;i++){
            GPS.checksum_calc=GPS.checksum_calc^GPS.message2[i];
        
        }
            printf("berechnete checksumme: 0x%x",GPS.checksum_calc);
            
        //Wenn die empfangene Chekcsumme = berechnette Checksumme sind die Daten Korrekt
            if(GPS.checksum_calc==GPS.checksum){
                GPS.valid = true;
                GPS.FIXTIME = (HAL_gettick());
            printf("\nZEIT UTC: %d:%d:%d.%d\n",GPS.hour,GPS.min,GPS.sec,GPS.millis);
            }
    }
    
    
}

int main()
{
    GPS.message="dddddddddddd$GGGGA *$GNGGA,151017.00,4216.72813,N,01127.64911,E,1,09,0.93,435.5,M,46.6,M,,*4A\r\nFerwdf";          //teststring mit gedaddel vorn und hinten
    GPS_p();
    long int test;
    test = HAL_gettick();                                                        //Hole Zeit von Systicktimer
    if(!((GPS.FIXTIME+1000) >test)){GPS.valid = false;}                          //Fix verfällt noch 1 sekunde
    
    printf("GPS aktiv: %d\n",GPS.valid);
    printf("%li %li",GPS.FIXTIME,test);
    
    return 0;
}
