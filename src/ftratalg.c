//PRJ0257005 - Revisado tokenizacion
/***
   DESCRIPCION: FUNCION QUE GRABA EN EL LOG LA OPERACION
                PENSADA PARA LAS NOTIFICACIONES RECIBIDAS QUE  NO REQUIEREN RESPUESTA
				Y QUE SOLO QUEREMOS DEJAR REGISTRADAS EN EL SISTEMA  
***/

 int retorno;
 int fverfi(formato_interno fi);
//int lee_cuenta_bloquea(formato_interno *pforitn, NPSCUEN * pregcuen);
/***************************************************************/   
/***   funcionlg: autorizacion diferidos con tarjeta propia  ***/
/***************************************************************/

int funcionlg(formato_interno * pforitn)
{
    fprintLog(nomProg,"******************\n");
    fprintLog(nomProg,"***  FTRATALG  ***\n");
    fprintLog(nomProg,"******************\n\n");

    strcpy(pforitn->fidatpro.firesfin,"00");
    strcpy(pforitn->fidatpro.ficodfin, "000");
	
	fverfi(*pforitn);
    
    retorno = fgraba_log(pforitn);    
    if(retorno)
	{
		//Añadido para Monitorizacion SISMA 
		fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Escritura BBDD tabla NPTLOGO codigo de error 222\n");

		fgraba_incidencia_cp("222",pforitn->fidatpro.finumope,NULL,NULL);   
		// se graba incidencia pero no se fuerza EO pq no contestamos
    }
	
    return(finalizar(11)); //continuar con el proceso
}

