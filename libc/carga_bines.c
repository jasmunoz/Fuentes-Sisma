/**********************************************/
#include "svm.h"
#include "deffich.h"
#include "datagen.h"
#include "nptbine.h"


// PRJ0202598 cambio bin de 6 posiciones 8 se cambia TAM_BIN y cambia el tamaño total de tbbine
void carga_bin(tbbine *pbine,NPSBINE rbin);
void limpia_bin(tbbine *Svmbine);

int carga_bines(tbbine *psvmbine)
{
  
 // PRJ0202598 cambio bin de 6 posiciones 8 se cambia TAM_BIN y cambia el tamaño total de regbine
  NPSBINE regbine;
  int posicion,err,ll,yy,mm,salir=0;
  int err1;
  int retorno;
  char cadena_aux[30];
  // PRJ0202598 cambio bin de 6 posiciones 8 se cambia el tamaño de la clave claveB 
  //porque es donde se va a pasar regbine.biclave0 que ahora es de 8 posiciones
  char claveB[8];


#if defined (TRAZA) && (TRAZA > 1)
fprintf(stdout, "========fcarga_bines \n");  
#endif

for (ll=0 ; ll<MAX_REG_TBBINE ; ll++)
   {
   limpia_bin(psvmbine+ll);
   }  

err=tratar_fich("NPTBINE", PRIMERO, &regbine, NULL);
printf ("carga_bine NPTBINE PRIMERO REGBINE %s\n", &regbine);
if(err)
{
 if(err != 101)
 {
#if defined (TRAZA) && (TRAZA > 1) 
  fprintf(stdout, "Error %d en PRIMERO Fichero BINE\n",err);  
#endif  
  return(-2);
 }
}

ll=mm=0;
while(!err)
{
#if defined (TRAZA) && (TRAZA > 100)

 fprintf(stdout, "cargando  bb:%.*s   ",
         TAM_BIN, regbine.biclave0);
printf ("carga_bine cargando REGBINE %s\n", regbine);
#endif
   if (mm < MAX_REG_TBBINE)
     {
#if defined (TRAZA) && (TRAZA > 10)	 
      fprintf(stdout, "en mm >%d<\n", mm);
#endif	  
      carga_bin((psvmbine+mm),regbine);
      printf("despues de carga_bin \n");
      mm++;
     }
   else
     {
#if defined (TRAZA) && (TRAZA > 10)	 	 
      fprintf(stdout, "No cargado en mm >%d<    MAX_REG_TBBINE(%d)\n",
                      mm,MAX_REG_TBBINE);
#endif					  
     }
 
 // PRJ0202598 cambio bin de 6 posiciones 8 se cambia el tamaño de la clave claveB 
  //porque es donde se va a pasar regbine.biclave0 que ahora es de 8 posiciones
 //strncpy(claveB, regbine.biclave0,6);
 //claveB[6]='\0';
 strncpy(claveB, regbine.biclave0,8);
 claveB[8]='\0';
 err=tratar_fich("NPTBINE",SIGUIENTE,&regbine,claveB);
 printf ("carga_bine NPTBINE SIGUIENTE REGBINE %s\n", &regbine);
 if (err)
 {
  if(err == 101)
   break;
  else
  {
#if defined (TRAZA) && (TRAZA > 1)
   fprintf(stdout, "Error %d en SIGUIENTE Fichero BINE\n",err);
#endif
   salir = -3;
  }
 }
}   /***   fin del bucle   ***/

return(salir);
}

void carga_bin(tbbine *pbine,NPSBINE rbin)
{ 
 // PRJ0202598 cambio bin de 6 posiciones 8 se cambia TAM_BIN y los limites superiores e inferiores
 printf("carga_bin entrando ....\n"); 
 strncpy(pbine->ficbin,rbin.biclave0, TAM_BIN); 
  strncpy(pbine->birnginf, rbin.birnginf, 11);
 strncpy(pbine->birngsup, rbin.birngsup, 11);
   memset(pbine->fictar , ' ' , TAM_TIPTAR);
   pbine->idcuad[0] = '0';
 // printf("carga_bin saliendo pbine  birnginf %s\n",pbine->birnginf);

 return;
}

void limpia_bin(tbbine *Svmbine)
{
   // PRJ0202598 cambio bin de 6 posiciones 8 se cambia TAM_BIN y limites inferiores y superiores
   memset(Svmbine->ficbin , ' ' , TAM_BIN);
   memset(Svmbine->birnginf , ' ' , 11);
   memset(Svmbine->birngsup , ' ' , 11);
   memset(Svmbine->fictar , ' ' , TAM_TIPTAR);
   Svmbine->idcuad[0] = '0';
}

/*
Retorna:
	   -1  -> -> No existe bin
	   >=0 -> Posicion donde se encuentra el bin buscado
 
*/
int busca_svmbin(tbbine *PsvmBine, char *pPanOpe, int lonPan)
{
  // PRJ0202598 cambio bin de 6 posiciones 8 se cambia TAM_BIN
  // se tiene que cambiar bin[6] porque pasa a ser de 8
  int col;
  char bin[8];
  char *rango;

#if defined (TRAZA) && (TRAZA > 99)
 fprintf(stdout,"===========busca_smvbin\n");
#endif

  strncpy(bin, pPanOpe, TAM_BIN);
  strncpy(rango, pPanOpe + TAM_BIN, (lonPan - TAM_BIN));
  
  for(col=0 ; col < MAX_REG_TBBINE; col++)
  {
   if (   strncmp(bin, (PsvmBine+col)->ficbin,TAM_BIN)==0
       && strncmp(rango, (PsvmBine+col)->birnginf, (lonPan - TAM_BIN)) <= 0
	&& strncmp(rango, (PsvmBine+col)->birngsup, (lonPan - TAM_BIN)) >= 0
	   )
      return (col);
    /* Encontrado */
 }

 return(-1);
}
