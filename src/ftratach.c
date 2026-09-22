
/***********************************************************************
  FUNCION   funcionCH

  DESCRIPCION : tratamiento de mensajes de control 
  origen 01 ó 00
  recibe formato_interno y modifica el semaforo en funcion de la respuesta
***********************************************************************/

#include "cforitno.h" 
#include "nptpars.h"
#include "deffich.h"
#include "nptpara.h"

#define  ACEPTADO     1
#define  DENEGADO     0
#define  NOCONOCIDO   9

int f_trata_respuesta_control(formato_interno *pforitn);
int modifica_semaf_reso(NPSPARS *preg_pars, char estado);

NPSPARA  *reg_para;

int funcionch(formato_interno *pforitn)
{
    NPSPARS reg_pars;
    int retorno;
    int tipomsg;
	int ll;

    fprintLog(nomProg,"=============\n");
    fprintLog(nomProg,"= funcionCH =\n");
    fprintLog(nomProg,"=============\n");

    tipomsg = atoi(pforitn->fiisocom.fiiso000);

    fprintLog(nomProg,"tipomsg >%d<\n",tipomsg);

    switch(tipomsg)
    {
        case 1804:
            if(!strncmp(pforitn->fidatpro.ficodfin, "297", 3) || /* No contestado */
            !strncmp(pforitn->fidatpro.ficodfin, "   ", 3)) /* Emisor no responde */
            {
                strcpy(pforitn->fiisocom.respusal, "912");
                strncpy(reg_pars.pasubsis,pforitn->fidatpro.firespri, 2);
                strncpy(reg_pars.pasubapl,pforitn->fidatpro.fisubres,2);
                fprintLog(nomProg,"subapl >%.2s<\n",reg_pars.pasubapl);

                modifica_semaf_reso(&reg_pars, '0');

				//RAQUEL cambiado para comparar con datos de bbdd por entidad
			   for (ll=0 ; ll<2 ; ll++) 
               {

                if((!strncmp(pforitn->fidatpro.firespri, reg_para[ll].psnument, 2) ) //test a red
				   && (strncmp(pforitn->fidatpro.fiorigen,reg_para[ll].psmonito,2) ))//si origen no pantalla
				{
                    if (!strncmp(pforitn->fidatpro.fisubres, "01", 2))
                         strncpy(reg_pars.pasubapl,"02",2);
                    else
                         strncpy(reg_pars.pasubapl,"01",2);
                         modifica_semaf_reso(&reg_pars, '0');
                }
			
			   }
                return(11);
            }
        case 1814: //respuesta a un msj de control enviado desde sisma
            strncpy(reg_pars.pasubsis, pforitn->fidatpro.firespri, 2);
            strncpy(reg_pars.pasubapl, pforitn->fidatpro.fisubres, 2); 
			
			//RAQUEL cambiado para recoger los datos por bbdd
			 for (ll=0 ; ll<2 ; ll++) 
             {           
		    	if (strncmp(pforitn->fidatpro.firespri, reg_para[ll].psnument, 2) !=0)
		    	{

               // modifica el semáforo entre 0 y 1 - SEMAF00
			      if(!strncmp(pforitn->fiisocom.respusal, "99", 3)||!strncmp(pforitn->fiisocom.respusal, "800", 3))
                  {
            
                   retorno = modifica_semaf_reso(&reg_pars, '1');
                  }
                   else
                 {
              
                  retorno = modifica_semaf_reso(&reg_pars, '0');
                 }
               }
			//respuesta a un test a redsys
               else
              {			
			   // modifica pasemaf1 y 2 segun el proc alm SEMAF
			     retorno = f_trata_respuesta_control(pforitn);
              }
			 }
            break; 
        default:
            fprintLog(nomProg,"recibido mensaje no contemplado :%d\n",tipomsg);
            strcpy(coderror, "280");
            //AÃ±adido para Monitorizacion SISMA 
            fprintLog(nomProg,"TIPO FUNCIONAL - TIPO DE MENSAJE NO CONTEMPLADO: Tipo mensaje >%d< codigo de error >%.3s<\n",tipomsg,coderror);

            strcpy(pforitn->fidatpro.ficodfin, "280");
            strcpy(pforitn->fidatpro.firesfin, "00");
            retorno = -1;
    };

    if (retorno != 0)
    {
        return (finalizar(98));
    }

    return (finalizar(11));
}

/***********************************************************************
  FUNCION   f_trata_respuesta_control

  DESCRIPCION : tratamiento de mensajes de control procedentes de PRICE

     retorna :     0 . operacion aceptada
                  -1 . operacion denegada
***********************************************************************/

int f_trata_respuesta_control(formato_interno *pfi)
{
  int retorno;
  char auxsub[5];
  int posisubs;

 typedef struct Clave
 {
   char   subsis[2];       /*  Subsistema                                      */
   char   subapl[2];       /*  Subaplicacion                                   */
   char   txtresp[8];      /*  texto para la respuesta del diálogo,
                               “DENEGADO”, “ACEPTADO”
							   (solo actualizo semaforo si es ACEPTADO.        */ 
   char   codfunc[3];      /*  código de función = fiiso024                    */
 } Clave;

 typedef struct RegistroC
 {
   char   numses[3];       /*  número de sesión                                */
   char   fecses[6];       /*  fecha de sesión en formato aammdd               */
   char   semaf01[1];      /*  valor del semaforo retornado                    */
 } RegistroC;

 Clave clave;
 RegistroC registroc;

 fprintLog(nomProg,"f_trata_respuesta_control\n");

  // solo actualizamos el semaforo si la respuesta al mensaje es aceptada
  if(  (!strncmp(pfi->fiisocom.respusal, "800", 3))
    ||(!strncmp(pfi->fiisocom.respusal, "880", 3)))
 {
    strncpy(clave.txtresp, "ACEPTADO",8);
 }
 else
 {
    // DENEGADO
	return(-1);
 }

  // subsistema resolutor
 strncpy(clave.subsis, pfi->fidatpro.firespri, 2);
 strncpy(clave.subapl, pfi->fidatpro.fisubres,2);
 strncpy(clave.codfunc, pfi->fiisocom.fiiso024, 3);
 fprintLog(nomProg," subsistema pfi->fidatpro.fisubres %s\n",pfi->fidatpro.fisubres);
 fprintLog(nomProg,"Clave actualiza SEMAF >>>%.15s<<< \n", clave.subsis);

 // actualiza en bbdd
 /***   siempre que se recibe algo de puc/price, si el estado es desconectado
       debe pasar a conectado, y despues variar el estado segun el
       mensaje recibido   ***/
//.pasemaf1 es el semáforo actual
//.pasemaf2 es el semaforo anterior

 retorno = tratar_fich("NPTPARS", SEMAF, &registroc, &clave);
 if (retorno)
 {
   //AÃ±adido para Monitorizacion SISMA 
   fprintLog(nomProg,"TIPO SISTEMA - ACCESO A BASE DE DATOS: Tipo reescribir NPTPARS codigo de error >%d< \n",retorno);

   fprintLog(nomProg,"error en reescribir ->%d<-\n", retorno);
   return(-1);
 }
#if defined (TRAZA) && (TRAZA > 1)
    fprintLog(nomProg,"actualizado el semaforo en BBDD\n");
#endif
 

 // actualiza en svm
 memcpy(auxsub, clave.subsis, 2);
 memcpy(auxsub + 2, clave.subapl, 2);
 auxsub[4] = '\0';
 posisubs= encuentra_svm(psvm, auxsub, TIP_SUBSIS);
 if (posisubs < 0)
 {
#if defined (TRAZA) && (TRAZA > 1)
    fprintf(stderr,"***** ERROR: no encuentra subsistema >%s< en svm : %d\n",
           auxsub, posisubs);
#endif
  return(-1);
 }
 (psvm+posisubs)->str_svm.svmpars.pasemsub=(registroc.semaf01[0]-'0'); 
#if defined (TRAZA) && (TRAZA > 1)
    fprintLog(nomProg,"actualizado semaf SVM >%c< >%d<\n", registroc.semaf01[0], (psvm+posisubs)->str_svm.svmpars.pasemsub);
#endif

 strcpy(pfi->fidatpro.ficodfin , "000");
 return(0);
}


