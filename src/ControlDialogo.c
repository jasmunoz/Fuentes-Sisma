
/*******************
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cnetplus.h"
#include "svm.h"
#include "procesos.h"
#include "nptpara.h"

NPSPARA  *reg_para;

/*********************
*********************/
int main(int argc, char *argv[])
{

int iOpcion = 0;
int iAplicacion = 0;
int iLong=0;
char NOMCOLA[30];
int idHj=0;
int iTimeOut = 0;
int retorno = 0;
int ll;

//ESTRUCTURA DE DATOS DE ENTRADA
typedef struct
{
   char ptsubsis[2];
   char ptsubapl[2];
   char pttipmsj[3];
   char caractedialogo[3];
}formtPant;
formtPant datEnt;

//ESTRUCTURA DE DATOS DE SALIDA
typedef struct
   {  char subsistema[2];
      char subaplicacion[2];
      char codigo[4];
      char cod039[3];
   }formtPantS;
formtPantS datSal;

char MsgEntrada[11];
char MsgSalida[4096];


int  damecola(SVM *psvm_par,char *proce,char *subsis);
int EscribirCola_c(char *Nombcola, char* Msj, int LonMsj, int* idHj, int tiempo);
int leerCola_c(char *Nombcola, char* Msj, int *LonMsj, int idHj, int tiempo);

    //RAQUEL NO SE UTILIZA PERO LO DEJAMOS POR SI SE QUIERE MONITORIZAR
    printf("Control de Diálogo con Redsys. Seleccionar opción\n");
    printf("1-Test\n");
    printf("2-Sign on\n");
    printf("3-Sign off\n");	
	
    scanf("%d", &iOpcion); 
	
	if(iOpcion != 1 && iOpcion != 2 && iOpcion != 3)
	{
		printf("Seleccionar una de las opciones validas.\n");
		return(0);
	}
		
	printf("Seleccionar Aplicación de Redsys\n");
    printf("2-Aplicación Resolutor AR-Operaciones de Tarjeta\n");
    printf("4-Aplicación Recepción de Ficheros AF-Comunicación de Bloqueos\n");
	
	scanf("%d", &iAplicacion); 

	if(iAplicacion != 2 && iAplicacion != 4)
	{
		printf("Seleccionar una de las opciones validas.\n");
		return(0);
	}
		
   for (ll=0 ; ll<2 ; ll++) 
   {   
     if (strncmp(datEnt.ptsubsis,reg_para[ll].psnument,2)==0)
     {		 
			strncpy(datEnt.ptsubsis, reg_para[ll].psnument, 2);
			break;
	 }
   }	 
	
   sprintf(datEnt.ptsubapl, "0%d", iAplicacion);
   
   if(iOpcion == 1)
      strncpy(datEnt.pttipmsj, "TST",3);
   if(iOpcion == 2)
      strncpy(datEnt.pttipmsj, "SNN",3);
   if(iOpcion == 3)
      strncpy(datEnt.pttipmsj, "SNF",3);
  
   strncpy(datEnt.caractedialogo,"TOT",3);
   
   iLong = 10;
   iTimeOut = 5;
   
   strncpy(MsgEntrada, datEnt.ptsubsis, 10);
   MsgEntrada[10]='\0';
   
   retorno = EscribirCola_c(PRECEP01, MsgEntrada, iLong, &idHj, iTimeOut);
    if (retorno)
    {
	   printf("error al escribir en la cola %d\n", retorno);   
	   return(0);
    }		
	else
	{
		printf("Enviado esperando respuesta\n");  
	}
    	
	iLong = 11;
	retorno = leerCola_c(PENVIO01, MsgSalida, &iLong, getpid(), iTimeOut);
    if (retorno)
    {
	   printf("error al leer de la cola %d\n", retorno);  
	   return(0);
    }		
    //printf("Leido >%.11s<\n", MsgSalida);  
	
	memcpy(&datSal, MsgSalida, 11);
	
	if(strncmp(datSal.codigo,"1814",4)==0
	&& strncmp(datSal.cod039,"800",3)==0)  
	    printf(">%.3s< CORRECTO\n", datSal.cod039);
	else
		printf(">%.3s< ERROR\n", datSal.cod039);
				   
	return(0); 
		
}//fin main
