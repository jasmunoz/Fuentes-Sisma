//PRJ0257005 - Revisado tokenizacion
//PRJ0247167 - Revisado ampliación P48
/***************************************************************
 *  funcion para buscar la previa de anulaciones                 *
 ***************************************************************/

int funcionpr(formato_interno * pforitn)
{
  int retorno;
  char longP[3];
  char clave_log[21];
  NPSLOGO reg_logo;
  tpclav02 * loclavpr;
  iso_autorizaciones * fiautorizacion;  
  iso_autorizaciones * loautorizacion;
  formato_interno auxlogforitn;

  char clave_pr[35];
  int ll;


#ifdef TRAZA
  fprintLog(nomProg,"*********************************\n");
  fprintLog(nomProg,"*** BUSCAR PREVIA ANULACIONES ***\n");
  fprintLog(nomProg,"*********************************\n\n");
#endif

  
 loclavpr = (tpclav02 *) &reg_logo.loclavpr;
 fiautorizacion = &pforitn->fidatiso.fiisoaut;

 fverfi(*pforitn);
 
 //guardamos datos para posible incidencia
 strcpy(reginci.innomfic, "trataPR"); 
 strncpy(reginci.innumope, pforitn->fidatpro.finumope, 6);
 
 memset(clave_pr, ' ', sizeof(clave_pr));
 
 /***   montamos la clave de log para buscar la previa   ***/
 memcpy(clave_pr, fiautorizacion->fiaut056.datos,
        atoi(fiautorizacion->fiaut056.longitud));
		
 clave_pr[35]='\0';
 strncpy(clave_pr + strlen(clave_pr),"               ",(35-strlen(clave_pr)));
 fprintLog(nomProg,"sizeof(clave_pr) %d\n", sizeof(clave_pr));
 fprintLog(nomProg,"clave_pr **********12345678901234567890123456789012345<----\n");
 fprintLog(nomProg,"clave_pr **********%.*s<----\n",
 sizeof(clave_pr), clave_pr);
//   fhexdump(fichsal, clave_pr, sizeof(clave_pr));

   
 retorno = 0;

 retorno = tratar_fich("PREVIAA",IGUAL,&reg_logo,clave_pr);

  fprintLog(nomProg,"IGUALPREVIAA retorno=%d\n",retorno);
  
   /* la operacion previa no existe en el log actual ni en el anterior */
 //20160624- llegan anulaciones a la vez que la previa que no hemos grabado aun
 //la primera vez no contestamos para que redsys la repita
 //si es una repetición ya si contestamos denegando con 223 //PTASK0022158 - se reemplaza 223 por 480
 if (retorno)
 {
  /*****************************
  comprobar antes que tipo de mensaje es 0440  ANULACIONES AUTOMATICAS 7 DIAS
  *************************/
    fprintLog(nomProg,"previa no encontrada, ret=%d\n",retorno);
	if (strncmp(pforitn->fiisocom.fiiso000, "1420",4)==0)
	{
	   fprintLog(nomProg,"es 1420 no repeticion - no contestamos\n",retorno);
       strcpy(reginci.incoderr, "480"); //previa no encontrada //PTASK0022158 - se reemplaza 223 por 480
	   strcpy(coderror,"223");
	   return(finalizar(97));
	}
	else
	{
       strcpy(pforitn->fidatpro.ficodfin,"480"); //PTASK0022158 - se reemplaza 223 por 480
       strcpy(pforitn->fidatpro.firesfin,"00");
	   fprintLog(nomProg,"repeticion - codfin 480\n",retorno); //PTASK0022158 - se reemplaza 223 por 480
      return(finalizar(98));
    }
 }

   /* la operacion previa ya esta anulada */
 if (reg_logo.loesanul[0] == '*')
 {
  fprintLog(nomProg,"previa ya esta anulada\n");
  strcpy(pforitn->fidatpro.ficodfin,"224");
  //strcpy(pforitn->fidatpro.ficodfin,"000");
  strcpy(pforitn->fidatpro.firesfin,"00");
  return(finalizar(98));
 }
 
memcpy(&auxlogforitn, (formato_interno *)reg_logo.loforitn, sizeof(formato_interno));
fprintLog(nomProg,"previa numope >%.6s< iso000 >%.4s<\n"
          , auxlogforitn.fidatpro.finumope, auxlogforitn.fiisocom.fiiso000);
// se guarda la operacion previa
sprintf(pforitn->fidatpro.finumprv,"%.6s",auxlogforitn.fidatpro.finumope);
// si la previa no era una operación contable, la anulación tampoco lo es
if(auxlogforitn.fiisocom.fiiso000[1]!= '2') //si la posición 2 del tipo msj no es '2' no es contable
  pforitn->fidatpro.finototal[0] = 'N';   //pongo marca para no totalizar

loautorizacion = &(auxlogforitn.fidatiso.fiisoaut);
// PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud de finumPAN
strncpy(longP,loautorizacion->finumPAN.longitud, 2);
longP[2]='\0';

// 20160601 - si esta en vuelo no contestamos a redsys para que nos repita la operación.
if(strncmp(auxlogforitn.fidatpro.ficodfin, "   ", 3)==0)
 {
    fprintLog(nomProg,"previa en vuelo, no contesto para que manden repeticion\n");
	return(97);
 }


// si la operacion a anular no esta autorizada, no se procesa 
if(strncmp(auxlogforitn.fidatpro.ficodfin, "000", 3))
 {
    fprintLog(nomProg,"previa no autorizada, ficodfin >%.3s<\n",
           auxlogforitn.fidatpro.ficodfin);

    fgraba_incidencia_cp("225", pforitn->fidatpro.finumope, "trataPR", NULL);


  strcpy(pforitn->fidatpro.ficodfin,"225");
  strcpy(pforitn->fidatpro.firesfin,"00");
  return(finalizar(98));
 }

/* si no coinciden los datos de */
/*             importe  */
/*             tarjeta                      */
/*             codigo de proceso            */

/* no se procesa */

// PRJ0202598 cambio bin de 6 posiciones 8 no afecta la longitud total de finumPAN ni fipancif
 if(strncmp(fiautorizacion->fiaut004, loautorizacion->fiaut004, 12) ||
    strncmp(fiautorizacion->finumPAN.longitud, loautorizacion->finumPAN.longitud, 2)  ||
	strncmp(pforitn->fidatpro.fipancif, loautorizacion->finumPAN.datos, atoi(longP)) ||
    strncmp(fiautorizacion->fiaut003.dig12, loautorizacion->fiaut003.dig12,2)  ||
	strncmp(fiautorizacion->fiaut003.dig34, loautorizacion->fiaut003.dig34,2)  ||
    strncmp(fiautorizacion->fiaut003.dig56, loautorizacion->fiaut003.dig56,2)  ||
	strncmp(fiautorizacion->fiaut049, loautorizacion->fiaut049,3)  ||
	( !strncmp(pforitn->fiisocom.fiiso000,"1424",4) &&  
	   strncmp(fiautorizacion->fiaut048.comun01.fiiso4813,
	          loautorizacion->fiaut048.comun01.fiiso4813,3)
	))
 {
/* datos de la original distintos de los de la anulacion */
#ifdef TRAZA
   fprintLog(nomProg,"datos no coinciden \n");
   fprintLog(nomProg,"importe op>%.12s< pr>%.12s\n<",fiautorizacion->fiaut004, loautorizacion->fiaut004);
   fprintLog(nomProg,"lonpan  op>%.2s< pr>%.2s\n<",fiautorizacion->finumPAN.longitud, loautorizacion->finumPAN.longitud);
   fprintLog(nomProg,"pan     op>%.16s< pr>%.16s\n<",pforitn->fidatpro.fipancif, loautorizacion->finumPAN.datos);
   fprintLog(nomProg,"codpro  op>%.2s%.2s%.2s< pr>%.2s%.2s%.2s\n<",
                fiautorizacion->fiaut003.dig12, fiautorizacion->fiaut003.dig34,fiautorizacion->fiaut003.dig56,  
				loautorizacion->fiaut003.dig12, loautorizacion->fiaut003.dig34,loautorizacion->fiaut003.dig56);
   fprintLog(nomProg,"moneda op>%.3s< pr>%.3s\n<",fiautorizacion->fiaut049, loautorizacion->fiaut049);
   fprintLog(nomProg,"48.13>%.3s< pr>%.3s<\n",
            fiautorizacion->fiaut048.comun01.fiiso4813, 
			loautorizacion->fiaut048.comun01.fiiso4813);
#endif   
       
   // PRJ0202598 cambio bin de 6 posiciones 8 no cambia la longitud total de finumPAN ni de fipancif
   if(strncmp(loautorizacion->fiaut048.comun01.flagImpParcial, "1", 1) == 0
   && strncmp(fiautorizacion->finumPAN.longitud, loautorizacion->finumPAN.longitud, 2) == 0
   && strncmp(pforitn->fidatpro.fipancif, loautorizacion->finumPAN.datos, atoi(longP)) == 0
   && strncmp(fiautorizacion->fiaut003.dig12, loautorizacion->fiaut003.dig12,2) == 0
   && strncmp(fiautorizacion->fiaut003.dig34, loautorizacion->fiaut003.dig34,2) == 0
   && strncmp(fiautorizacion->fiaut003.dig56, loautorizacion->fiaut003.dig56,2) == 0
   && strncmp(fiautorizacion->fiaut049, loautorizacion->fiaut049,3)== 0 )
   {
     fprintLog(nomProg,"Anulacion previa con flag autorizacion parcial\n");	
     fprintLog(nomProg,"pr>importeParcial >%.12s<\n<",
	                    loautorizacion->fiaut048.comun01.importeParcial);	
     fprintLog(nomProg,"op>fiaut051 >%.3s<\n<", fiautorizacion->fiaut051);	
     fprintLog(nomProg,"op>fiaut006 >%.12s<\n<", fiautorizacion->fiaut006);		 
     if( (strncmp(fiautorizacion->fiaut049, "978", 3) == 0
	   && strncmp(fiautorizacion->fiaut004, loautorizacion->fiaut048.comun01.importeParcial, 12) == 0)
     || ( strncmp(fiautorizacion->fiaut051, "978", 3) == 0
	   && strncmp(fiautorizacion->fiaut006, loautorizacion->fiaut048.comun01.importeParcial, 12) == 0))
              fprintLog(nomProg,"Anula importe parcial >%.12s<\n<",
	                    loautorizacion->fiaut048.comun01.importeParcial);	 
	}		       
   else
   {
     // SI NO COINCIDEN LOS DATOS, NO PROGRESA LA ANULACIÓN	   
     strcpy(pforitn->fidatpro.ficodfin,"263");
     strcpy(pforitn->fidatpro.firesfin,"00");
     return(finalizar(98));
   }	 
 }

#ifdef TRAZA 
     fprintLog(nomProg,"PR marca original *\n");
#endif
   /* marca la original en el log como anulada */

   //reg_logo.loesanul[0] = '*';
   fprintLog(nomProg,"reg_logo.lofecha0 >%.8s<\n", reg_logo.lofecha0);
   fprintLog(nomProg,"reg_logo.lohora00 >%.6s<\n", reg_logo.lohora00);
   fprintLog(nomProg,"reg_logo.lonumope >%.6s<\n", reg_logo.lonumope);
   
   strncpy(clave_log    ,reg_logo.lofecha0,8);
   strncpy(clave_log+8  ,reg_logo.lohora00,6);
   strncpy(clave_log+14 ,reg_logo.lonumope,6);
   clave_log[20]='\0';

   fprintLog(nomProg,"clave_log >%s<\n", clave_log);
   fprintLog(nomProg,"opcion tratar_fich %d\n", REESCRIBIR);
   
   retorno = tratar_fich ("ANULAOP", REESCRIBIR, NULL, &clave_log);

   if(retorno)
    {  // error al reescribir en el log 
      fprintLog(nomProg,"error al reescribir %d\n", retorno);
      strcpy(pforitn->fidatpro.ficodfin,"222");
      strcpy(coderror,"222");
      strcpy(pforitn->fidatpro.firesfin,"00");
      return(finalizar(98));
    }

 /* la operacion de anulacion ha superado las validaciones */
 /* movemos campos de la previa a la anulacion             */
 /* se rellenan aqui los campos no cumplimentados, y que se necesitan
   para tratamientos posteriores (envio a semp).
    El punto de venta no manda el importe de la anulacion, ni el tipo
   de cuenta, ni el campo p56 informado segun el iso/price       */

  strncpy(fiautorizacion->fiaut018, loautorizacion->fiaut018,4);
  strncpy(fiautorizacion->fiaut022, loautorizacion->fiaut022,12 );
  strncpy(fiautorizacion->fiaut026, loautorizacion->fiaut026,4);

 strncpy(fiautorizacion->fiaut042, loautorizacion->fiaut042, 15);
 fiautorizacion->fiaut042[15]='\0';
 strncpy(fiautorizacion->fiaut043, loautorizacion->fiaut043, 40);
 fiautorizacion->fiaut043[40]='\0';
 strncpy(fiautorizacion->fiaut003.dig12, loautorizacion->fiaut003.dig12, 2); 
 fiautorizacion->fiaut003.dig12[2]='\0';
 strncpy(fiautorizacion->fiaut003.dig34, loautorizacion->fiaut003.dig34, 2);
 fiautorizacion->fiaut003.dig34[2]='\0';
 strncpy(fiautorizacion->fiaut003.dig56, loautorizacion->fiaut003.dig56, 2);
 fiautorizacion->fiaut003.dig56[2]='\0';

/**** PRICE ***/
/***   para que las anulaciones queden informadas
       con el terminal donde se originaron */
  strncpy(fiautorizacion->fiaut048.comun01.termin, loautorizacion->fiaut048.comun01.termin, 12);
  fiautorizacion->fiaut048.comun01.termin[12]='\0';
  
// IMPORTE AUTORIZADO DE LA ORIGINAL EN CASO DE QUE FUERA AUTORIZACION PARCIAL
  strncpy(fiautorizacion->fiaut048.comun01.importeParcial, loautorizacion->fiaut048.comun01.importeParcial, 12);
  fiautorizacion->fiaut048.comun01.importeParcial[12]='\0'; 


 return(finalizar(11));
}
