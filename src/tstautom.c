
#include "nptpars.h"

int llena_tst_comun(char * subapl,formato_interno * fiforitn);
int completa_tst(formato_interno * fiforitn);
int abre_semaforo(char * subapl,int opcion, char* claveU);
int fnumope (char *numope, char * numcross, char Indice);
int inicfori (int tipomsg, formato_interno *pfi);
int fverfi(formato_interno fi);

/*************************************************************************
   funcion de tratamiento de tst
**************************************************************************/
int trata_tst(char * subsistema_general,char * subapl)
{

  int retorno, respondido;
  char subsis[5];

  formato_interno   fiforitn;
	fprintLog(nomProg,"------- Entrada tstautom.c en trata_tst -------- Valor de retorno <%d> y respondido <%d>\n", retorno, respondido); //PRJ0258542
  strncpy(subsis, subsistema_general ,2);
  strcpy(subsis + 2, subapl);
  

  respondido = 0;
  retorno = 0;
  retorno = llena_tst_comun(subapl,&fiforitn);
  fprintLog(nomProg,"Valor de retorno 1<%d> y respondido <%d>\n", retorno, respondido); //PRJ0258542
  if(retorno)
   return(retorno);

	fprintLog(nomProg,"Valor de retorno 2<%d> y respondido <%d>\n", retorno, respondido); //PRJ0258542
	while(respondido != 1 && (psvm+rtrata_svm)->estado != PARADA)
  {
  	retorno = completa_tst(&fiforitn);
  
#if defined (TRAZA) && (TRAZA > 1)
    fprintLog(nomProg,"envio tst automaticos *** TSTAUTOM *** %s\n", subsis);
#endif

    fverfi(fiforitn);

    retorno=envia_diferido(&fiforitn);
    fprintLog(nomProg,"Valor de retorno 3<%d> y respondido <%d>\n", retorno, respondido); //PRJ0258542
    if(!retorno)   //respondido
    {
    	//if(!strcmp(fiforitn.fidatpro.ficodfin,"290")) //ESTADO SUSPENDIDO
			// si se contesta pero no aceptado, ponemos sleep para no mandar test tan a menudo
			if(strcmp(fiforitn.fidatpro.ficodfin,"000") !=0 )
      {
      	fprintLog(nomProg,"NO SE ACEPTA EL TEST, vamos al sleep 5 segundos\n");
       	sleep(5);
		  	fprintLog(nomProg,"despierta de sleep 5 segundos\n");
       	retorno = llena_tst_comun(subapl,&fiforitn);
       	fprintLog(nomProg,"Valor de retorno 4<%d> y respondido <%d>\n", retorno, respondido); //PRJ0258542
       	if(retorno)
       		return(retorno);
      }
      else
      {
      	//no abrimos el semaforo pq el test ya lo abre en el tratami en la funcion CH
        //retorno=abre_semaforo(subapl,1,subsis );
#if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg,"RESPONDIDO A TEST  *** TSTAUTOM *** %s\n", subsis);
#endif
        respondido = 1;
        fprintLog(nomProg,"Valor de retorno 5<%d> y respondido <%d>\n", retorno, respondido); //PRJ0258542
   		}
   	}
	 	else //no respondido
    {
#if defined (TRAZA) && (TRAZA > 1)
    	fprintLog(nomProg,"NO RESPONDIDO TEST  *** TSTAUTOM *** %s\n", subsis);
			fprintLog(nomProg,"subsis_svm %d\n", subsis_svm);
#endif
      if((psvm+subsis_svm)->str_svm.svmpars.pasemsub == 1)
      {
#if defined (TRAZA) && (TRAZA > 1)
      	fprintLog(nomProg,"Semaforo abierto %s\n", subsis);
#endif
        respondido = 1;
        fprintLog(nomProg,"Valor de retorno 6<%d> y respondido <%d>\n", retorno, respondido); //PRJ0258542
     	}
#if defined (TRAZA) && (TRAZA > 1)
      else
      	fprintLog(nomProg,"Semaforo cerrado %s\n", subsis);
#endif
 		}
 	}/* fin While */
	fprintLog(nomProg,"Valor de retorno 7<%d> y respondido <%d>\n", retorno, respondido); //PRJ0258542
 	return (respondido);
	fprintLog(nomProg,"------- Salida tstautom.c en trata_tst -------- Valor de retorno <%d> y respondido <%d>\n", retorno, respondido); //PRJ0258542
}


/* **************************************** */
int completa_tst(formato_interno * fiforitn)

{
  char    numoper[6];
  int     retorno;
  struct  tm_dec tm;
  char    p012[5];
  char    numcross[11];
  char    Indiceaux;
  char    auxHora[9];

 Indiceaux = '0';
 retorno = 0;
 retorno = fnumope(numoper,numcross,Indiceaux);
 if(retorno)
  retorno = 1;
 else
 {
  strncpy(fiforitn->fidatpro.finumope, numoper,6);
  fiforitn->fidatpro.finumope[6] ='\0';

  fechorsys(&tm);
     
  //sprintf(fiforitn->fidatpro.fifecha0, "%d", tm.tm_siglo);
  
  sprintf(fiforitn->fidatpro.fifecha0, "%04d", tm.tm_year);
  sprintf(fiforitn->fidatpro.fifecha0 + 4, "%02d", tm.tm_mon);
  sprintf(fiforitn->fidatpro.fifecha0 + 6, "%02d", tm.tm_mday);
  
  sprintf(auxHora, "%02d", tm.tm_hour);
  sprintf(auxHora + 2, "%02d", tm.tm_min);
  sprintf(auxHora + 4, "%02d", tm.tm_sec);
  sprintf(auxHora + 6, "%02d", tm.tm_mls/10); //solo guardamos centesimas de segundo
  strncpy(fiforitn->fidatpro.fihora00, auxHora,8);
   
/****    ISO comunes   ****/
  strncpy(fiforitn->fiisocom.fiidtran, numoper, 6);
  fiforitn->fiisocom.fiidtran[6] = '\0'; 

  sprintf(p012, "%04d", tm.tm_year);
  strcpy(fiforitn->fiisocom.fifechor.aa, p012+2);
  sprintf(p012, "%02d", tm.tm_mon);
  strcpy(fiforitn->fiisocom.fifechor.mm, p012);
  sprintf(p012, "%02d", tm.tm_mday);
  strcpy(fiforitn->fiisocom.fifechor.dd, p012);
  sprintf(p012, "%02d", tm.tm_hour);
  strcpy(fiforitn->fiisocom.fifechor.hh, p012);
  sprintf(p012, "%02d", tm.tm_min);
  strcpy(fiforitn->fiisocom.fifechor.mi, p012);
  sprintf(p012, "%02d", tm.tm_sec);
  strcpy(fiforitn->fiisocom.fifechor.ss, p012);
 }

 return(retorno); 
}


/* **************************************** */
int llena_tst_comun(char * subapl,formato_interno * fiforitn)

{
  int     retorno;
  NPSPARS registro;
  char claveT[5];

  retorno = 0;
  retorno = inicfori(1804,fiforitn);
  if(retorno)
  return(retorno);

/****    datos procesales   ****/

 strcpy(fiforitn->fidatpro.fiorigen, "00");
 strcpy(fiforitn->fidatpro.firespri, subsistema_general);
 strcpy(fiforitn->fidatpro.firesfin, "  ");
 strcpy(fiforitn->fidatpro.ficodfin, "   ");

 sprintf(fiforitn->fidatpro.fipidori, "%d", getpid());
 
 strncpy(fiforitn->fidatpro.fisubres, subapl,2);
 fiforitn->fidatpro.fisubres[2]='\0';

/****    ISO comunes   ****/

 strcpy(fiforitn->fiisocom.fiiso000, "1804");

/*     leer registro en nptpars   */
 strncpy(claveT, subsistema_general,  2);
 strncpy(claveT + 2, subapl, 2);
 claveT[4]='\0';
 
 retorno = tratar_fich("NPTPARS",IGUAL,&registro, claveT);
 if(retorno)
  return(retorno); 

 strncpy(fiforitn->fidatiso.fiisoctr.fictr093.longitud, registro.pacidesti,2);
 fiforitn->fidatiso.fiisoctr.fictr093.longitud[2] = '\0';
 strncpy(fiforitn->fidatiso.fiisoctr.fictr093.datos, registro.pacidesti + 2,11); 
 fiforitn->fidatiso.fiisoctr.fictr093.datos[11] = '\0';
 strncpy(fiforitn->fidatiso.fiisoctr.fictr094.longitud, registro.paciadqui,2); //PRJ0258542
 fiforitn->fidatiso.fiisoctr.fictr094.longitud[2] = '\0'; //PRJ0258542
 strncpy(fiforitn->fidatiso.fiisoctr.fictr094.datos, registro.paciadqui + 2,11); //PRJ0258542
 fiforitn->fidatiso.fiisoctr.fictr094.datos[11] = '\0'; //PRJ0258542

 strncpy(fiforitn->fiisocom.fi032094.longitud, registro.paciadqui,2);
 fiforitn->fiisocom.fi032094.longitud[2] = '\0';
 strncpy(fiforitn->fiisocom.fi032094.datos, registro.paciadqui + 2,11); 
 fiforitn->fiisocom.fi032094.datos[11] = '\0';
 
 //// trazas marga
 fprintLog(nomProg,"pacidesti>%.13s< paciadqui>%.13s<\n", registro.pacidesti, registro.paciadqui);

 strcpy(fiforitn->fiisocom.fiiso024, "831"); 
 
 strcpy(fiforitn->fiisocom.respusal, "   ");
 
 return(0);
}

