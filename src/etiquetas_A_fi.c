#include <stdio.h>
#include "cnetplus.h" 
#include "cforitno.h"

#include "traeetiq.h"

#include "etiqEMV.h"
#include "max_min.h"


#define TTAG      (str_etiq->etiqueta[nn])

void ltrim(char *bufer,char c);
int fexplotabit(int opciex, char *pbyte_exp, char *pbyte_com);
int fdesepin(int longi, char *sempaq, char *sdesem);

void fetiquetas_A_fi(formato_interno * pfi, petiquetas * str_etiq)
{
  int nn, retorno;
  int nn_old;
#define L_SzAux  64
  char SzAux[L_SzAux];
 
  
#if defined (TRAZA) && (TRAZA > 10) 
 //fprintf(stdout, "dentro de fetiquetas_A_fi\n");
 //fprintf(stdout, "n_etiquetas >%d<\n", str_etiq->n_etiquetas);
 //fflush(stdout);
#endif


 nn=0;
 nn_old = -1;
 while(nn < str_etiq->n_etiquetas)
 {   
   if(nn==nn_old)  /** Ha llegado al final sin reconocer la etiqueta ***/
      nn++;
   nn_old=nn;

#if defined (TRAZA) && (TRAZA > 10) 
   //fprintf(stdout, "ETIQUETA >%.4s<\n", str_etiq->etiqueta[nn].etiqueta);
   //fflush(stdout);
#endif

//TAG de moneda : en las consultas puede no venir informado
  if(!memcmp(TTAG.etiqueta, TAG_5F2A, TAM_ETIQ))
  {
    //memset(PFI_AUTOR.fiaut049,0,sizeof(PFI_AUTOR.fiaut049));
    memset(PFI_055.tag55.tag_5F2A,0,sizeof(PFI_055.tag55.tag_5F2A));
    memset(SzAux,0,L_SzAux);
    //memcpy(PFI_055.tag55.tag_5F2A,TTAG.p_campo,TTAG.l_campo);

    fdesempa(2*TTAG.l_campo,TTAG.p_campo,SzAux);
    memcpy(PFI_055.tag55.tag_5F2A,SzAux,2*TTAG.l_campo);
    ltrim(SzAux,'0');
    //memcpy(PFI_AUTOR.fiaut049,SzAux,
      //        min(strlen(SzAux),sizeof(PFI_AUTOR.fiaut049)-1));
    nn++;
    continue;
  }

//TAG de salida, guardamos la longitud
  if(!memcmp(TTAG.etiqueta, TAG_71, TAM_ETIQ))
  {
//#if defined (TRAZA) && (TRAZA > 10) 
//    fprintf(stdout, "Script Previo de Emisor\n");
//    fflush(stdout);
//#endif
    memset(PFI_055.resp55.tag_71,0,sizeof(PFI_055.resp55.tag_71));
/** Tag 71  Guardamos tambien la longitud del campo **/
    memcpy(PFI_055.resp55.tag_71,TTAG.p_campo - 1,TTAG.l_campo);
    nn++;
    continue;
  }
//TAG de salida, guardamos la longitud
  if(!memcmp(TTAG.etiqueta, TAG_72, TAM_ETIQ))
  {
//#if defined (TRAZA)
//    fprintf(stdout, "Script Posterior de Emisor\n");
//    fflush(stdout);
//#endif
    memset(PFI_055.resp55.tag_72,0,sizeof(PFI_055.resp55.tag_72));
/** Tag 72  Guardamos tambien la longitud del campo **/
    memcpy(PFI_055.resp55.tag_72,TTAG.p_campo - 1,TTAG.l_campo );
    nn++;
    continue;
  }

  if(!memcmp(TTAG.etiqueta, TAG_82, TAM_ETIQ))
  {
//#if defined (TRAZA)
//    fprintf(stdout, "Perfil de Intercambio de la aplicacion\n");
//    fflush(stdout);
//#endif
    memset(PFI_055.tag55.tag_82,0,sizeof(PFI_055.tag55.tag_82));
    memset(SzAux,0,L_SzAux);
    //memcpy(PFI_055.tag55.tag_82,TTAG.p_campo,TTAG.l_campo);
    fdesepin(TTAG.l_campo,TTAG.p_campo,SzAux);
    memcpy(PFI_055.tag55.tag_82,SzAux,2*TTAG.l_campo);
    nn++;
    continue;
  }
//TAG de salida, guardamos la longitud
  if(!memcmp(TTAG.etiqueta, TAG_8A, TAM_ETIQ))
  {
//#if defined (TRAZA)
//    fprintf(stdout, "Codigo de respuesta a la autorizacion\n");
//    fflush(stdout);
//#endif
    memset(PFI_055.resp55.tag_8A,0,sizeof(PFI_055.resp55.tag_8A));
/** Tag 8A  Guardamos tambien la longitud del campo **/
    memcpy(PFI_055.resp55.tag_8A,TTAG.p_campo - 1,TTAG.l_campo);
    nn++;
    continue;
  }
//TAG de salida, guardamos la longitud
  if(!memcmp(TTAG.etiqueta, TAG_91, TAM_ETIQ))
  {
//#if defined (TRAZA)
    //fprintf(stdout, "Datos del emisor para autenticacion\n");
    //fflush(stdout);
//#endif
    memset(PFI_055.resp55.tag_91,0,sizeof(PFI_055.resp55.tag_91));
/** Tag 91  Guardamos tambien la longitud del campo **/
    memcpy(PFI_055.resp55.tag_91,TTAG.p_campo - 1,TTAG.l_campo );
    nn++;
    continue;
  }
  if(!memcmp(TTAG.etiqueta, TAG_95, TAM_ETIQ))
  {
//#if defined (TRAZA)
    //fprintf(stdout, "Resultado de la verificacion en el Terminal\n");
    //fflush(stdout);
//#endif
    memset(PFI_055.tag55.tag_95,0,sizeof(PFI_055.tag55.tag_95));
    //muevo dato para rellenar los bits.
    memcpy(PFI_055.tag55.tag_95,TTAG.p_campo,TTAG.l_campo);
	
	memset(SzAux,0,L_SzAux);
    fdesepin(TTAG.l_campo,TTAG.p_campo,SzAux);
    //fprintf(stdout, "despues de desepin\n");
    //fflush(stdout);	
	
    //machaco dato con el expandido
    memcpy(PFI_055.tag55.tag_95,SzAux,2*TTAG.l_campo);
    //fprintf(stdout, "despues de dato expandido\n");
    //fflush(stdout);	
	
    nn++;
    continue;
  }

  if(!memcmp(TTAG.etiqueta, TAG_9A, TAM_ETIQ))
  {
//#if defined (TRAZA)
//    fprintf(stdout, "Fecha de la transaccion(p-12)\n");
//    fflush(stdout);
//#endif

// No memset al iso12 por que en este tag solo viene la fecha 
//   memset(PFI_COMUN.fifechor.aa,0,sizeof(PFI_COMUN.fifechor.aa));
//   memset(PFI_COMUN.fifechor.mm,0,sizeof(PFI_COMUN.fifechor.mm));
//   memset(PFI_COMUN.fifechor.dd,0,sizeof(PFI_COMUN.fifechor.dd));
   memset(PFI_055.tag55.tag_9A,0,sizeof(PFI_055.tag55.tag_9A));
   memset(SzAux,0,L_SzAux);
   //memcpy(PFI_055.tag55.tag_9A,TTAG.p_campo,TTAG.l_campo);
   fdesempa(2*TTAG.l_campo,TTAG.p_campo, SzAux);
   memcpy(PFI_055.tag55.tag_9A,SzAux,2*TTAG.l_campo);

   nn++;
   continue;
  }

  if(!memcmp(TTAG.etiqueta, TAG_9C, TAM_ETIQ))
  {
//#if defined (TRAZA) 
//    fprintf(stdout, "Tipo de Transaccion (p-3.dig12)\n");
//    fflush(stdout);
//#endif

/* No memset al iso3 por que en este tag solo viene dig12 */
    memset(PFI_055.tag55.tag_9C,0,sizeof(PFI_055.tag55.tag_9C));
    //memcpy(PFI_055.tag55.tag_9C,TTAG.p_campo,TTAG.l_campo);
    memset(SzAux,0,L_SzAux);
    fdesempa(2*TTAG.l_campo,TTAG.p_campo, SzAux);
    memcpy(PFI_055.tag55.tag_9C,SzAux,2*TTAG.l_campo);
	//20160523 - no informo dig12 del fi pq no coincide con el p03 y machaco su valor
    //memset(PFI_AUTOR.fiaut003.dig12,0,sizeof(PFI_AUTOR.fiaut003.dig12));
    //memcpy(PFI_AUTOR.fiaut003.dig12,SzAux, min(strlen(SzAux),sizeof(PFI_AUTOR.fiaut003.dig12)-1));
    nn++;
    continue;
  }

  if(!memcmp(TTAG.etiqueta, TAG_9F02, TAM_ETIQ))
  {
//#if defined (TRAZA) 
    //fprintf(stdout, "Importe de la Transaccion (p-6)\n");
    //fflush(stdout);
//#endif
    memset(PFI_055.tag55.tag_9F02,0,sizeof(PFI_055.tag55.tag_9F02));
    //memcpy(PFI_055.tag55.tag_9F02,TTAG.p_campo,TTAG.l_campo);
    memset(SzAux,0,L_SzAux);
    fdesempa(2*TTAG.l_campo,TTAG.p_campo, SzAux);
    memcpy(PFI_055.tag55.tag_9F02,SzAux,2*TTAG.l_campo);
    //PFI_AUTOR.fiaut006=atof(SzAux); //Temporalmente comentado porque machaca el importe
    nn++;
    continue;
  }

    if(!memcmp(TTAG.etiqueta, TAG_9F03, TAM_ETIQ))
  {
//#if defined (TRAZA) 
//    fprintf(stdout, "Importe de la Transaccion (p-6)\n");
//#endif
    memset(PFI_055.tag55.tag_9F03,0,sizeof(PFI_055.tag55.tag_9F03));
    //memcpy(PFI_055.tag55.tag_9F03,TTAG.p_campo,TTAG.l_campo);
    memset(SzAux,0,L_SzAux);
    fdesempa(2*TTAG.l_campo,TTAG.p_campo, SzAux);
    memcpy(PFI_055.tag55.tag_9F03,SzAux,2*TTAG.l_campo);
//  PFI_AUTOR.fiaut006=atof(SzAux); //Temporalmente comentado porque machaca el importe
    nn++;
    continue;
  }

  if(!memcmp(TTAG.etiqueta, TAG_9F10, TAM_ETIQ))
  {
//#if defined (TRAZA) 
//    fprintf(stdout, "Datos aplicacion del emisor\n");
//#endif
    memset(PFI_055.tag55.tag_9F10,0,sizeof(PFI_055.tag55.tag_9F10));
    //memcpy(PFI_055.tag55.tag_9F10,TTAG.p_campo,TTAG.l_campo);
    ltrim(SzAux,'0');
    fdesepin(TTAG.l_campo,TTAG.p_campo,SzAux);
    memcpy(PFI_055.tag55.tag_9F10,SzAux,2*TTAG.l_campo);

    nn++;
    continue;
  }

  if(!memcmp(TTAG.etiqueta, TAG_9F1A, TAM_ETIQ))
  {
//#if defined (TRAZA) 
//    fprintf(stdout, "Codigo del pais\n");
//#endif
    memset(PFI_AUTOR.fiaut019,0,sizeof(PFI_AUTOR.fiaut019));
    memset(PFI_055.tag55.tag_9F1A,0,sizeof(PFI_055.tag55.tag_9F1A));
    memset(SzAux,0,L_SzAux);
    //memcpy(PFI_055.tag55.tag_9F1A,TTAG.p_campo,TTAG.l_campo);
    fdesempa(2*TTAG.l_campo,TTAG.p_campo,SzAux);
    memcpy(PFI_055.tag55.tag_9F1A,SzAux,2*TTAG.l_campo);
    ltrim(SzAux,'0');
    memcpy(PFI_AUTOR.fiaut019,TTAG.p_campo,
              min(strlen(SzAux),sizeof(PFI_AUTOR.fiaut019)-1));
    nn++;
    continue;
  }

  if(!memcmp(TTAG.etiqueta, TAG_9F26, TAM_ETIQ))
  {
//#if defined (TRAZA) 
//    fprintf(stdout, "Criptograma de peticion\n");
//#endif
    memset(PFI_055.tag55.tag_9F26,0,sizeof(PFI_055.tag55.tag_9F26));
    //memcpy(PFI_055.tag55.tag_9F26,TTAG.p_campo,TTAG.l_campo);
    ltrim(SzAux,'0');
    fdesepin(TTAG.l_campo,TTAG.p_campo,SzAux);
    memcpy(PFI_055.tag55.tag_9F26,SzAux,2*TTAG.l_campo);

    nn++;
    continue;
  }

  if(!memcmp(TTAG.etiqueta, TAG_9F27, TAM_ETIQ))
  {
//#if defined (TRAZA) 
//    fprintf(stdout, "Datos de Informacion de criptogramas\n");
//#endif
    //memcpy(PFI_055.tag55.tag_9F27,TTAG.p_campo,1);;/* esto hay que mirarlo */
    ltrim(SzAux,'0');
    fdesepin(TTAG.l_campo,TTAG.p_campo,SzAux);
    memcpy(PFI_055.tag55.tag_9F27,SzAux,2*TTAG.l_campo);

    nn++;
    continue;
  }

  if(!memcmp(TTAG.etiqueta, TAG_9F33, TAM_ETIQ))
  {
//#if defined (TRAZA) 
//    fprintf(stdout, "Capacidad del PinPad\n");
//#endif
    memset(PFI_055.tag55.tag_9F33,0,sizeof(PFI_055.tag55.tag_9F33));
    //memcpy(PFI_055.tag55.tag_9F33,TTAG.p_campo,TTAG.l_campo);
    ltrim(SzAux,'0');
    fdesepin(TTAG.l_campo,TTAG.p_campo,SzAux);
    memcpy(PFI_055.tag55.tag_9F33,SzAux,2*TTAG.l_campo);

    nn++;
    continue;
  }
  if(!memcmp(TTAG.etiqueta, TAG_9F34, TAM_ETIQ))
  {
//#if defined (TRAZA) 
//   fprintf(stdout, "Resultado del metodo de verificacion del titular\n");
//#endif
   memset(PFI_055.tag55.tag_9F34,0,sizeof(PFI_055.tag55.tag_9F34));
   //memcpy(PFI_055.tag55.tag_9F34,TTAG.p_campo,TTAG.l_campo);
   ltrim(SzAux,'0');
   fdesepin(TTAG.l_campo,TTAG.p_campo,SzAux);
   memcpy(PFI_055.tag55.tag_9F34,SzAux,2*TTAG.l_campo);

   nn++;
   continue;
  }

  if(!memcmp(TTAG.etiqueta, TAG_9F36, TAM_ETIQ))
  {
//#if defined (TRAZA) 
//    fprintf(stdout, "Contador de transacciones de la aplicacion\n");
//#endif
    memset(PFI_055.tag55.tag_9F36,0,sizeof(PFI_055.tag55.tag_9F36));
    //memcpy(PFI_055.tag55.tag_9F36,TTAG.p_campo,TTAG.l_campo);
    ltrim(SzAux,'0');
    fdesepin(TTAG.l_campo,TTAG.p_campo,SzAux);
    memcpy(PFI_055.tag55.tag_9F36,SzAux,2*TTAG.l_campo);

   nn++;
   continue;
  }

  if(!memcmp(TTAG.etiqueta, TAG_9F6E, TAM_ETIQ))
  {
	fprintf(stdout, "etiqueta 9F6E\n");
    memset(PFI_055.tag55.tag_9F6E,0,sizeof(PFI_055.tag55.tag_9F6E));
    ltrim(SzAux,'0');
    fdesepin(TTAG.l_campo,TTAG.p_campo,SzAux);
    memcpy(PFI_055.tag55.tag_9F6E,SzAux,2*TTAG.l_campo);

    nn++;
	
    continue;
  }
  
  
  if(!memcmp(TTAG.etiqueta, TAG_9F37, TAM_ETIQ))
  {
//#if defined (TRAZA) 
//    fprintf(stdout, "Numero aleatorio para criptogramas\n");
//#endif
    memset(PFI_055.tag55.tag_9F37,0,sizeof(PFI_055.tag55.tag_9F37));
    //memcpy(PFI_055.tag55.tag_9F37,TTAG.p_campo,TTAG.l_campo);
    ltrim(SzAux,'0');
    fdesepin(TTAG.l_campo,TTAG.p_campo,SzAux);
    memcpy(PFI_055.tag55.tag_9F37,SzAux,2*TTAG.l_campo);
 
    nn++;
	
    continue;
  }

  if(!memcmp(TTAG.etiqueta, TAG_9F17, TAM_ETIQ))
  {
//#if defined (TRAZA) 
//    fprintf(stdout, "Contador de intentos de PIN\n");
//#endif
    memset(PFI_055.tag55.tag_9F17,0,sizeof(PFI_055.tag55.tag_9F17));
    ltrim(SzAux,'0');
    fdesepin(TTAG.l_campo,TTAG.p_campo,SzAux);
    memcpy(PFI_055.tag55.tag_9F17,SzAux,2*TTAG.l_campo);

    nn++;
    continue;
  }

 } /* del while */
 
 fprintf(stdout, "PFI_055.max055 >%.256s\n",PFI_055.max055 ); 

 return;
}


