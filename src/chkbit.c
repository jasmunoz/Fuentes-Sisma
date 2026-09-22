//PRJ0257005 - Revisado tokenizacion 
//PRJ0247167 - Revisado ampliación P48
/*************************************************************************
   FUNCION  FCHEQBIT        llamada por traductor  

   DESCRIPCION: Funcion que configura y comprueba los mapas de bits
                recibidos y enviados por el nodo, segun protocolo. 

   AREA DE PASO :
- entero con el sentido de traduccion    1.- nodo--->netplus
                                         2.- netplus--->nodo
- puntero a char con el mensaje en formato nodo
- puntero a char con la 'plantilla'(condiciones de cada bit para ese mensaje)
- puntero a char con el mapa de bits primario expandido
- puntero a char con el mapa de bits secundario expandido
- puntero a tipo formato_intermedio con el mensaje en datos aplicacion
- puntero a entero con codigo de error de la traduccion
- puntero a entero con el numero de bit que ha dado el error
*************************************************************************/

int nlow_values (char *c,int n);

char * global_intermedio;
int global_long_efec;
int bizum = 0; //Bizum - indica si es operativa bizum = 1
char codpro[6]; //Bizum - para guardar el P03

/***  --------- prototipos de funciones locales  ----------------  ***/
int     fclase_condicion(int condicion);
int     fmirar_condi1(int condicion_num,int ll,
                        char bitmap[128],char *condicion_alf);

int     fmirar_condi2(int condicion_num, int ll, formato_intermedio *mensaje_inter);

int     fcheqbit(int sentido, char *plantilla,
                 char *bitmapp, char *bitmaps,
                 formato_intermedio *mensaje_inter,
                 int *error_code, int *error_bit, int suba_iso, char * red)

{
  int   ll, tope_bm, retorno, condicion_num;
  char  bitmapt[129];
  char  condicion_alf[4];
  char *interchar = (char*) mensaje_inter;
  int   long_campol;
  int   long_efec;


 global_intermedio = interchar;

#if defined (TRAZA) && (TRAZA > 10) 
 fprintLog(nomProg,"========== fcheqbit sentido %d\n", sentido);
#endif 
	//fprintLog(nomProg," Plantilla entrada>%s<\n", plantilla); //PRJ0258542
	//fprintLog(nomProg," Mensaje   entrada>%s<\n", interchar); //PRJ0258542
	//fprintLog(nomProg," Bitmap 1  entrada>%.64s<\n", bitmapp); //PRJ0258542
	//fprintLog(nomProg," Bitmap 2  entrada>%.64s<\n", bitmaps); //PRJ0258542
	/*
	//PRJ0258542 - INI
	fprintf(stdout,"Dentro de chkbit mensaje >%.4s< \n", mensaje_inter->adipre000);
	fprintf(stdout,"  Valor del P11          >%.6s< \n", mensaje_inter->adipre011);
	fprintf(stdout,"  Valor del P12          >%.12s< \n", mensaje_inter->adipre012);
	fprintf(stdout,"  Valor del P24          >%.3s< \n", mensaje_inter->adipre024);
	fprintf(stdout,"  Valor del P25          >%.4s< \n", mensaje_inter->adipre025);
	fprintf(stdout,"  Valor del P33          >%.2s< >%.11s< \n", mensaje_inter->adi033l, mensaje_inter->adi033d);
	fprintf(stdout,"  Valor del P39          >%.3s< \n", mensaje_inter->adipre039);
	fprintf(stdout,"  Valor del P53          >%.24s< \n", mensaje_inter->adipre053);
	fprintf(stdout,"  Valor del S93          >%.2s< >%.11s< \n", mensaje_inter->adi093l, mensaje_inter->adi093d);
	fprintf(stdout,"  Valor del S94          >%.2s< >%.11s< \n", mensaje_inter->adi094l, mensaje_inter->adi094d);
	fprintf(stdout,"  Valor del S96          >%.3s< >%.360s< \n", mensaje_inter->adi096l, mensaje_inter->adi096d);
	fprintf(stdout,"  Valor del S128         >%.8s< \n", mensaje_inter->adipre128);
	//PRJ0258542 - FIN
	*/

	switch(sentido)
 	{
	case 1:      /***--------comprueba el bitmap recibido-------------***/

 		/*---se construye asi para que coincida el bit con la posicion del array---*/
   	bitmapt[0] = '-';
   	strncpy(bitmapt+1,bitmapp,64);
   	strncpy(bitmapt+65,bitmaps,64);
   
   	tope_bm = ((bitmapt[1]) == '1') ? 129 : 65;

   	for (ll=1 ; ll<tope_bm ; ll++)
   	{
    	strncpy (condicion_alf,plantilla + ((ll-1)*3),3);
    	condicion_alf[3] = '\0';
    	condicion_num = atoi(condicion_alf);
   
    	retorno = fclase_condicion(condicion_num);
			//fprintLog(nomProg, "II >%d< condicion >%d<\n", ll, condicion_num);
			//printf("bit <%d> bitmap <%c> condicion <%s> retorno <%d>\n", ll, bitmapt[ll], condicion_alf, retorno);    
    	if (retorno == DESCONOCIDO)
    	{
#if defined (TRAZA) && (TRAZA > 1)	
     		printf("*** Modulo chkbit bit <%d> bitmap <%c> condicion <%s> retorno <%d>\n", ll, bitmapt[ll], condicion_alf, retorno);  //PRJ0257005
     		fprintLog(nomProg, " * * * Condicion ->%d<-\n", condicion_num);
#endif	 
     		*error_code = 8;
     		*error_bit = ll;
     		return(-1);
    	}

    	if (bitmapt[ll] == '1')      /***--- comprueba unos ---***/
    	{
     		if ((retorno == OBLIGATORIO) || (retorno == OPCIONAL) || (retorno == CONDICIONAL))
     		{   /***    correcto   ***/
     		}

     		if ((retorno == PROHIBIDO) || (retorno == NO_USADO))
     		{
#if defined (TRAZA) && (TRAZA > 1)	
     			printf("*** Modulo chkbit bit <%d> bitmap <%c> condicion <%s> retorno <%d>\n", ll, bitmapt[ll], condicion_alf, retorno);   //PRJ0257005 
     			fprintLog(nomProg, "PROHIBIDO O NO_USADO condicion ->%d<-\n", condicion_num);
#endif	 	 
      		*error_code = 2;
      		*error_bit = ll;
      		return(-1);
     		}
    	}
    	else                       /***--- comprueba ceros ---***/
    	{
     		if ((retorno == PROHIBIDO) || (retorno == NO_USADO) || (retorno == OPCIONAL))
     		{   /***    correcto   ***/
				}

     		if (retorno == OBLIGATORIO)
     		{
 					/*
      		if (((suba_iso == 3)||(suba_iso == 5)||(suba_iso == 4))
					*/
      		if (suba_iso >= 3 && (ll==64||ll==128))
      		{
          	/* Ceca no envia el bit del MAC */
            /* Serinor no envia el bit del MAC */
      		}
      		else
      		{
       			printf("*** Modulo chkbit bit <%d> bitmap <%c> condicion <%s> retorno <%d>\n", ll, bitmapt[ll], condicion_alf, retorno);    //PRJ0257005
       			*error_code = 1;
       			*error_bit = ll;
       			return(-1);
      		}
     		}

     		if (retorno == CONDICIONAL)
     		{
					retorno = fmirar_condi1(condicion_num,ll, bitmapt,condicion_alf);
      		if (retorno == 0)  /***--    error moverlo=0    --***/
      		{
#if defined (TRAZA) && (TRAZA > 1)	  
       			printf("*** Modulo chkbit bit <%d> bitmap <%c> condicion <%s> retorno <%d>\n", ll, bitmapt[ll], condicion_alf, retorno);    //PRJ0257005
       			fprintLog(nomProg, " * * * * error condicion >%d<\n", condicion_num);
#endif	   
       			*error_code = 7;
       			*error_bit = ll;
       			return(-1);
      		}
      		else
      		{
       			strncpy(plantilla + ((ll-1)*3),condicion_alf,3);
      		}
     		}
    	}
		}      /*   fin del bucle for  del case 1 */
		break;
	case 2:     /***--------contruye un bitmap provisional-----------***/
		bitmapt[0] = '-';
    /***   pone el bit 1   ***/
   	strncpy(condicion_alf,plantilla,3);
   	condicion_alf[3] = '\0';
   	condicion_num = atoi(condicion_alf);
   	retorno = fclase_condicion(condicion_num);
   	if (retorno == OBLIGATORIO )
    	bitmapt[1] = '1';
   	else
    	bitmapt[1] = '0';
		
      /***   pone el bit 128   ***/
   	strncpy(condicion_alf,plantilla + (127*3),3);
   	condicion_alf[3] = '\0';
   	condicion_num = atoi(condicion_alf);
   
#if defined (TRAZA) && (TRAZA > 10)   
   	fprintLog(nomProg," * * * Condicion para MAC en 128 %i\n", condicion_num);  //PRJ0257005
#endif	 

   	retorno = fclase_condicion(condicion_num);
   	if (retorno == OBLIGATORIO)
   	{
    	bitmapt[128] = '1';
    	//tope_bm = 128;
   	}
   	else
   	{
    	bitmapt[128] = '0';
    //tope_bm = 65;
   	}
		if (strncmp(mensaje_inter->adipre024,"811",3) == 0 || strncmp(mensaje_inter->adipre024,"815",3) == 0 || strncmp(mensaje_inter->adipre024,"816",3) == 0) //PRJ0258542
		{
			bitmapt[128] = '1'; //PRJ0258542 - obligatorio siempre en estos mensajes de claves
			bitmapt[1] = '1'; //PRJ0258542 - obligatorio siempre en estos mensajes de claves
	   	fprintLog(nomProg," * * * Cambia a obligatorio forzado bit del 128\n");  //PRJ0258542
		}
   	//asteriscamos el tbop_bm, ya que quien debe determinar el numero de bits a revisar
   	// es el IF anteriro, que comprueba el bit[1] (condicion_alf,plantilla,3) no el 128
   	// ya que en el cierre, por ejemplo, este bit no viene activo

   	//for (ll=2 ; ll < tope_bm ; ll++)
   	for (ll=2 ; ll < 128 ; ll++)
   	{
    	strncpy(condicion_alf,plantilla + ((ll-1)*3),3);
    	condicion_alf[3] = '\0';
    	condicion_num = atoi(condicion_alf);
    	//fprintLog(nomProg, "II >%d< condicion >%d<\n", ll, condicion_num);
    	retorno = fclase_condicion(condicion_num);

     	switch (retorno)
    	{
     		case NO_USADO:
     		case PROHIBIDO:
      		bitmapt[ll] = '0';
      		break;
     		case OBLIGATORIO:
      		bitmapt[ll] = '1';
      		break;
     		case OPCIONAL:
      		bitmapt[ll] = '2';
#if defined (TRAZA) && (TRAZA > 10)	  
      		//fprintLog(nomProg, "*** Posicion >%d< condicion >%d<\n", ll, condicion_num); //PRJ0257005
#endif	  
      		break;
     		case CONDICIONAL:
      		bitmapt[ll] = '3';
      		break;
     		case DESCONOCIDO:
     		default:
#if defined (TRAZA) && (TRAZA > 1)	 
      		fprintLog(nomProg, " * * * condicion ->%d<-\n", condicion_num); //PRJ0257005
#endif	  
      		*error_code = 8;
      		*error_bit = ll;
      		return(-1);
    	}
     	/***   este bit se rellena mas adelante   ***/
    	if (ll==64)
     		continue;

    	if (bitmapt[ll] == '0')
     		continue;

    	if (flee_datoapl(ll))
    	{
     		*error_code = 3;
     		*error_bit = ll;
     		return(-1);
    	}

    	if (long_fc > 1000)
    	{
     		long_campol = long_fc - 1000;
     		strncpy(cadena_verif, interchar + off_fc - long_campol, long_campol);
     		cadena_verif[long_campol] = '\0';
     		long_efec = atoi(cadena_verif);
    	}
    	else
     		long_efec = long_fc;
	 
#if defined (TRAZA) && (TRAZA > 10)   
    	//fprintLog(nomProg,"valor parcial es %i\n", off_fc); 
#endif	

    	global_long_efec = long_efec;
	
#if defined (TRAZA) && (TRAZA > 10)	
    fprintLog(nomProg,"campo >%03d< bit->%c<- >>>%.*s<<<\n", ll, bitmapt[ll], long_efec, interchar + off_fc);
#endif	
		//Bizum - inicio
		if (ll == 3)
		{
			bizum = 0;
			strncpy(codpro, interchar + off_fc, 6);
			codpro[6] = '\0';
			fprintLog(nomProg,"----- Valor codpro leido -----> %6s y valor bizum %d\n", codpro, bizum);
			if  (!strncmp(codpro,"10",2) || !strncmp(codpro,"29",2))
			{
				fprintLog(nomProg,"----- Se activa Bizum para desactivar P48 -----\n");
				bizum = 1;
			}
			else
			{
				bizum = 0;
			}
		}
		//Bizum - final

/***   analiza campo opcional   ***/
    	if (bitmapt[ll] == '2')
    	{    /***-------------------------   analiza campo opcional   ***/
/**/
#if defined (TRAZA) && (TRAZA > 10)
    		fprintLog(nomProg,"cadena_verif ->%s<-\n", cadena_verif);
    		fprintLog(nomProg,"long_fc ->%d<-\n", long_fc);
    		fprintLog(nomProg,"tipo_fc ->%c<- long_efec ->%d<-\n", tipo_fc, long_efec);
    		fflush(stdout);
#endif	  
/**/

	    	if ((tipo_fc == 'X') || (tipo_fc == 'B'))
	   		{
	      	if (nspaces(interchar + off_fc, long_efec))
	       		bitmapt[ll] = '0';
	      	else
	    			bitmapt[ll] = '1';
	    	}
	    	else if (tipo_fc == '9')
	   		{
	      	if (nzeroes(interchar + off_fc,long_efec) || !ncampo_numerico(interchar + off_fc,long_efec))
	       		bitmapt[ll] = '0';
	      	else
	       		bitmapt[ll] = '1';
	    	}
   			//Bizum - inicio
				if ((ll == 48) && (bizum == 1))
				{
					fprintLog(nomProg,"----- Se vacia el contenido P48 ----- y valor bizum %d\n", bizum);
					bitmapt[ll] = '0';
				}
   			//Bizum - final
    	}	/***---------------------   fin analiza campo opcional   ***/

    	if (bitmapt[ll] == '3')
    	{    /***----------------------   analiza campo condicional   ***/
    		retorno = fmirar_condi2(condicion_num,ll,mensaje_inter);
     		if (retorno == 0)    /***    error   moverlo=0    ***/
     		{
      		*error_code = 7;
      		*error_bit = ll;
      		return(-1);
     		}
     		else
     		{
      		if (retorno == 1)
      		{
       			bitmapt[ll] = '1';
       			strncpy(plantilla + ((ll-1)*3),"997",3);
      		}
      		else
      		{
       			bitmapt[ll] = '0';
       			strncpy(plantilla + ((ll-1)*3),"000",3);
      		}
     		}
    	}    /***------------------   fin analiza campo condicional   ***/

			if (bitmapt[ll] == '1')
    	{    /***----------------------   analiza campo obligatorio   ***/
     		if ((tipo_fc == 'X') || (tipo_fc == 'B'))
     		{
      		if(nspaces(interchar + off_fc, long_efec))
      		{
       			if (ll != 42)
       			{
#if defined (TRAZA) && (TRAZA > 1)	   
        			fprintLog(nomProg,"bit >%i<\n", ll);
        			fprintLog(nomProg,"long_efec >%i<\n", long_efec);
        			fprintLog(nomProg," off_fc >%i<\n", off_fc);
        			fprintLog(nomProg,">%s<\n", interchar);
        			fprintLog(nomProg,">%.*s<\n", long_efec, interchar+off_fc);
#endif		
        			*error_code = 4;
        			*error_bit = ll;
        			return(-1);
       			}
      		}
     		}
     		else if (tipo_fc == '9')
     		{
      		if (!ncampo_numerico(interchar + off_fc, long_efec))
      		{
#if defined (TRAZA) && (TRAZA > 1)	  
       			fprintLog(nomProg, "ncampo_numerico == 0\n");
#endif	   
       			*error_code = 5;
       			*error_bit = ll;
       			return(-1);
      		}
         	/***   excepciones :
              - los totales pueden llevar valores cero en los cierres

              - solo se permite el P-4 con ceros cuando el P-30 contiene
                algo distinto de ceros y el codigo de condicion del
                bit 30 en plantilla sea 021, o sean consultas

              - solo se permite el P-30 con ceros cuando sean consultas
         	***/
      		if((ll < 74 || ll > 77) && (ll < 80 || ll > 81) && (ll < 85 || ll > 89) && (ll != 97) && (ll < 105 || ll > 110) && ll != 4  &&  ll != 30)
      		{
       			if (nzeroes(interchar + off_fc, long_efec))
       			{
#if defined (TRAZA) && (TRAZA > 1)	   
        			fprintLog(nomProg, "nzeroes != 0\n");
#endif		
        			*error_code = 5;
        			*error_bit = ll;
        			return(-1);
       			}
      		}
      		else
      		{
#if defined (TRAZA) && (TRAZA > 10)	  
       			fprintLog(nomProg, "EXCEPCION DE REVISION DE CEROS\n");
       			fprintLog(nomProg, "BIT ->%d<-\n", ll);
       			fprintLog(nomProg, "adipre003 ->%.*s<-\n",
          	sizeof(mensaje_inter->adipre003), mensaje_inter->adipre003);
       			fprintLog(nomProg, "adipre030 ->%.*s<-\n",
        		sizeof(mensaje_inter->adipre030), mensaje_inter->adipre030);
       			fprintLog(nomProg, "condicion de 30 ->%.3s<-\n", plantilla + (29*3));
#endif	   
      		}
     		}
    	}    /***------------------   fin analiza campo obligatorio   ***/
		}      /*   fin del bucle for  del case 2 */


/***---   mira la condicion del bit 1   ---***/
		strncpy(condicion_alf,plantilla,3);
   	condicion_alf[3] = '\0';
   	condicion_num = atoi(condicion_alf);

   	if (condicion_num == 310)
   	{
    	if (nzeroes(bitmapt + 65 , 63) != 0)
   		{
     		bitmapt[1] = '0';
     		strncpy(plantilla,"000",3);

     		bitmapt[128] = '0';
     		strncpy(plantilla + (127*3),"000",3);
    		strncpy(condicion_alf,plantilla + (63*3),3);
     		condicion_alf[3] = '\0';
     		condicion_num = atoi(condicion_alf);
     		if(condicion_num == 311)
    		{
      		bitmapt[64] = '1';
      		strncpy(plantilla + (63*3),"997",3);
     		}
    	}
    	else
    	{
     		bitmapt[1] = '1';
     		strncpy(plantilla,"997",3);
     		strncpy(condicion_alf,plantilla + (63*3),3);
     		condicion_alf[3] = '\0';
     		condicion_num = atoi(condicion_alf);
     		if ((condicion_num == 311) || (condicion_num == 997))
     		{
      		bitmapt[64] = '0';
      		strncpy(plantilla + (63*3),"000",3);
      		bitmapt[128] = '1';
      		strncpy(plantilla + (127*3),"997",3);
     		}
    	}
   	}

   	strncpy(bitmapp , bitmapt + 1 , 64);
   	strncpy(bitmaps , bitmapt + 65 , 64);
   
#if defined (TRAZA) && (TRAZA > 10)   
  	fprintLog(nomProg, "Bipmap primario   >%.64s<\n", bitmapp);
  	fprintLog(nomProg, "Bipmap secundario >%.64s<\n", bitmaps);
#endif   

   	break;
	default:
   	*error_code = 24;
   	*error_bit = 0;
   	return(-1);
   	break;
 	}        /*   fin del switch   */

 	return(0);
}

/*************************************************************************
   FUNCION  FCLASE_CONDICION       llamada por fcheqbit , sentidos 1 y 2

   DESCRIPCION: Funcion que analiza el tipo de condicion que tiene un
                bit en un mensaje determinado

   AREA DE PASO :
- entero con el valor de la condicion

* devuelve el tipo de condicion, siendo los valores posibles :
             DESCONOCIDO
             NO_USADO                 (estos valores estan
             PROHIBIDO                definidos en  ftradpri.h    )
             OBLIGATORIO
             OPCIONAL
             CONDICIONAL
*************************************************************************/

int fclase_condicion(int condicion)
{
  int retorno;

switch(condicion)
   {
   case   0:
   case   7:
   case   8:
   case   9:
   case  10:
   case  11:
   case  12:
   case  14:
   case  15:
   case  19:
   case 300:
      retorno = NO_USADO;
      break;

   case 999:
      retorno = PROHIBIDO;
      break;

   case 600:
   case 667:
   case 994:
   case 998:
      retorno = OPCIONAL;
      break;

   case 400:
   case 995:
   case 996:
   case 997:
      retorno = OBLIGATORIO; 
      break;

   case   1:
   case   2:
   case   4:
   case   5:
   case  13:
   case  16:
   case  21:
   case 100:
   case 301:
   case 303:
   case 305:
   case 306:
   case 310:
   case 311:
   case 313:
   case 312:
   case 314:
   case 315:
   case 320:
   case 321:
   case 666:
   case 800:
   case 900:
   case 901:
      retorno = CONDICIONAL;
      break;

   default:
      retorno = DESCONOCIDO;
#if defined (TRAZA) && (TRAZA > 1)	  
      fprintLog(nomProg, "condicion >%d< desconocida\n", condicion);
#endif
      break;

   };          /*    fin del switch   */

return(retorno);
}


/*************************************************************************
   FUNCION  FMIRAR_CONDI1          llamada por fcheqbit   sentido 1

   DESCRIPCION: Funcion que comprueba para los bits con condicion de valor
                condicional si se cumplen las condiciones verificables con
                los valores del mapa de bits


   AREA DE PASO :
- entero con la condicion tipo condicional a analizar
- entero con el bit que estamos tratando
- array de 129 caracteres, con el bitmap total
- puntero a char con la condicion en formato array

* devuelve los valores :       0 : error al validar la condicion
*                              1 : condicion verificada y ejecutada,
                                   o pendiente de validacion posterior
*************************************************************************/

int fmirar_condi1(int condicion_num,int ll,
                        char bitmapt[129],char *condicion_alf)
{
int moverlo=0;

/*
fprintLog(nomProg,"========== fmirar_condi1 : %d\n",condicion_num);
*/


 switch(condicion_num)
 {
/***--- 001 : opcional si existe y no existe pista 2 ni pista 1--***/
  case 1:
   if(bitmapt[35] == '0' && bitmapt[45] == '0')
    strncpy(condicion_alf,"998",3);
   else
    strncpy(condicion_alf,"000",3);
   moverlo = 1;
   break;
  case 2:
/***--- 002 : opcional si existe y no existe pista 2 --***/
   if (bitmapt[35] == '0')
    strncpy(condicion_alf,"998",3);
   else
    strncpy(condicion_alf,"000",3);
   moverlo = 1;
   break;
  case 4:
/***--- 004 : obligatorio si existe bit 6 ---***/
   if (bitmapt[6] == '1')
    strncpy(condicion_alf,"997",3);
   else
    strncpy(condicion_alf,"000",3);
   moverlo = 1;
   break;
  case 5:
/***--- 005 : obligatorio si existe bit 5 ---***/
   if (bitmapt[5] == '1')
    strncpy(condicion_alf,"997",3);
   else
    strncpy(condicion_alf,"000",3);
   moverlo = 1;
   break;
  case 13:
/***--- 013 : esta condicion se resuelve en ftraefec ---***/
  case 100:
/***--- 100 : obligatorio si codigo de funcion (24) es  103 o 107 ---***/
/***--- esta condicion se resuelve en ftraefec ---***/
  case 021:
/***--- 021 : esta condicion se resuelve en ftraefec ---***/
  case 303:
/***--- 303 : esta condicion se resuelve en ftraefec ---***/
  case 305:
/***--- 305 : esta condicion se resuelve en ftraefec ---***/
   moverlo = 1;
   break;
  case 306:
/***--- 306 : obligatorio si codigo de funcion (24) es  305 ---***/
/***---       prohibido en caso contrario     ---***/
/*  plantilla + ((24-1)*3)    */
   if(!strncmp("305", plantilla + 69, 3))
    strncpy(condicion_alf,"997",3);
   else
    strncpy(condicion_alf,"000",3);
   moverlo = 1;
   break;
  case 310:
/***--- 310 : obligatorio si existe bitmap secundario ---***/
   if(!nspaces(bitmapt + 65,64))  /* mapa secundario no espacios */
    strncpy(condicion_alf,"997",3);
   else
    strncpy(condicion_alf,"000",3);
   moverlo = 1;
   break;
  case 311:
/***--- 311 : obligatorio si no existe bitmap secundario ---***/
   if(nspaces(bitmapt + 65,64))  /* mapa secundario no espacios */
    strncpy(condicion_alf,"997",3);
   else
    strncpy(condicion_alf,"000",3);
   moverlo = 1;
   break;
  case 312:
/***--- 312 : esta condicion se resuelve en ftraefec ---***/
  case 313:
/***--- 313 : esta condicion se resuelve en ftraefec ---***/
  case 314:
/***--- 314 : esta condicion se resuelve en ftraefec ---***/
  case 315:
/***--- 315 : esta condicion se resuelve en ftraefec ---***/
  case 321:
/***--- 321 : esta condicion se resuelve en ftraefec ---***/
   moverlo = 1;
   break;
/***--- 600 : obligatorio si p-24 es 600,602,603 ó 604 ---***/
  case 600:
/*  plantilla + ((24-1)*3)    */
   if(!strncmp("600", plantilla + 69, 3) ||
      !strncmp("602", plantilla + 69, 3) ||
      !strncmp("603", plantilla + 69, 3) ||
      !strncmp("605", plantilla + 69, 3) )
    strncpy(condicion_alf,"997",3);
   else
    strncpy(condicion_alf,"000",3);
   moverlo = 1;
   break;
 
  case 800:
/***--- 800 : Obligatorio si distinto de P-32 o S-94 ---***/
/***--- esta condicion se resuelve en ftraefec ---***/
  case 900:
/***--- 900 : obligatorio en operaciones de monedero electronico ---***/
/***--- esta condicion se resuelve en ftraefec ---***/
  case 901:
/***--- 901 : obligatorio en operaciones de monedero electronico ---***/
/***--- carga y cambio de pin ---***/
/***--- esta condicion se resuelve en ftraefec ---***/
  case 666:
  case 667:
   moverlo = 1;
   break;
 }

 return(moverlo);
}

/*************************************************************************
   FUNCION  FMIRAR_CONDI2          llamada por fcheqbit   sentido 2

   DESCRIPCION: Funcion que comprueba para los bits condicionales si se
                cumplen las condiciones entre campos del formato intermedio

   AREA DE PASO :
- entero con la condicion tipo condicional a analizar
- entero con el bit que estamos tratando
- puntero a formato_intermedio con el mensaje en este formato

* devuelve los valores :   0 : error al validar la condicion
                           1 : la condicion se cumple: se ponde el bit a '1'
                               y la condicion a obligatoria
                           2 : la condicion no se cumple: se ponde el bit a '0'
                               y la condicion a prohibida
*************************************************************************/

int     fmirar_condi2(int condicion_num,int ll,
                        formato_intermedio *mensaje_inter)
{
  int moverlo=0;
  char * pcampo;
  int l_campo;

#if defined (TRAZA) && (TRAZA > 10)  
fprintLog(nomProg,"========== fmirar_condi2 : %d\n",condicion_num);
#endif

 pcampo = global_intermedio + off_fc;
 l_campo = global_long_efec;

 switch(condicion_num)
 {
  case 1:
/***--- 001 : opcional si existe y no existe pista 2 ni pista 1--***/
   if (nspaces(pcampo, l_campo)) /* No presente */
    moverlo = 2;
   else
   {
    if (!nspaces(mensaje_inter->adi035d,37) ||
        !nspaces(mensaje_inter->adi045d,76))
     moverlo = 0;
    else
     moverlo = 1;
   }
   break;
  case 2:
/***--- 002 :  ---***/
   if (nspaces(pcampo, l_campo)) /* No presente */
    moverlo = 2;
   else
   {
    if (!nspaces(mensaje_inter->adi035d,37))
     moverlo = 0;
    else
     moverlo = 1;
   }
   break;
  case 4:
/***--- 004 :  ---***/
   if(nzeroes(mensaje_inter->adipre006, sizeof(mensaje_inter->adipre006)))
    moverlo = 2;
   else
    moverlo = 1;
   break;
  case 5:
/***--- 005 :  ---***/
   if(nzeroes(mensaje_inter->adipre005, sizeof(mensaje_inter->adipre005)))
    moverlo = 2;
   else
    moverlo = 1;
   break;
  case 13:
/***--- 13 :  ---***/            /***   solo en respuestas   ***/
   moverlo = 1;
   break;
  case 21:
/***--- 021 :  ---***/           /***   solo en respuestas   para el p30***/
         /***   excepciones :
          en consultas no se verifica
         ***/
   // si es una operación denegada
   if (memcmp(mensaje_inter->adipre039, "0", 1) &&  //todas las que empiezan por 0 en el p39 estan aprobadas
       memcmp(mensaje_inter->adipre039, "085", 3) &&   
       memcmp(mensaje_inter->adipre039, "400", 3) &&
       memcmp(mensaje_inter->adipre039, "481", 3) &&
       memcmp(mensaje_inter->adipre039, "900", 3))
   {
      //si el p4 es cero se mueve
      //if (!nzeroes(mensaje_inter->adipre004,12))
      //  moverlo = 0;
      //else
        moverlo = 1;
   }
   else           /* Autorizada */
   {
    if(!strncmp(mensaje_inter->adipre003, "00", 2) ||
       !strncmp(mensaje_inter->adipre003, "01", 2) ||
       !strncmp(mensaje_inter->adipre003, "20", 2))
    {
      moverlo = 2;
    }
    else
     moverlo = 2;
   }
   break;
  case 100:
/***--- 100 : obligatorio si codigo de funcion (24) es  103 o 107 ---***/
   if ((!memcmp(mensaje_inter->adipre024, "103", 3) ||
       !memcmp(mensaje_inter->adipre024, "107", 3))|| 
       (!memcmp(mensaje_inter->adipre000, "1220",4) && 
       ((!memcmp(mensaje_inter->adipre003, "40", 2))||(!memcmp(mensaje_inter->adipre003, "49", 2)))))
    moverlo = 1;
   else
    moverlo = 2; 
   break;
/***--- 666 : obligatorio si operacion 1200 1220 1221 y codigo de proceso es  40 o 49 ---***/   
  case 666:

   if ((!memcmp(mensaje_inter->adipre000, "1200",4) ||!memcmp(mensaje_inter->adipre000, "1220",4) ||
        !memcmp(mensaje_inter->adipre000, "1221",4) ||!memcmp(mensaje_inter->adipre000, "1210",4)) &&
     ((!memcmp(mensaje_inter->adipre003, "40", 2)) ||(!memcmp(mensaje_inter->adipre003, "49", 2))))
    moverlo = 1;
   else
    moverlo = 2; 
   fprintLog(nomProg, "666 moverlo >%i<\n",moverlo);   
   break;
  
   
/***--- 667 : obligatorio si no transferencia ---***/  
   case 667:
//*  plantilla + ((24-1)*3)    */
   if ((!memcmp(mensaje_inter->adipre000, "1200",4) ||!memcmp(mensaje_inter->adipre000, "1220",4) ||!memcmp(mensaje_inter->adipre000, "1221",4)) &&
     ((!memcmp(mensaje_inter->adipre003, "40", 2)) ||(!memcmp(mensaje_inter->adipre003, "49", 2))))
    moverlo = 2;
   else
    moverlo = 1;
   fprintLog(nomProg, "667 moverlo >%i<\n",moverlo);   
   break; 
   
  case 303:
/***--- 303 :  ---***/            /***   solo si ha sido autorizada   ***/

    if (!nspaces(mensaje_inter->adipre038,6))
	   moverlo = 1; //si está informado se mueve
    else
	   moverlo = 2; //si no esta informado no se mueve

#if defined (TRAZA) && (TRAZA > 10)   
   fprintLog(nomProg,"038 moverlo >%d<\n", moverlo);
#endif   
   break;
  case 305:
/***--- 305 : prohibido si codigo de funcion (24) es  305 ---***/
/***---       obligatorio en caso contrario     ---***/
   if (nspaces(pcampo, l_campo)) /* No presente */
    if(memcmp(mensaje_inter->adipre024, "305", 3))
     moverlo = 0;
    else
     moverlo = 2;
   else
    if(memcmp(mensaje_inter->adipre024, "305", 3))
     moverlo = 1;
    else
     moverlo = 0;
   break;
  case 306:
/***--- 306 : obligatorio si codigo de funcion (24) es  305 ---***/
/***---       prohibido en caso contrario     ---***/
   if (nspaces(pcampo, l_campo)) /* No presente */
    if (memcmp(mensaje_inter->adipre024, "305", 3))
     moverlo = 2;
    else
     moverlo = 0;
   else
    if (memcmp(mensaje_inter->adipre024, "305", 3))
     moverlo = 0;
    else
     moverlo = 1;
   break;
  case 313:
      /***   suponemos que HE gestiona las sesiones   ***/
/***--- 313 :  ---***/     /***   obligatorio si HE gestiona la sesiones   ***/
   moverlo = 1;
   break;
  case 314:
                                  /***   solo en respuestas   ***/
/***--- 314 :  ---***/     /***   obligatorio si HCP gestiona la sesiones   ***/
    moverlo = 2;
   break;
  case 315:
/***--- 315 :  ---***/            /***   solo en respuestas   ***/
   moverlo = 1;
   break;
  case 320:
/***--- 320 :  ---***/     /***   obligatorio si 101 es EXCNEG o CASTAR   ***/
                           /*** resto prohibido                           ***/
   if(!strncmp(mensaje_inter->adi101d, "EXCNEG", 6) ||
      !strncmp(mensaje_inter->adi101d, "CASTAR", 6))
    moverlo = 1;
   else
    moverlo = 2;
   break;
  case 321:
/***--- 321 :  ---***/     /***   obligatorio si 101 es CASCUE   ***/
                           /*** resto prohibido                           ***/
   if(!strncmp(mensaje_inter->adi101d, "CASCUE", 6))
    moverlo = 1; 
   else
    moverlo = 2;
   break;
  case 800:
/***--- 800 :  ---***/     /***   prohibido si no aplicacion 02   ***/
   moverlo = 2; 
   break;
  case 900:
/***--- 900 : obligatorio en operaciones de monedero electronico ---***/
/*** ojo hay que tratar esta condicion */
   moverlo = 2;
   break;
  case 901:
      /***   suponemos que HE gestiona las sesiones   ***/
/***--- 901 :  ---***/     /***   obligatorio si HCP gestiona la sesiones   ***/
/*** ojo hay que tratar esta condicion */
   moverlo = 2;
   break;
 } 
 
 return(moverlo);
}

