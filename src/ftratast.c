//CGH-INI
#include "cforitno.h"
#include "nptlogo.h"
#include "deffich.h"
//CGH-FIN

int fct_trata_rearranque(formato_interno *pfi);
int fct_trata_fin_recup(formato_interno *pfi);
int fct_trata_informativo(formato_interno *pfi); 
int factualiza_incidencia_cp(char *coderror);

/***********************************************************************
  FUNCION.....: funcionST

  DESCRIPCION : tratamiento de mensajes de control origen 30, recepcion
                de peticiones de control de 4B a la entidad, recibe
                formato_interno, y actualiza el estado de la conexion.
***********************************************************************/

int funcionst(formato_interno *pforitn)
{
    int fct_trata_control(formato_interno *pforitn);
    int estado_actual;
    int retorno;
    int tipomsg;
//CGH-INI
	char coderror;
//CGH-FIN

    fprintLog("=============\n");
    fprintLog("= funcionST=\n");
    fprintLog("=============\n");

    tipomsg = atoi(pforitn->fiisocom.fiiso000);

    //Cuando es un mensaje de control ponemos la fecha de la operacion la del sistema a la entrada

    switch(tipomsg)
    {
        case 1811:
            fprintLog("dentro del 1811 respusal >%.3s<\n",pforitn->fiisocom.respusal);
            if(strncmp(pforitn->fiisocom.respusal,"001",3) && strncmp(pforitn->fiisocom.respusal,"000",3))
            {
                if(!strncmp(pforitn->fiisocom.respusal,"099",3))
                    retorno=festado_conexion(2,pforitn->fidatcab.fiioocpd, "99", '2');
                else
                    estado_actual=festado_conexion(2,pforitn->fidatcab.fiioocpd,"00",'2');

                fgraba_incidencia_cp(pforitn->fiisocom.respusal, pforitn->fidatpro.finumope, "No aceptado APERTURA   4B 1811         ", NULL, NULL, NULL);
            }
            else
            {
                estado_actual=festado_conexion(2,pforitn->fidatcab.fiioocpd, "09", '2');
            }
            retorno=1;
            //Se activa el byte de que es una respuesta a mensaje de control
            break;
        case 1802:
            retorno = fct_trata_rearranque(pforitn);
            //Se activa el byte de que es una respuesta a mensaje de control
            strncpy(pforitn->fidatcab.fiibit1,"00000011",8);
            strncpy(pforitn->fidatcab.fiibit2,"00000000",8);
            strncpy(pforitn->fidatcab.fiibit3,"00000000",8);
            fgraba_incidencia_cp("802", pforitn->fidatpro.finumope, "       MENSAJE REARRANQUE 1812         ", NULL, NULL, NULL);
            break;
        case 1812:
            fprintLog("dentro del 1812\n");
            retorno = fct_trata_rearranque(pforitn);
            break;
        case 1813:
            fprintLog("dentro del 1813\n");
            if (strncmp(pforitn->fiisocom.respusal,"001",3) && strncmp(pforitn->fiisocom.respusal,"000",3))
            {
                if(!strncmp(pforitn->fiisocom.respusal,"099",3))
                    retorno=festado_conexion(2,pforitn->fidatcab.fiioocpd, "99", '2');
                else
                    estado_actual=festado_conexion(2,pforitn->fidatcab.fiioocpd,"09",'2');
                fgraba_incidencia_cp(pforitn->fiisocom.respusal,pforitn->fidatpro.finumope, "No acept. INTERRUPCION 4B 1813         ");
            }
            else
            {
                estado_actual=festado_conexion(2,pforitn->fidatcab.fiioocpd, "29", '2');
            }
            retorno=1;
            break;
        case 1815:
            fprintLog("dentro del 1815 respusal >%.3s<\n",pforitn->fiisocom.respusal);
            if (strncmp(pforitn->fiisocom.respusal,"001",3) && strncmp(pforitn->fiisocom.respusal,"000",3))
            {
                //estado_actual=festado_conexion(2,pforitn->fidatcab.fiioocpd,"99",'2');
                if(!strncmp(pforitn->fiisocom.respusal,"099",3))
                    retorno=festado_conexion(2,pforitn->fidatcab.fiioocpd, "99", '2');
                fgraba_incidencia_cp(pforitn->fiisocom.respusal, pforitn->fidatpro.finumope, "No acept.  REANUDACION 4B 1815         ", NULL, NULL, NULL);
            }
            else
            {
                estado_actual=festado_conexion(2,pforitn->fidatcab.fiioocpd, "09", '2');
            }
            retorno=1;
            break;
        case 1806:
            fprintLog("dentro del 1806\n");
            retorno = fct_trata_fin_recup(pforitn);
            fprintLog("DESPUES DE  fct_trata_fin_recup retorno >%d<\n",retorno);
            //Se activa el byte de que es una respuesta a mensaje de control, nos ha llegado una peticion
            strncpy(pforitn->fidatcab.fiibit1,"00000011",8);
            strncpy(pforitn->fidatcab.fiibit2,"00000000",8);
            strncpy(pforitn->fidatcab.fiibit3,"00000000",8);

            factualiza_incidencia_cp("802");
            fprintLog("FIN DEL CASO 1806\n");
            break;
        case 1817:
            fprintLog("dentro del 1817\n");
            retorno = fct_trata_informativo(pforitn);
            //Se activa el byte de que es una respuesta a mensaje de control
            break;
        default:
            fprintLog("recibido mensaje no contemplado :%d\n",tipomsg);
            strcpy(coderror, "280");
            strcpy(pforitn->fidatpro.ficodfin, "280");
            strcpy(pforitn->fidatpro.firesfin, "00");
            retorno = -1;
    } //fin del tipo de mensaje

    if (retorno != 1)
    {
        return (finalizar(98));
    }
    return (finalizar(11));
}


/***********************************************************************
  FUNCION.....: fct_trata_rearranque

  DESCRIPCION : tratamiento de peticion de rearranque origen 30

      retorna :    1 . operacion aceptada
                  -1 . operacion denegada
***********************************************************************/

int fct_trata_rearranque(formato_interno *pfi)
{
    int estado_actual;
    char opcenvoff;
//CGH-INI
    int retorno;
//CGH-FIN

    fprintLog("trata_control en fct_trata_rearranque valor codopcion >%.2s<\n",pfi->fidatiso.fiisoctr.codopcion[0]));

    estado_actual=festado_conexion(1,pfi->fidatcab.fiioocpd, "00", opcenvoff);


    if (estado_actual==9 || estado_actual==69 || estado_actual==99 || estado_actual== 0)
    {
        //retorno=festado_conexion(2,pfi->fidatcab.fiioocpd, "09", pfi->fidatiso.fiisoctr.codopcion[0]);
        //tenemos que poner la sesion en rearranque y no abierta
        retorno=festado_conexion(2,pfi->fidatcab.fiioocpd, "69", pfi->fidatiso.fiisoctr.codopcion[0]); //PRJ0258542 
				//retorno=festado_conexion(2,pfi->fidatcab.fiioocpd, "69", "0"); //PRJ0258542 
				
        strcpy(pfi->fidatpro.firesfin , "00");
        strcpy(pfi->fidatpro.ficodfin , "000");
        strncpy(pfi->fidatcab.ficodrech,"00",2);
    }
    else
    {
        //denegamos el rearranque pero no, lo rechazamos. 24/09/2012
        strncpy(pfi->fidatcab.ficodrech,"00",2);
        strcpy(pfi->fidatpro.firesfin , "00");
        strcpy(pfi->fidatpro.ficodfin , "012");
        strcpy(pfi->fiisocom.respusal , "012");
    }
    //Finaliza para que pase a la siguiente funcion de tabla de operaciones que sera ftrataeo
    return(1);
}

/***********************************************************************
  FUNCION.....: fms_trata_finrecup

  DESCRIPCION : tratamiento de fin de recuperacion origen 30

      retorna :     1 . operacion aceptada
                   -1 . operacion denegada
***********************************************************************/

int fct_trata_fin_recup(formato_interno *pfi)
{
    NPSLOGO reg_logo; 
    char  clave_pr[35];
    int estado_actual;
    char opcenvoff;
    int return_pr;
//CGH-INI
    int retorno;
    int num_origen;
//CGH-FIN

    fprintLog("tratamiento de fin de recuperacion \n");

    estado_actual=festado_conexion(1,pfi->fidatcab.fiioocpd, "00",opcenvoff);
    opcenvoff = '0';

    switch(estado_actual)
    {
        case 9: //sesion abierta;
        case 69://sesion en rearranque;
            /*
            aqui tendria que saber si al enviar el mensaje 1816 queremos interrupir la sesion o queremos finalizar el
            rearranque de con sesion abierta.
            Para ello, tengo que comprobar si hay algun mensaje 1803 sin contestar y con origen sisma y del canal sea igual al 1816.
            Por lo tanto tenemos que cambiar el valor codopcion por 1.
            */

            fprintLog("----antes de montar la clave estado actual>%d< --- \n",estado_actual);

            /*** montar la clave de la previa **/
            fprintLog("DENTRO DEL 1806 Y ORIGEN >%d<\n",num_origen);
            memset(clave_pr,' ',sizeof(clave_pr));
            //strncpy(claprice ->lonumepr,pfi->fidatpro.finumope, 6);
            strncpy(clave_pr,"1803",4);
            strncpy(clave_pr +4,"000000", 6); // para que el mensaje 1816 encuentre el mensaje 1803
            strncpy(clave_pr +10,pfi->fidatpro.fifecha0 +2, 6);
            strncpy(clave_pr +16,pfi->fidatpro.fihora00,2);
            strncpy(clave_pr +18,"0000",4); // para que el mensaje 1816 encuentre por hora;
            strncpy(clave_pr +22,"06",2);
            strncpy(clave_pr +24,pfi->fidatcab.fiiodcsb, 4);
            strncpy(clave_pr +28, pfi->fidatcab.fiioocpd, 2);
            clave_pr[30] ='C';

            fprintLog("---CLAVE DE PREVIA >%.35s<: \n",clave_pr);

            return_pr = tratar_fich(INPTLOG40,IGUAL, &reg_logo, clave_pr);

            fprintLog("retorno de buscar la previa del 1803 es >%d<\n",return_pr);

            if (return_pr == 0)
            {
                strncpy(pfi->fidatiso.fiisoctr.codopcion,"1",1); //PRJ0258542
                retorno=festado_conexion(2,pfi->fidatcab.fiioocpd, "29", pfi->fidatiso.fiisoctr.codopcion[0]); //PRJ0258542
                //retorno=festado_conexion(2,pfi->fidatcab.fiioocpd, "29", "1"); //PRJ0258542
                strncpy(pfi->fidatcab.ficodrech,"00",2);
                strcpy(pfi->fidatpro.firesfin , "00");
                strcpy(pfi->fidatpro.ficodfin , "000");
                fprintLog("CAMBIAMOS A SESION DE INTERRUPCION \n");
            }
            else if (return_pr == 101)
            {
                retorno=festado_conexion(2,pfi->fidatcab.fiioocpd, "09", pfi->fidatiso.fiisoctr.codopcion[0]); //PRJ0258542
                strncpy(pfi->fidatcab.ficodrech,"00",2);
                strcpy(pfi->fidatpro.firesfin , "00");
                strcpy(pfi->fidatpro.ficodfin , "000");
                fprintLog("CAMBIAMOS A SESION ABIERTA \n");
            }
            else
            {
                strncpy(pfi->fidatcab.ficodrech,"60",2);
                strcpy(pfi->fidatpro.firesfin , "00");
            }
            break;
        case 0: //estado inicio;
            if (opcenvoff=='0')
            {
                retorno=festado_conexion(2,pfi->fidatcab.fiioocpd, "09", pfi->fidatiso.fiisoctr.codopcion[0]); //PRJ0258542
                strncpy(pfi->fidatcab.ficodrech,"00",2);
                strcpy(pfi->fidatpro.firesfin , "00");
                strcpy(pfi->fidatpro.ficodfin , "000");
            }
            else
            {
                if (opcenvoff=='1')
                {
                    retorno=festado_conexion(2,pfi->fidatcab.fiioocpd, "29", pfi->fidatiso.fiisoctr.codopcion[0]); //PRJ0258542
                    strncpy(pfi->fidatcab.ficodrech,"00",2);
                    strcpy(pfi->fidatpro.firesfin , "00");
                    strcpy(pfi->fidatpro.ficodfin , "000");
                }
                else
                {
                    strncpy(pfi->fidatcab.ficodrech,"60",2);
                    strcpy(pfi->fidatpro.firesfin , "00");
                }
            }
            break;
        default:
            strncpy(pfi->fidatcab.ficodrech,"12",2);
            strcpy(pfi->fidatpro.firesfin , "00");
    }//fin estado_actual;

    fprintLog("TERMINAMOS LA SESION \n");
    //Finaliza para que pase a la siguiente funcion de tabla de operaciones que sera ftrataeo
    return(1);
}

/***********************************************************************
  FUNCION   fct_trata_informativo

  DESCRIPCION : tratamiento de respuesta a mensaje informativo

     retorna :     1 . operacion aceptada
                  -1 . operacion denegada
***********************************************************************/

int fct_trata_informativo(formato_interno *pfi)
{
    char respuesta[4];
    int nrespuesta; 
    char opcenvoff;
//CGH-INI
    int retorno;
//CGH-FIN

    strncpy(respuesta, pfi->fiisocom.respusal+1,2);
    respuesta[2]='\0';

    nrespuesta=atoi(pfi->fiisocom.respusal);

    opcenvoff = pfi->fidatiso.fiisoctr.codopcion[0];

    switch (nrespuesta)
    {
        case 0:/* ACEPTADO*/
            retorno=festado_conexion(2,pfi->fidatcab.fiioocpd, "09", opcenvoff); 
            break;
        case 1:/* SESION EN RECUPERACIÓN */
            retorno=festado_conexion(2,pfi->fidatcab.fiioocpd, "69", opcenvoff);
            break;
        case 2:/* SESION ACTIVA CON TRAFICO INTERRUMPIDO */
            retorno=festado_conexion(2,pfi->fidatcab.fiioocpd, "29", opcenvoff);
            break;
        /*
        aqui falta mas casos por validar 12,60,99 para el futuro
        mirar el protocolo SIO en los mensajes de INFORMATIVO MCD7
        */
        case 99: /* NO ACEPTADO POR OTRAS CAUSAS */ 
            retorno=festado_conexion(2,pfi->fidatcab.fiioocpd, "99", opcenvoff);
            fgraba_incidencia_cp(pfi->fiisocom.respusal,pfi->fidatpro.finumope, "No aceptado TST 4B        1817         ");
            break;
        case 60: /* no aceptado por detectarse errores en el MCD7P */
            //retorno = festado_conexion(2,pfi->fidatcab.fiioocpd,"99",opcenvoff); // no se modifica el estado de la sesion
            fgraba_incidencia_cp(pfi->fiisocom.respusal,pfi->fidatpro.finumope, "No aceptado TST 4B        1817         ");
            break;
        case 12: /* sesion no esta abierta */ 
            retorno = festado_conexion(2,pfi->fidatcab.fiioocpd,"00",opcenvoff);
            fgraba_incidencia_cp(pfi->fiisocom.respusal, pfi->fidatpro.finumope, "No aceptado TST 4B        1817         ", NULL, NULL, NULL);
            break; 
    } 

    return(1);
}

