#include <stdio.h>

#include "cnetplus.h"
#include "nptdife.h"
/***
   PROGRAMA: znpm208****.C
   DESCRIPCION: Graba una operación en diferidos

***/
void fechorsys(struct tm_dec *tmPtr); 
  tm_dec tmPtr;

formato_interno paux_diforitn;


int fgradife(formato_interno * pforitn);
int fgenera_diferidoI(formato_interno *pforitn);
int fverfi(formato_interno fi);

/**********************************************/
int funciondi(formato_interno *pforitn)
{

int err=0;
formato_interno *mipuntero, miformato;


mipuntero=&miformato;
memcpy(mipuntero,pforitn,sizeof(formato_interno));

#ifdef TRAZA
fprintLog(nomProg,"***********************\n");
fprintLog(nomProg,"********FTRATADI*******>%.6s<\n",pforitn->fidatpro.finumope );
fprintLog(nomProg,"***********************\n");
#endif

//para operaciones que no se han podido enviar al resolutor, se graba en diferidos
if (strncmp(pforitn->fidatpro.firesfin, "00", 2)==0 &&
    strncmp(pforitn->fidatpro.ficodfin, "000", 3)==0)
{
    fverfi(*pforitn);
    err=fgenera_diferidoI(mipuntero); 
    if (err != 0)
    {
        fprintLog(nomProg,"error al grabar diferido\n");
        strcpy(coderror, "203");
        return(finalizar(98));
    }
}
else
{
   fprintLog(nomProg,"La operacion tiene codigo finalizacion >%.3s<\n"
     , pforitn->fidatpro.ficodfin );
}
return(finalizar(11));
}


/************************************************************************
*      Funcion genera_diferido                                       *
************************************************************************/

int fgenera_diferidoI(formato_interno *pforitn)
{
  int segundos;
  int err=0;
  int err1=0;
  NPSDIFE regdife;

  
  
  strncpy(pforitn->fiisocom.fiiso000+2, "20",2);
  fprintLog(nomProg,"le ponemos 20 al fiiso000 >%s<\n",pforitn->fiisocom.fiiso000);      
  strcpy(pforitn->fiisocom.respusal,"   ");
  strcpy(pforitn->fidatpro.ficodfin, "   ");

  //20160701 - unificamos la grabacion en dife en una sola funcion   
  err = fgradife(pforitn);
  
 
 if(err) 
  return(1);

 return(0);
}
