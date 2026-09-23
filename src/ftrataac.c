/***
   FUNCION: FTRATAAC.C   
   DESCRIPCION: Funcion de tratamiento de una operacion de cierre
    hacia RED que ha sido contestada
    ***/

/**********************************************/
/***   funcionAC: ACTUALIZA CIERRE           ***/ 
/**********************************************/
int funcionac(formato_interno *pforitn)
{

    int retorno;
    FILE  *pf=(FILE *) NULL;

    NPSPARS  regpars;
    NPSTOTO  regtot;

    char lee_clave[6];
    char clave_est[9];

    typedef struct clave_toto
    {
        char  tosubsis[2];     /*  Codigo subsistema (clave)  como solo tenemos un resolutor y solo hay unos totales al ser
                                      aplicacion emisor, utilizamos el registro de totales que hay para el subsistema 03 subaplicacion
                                       "00" y orides Netplus "00" */

        char  tosubapl[2];     /*  Codigo subaplicacion (clave) */

        char  tosesion[3];     /*  Numero de sesion (clave) */
        char  toorides[2];     /*  Origen/destino (clave)   */
        char  tofecses[6];     /*  Fecha de Sesion  */
        char  tocanals[2];     /*  Canal SIO    */
        char  tofinal[1];      /*  delimitador del final de la estructura */
    } clave_toto;


    int     cod_err =0;


    ////////////////////////////////////////////////////////////////////////////////////////
    fprintLog(nomProg, "%.6s *** FTRATAAC ***\n", pforitn->fidatpro.finumope);

    fprintLog(nomProg, "origen  : >%.2s<\n", pforitn->fidatpro.fiorigen);
    fprintLog(nomProg, "fisubori: >%.2s<\n", pforitn->fidatpro.fisubori);

    //////// ACTUALIZA NPTPARS ///////////////////////////////

    strncpy(lee_clave, pforitn->fidatpro.fiorigen,2);
    strncpy(lee_clave + 2, pforitn->fidatpro.fisubori,2);

    if(strncmp(pforitn->fiisocom.respusal, "500", 3) == 0) //conciliacion ha cuadrado
    {
        fprintLog(nomProg,"ha cuadrado\n");
        strncpy(lee_clave + 4,"1",1);
   }
   else 
   if(strncmp(pforitn->fiisocom.respusal, "501", 3) == 0) //conciliacion ha descuadrado
   {
       fprintLog(nomProg,"ha descuadrado\n");
        strncpy(lee_clave + 4,"2",1);
   }
   else
   {
       fprintLog(nomProg, "conciliacion no disponible\n"); 
       strncpy(lee_clave + 4,"3",1);
   }
   lee_clave[5] = '\0';

   fprintLog(nomProg, "Clave para nptpars :%s Actualiza paconcil\n",lee_clave);


   retorno=tratar_fich("NPTPARS", PACONCIL, NULL, lee_clave);
   fprintLog(nomProg, "despues de ejecutar PACONCIL (reescribir nptpars) :>%d<\n",retorno);

   if (retorno == 101) // no existe el registro
        return(finalizar(98));
   //----------------------------------------------
   
   //ha resuelto sisma pq es una comunicación
   strncpy(pforitn->fidatpro.firesfin, "00", 2);  
   strncpy(pforitn->fidatpro.ficodfin, "000", 3); 

    return(finalizar(11));
}
