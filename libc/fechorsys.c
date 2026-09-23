// copia de fechorsys de sara //

/*funcion que recupera la fecha y la hora del sistema */
/*la funcion localtime devuelve la fecha y la hora minuto segundo*/

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include "datagen.h"


void fechorsys(struct tm_dec *tmPtr) { 

    time_t tt; 
    struct tm *tm11;
    struct timeval tim;

    tt = time(NULL); 
    tm11=localtime(&tt); 

    tmPtr->tm_mon= 0;
    tmPtr->tm_mon= tm11->tm_mon+1;
    tmPtr->tm_mday= tm11->tm_mday;
    tmPtr->tm_hour= tm11->tm_hour;
    tmPtr->tm_min= tm11->tm_min;
    tmPtr->tm_sec= tm11->tm_sec;
    tmPtr->tm_year= tm11->tm_year+1900;

    //mmilisegundos
    gettimeofday(&tim, NULL);
    tmPtr->tm_mls = tim.tv_usec/1000;

//    printf("Date is %d/%02d/%02d\n", tmPtr->tm_year, tmPtr->tm_mon, tmPtr->tm_mday);
}

// suma a la fecha del sistema el número de días indicado en intDias. Si es positivo a futuro
// si es negativo en pasado.
void sumafecha(struct tm_dec *tmPtr, int intDias) { 

    time_t tt; 
    struct tm *tm11;
    struct timeval tim; 

    tt = time(NULL); 
    tm11=localtime(&tt);
	tt = tt + intDias * 24 * 60 * 60;
	tm11=localtime(&tt);

    tmPtr->tm_mon= 0;
    tmPtr->tm_mon= tm11->tm_mon+1;
    tmPtr->tm_mday= tm11->tm_mday;
    tmPtr->tm_hour= tm11->tm_hour;
    tmPtr->tm_min= tm11->tm_min;
    tmPtr->tm_sec= tm11->tm_sec;
    tmPtr->tm_year= tm11->tm_year+1900;

    //mmilisegundos
    gettimeofday(&tim, NULL); 
    tmPtr->tm_mls = tim.tv_usec/1000;

    //printf("sumafecha is %d/%02d/%02d\n", tmPtr->tm_year, tmPtr->tm_mon, tmPtr->tm_mday);
}
