   
#ifndef CFORPANT_H
#define CFORPANT_H
/***
   FICHERO: CFORPANT.H
   DESCRIPCION: Fichero con la estructura del formato mensaje para comunicacion con las pantallas del Visual Basic.
   Ahora mismo no se utiliza asi porque no hay pantallas
                
***/

#include "cnetplus.h"

typedef struct formato_MCD{
	 
  char codigosubsistema[2];
  char subaplicacion[2];
  char codigo[4];

	char codresp[2];   // respuesta
	char codMCD[2];    // funcion del mensaje 
						// 01 apertura de sesion
						// 02 Recuperacion   Hay que cargar los disponibles para enviar 
						// 03 Peticion interrupcion
						// 04 Peticion de reanudacion
						// 05 Cierre de sesion Hay que cargar los totales para enviar
						// 07 Informativo
 
	char codopc[1]; 	// 0 Aceptar operaciones      
						// 1 No aceptar operaciones
						// 2 test
	char canal[2];      //CANAL SIO

} formato_MCD;

typedef struct formato_exce {
   
	 
  char codigosubsistema[2];
  char subaplicacion[2];
  char codigo[4];

	char cod039[3];
    // PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de tarjeta
    char tarjeta[19];
	char codfun[3];   /* codigo de funcion */
	char codexc[3];   /* codigo de excepcion */
	char fchexh[12];  /* fecha y hora actualizacion EXCNEG */
	char codlni[1];   /* codigo en lista negra internacional */
	char fchlnh[12];  /* fecha y hora actualizacion LNI */
	char fechca[4];   /* fecha de caducidad */
	char idetit[15];  /* identificacion del titular Tipo y Numero */
	char idecon[4];   /* identificacion del comunicante */
#if 0
/***	char tipdoc[3];  //  tipo 
	char numdoc[12];   //numero 
	char datos[256];//creo que este campo me sobra***/
#endif

} formato_exce;

typedef struct formato_conex {
  
  char codigosubsistema[2];
  char subaplicacion[2];
  char codigo[4];
  char cod039[3];
  char comando[3];
  char caractedialogo[3];
  

 } formato_conex;

typedef struct formato_sesion {
	char codigosubsistema[2];
	char subaplicacion[2];
	char codigo[4];
	char cod039[3];
	char sesion[3];/* numero de sesion **/
	char fechsesion[6];/*fecha de sesion **/
} formato_sesion;

#endif
/* De CFORPANT_H */
