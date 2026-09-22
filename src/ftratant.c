/***
   FUNCION: ftratant.c
   DESCRIPCION: TRATA LOS DIFERIDOS A CARDPLUS SIN TARJETA  TRANSFERENCIAS


  de momento solo graba en diferidos

***/

THREADVAR int codigo_operacion;
THREADVAR int tipo_cuenta;
THREADVAR int tipomsg;
THREADVAR int numclatar;
THREADVAR int codigo_dispos;
THREADVAR int codigo_activ18;
THREADVAR int codigo_activ26;
THREADVAR int tipo_operacion;
THREADVAR int retorno;

#include "nptdife.h"
#include "nptpara.h"

NPSPARA  *reg_para;

int fgradife(formato_interno * pforitn);
/***************************************************************/
/***   funcionnt: autorizacion diferidos con tarjeta propia  ***/
/***************************************************************/

int funcionnt(formato_interno * pforitn)
{
   int ll;

    fprintLog("\n\n**************\n");
    fprintLog("*** FTRATANT znpm203***\n");
    fprintLog("****************\n\n");

    // strcpy(reginci.innompro, "TRATANT"); Comentado para no perder el nombre del servicio
    tipomsg = atoi(pforitn->fiisocom.fiiso000);
    codigo_operacion = atoi(pforitn->fidatiso.fiisoaut.ficodpro.dig12); 
    tipo_cuenta = atoi(pforitn->fidatiso.fiisoaut.ficodpro.dig34);


    fprintLog("codigo_operacion->%d\n", codigo_operacion);
    fprintLog("tipo_cuenta->%d\n", tipo_cuenta);
    fprintLog("tipomsg->%d\n", tipomsg);

    //RAQUEL AÑADIDO PARA ARRANQUE POR BBDD
    for (ll=0 ; ll<2 ; ll++) 
    { 

      if(!strncmp(pforitn->fidatpro.fiorigen, reg_para[ll].psnument, 2))
      {
        switch(codigo_operacion)
        {
        case 22:
        case 06:
        case 29:
        case 30:

            retorno = fgradife(pforitn);

            if(retorno)
                return (finalizar(98));

            strcpy(pforitn->fidatpro.firesfin,"00"); 
            strcpy(pforitn->fidatpro.ficodfin, "000");  
            return(finalizar(11));

        break;
        }
		break;
      }
	}

    //quito el warning (si no sale por los return del if y de los case, saldrá por aquí
    return(finalizar(11));
}


