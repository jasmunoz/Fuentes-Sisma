#ifndef TRAEETIQ_C
#define TRAEETIQ_C

#include "traeetiq.h"

int fdesepin(int longi, char *sempaq, char *sdesem);
char  chmasc_55 = 0x1F;       /** bits <0001 1111> 1F ***/
void trae_etiquetas(petiquetas* salida, int long_campo, char *p_campo)
{
  char codialf[3];
  char codiaux;
  char codialf4[4];
  int offset;
  int longinum;
  int nn;
  
 offset = 0;
 longinum = 0;

#if defined (TRAZA) && (TRAZA > 10) 
 //fprintf(stdout, "LONG_CAMPO >%d<\n", long_campo);
 //fhexdump(stdout, p_campo, long_campo);
 //fflush(stdout);
#endif 

 for(nn = 0;nn < NUM_ETIQUETAS_REG && offset < long_campo; nn++)
 {
  strcpy(codialf , "00");
  codiaux = *(p_campo + offset);

  codiaux = codiaux & chmasc_55;

  if(codiaux != chmasc_55)
  { /** un caracter ***/
   codialf[0] = *(p_campo + offset);

   fdesepin(1, codialf, codialf4);
#if defined (TRAZA) && (TRAZA > 10) 
  //fprintf(stdout,"despues de fdesepin codialf4 1>%.2s<\n",codialf4);
  //fflush(stdout);
#endif  

  /*************** PARA PRUEBAS *******************
  fprintf(stdout,"llamada FDESEMPA \n");

    fprintf(stdout,"antes despues codialf >%.3s<\n",codialf);
  fflush(stdout);

   fdesempa(4, codialf, codialf4);

     fprintf(stdout,"despues de fdesempa codialf4 1>%.4s<\n",codialf4);
  fflush(stdout);

  ***********FIN DE PRUEBAS **************************/
 
   memset(codialf4 + 2, 'X', 2);
   offset++;
  }
  else
  {
   memcpy(codialf, p_campo + offset, 2);

   fdesepin(2, codialf, codialf4);

#if defined (TRAZA) && (TRAZA > 10)    
    //fprintf(stdout,"codialf4 2>%.4s<\n",codialf4);
    //fflush(stdout);
#endif	
   offset += 2;
  }
  strncpy(salida->etiqueta[nn].etiqueta, codialf4, 4);

  longinum = *(p_campo + offset);

#if defined (TRAZA) && (TRAZA > 10)   
  //fprintf(stdout,"longinum >%d<\n",longinum);
  //fflush(stdout);
#endif  

  salida->etiqueta[nn].l_campo = longinum;
  offset++;

  salida->etiqueta[nn].p_campo = p_campo + offset;
  offset += longinum;

 }         /***   fin del for    ***/
 salida->n_etiquetas = nn;  

#if defined (TRAZA) && (TRAZA > 10)  
 fprintf(stdout,"en trae_etiquetas n_etiquetas >%d<\n",salida->n_etiquetas);   
 fflush(stdout);
#endif
}
#endif
