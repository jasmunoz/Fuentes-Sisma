#include <stdio.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include "datagen.h"
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h> 
#include <errno.h>

int nspaces (char *c,int n);

//Funcion para recuperar la fecha y hora del sistema
void fechorsys(struct tm_dec *tmPtr);

//Funcion para la escritura de trazas
void fprintLog(char ficheroPlantilla[50], char* format, ...){

//Declaracion de variables
    static char fileNameOld[300]; //hay que definirla estatica para que guarde el valor en la siguiente llamada
    char fichero[300]="";
    char fichaux[300]="";
    char fichaux1[300]="";
    char fichaux2[300]="";
    char fichaux1time[300]="";
    char fichaux2time[300]="";
    int p=0;
    va_list ap;
    char msg[1000]="";
    char fechaAct[20];
    static char fechaOld[20];
    tm_dec tm;
    mode_t oldmode;
    int pid;
    char comando[200];

// incluido para recuperar la variable de entorno y crear las trazas en su entorno correspondiente
// segun el usuario que arranque

  char* HOME;
  HOME = getenv ("SF_HOME");


//coger fecha del sistema
    fechorsys(&tm);

//formateo fecha
    sprintf(msg,"%02d/%02d/%d %02d:%02d:%02d:%03d ",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
    sprintf(fechaAct,"_%02d%02d%d_%02d",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour);

// Montamos el nombre del fichero segun la fecha actual
   sprintf(fichero, "%s%s/%s_%s_1.traza",HOME,DIRTRAZA, ficheroPlantilla, fechaAct); //traza con fecha-hora
   sprintf(fichaux, "%s%s/%s",HOME,DIRTRAZA, ficheroPlantilla); //traza sin fecha-hora

    if (ficheroPlantilla[0] == '\0')
    {
       pid=getpid();
       fprintf(stdout,"entramos en ficheroPlantilla vacio %s  pid %i\n",ficheroPlantilla, pid);
       fflush(stdout);
    }
  
// /////////// SOLO ABRE SI CAMBIA LA FECHA Y HORA /////////////////   && (fileNameOld[0] != '\0')
    if(strncmp(fileNameOld, fichero, strlen(fichero))!=0 )
    { //// si ha cambiado o es nuevo

       if (fileNameOld[0] != 0) //si NO es la primero vez, la primera vez no se tiene que mover nada
       {
          sprintf(fichaux1, "%s%s/%s_1.traza",HOME,DIRTRAZA, ficheroPlantilla);
          sprintf(fichaux1time, "%s%s/%s_%s_1.traza",HOME,DIRTRAZA, ficheroPlantilla, fechaOld);
          rename(fichaux1, fichaux1time);       

          sprintf(fichaux2, "%s%s/%s_2.traza",HOME,DIRTRAZA, ficheroPlantilla);
          sprintf(fichaux2time, "%s%s/%s_%s_2.traza",HOME,DIRTRAZA, ficheroPlantilla, fechaOld); 
          rename(fichaux2, fichaux2time);       
       }
       strcpy(fileNameOld,fichero); //Guardamos el nombre y la fecha de cuando cambiamos
       strncpy(fechaOld, fechaAct ,20);
       
       oldmode = umask(00000);

       sprintf(fichaux, "%s%s/%s_2.traza",HOME,DIRTRAZA, ficheroPlantilla); ///ficheroaux
       freopen(fichaux,"a",stderr); ///ficheroaux
       sprintf(fichaux, "%s%s/%s_1.traza",HOME,DIRTRAZA, ficheroPlantilla); ///ficheroaux
       freopen(fichaux,"a",stdout); ///ficheroaux
       umask(oldmode);
    }
		 
// Establecemos la lista variable de argumentos 
        va_start(ap, format); 
// Concatenamos al inicio del mensaje, la cadena de formato recibida
       strcat(msg, format);
// Escribimos el mensaje en el fichero
       vfprintf(stdout, msg, ap);
       fflush(stdout);

}
