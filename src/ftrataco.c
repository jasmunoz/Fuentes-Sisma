//PRJ0257005 - Revisado tokenizacion
#include <stdio.h>
#include "cnetplus.h" 
#include "nptdife.h"
#include "nptpara.h"
/***
   PROGRAMA: CORRECTORA.C
   DESCRIPCION: Genera correctora , Función que se llama desde rtratami
   para generar anulaciones de peticiones que no han sido
   contestadas a tiempo.

***/

/**********************************************/
//int fgraba_diferidos(formato_interno *pforitn);
int fgradife(formato_interno * pforitn);

NPSPARA  *reg_para;

int funcionco(formato_interno *pforitn)
{
  NPSPARS      regpars;

  int err=0;
  int posisubs=0;
  char clave[7];
  char auxsub[4];

  formato_interno *pforitn_corr, miformato;

#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"***************\n");
  fprintLog(nomProg,"Inicio FTRATACO\n");
  fprintLog(nomProg,"Principio CORRECTORA\n");
  fprintLog(nomProg,"***************\n");
#endif

  pforitn_corr=&miformato;
  memcpy(pforitn_corr,pforitn,sizeof(formato_interno));

#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"fiorigen >%.2s<\n", pforitn_corr->fidatpro.fiorigen);
#endif

  if ((!strncmp(pforitn->fiisocom.fiiso000, "0100", 4)  ||
       !strncmp(pforitn->fiisocom.fiiso000, "0200", 4)  ||
       !strncmp(pforitn->fiisocom.fiiso000, "1100", 4)  ||
       !strncmp(pforitn->fiisocom.fiiso000, "1200", 4)))
 {
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"Cumple condicion CO\n");
#endif
  err=fgenera_correctora(pforitn_corr);

  if (err)
  {
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"error al generar correctora\n");
#endif
   strcpy(coderror, "262");
   return(finalizar(98));
  }
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"fiorigen >%s<\n", pforitn_corr->fidatpro.fiorigen);
#endif
  err=fgraba_log(pforitn_corr);
  if (err != 0)
  {
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"error al grabar log >%d<\n", err);
#endif
   strcpy(coderror, "222");
   return(finalizar(98));
  }


/***   cierra el semaforo con el resolutor   en B. A. no esta pero lo dejo de momento***/
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"firespri >%2.s< \n", pforitn->fidatpro.firespri);
  fprintLog(nomProg,"firesfin >%3.s< \n", pforitn->fidatpro.firesfin);
  fprintLog(nomProg,"ficodfin >%3.s< \n", pforitn->fidatpro.ficodfin);
  fprintLog(nomProg,"fisubapl >%2.s< \n", pforitn->fidatpro.fisubres);
#endif

  if(!strncmp(pforitn->fidatpro.ficodfin, "297", 3) && //emisor no responde
     !strncmp(pforitn->fidatpro.firesfin, "00", 2))
  {
                    /*     leer regpars en nptpars   */
   strncpy(regpars.pasubsis,pforitn->fidatpro.firespri,2);

   strncpy(regpars.pasubapl, pforitn->fidatpro.fisubres, 2);

#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"err leyendo subsistema >%d< \n", err);
#endif

/*** cuando no hay respuesta siempre debe pasar a desconectado ****/
   strncpy(clave, regpars.pasubsis, 2);
   strncpy((clave + 2), regpars.pasubapl, 2);

   strncpy(clave+4, regpars.pasemaf1,1 );
   strncpy(clave+5, regpars.pasemaf2,1 );
   clave[6] = '\0';

   err = tratar_fich("NPTPARS", SEMAF00, &regpars, clave);
/*** A continuación se modifica el valor del semáforo del subsistema
      en svm  ***/
   auxsub[4] = '\0';
   memcpy(auxsub, pforitn->fidatpro.firespri, 2);
   memcpy(auxsub + 2, pforitn->fidatpro.fisubres, 2);
   posisubs= encuentra_svm(psvm, auxsub, TIP_SUBSIS);

   if (posisubs < 0)
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg,"ERROR: no encuentra subsistema=%s\n", auxsub);
#endif
   else
    (psvm+posisubs)->str_svm.svmpars.pasemsub=(regpars.pasemaf1[0]-'0');

  }
/***   cierra el semaforo con el resolutor  ***/
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"a fgraba_diferidos\n");
#endif

  //20160701-unificamos la grabacion de diferidos en una función solo
  //err=fgraba_diferidos(pforitn_corr);
  err=fgradife(pforitn_corr);

  if (err )
  {
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"error al grabar diferido err->%d<-\n", err);
#endif

   strcpy(coderror, "203");
   return(finalizar(98));
  }
 }

 return(finalizar(11));
}

/************************************************************************
*      Funcion genera_correctora                                        *
************************************************************************/

int fgenera_correctora(formato_interno *pforitn)

{
  int err=0,ll;
  char numope[7];
  char numcross[11];
  tm_dec  tt;
  int j;

  
  pforitn->fidatpro.fiorigen[2]='\0';
  num_origen = atoi(pforitn->fidatpro.fiorigen);
  
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"Generar correctora origen >%d<\n", num_origen);
#endif
 
 //grabo el numero de operacion de la previa, que es el de la operacion que genera la correctora
 strncpy(pforitn->fidatpro.finumprv, pforitn->fidatpro.finumope,6);
 pforitn->fidatpro.finumprv[6]='\0';
 

//RAQUEL cambiado para recoger los datos de bbdd por entidad
   for (j=0 ; j<2 ; j++) //PRJ0258542 - cambio ll++ por j++
   {
     if (strncmp(pforitn->fidatpro.fiorigen,reg_para[j].psnument,2)==0)
     {   
       fprintLog(nomProg, "origen PRICE\n");
       
       strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos, pforitn->fiisocom.fiiso000, 4);
       strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+4, pforitn->fiisocom.fiidtran,6);
       strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+10, pforitn->fiisocom.fifechor.aa, 2);
       strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+12, pforitn->fiisocom.fifechor.mm, 2);
       strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+14, pforitn->fiisocom.fifechor.dd, 2);
       strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+16, pforitn->fiisocom.fifechor.hh, 2);
       strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+18, pforitn->fiisocom.fifechor.mi ,2);
       strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+20, pforitn->fiisocom.fifechor.ss ,2);
       strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+22, pforitn->fiisocom.fi032094.longitud,2);
       strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+24, pforitn->fiisocom.fi032094.datos, 11);
       for (ll=24; ll < 35; ll++)
        if (pforitn->fidatiso.fiisoaut.fiaut056.datos[ll] == ' ')
         break;
       sprintf(pforitn->fidatiso.fiisoaut.fiaut056.longitud,"%02d", ll);
     
       strcpy(pforitn->fiisocom.fiiso000, "1420");
       strcpy(pforitn->fiisocom.fiiso024,"400");
       strcpy(pforitn->fidatiso.fiisoaut.fiaut025,"4006");  // motivo de la correctora: temporizador
       
       #if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"iso000 >%.4s<\n", pforitn->fiisocom.fiiso000);
       fprintLog(nomProg,"iso056 >%.35s<\n", pforitn->fidatiso.fiisoaut.fiaut056.datos);
       #endif
     break;
    }

   }
 pforitn->fidatiso.fiisoaut.fiaut056.datos[35]='\0';
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"iso000 %.4s\n", pforitn->fiisocom.fiiso000);
  fprintLog(nomProg,"iso056 %.35s\n", pforitn->fidatiso.fiisoaut.fiaut056.datos);
  fprintLog(nomProg,"finumprv %d\n", pforitn->fidatpro.finumprv);
#endif

 strcpy(pforitn->fidatpro.ficodfin, "   ");
 strcpy(pforitn->fidatpro.firesfin, "  ");
 strcpy(pforitn->fidatpro.fiorigen, "00");
 strcpy(pforitn->fiisocom.respusal , "   ");

/* Obtencion de la fecha y hora del sistema */

   fechorsys(&tt);

 sprintf(pforitn->fidatpro.fifecha0   , "%4d",  tt.tm_year);
 sprintf(pforitn->fidatpro.fifecha0+4 , "%02d", tt.tm_mon);
 sprintf(pforitn->fidatpro.fifecha0+6 , "%02d", tt.tm_mday);
 sprintf(pforitn->fidatpro.fihora00   , "%02d", tt.tm_hour);
 sprintf(pforitn->fidatpro.fihora00+2 , "%02d", tt.tm_min);
 sprintf(pforitn->fidatpro.fihora00+4 , "%02d", tt.tm_sec);
 strncpy(pforitn->fidatpro.fihora00+6 , "00", 2);

 strncpy(pforitn->fiisocom.fifechor.aa,pforitn->fidatpro.fifecha0 +2,2);
 sprintf(pforitn->fiisocom.fifechor.mm, "%02d", tt.tm_mon);
 sprintf(pforitn->fiisocom.fifechor.dd ,"%02d", tt.tm_mday);
 sprintf(pforitn->fiisocom.fifechor.hh, "%02d", tt.tm_hour);
 sprintf(pforitn->fiisocom.fifechor.mi, "%02d", tt.tm_min);
 sprintf(pforitn->fiisocom.fifechor.ss, "%02d", tt.tm_sec);

// PRJ0202598 cambio bin de 6 posiciones 8  no afecta al tamañao total del finumPAN
/* Se obtiene un nuevo numero de operacion y se asigna */
 err=fnumope(numope,numcross, pforitn->fidatiso.fiisoaut.finumPAN.datos[15]);
 if(err)
 {
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"ERROR AL FNUMOPE \n");
#endif

  strcpy(coderror, "264");
  return(err);
 }
 else
 {

  strncpy(pforitn->fidatpro.finumope, numope,6);
  pforitn->fidatpro.finumope[6] ='\0';
  strncpy(pforitn->fiisocom.fiidtran, numope,6);
  pforitn->fiisocom.fiidtran[6] ='\0';
  numcross[10]='\0';
 }

 return(err);
}

