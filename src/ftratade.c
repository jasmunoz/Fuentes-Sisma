//PRJ288711 - Tratamiento diferidos erróneos
#include <stdio.h>
#include "cnetplus.h"
#include "npterrd.h"
/***
   PROGRAMA: 		ftratade.c
   DESCRIPCION: Graba una operación diferida que se responde con error
   							en el servidor financiero

***/
char *itoa_ux(int valor);
void lpad(char *bufer,char c,int lon);
void fechorsys(struct tm_dec *tmPtr);
int funciongd(formato_interno *pforitn, char* accion)
{
/******************************************************************/ 
/*--->SE GRABA EN LA TABLA DE DIFERIDOS ERRONES, POR ERROR EN SF  */
/******************************************************************/
  NPSERRD regdier;
  tm_dec tmPtr;
  int tamano=0;


 	fechorsys(&tmPtr);
 
	strncpy(regdier.eraccion, accion, 1);
	regdier.eraccion[1] = '\0';
 	sprintf(regdier.ersubsis, "%.2s", pforitn->fidatpro.firespri);
 	regdier.ersubsis[2]= '\0';
 	sprintf(regdier.ersubapl, "%.2s", pforitn->fidatpro.fisubres); 
 	regdier.ersubapl[2]= '\0';
	//fprintLog(nomProg, "-- Variables ftratade.c ersubsis >%.2s< y ersubapl >%.2s<\n", regdier.ersubsis, regdier.ersubapl);

 	strncpy(regdier.ernumope, pforitn->fidatpro.finumope, 6);
 	regdier.ernumope[6]= '\0';
 	fprintLog(nomProg, "-- Variables ftratade.c pforitn->fiisocom.respusal >%.4s< \n", pforitn->fiisocom.respusal);
 	strncpy(regdier.ernumerr, pforitn->fiisocom.respusal, 4);
 	regdier.ernumerr[4]= '\0';
	//fprintLog(nomProg, "-- Variables ftratade.c ernumope >%.6s< y ernumerr >%.4s<\n", regdier.ernumope, regdier.ernumerr);

 	fprintLog(nomProg, "-- Variables ftratade.c cadena >%.2s%.2s%.6s%.4s< \n", regdier.eraccion, regdier.ersubsis, regdier.ersubapl, regdier.ernumope, regdier.ernumerr);
 	
  fprintLog(nomProg,"*** *** ftratade.c, NPTERRD - ESCRIBIR\n"); 
  retorno = tratar_fich("NPTERRD",ESCRIBIR,&regdier,NULL);  
  fprintLog(nomProg,"-- -- retorno de escribir en NPTERRD es >%d<\n",retorno); 

  return(retorno);
}
