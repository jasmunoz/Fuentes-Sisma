//PRJ0257005 - Revisado tokenizacion
/*************************************************************************
   funcion de tratamiento de diferido
   devuelve 2 - error o no contestado, provoca que se cierre el semaforo 
            1 - no hay mas diferidos
			0 - contestado, seguir  
			
**************************************************************************/
int trata_diferido()
{
  int atoi(const char *str); 
  int retorno, respondido,repeticiones; 
  int tipomsg;
  char cl_dif[25];
  int nocaducada=1;
  int calculo=0;
  struct tm_dec tm;
  char diahoy[2];
  char hora[3];
  char minu[3];
  char seg[3];
  char clavedife[25];
  int nocontesta=0;
    
  void *pforitnaux;
  formato_interno *pforitndat;

/*   lee diferido  */
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"lee fichero de diferidos  \n");
#endif
   strncpy(cl_dif, subsistema_general,2);
   strncpy(cl_dif+2, Csubapl,2);
   strncpy(cl_dif+4,"00000000000000000000", 20);
   cl_dif[24]='\0';

#if defined (TRAZA) && (TRAZA > 100)
   fprintLog(nomProg,"cl_dif >%s<\n",cl_dif);
#endif
   retorno = tratar_fich("NPTDIFE",MAYORIGUAL,&regdife, &cl_dif);
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"leido retorno=%d \n",retorno); 
#endif

   pforitnaux = regdife.diforitn;
   pforitndat = pforitnaux ;
   nocaducada=1;
 
   while (nocaducada)
   {
     if((retorno) ||
         memcmp(regdife.disubsis, cl_dif, 2) ||
        memcmp(regdife.disubapl, cl_dif+2, 2)) /* no hay diferidos */
     {
#if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg,"No hay diferidos\n");
#endif
        return(1);
     }
     if(retorno) /* error acceso a fichero de diferidos */
     {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"error no contemplado en lectura de diferidos\n");
#endif
       strcpy(coderror, "256");
       strcpy(reginci.innomfic, "nptdife");
       return (2);
     }

#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg,"Encontrado diferido\n");
     fprintLog(nomProg,"subsis>%.2s%.2s< numope>%.6s< fecha>%.8s%.6s iso000>%.4s<\n",
           regdife.disubsis,
           regdife.disubapl, 
           regdife.dinumope,
           regdife.difecha0,
           regdife.dihora00,
           pforitndat->fiisocom.fiiso000);
		   
     if(!strncmp(pforitndat->fiisocom.fiiso000, "1520", 4))
     {
         fprintLog(nomProg,"fitot029: %s /\t ",
             pforitndat->fidatiso.fiisotot.fitot029);
         fprintLog(nomProg,"fitot028: %s%s%s /\n",
             pforitndat->fidatiso.fiisotot.fitot028.aa,
             pforitndat->fidatiso.fiisotot.fitot028.mm,
             pforitndat->fidatiso.fiisotot.fitot028.dd);
     }
#endif

     pforitndat->fiisocom.fiiso000[4]= '\0';
     tipomsg=atoi(pforitndat->fiisocom.fiiso000);

     if (tipomsg != 1200) /* Venta Real */
        nocaducada=0;
     else
     {
      /* Calcular si ha caducado */
         fechorsys(&tm);
         sprintf(diahoy, "%02d", tm.tm_mday);

         if (strcmp(diahoy, (regdife.difecha0 + 6)) != 0);
         tm.tm_hour=24;
         calculo=tm.tm_hour*3600+tm.tm_min*60+tm.tm_sec;

         hora[0] = regdife.dihora00[0];
         hora[1] = regdife.dihora00[1];
	 hora[2]='\0';
         minu[0] = regdife.dihora00[2];
         minu[1] = regdife.dihora00[3];
	 minu[2]='\0';
         seg[0]  = regdife.dihora00[4];
         seg[1]  = regdife.dihora00[5];
	 seg[2]='\0';

         calculo-= atoi(hora)*3600+atoi(minu)*60+ atoi(seg);

/* SE ASIGNA UN VALOR A TIEMPO_ESPERA PROVISIONAL PENDIENTE DE
DEFINIR O VER CUAL ES*/
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg,"Lleva %d y debe esperar %d\n",calculo,TIEMPO_ESPERA);
#endif
         if (calculo < TIEMPO_ESPERA) /* No ha caducado, tengo que leer otra */
         {
            retorno = tratar_fich("NPTDIFE",SIGUIENTE,&regdife, NULL);
            pforitnaux = regdife.diforitn;
            pforitndat = pforitnaux ;
         }
         else
           nocaducada=0;
     }
   } /* Del while(nocaducada) */

   repeticiones=5;
   respondido = 0;
   while((repeticiones>0) && (respondido != 1))
   {
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg,"trata_diferido envio diferidos. repeticiones=%d\n",
                                   repeticiones);
#endif
     retorno=envia_diferido(pforitndat);
     if(!retorno)        /*respondido */
     {
        respondido = 1;
        repeticiones=0;
        break;
     }
     else
     {
       if ((psvm+rtrata_svm)->estado==PARADA) /* orden de cierre */
       {
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg,"orden de cierre \n");
#endif
         strcpy(coderror, "999");
         strcpy(reginci.innomfic, "nptdife");
         return (2);
       }
	   if(retorno== -2)
	   { //error fatal
	      return(2);
	   }
	   
	   //esto es cuando retorno es 1, para que repita
       switch (tipomsg)
       {
       case 1120:
          strcpy(pforitndat->fiisocom.fiiso000,"1121");
          break;
       case 1220:
         strcpy(pforitndat->fiisocom.fiiso000,"1221");
         break;
       case 1420:
         strcpy(pforitndat->fiisocom.fiiso000,"1421");
       }
     }
     repeticiones--;
   }/* fin While repeticiones*/

  ///////////////////NO RESPONDIDO ////////////////////////////////////
  if (respondido == 0) /* graba en descuadres */
  {
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg,"No respondido, se graba en descuadres numope>%.6s< iso000>%.4s<\n",
	        pforitndat->fidatpro.finumope,
            pforitndat->fiisocom.fiiso000);
#endif
      //encriptar la tarjeta antes de guardarla. En diferidos no se guarda encriptada, en descuadres si
      // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud total de la tarjeta finumPAN ni cifrada fipancif
	  pforitndat->fidatiso.fiisoaut.finumPAN.longitud[2]='\0';
	  strncpy(pforitndat->fidatiso.fiisoaut.finumPAN.datos,pforitndat->fidatpro.fipancif,
	           atoi(pforitndat->fidatiso.fiisoaut.finumPAN.longitud));
      retorno = tratar_fich("NPTDSCD",ESCRIBIR,&regdife,NULL);
	  //20160704 - graba en descuadres, no hace falta grabar incidencia
      //fgraba_incidencia_cp("203", NULL, NULL, "TRTDIFER");	  
	  nocontesta=1;
   } 
   else
   {//////////////// SI RESPONDIDO ////////////////////////////////////////////////
#if defined (TRAZA) && (TRAZA > 1)
      fprintLog(nomProg,"Diferido enviado y respondido numope>%.6s< iso000>%.4s<\n",
	        pforitndat->fidatpro.finumope,
            pforitndat->fiisocom.fiiso000);
#endif
      /////20160302 - si se ha respondido pero no ha sido aceptado y no es un test lo grabamos en descuadres
	  //// 20160704 - excepto codfin 224 previa ya anulada
	  ////          - excepto codfin 501 conciliacion descuadrada
	  if(strncmp(pforitndat->fidatpro.ficodfin, "000", 3) !=0 // ok
	  && strncmp(pforitndat->fidatpro.ficodfin, "224", 3) !=0 //previa ya anulada
	  && strncmp(pforitndat->fidatpro.ficodfin, "501", 3) !=0 //conciliacion descuadrada
	  && strncmp(pforitndat->fiisocom.fiiso000,"18",2)!=0)
	  {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"Diferido no aceptado. Se graba en descuadres numope>%.6s< iso000>%.4s<\n",
	        pforitndat->fidatpro.finumope,
            pforitndat->fiisocom.fiiso000);
#endif
        //encriptar la tarjeta antes de guardarla. En diferidos no se guarda encriptada, en descuadres si
        // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud total de la tarjeta finumPAN ni a la cifrada fipancif 
	    pforitndat->fidatiso.fiisoaut.finumPAN.longitud[2]='\0';
	    strncpy(pforitndat->fidatiso.fiisoaut.finumPAN.datos,pforitndat->fidatpro.fipancif,
	           atoi(pforitndat->fidatiso.fiisoaut.finumPAN.longitud));
        retorno = tratar_fich("NPTDSCD",ESCRIBIR,&regdife,NULL);
		nocontesta=0;
	  }
   }   
       /* borra diferido */
   fprintLog(nomProg,"Borra el diferido\n"); 
   strncpy(clavedife, regdife.disubsis,2);
   strncpy(clavedife+2, regdife.disubapl,2);
   strncpy(clavedife+4, regdife.difecha0,8);
   strncpy(clavedife+12, regdife.dihora00, 6);
   strncpy(clavedife+18, regdife.dinumope, 6);
   clavedife[24]='\0';
      
   retorno = tratar_fich("NPTDIFE",BORRAR,NULL, clavedife); 
   if(retorno) /* error a borrar */
   {
#if defined (TRAZA) && (TRAZA > 1) 
      fprintLog(nomProg,"error borrar diferidos\n");
#endif
      fgraba_incidencia_cp("256", NULL, NULL, "TRTDIFER");
      return(2);
    }
   
   if(nocontesta)
      return(2);
   else
      return(0);
}
