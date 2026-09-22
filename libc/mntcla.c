
/***************************************************************************
   FUNCION   FMONTA_CLAVES  
   DESCRIPCION: Pone las claves de operacion que busca luego en NPTTBOP
   para saber hacia donde encaminar el mensaje
***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <time.h>  
#include <errno.h>
#include <signal.h>
#include <sys/ipc.h>  
#include <sys/shm.h>
#include <sys/msg.h>
#include "cforitno.h"
#include "nptpara.h"

NPSPARA * reg_para;
int fmonta_claves (formato_interno *pforitn)
{
int tiponum,codinum;
tiponum = atoi(pforitn->fiisocom.fiiso000); 
int codigo_operacion = atoi(pforitn->fidatiso.fiisoaut.fiaut003.dig12);

strncpy(pforitn->fidatpro.ficlatab + 2 , "00",2);
strncpy(pforitn->fidatpro.ficlatab + 4 , "00",2);
strncpy(pforitn->fidatpro.ficlatab + 6 , "00",2);
strncpy(pforitn->fidatpro.ficlatab + 8 , "00",2);
strncpy(pforitn->fidatpro.ficlatab +10 , pforitn->fidatpro.fiorigen, 2);

    /*      ficlatab + 0 = tipo de mensaje: 11 peticiones
                                        12 diferidos
                                        13 excepciones
                                        14 anulaciones
																				15 conciliacion
                                        16 incidencias
                                        18 mensajes de control

    ficlatab + 2 = tipo de operacion:   00 cargos
                                        20 abonos
                                        30 consultas saldo/movimientos
																				44 conciliacion terminales en captura
                                        51 mensajes gema oro
                                        91 uso privado
										
    ficlatab + 4 = resolutor (sólo en operaciones sin tarjeta, resto no usado).
    ficlatab + 6 = 20 primera vez
                   21 repeticiones
                   00 no usado
    ficlatab + 8 = estado de la operacion   00 peticion
                                            50 respuesta
                                            41 Time-Out
                                            40 Semaforo cerrado
                                            42 Emisor no responde
    ficlatab + 10 = origen de la operacion */

//DMND0036176 revisado que estan todas las preautorizaciones montadas

switch(tiponum)
{
case 1200:
case 1100:
   strncpy(pforitn->fidatpro.ficlatab     , "11",2);
   switch(codigo_operacion)
     {
   /* Solicitud de informacion */
      case 30:             /* consulta de saldos */
      case 31:             /* consulta de movimientos */
       strncpy(pforitn->fidatpro.ficlatab + 2 , "30",2);
       break;
   /* Uso privado */
      case 90:
      case 91:
      case 92:
      case 93:
      case 94:
      case 97:
       strncpy(pforitn->fidatpro.ficlatab + 2 , "90",2);
       break;
     }
     break;

case 1220:
case 1120:
   strncpy(pforitn->fidatpro.ficlatab     , "12",2);
   switch(codigo_operacion)
     {
   /* Solicitud de informacion */
      case 30:             /* consulta de saldos */
      case 31:             /* consulta de movimientos */
       strncpy(pforitn->fidatpro.ficlatab + 2 , "30",2);
       break;
   /* Uso privado */
      case 90:
      case 91:
      case 92:
      case 93:
      case 94:
      case 97:
       strncpy(pforitn->fidatpro.ficlatab + 2 , "90",2); 
       break;
     }
     strncpy(pforitn->fidatpro.ficlatab + 6 , "20",2);
     break;

case 1221:
case 1121:
   strncpy(pforitn->fidatpro.ficlatab     , "12",2);
   switch(codigo_operacion)
     {
   /* Solicitud de informacion */
      case 30:             /* consulta de saldos */
      case 31:             /* consulta de movimientos */
       strncpy(pforitn->fidatpro.ficlatab + 2 , "30",2);
       break;
   /* Uso privado */
      case 90:
      case 91:
      case 92:
      case 93:
      case 94:
      case 97:
       strncpy(pforitn->fidatpro.ficlatab + 2 , "90",2);
       break;
     }
     strncpy(pforitn->fidatpro.ficlatab + 6 , "21",2);
     break;

// comunicación de operaciones en terminales en captura (price adquirente)
// el resolutor es la entidad adquirente, no depende del bin de la tarjeta
//  // PRJ0202598 cambio bin de 6 posiciones 8 no afecta
case 1224:
case 1124:
   strncpy(pforitn->fidatpro.ficlatab     , "12",2); // diferidos
 // if ((strncmp(pforitn->fidatpro.fiorigen,"20",2)==0))
//	    strncpy(pforitn->fidatpro.ficlatab + 4 , "05",2); // resolutor: se envia al resolutor 05 para notificar el cierre 
 // else
 //     strncpy(pforitn->fidatpro.ficlatab + 4 , "03",2); // resolutor: se envia a la entidad adquirente (03)	     
    
   
   if ((strncmp(reg_para[0].psnument,pforitn->fidatpro.fiorigen,2)==0))
      strncpy(pforitn->fidatpro.ficlatab + 4 , reg_para[0].psresolu,2);	  
   else 
	  strncpy(pforitn->fidatpro.ficlatab + 4 , "03",2);
   strncpy(pforitn->fidatpro.ficlatab + 6 , "20",2); // comunicación
   break;

// repeticiones de comunicación de operaciones en terminales en captura (price adquirente)
// el resolutor es la entidad adquirente, no depende del bin de la tarjeta
 // PRJ0202598 cambio bin de 6 posiciones 8 no afecta
case 1225:
case 1125:
   strncpy(pforitn->fidatpro.ficlatab     , "12",2); // diferidos
   
   if ((strncmp(reg_para[0].psnument,pforitn->fidatpro.fiorigen,2)==0))
      strncpy(pforitn->fidatpro.ficlatab + 4 , reg_para[0].psresolu,2);	  
   else 
	  strncpy(pforitn->fidatpro.ficlatab + 4 , "03",2);    
   strncpy(pforitn->fidatpro.ficlatab + 6 , "21",2); // repetición de comunicación
   break;
	 
	 
case 1304:
case 1305:
  strncpy(pforitn->fidatpro.ficlatab     , "13",2);
  break;
case 1324:
  strncpy(pforitn->fidatpro.ficlatab     , "13",2);
  //fprintf(fichsal, "mensaje 1324 \n");
  strncpy(pforitn->fidatpro.ficlatab + 6 , "24",2);
  break;
case 1325:
  strncpy(pforitn->fidatpro.ficlatab     , "13",2);
  strncpy(pforitn->fidatpro.ficlatab + 6 , "25",2);
  break;

case 1420:
     strncpy(pforitn->fidatpro.ficlatab     , "14",2);
     strncpy(pforitn->fidatpro.ficlatab + 6 , "20",2);
   break;
case 1421:
     strncpy(pforitn->fidatpro.ficlatab     , "14",2);
     strncpy(pforitn->fidatpro.ficlatab + 6 , "21",2);
   break;
   
// anulación de comunicación de operaciones en terminales en captura (price adquirente)
// el resolutor es la entidad adquirente, no depende del bin de la tarjeta
// PRJ0202598 cambio bin de 6 posiciones 8 no afecta
case 1424:
     strncpy(pforitn->fidatpro.ficlatab     , "14",2); // anulación
   
   if ((strncmp(reg_para[0].psnument,pforitn->fidatpro.fiorigen,2)==0))
      strncpy(pforitn->fidatpro.ficlatab + 4 , reg_para[0].psresolu,2);	  
   else 
	  strncpy(pforitn->fidatpro.ficlatab + 4 , "03",2);
  strncpy(pforitn->fidatpro.ficlatab + 6 , "20",2); // comunicacion
   break;
case 1425:
     strncpy(pforitn->fidatpro.ficlatab     , "14",2); // anulación
   
   if ((strncmp(reg_para[0].psnument,pforitn->fidatpro.fiorigen,2)==0))
      strncpy(pforitn->fidatpro.ficlatab + 4 , reg_para[0].psresolu,2);	  
   else 
	  strncpy(pforitn->fidatpro.ficlatab + 4 , "03",2);	 
     strncpy(pforitn->fidatpro.ficlatab + 6 , "21",2); // repetición de comunicación
   break;
   
case 1524:
  strncpy(pforitn->fidatpro.ficlatab     , "15",2);
   
   if ((strncmp(reg_para[0].psnument,pforitn->fidatpro.fiorigen,2)==0))
      strncpy(pforitn->fidatpro.ficlatab + 4 , reg_para[0].psresolu,2);	  
   else 
	  strncpy(pforitn->fidatpro.ficlatab + 4 , "03",2);

case 1525:
  strncpy(pforitn->fidatpro.ficlatab     , "15",2);
   
   if ((strncmp(reg_para[0].psnument,pforitn->fidatpro.fiorigen,2)==0))
      strncpy(pforitn->fidatpro.ficlatab + 4 , reg_para[0].psresolu,2);	  
   else 
	  strncpy(pforitn->fidatpro.ficlatab + 4 , "03",2);
  	strncpy(pforitn->fidatpro.ficlatab + 6 , "25",2); // repeticion
  break;

case 1544:
  strncpy(pforitn->fidatpro.ficlatab     , "15",2);
  strncpy(pforitn->fidatpro.ficlatab + 2 , "44",2); //conciliacion de terminales en captura
  break;

case 1644:
   strncpy(pforitn->fidatpro.ficlatab     , "16",2);
   break;

case 1804:
case 1805: //PRJ0258542
case 1814: //PRJ0258542
   strncpy(pforitn->fidatpro.ficlatab     , "18",2);
   break;
   
case 1824:
case 1825: //PRJ0258542
   strncpy(pforitn->fidatpro.ficlatab     , "18",2);
   strncpy(pforitn->fidatpro.ficlatab + 6 , "24",2);
   break;
   
}

return (0);
}
/***************************************************************/
