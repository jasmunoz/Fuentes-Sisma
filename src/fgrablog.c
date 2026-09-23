//PRJ0257005 - Revisado tokenizacion
//PRJ0247167 - Revisado ampliación P48
#include "cforitno.h"
#include "deffich.h"
#include "datagen.h"


int fclase_tipomsg(int tipomsg); 
int fcifraPan(char *PanClaro, char *PanCifrado);

/**********************************************/
/***   funcion: graba log de operaciones     ***/
/**********************************************/
int fgraba_log(formato_interno *pforitn)
{
	int retorno, tipomsg, tiempoL;
  char tipomsgc[4]; //PRJ0258542
  NPSLOGO w_reglogo;
  NPSLOGO reglogo;
  clave_log clave_log1;
  tpclav01 * clamerca;   /* clave previa de 16    */ 
  tpclav02 * claprice;   /* clave previa de price */
  tpclav03 * clasio4b;   /* clave previa de SIO */
  tm_dec  tt;
  char fecTomorrow[9];
  char char03[3];
  int num_origen;
  int lon_pan;
  char auxClaPrevia[36];
  // PRJ0202598 cambio bin de 6 posiciones 8 no cambia el tamaño total de panClr ni pan Cfd
  char panClr[20];
  char panCfd[20];
  formato_interno log_fi;
  tm_dec tm; //PRJ0258542 - Fase 3 - Cambio 11052026
  fechorsys(&tm); //PRJ0258542 - Fase 3 - Cambio 11052026
	char numoper[6]; //PRJ0258542 - Fase 3 - Cambio 11052026
  char numcross[10]; //PRJ0258542 - Fase 3 - Cambio 11052026


	//PRJ0257005
 	fprintLog(nomProg,"=======================\n");
 	fprintLog(nomProg,"= Funcion GL 20260514 =\n");
 	fprintLog(nomProg,"= Graba en NPTLOGO    =\n");
 	fprintLog(nomProg,"=======================\n");

  fprintLog(nomProg,"** Numope: %.6s **\n", pforitn->fidatpro.finumope);


	memset(reglogo.loclavpr,' ',35); //PRJ0258542
	reglogo.loclavpr[35]= '\0'; //PRJ0258542 - Fase 3 - inicializa loclavpr
   
  fprintLog(nomProg, "fgraba_log numope >%.6s<\n", pforitn->fidatpro.finumope);

	//Primero se meten los datos en común que valen para todos los casos, y luego se separa por operaciones de cargo tratamiento de claves y resto
	pforitn->fidatpro.fiorigen[2]='\0';
	num_origen = atoi(pforitn->fidatpro.fiorigen); 
	pforitn->fiisocom.fiiso000[4] = '\0';
	tipomsg = atoi(pforitn->fiisocom.fiiso000);
	strncpy(tipomsgc, pforitn->fiisocom.fiiso000, 4);
	strncpy(reglogo.lonumope, pforitn->fidatpro.finumope,6);
	strncpy(reglogo.lofecha0, pforitn->fidatpro.fifecha0, 8);  
	strncpy(reglogo.lohora00, pforitn->fidatpro.fihora00, 6);

	//PRJ0258542-INI
  //if (strncmp(pforitn->fiisocom.fiiso000, "1644",4)== 0 && strncmp(pforitn->fiisocom.fiiso024,"892",3) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
  if (strncmp(pforitn->fiisocom.fiiso000, "1644",4)== 0) //PRJ0258542 - Fase 3 - Cambio 11052026
	{
		fprintLog(nomProg,"Entrada por mensaje %.4s con P24 = %.3s \n", pforitn->fiisocom.fiiso000, pforitn->fiisocom.fiiso024);
		strncpy(pforitn->fidatpro.fifecha0,"20",2); //PRJ0258542 - Fase 3 - Cambio 11052026
		sprintf(pforitn->fidatpro.fifecha0+2,"%02d", tm.tm_year - 2000); //PRJ0258542 - Fase 3 - Cambio 11052026
		sprintf(pforitn->fidatpro.fifecha0+4,"%02d", tm.tm_mon); //PRJ0258542 - Fase 3 - Cambio 11052026
		sprintf(pforitn->fidatpro.fifecha0+6,"%02d", tm.tm_mday); //PRJ0258542 - Fase 3 - Cambio 11052026
		sprintf(pforitn->fidatpro.fihora00,"%02d", tm.tm_hour); //PRJ0258542 - Fase 3 - Cambio 11052026
		sprintf(pforitn->fidatpro.fihora00+2,"%02d", tm.tm_min); //PRJ0258542 - Fase 3 - Cambio 11052026
		sprintf(pforitn->fidatpro.fihora00+4,"%02d", tm.tm_sec); //PRJ0258542 - Fase 3 - Cambio 11052026
	  if (strncmp(pforitn->fidatpro.finumope, "000000", 6) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
	  {
	  	retorno = fnumope(numoper, numcross, '0');
			if (retorno != 0)
			{
				fprintLog(nomProg,"error al asignar numero de operacion \n");
			  fgraba_incidencia_cp("264", NULL,NULL,NULL);
				strcpy(pforitn->fiisocom.respusal,"909");
			  return(retorno);
			}
			else
			{
			  strncpy(pforitn->fidatpro.finumope,numoper,6);
			  pforitn->fidatpro.finumope[6] = '\0';
			}
		} //PRJ0258542 - Fase 3 - Cambio 11052026
		strncpy(reglogo.lonumpan,"                   ",19);
		reglogo.lonumpan[19]='\0';
		strncpy(reglogo.locomerc,"               ",15);
		reglogo.locomerc[15]='\0';
	 	strncpy(reglogo.loesanul,"0", 1); 
	 	reglogo.loesanul[1]='\0';
	 	strncpy(reglogo.loeseven," ",1);
	 	reglogo.loeseven[1]='\0';
		strncpy(reglogo.loimporte,"            ",12);
		reglogo.loimporte[12]='\0';

		retorno = funcionss (pforitn);
		//fprintLog(nomProg,"Salida de recuperar fecha y sesion >%d< >%.3s< >%.2s%.2s%.2s<\n",retorno,pforitn->fidatiso.fiisoaut.fiaut029,pforitn->fidatiso.fiisoaut.fiaut028.aa,pforitn->fidatiso.fiisoaut.fiaut028.mm,pforitn->fidatiso.fiisoaut.fiaut028.dd);
		if (retorno == 11)
		{
	   	strncpy(reglogo.losesion,   pforitn->fidatiso.fiisoaut.fiaut029, 3);
	 		strncpy(reglogo.lofecses,   pforitn->fidatiso.fiisoaut.fiaut028.aa, 2);
	 		strncpy(reglogo.lofecses+2, pforitn->fidatiso.fiisoaut.fiaut028.mm, 2);
	 		strncpy(reglogo.lofecses+4, pforitn->fidatiso.fiisoaut.fiaut028.dd, 2);
		}
		else
		{
	   	strncpy(reglogo.losesion, "000", 3);
	 		strncpy(reglogo.lofecses, pforitn->fidatiso.fiisoadm.fiadm012, 6);
			fprintLog(nomProg,"Error al recuperar la fecha y sesion >%d<\n",retorno);
		}
		//fprintLog(nomProg,"Valores nptlogo fecha y sesion >%.3s< >%.6s<\n",reglogo.losesion,reglogo.lofecses);
		memset(reglogo.loclavpr, ' ',35);
		strncpy(reglogo.loclavpr, pforitn->fiisocom.fiiso000, 4);
		strncpy(reglogo.loclavpr + 4, pforitn->fidatiso.fiisoadm.fiadm011, 6);
		strncpy(reglogo.loclavpr + 10, pforitn->fidatiso.fiisoadm.fiadm012, 12);
		strncpy(reglogo.loclavpr + 22, pforitn->fidatiso.fiisoadm.fiadm094.longitud, 2);
		strncpy(reglogo.loclavpr + 24, pforitn->fidatiso.fiisoadm.fiadm094.datos, atoi(pforitn->fidatiso.fiisoadm.fiadm094.longitud));
		reglogo.loclavpr[35]= '\0';   
		//fprintLog(nomProg,"Valores nptlogo lofecha0 >%.8s<\n",reglogo.lofecha0);
		//fprintLog(nomProg,"Valores nptlogo lohora00 >%.6s<\n",reglogo.lohora00);
		//fprintLog(nomProg,"Valores nptlogo lonumope >%.6s<\n",reglogo.lonumope);
		//fprintLog(nomProg,"Valores nptlogo lonumpan >%.19s<\n",reglogo.lonumpan);
		//fprintLog(nomProg,"Valores nptlogo locomerc >%.15s<\n",reglogo.locomerc);
		//fprintLog(nomProg,"Valores nptlogo loeseven >%.1s<\n",reglogo.loeseven);
		//fprintLog(nomProg,"Valores nptlogo loesanul >%.1s<\n",reglogo.loesanul);
		//fprintLog(nomProg,"Valores nptlogo loimporte >%.12s<\n",reglogo.loimporte);
		//fprintLog(nomProg,"Valores nptlogo loforitn >%.250s<\n",reglogo.loforitn);
	}
	//PRJ0258542-FIN

	//PRJ0258542 - INI - Tiene que hacer un tratamiento distinto para las 18xx de cambio de cambio de clave y par los 1644
  if (strncmp(pforitn->fiisocom.fiiso000, "18",2)== 0
 		&& (strncmp(pforitn->fiisocom.fiiso024,"811",3) == 0 || strncmp(pforitn->fiisocom.fiiso024,"815",3) == 0 || strncmp(pforitn->fiisocom.fiiso024,"816",3) == 0))
	{
		fprintLog(nomProg,"Entrada por mensaje %.4s con P24 = %.3s \n", pforitn->fiisocom.fiiso000, pforitn->fiisocom.fiiso024);
		
		strncpy(reglogo.lonumpan,"                   ",19);
		lon_pan = 0; //PRJ0258542 - Evita poner tarjeta
		reglogo.lonumpan[19]='\0';
		strncpy(reglogo.locomerc,"               ",15);
		reglogo.locomerc[15]='\0';
	 	strncpy(reglogo.loesanul,"0", 1); 
	 	reglogo.loesanul[1]='\0';
	 	strncpy(reglogo.loeseven," ",1);
	 	reglogo.loeseven[1]='\0';
		strncpy(reglogo.loimporte,"            ",12);
		reglogo.loimporte[12]='\0';

		retorno = funcionss (pforitn);
		//fprintLog(nomProg,"Salida de recuperar fecha y sesion >%d< >%.3s< >%.6s<\n",retorno,pforitn->fidatiso.fiisocla.ficla029,pforitn->fidatiso.fiisocla.ficla028);
		if (retorno == 11)
		{
	   	strncpy(reglogo.losesion,   pforitn->fidatiso.fiisocla.ficla029, 3);
	 		strncpy(reglogo.lofecses,   pforitn->fidatiso.fiisocla.ficla028, 6);
		}
		else
		{
	   	strncpy(reglogo.losesion, "000", 3);
	 		strncpy(reglogo.lofecses, pforitn->fidatiso.fiisocla.ficla012, 6);
			fprintLog(nomProg,"Error al recuperar la fecha y sesion >%d<\n",retorno);
		}
		//fprintLog(nomProg,"Valores nptlogo fecha y sesion>%.3s< >%.6s<\n",reglogo.losesion,reglogo.lofecses);
		strncpy(reglogo.locomerc, " ", 15);
		memset(reglogo.loclavpr, ' ',35);
		if (strncmp(pforitn->fiisocom.fiiso000, "1834",4) == 0 || strncmp(pforitn->fiisocom.fiiso000, "1835",4) == 0)
		{
			tipomsg = 1824;
		}
		if (strncmp(pforitn->fiisocom.fiiso000, "1804",4) == 0 || strncmp(pforitn->fiisocom.fiiso000, "1805",4) == 0 || strncmp(pforitn->fiisocom.fiiso000, "1814",4) == 0)
		{
			tipomsg = 1804;
		}
		//tipomsg = atoi(pforitn->fiisocom.fiiso000) - 10; //Restamos 10 para que grabe el original en la clave
		strncpy(tipomsgc, itoa(tipomsg), 4);
		//tipomsgc = itoa(tipomsg);
		strncpy(reglogo.loclavpr, tipomsgc, 4);
		strncpy(reglogo.loclavpr + 4, pforitn->fidatiso.fiisocla.ficla011, 6);
		strncpy(reglogo.loclavpr + 10, pforitn->fidatiso.fiisocla.ficla012, 12);
		strncpy(reglogo.loclavpr + 22, pforitn->fidatiso.fiisocla.ficla094.longitud, 2);
		strncpy(reglogo.loclavpr + 24, pforitn->fidatiso.fiisocla.ficla094.datos, atoi(pforitn->fidatiso.fiisocla.ficla094.longitud));
		reglogo.loclavpr[35]= '\0';   
		memcpy(reglogo.loforitn, "                     ", sizeof(reglogo.loforitn));
	}
  //PRJ0258542 - FIN - Tiene que hacer un tratamiento distinto para las 18xx de cambio de cambio de clave, el resto se ha metido en un else
  else
  {
		strncpy(reglogo.locomerc, pforitn->fidatiso.fiisoaut.fiaut042, 15);
		//if (strncmp(pforitn->fiisocom.fiiso000, "1644",4)== 0 && strncmp(pforitn->fiisocom.fiiso024,"892",3) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
		if (strncmp(pforitn->fiisocom.fiiso000, "1644",4)== 0) //PRJ0258542 - Fase 3 - Cambio 11052026
		{
			strncpy(reglogo.locomerc, "               ", 15); //Los mensajes administrativos no llevan comercio
		}
		// PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud total de finummPAN
		pforitn->fidatiso.fiisoaut.finumPAN.longitud[2]='\0';
		lon_pan = atoi(pforitn->fidatiso.fiisoaut.finumPAN.longitud);
		fprintLog(nomProg,"Pan en fgraba_log lon_pan <%d> Long <%s> PAN <%s>\n", lon_pan, pforitn->fidatiso.fiisoaut.finumPAN.longitud, pforitn->fidatiso.fiisoaut.finumPAN.datos); 
		//if (strncmp(pforitn->fiisocom.fiiso000, "1644",4)== 0 && strncmp(pforitn->fiisocom.fiiso024,"892",3) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
		if (strncmp(pforitn->fiisocom.fiiso000, "1644",4)== 0) //PRJ0258542 - Fase 3 - Cambio 11052026
		{
			lon_pan = 0; //PRJ0258542 - Evita poner tarjeta en los mensajes administrativos
		}
		if(lon_pan > 0)
		{
			strncpy(panClr,pforitn->fidatiso.fiisoaut.finumPAN.datos, lon_pan);
			panClr[lon_pan]='\0';
			// PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud total de fipancif
			strncpy(panCfd,pforitn->fidatpro.fipancif, lon_pan);
			//	  panCfd[lon_pan]='\0';
			panCfd[2]='\0';
			  
			//      strncpy(reglogo.lonumpan,panCfd, lon_pan);
			strncpy(reglogo.lonumpan,panClr, lon_pan);

			//FINUMPAN lo guardo cifrado solo para que se almacene así, luego lo vuelvo a poner en claro para que viaje dentro de sisma
			//	  strncpy(pforitn->fidatiso.fiisoaut.finumPAN.datos, panCfd, lon_pan);

			//FIPANCIF ya está cifrado desde el receptor
			//lo guardo con la tjt en claro con asteriscos en el centro para poder utilizarlo en las consultas de la web
			//luego lo vuelvo a poner cifrado para que viaje así dentro de sisma y poder comunicarlo al resolutor
			strncpy(pforitn->fidatpro.fipancif, panClr, 6);
			strncpy(pforitn->fidatpro.fipancif +6, "******", 6);
			strncpy(pforitn->fidatpro.fipancif +12, panClr+12, (lon_pan -12));
		}
		else
		{
			// PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud total de finummPAN
			strncpy(reglogo.lonumpan,pforitn->fidatiso.fiisoaut.finumPAN.datos, lon_pan);
		}

		// PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud total de TAM_PAN
		memset(reglogo.lonumpan + lon_pan, ' ', TAM_PAN - lon_pan);

		if ((strncmp(pforitn->fiisocom.fiiso000,"1420",4)==0) || (strncmp(pforitn->fiisocom.fiiso000,"1421",4)==0) || (strncmp(pforitn->fiisocom.fiiso000,"1430",4)==0))
		{
			strncpy(reglogo.loesanul,"1", 1);
		  reglogo.loesanul[1]='\0';
		} 
		else 
		{
		 	strncpy(reglogo.loesanul,"0", 1); 
		 	reglogo.loesanul[1]='\0';
		}  
		 
		if(strncmp(pforitn->fiisocom.fiiso000,"15",2)==0)
		{
			strncpy(reglogo.losesion, pforitn->fidatiso.fiisotot.fitot029, 4);
			strncpy(reglogo.lofecses, pforitn->fidatiso.fiisotot.fitot028.aa, 2);
			strncpy(reglogo.lofecses+2, pforitn->fidatiso.fiisotot.fitot028.mm, 2);
			strncpy(reglogo.lofecses+4, pforitn->fidatiso.fiisotot.fitot028.dd, 2);
		}
		else
		{
			if(strncmp(pforitn->fiisocom.fiiso000,"13",2)==0)
			{		   
		 		//20161125 - para los mensajes de bloqueos grabamos la fecha de sesion = fecha sistema +1
		 		// para que pueda descargarla el proceso a la vez que descarga las de la aplicación de emisor
		   	strncpy(reglogo.losesion, pforitn->fidatiso.fiisoaut.fiaut029, 4);
				sumafecha(&tt,1);
		   	sprintf(fecTomorrow, "%4d%02d%02d", tt.tm_year, tt.tm_mon, tt.tm_mday);
			 	strncpy(reglogo.lofecses, fecTomorrow + 2, 2);
		 		strncpy(reglogo.lofecses+2, fecTomorrow + 4, 2);
		 		strncpy(reglogo.lofecses+4, fecTomorrow + 6, 2);
			}
			else
			{
		   	strncpy(reglogo.losesion, pforitn->fidatiso.fiisoaut.fiaut029, 4);
		 		strncpy(reglogo.lofecses, pforitn->fidatiso.fiisoaut.fiaut028.aa, 2);
		 		strncpy(reglogo.lofecses+2, pforitn->fidatiso.fiisoaut.fiaut028.mm, 2);
		 		strncpy(reglogo.lofecses+4, pforitn->fidatiso.fiisoaut.fiaut028.dd, 2);	    
			}
		}


		// IDENTIFICACION DE LA OPERACION PARA LA PREVIA 
		// si es peticion se graba la previa desde el er
		// si es comunicacion se graba desde el eo cuando ya es respuesta
		memset(reglogo.loclavpr,' ',35);
		reglogo.loclavpr[35]= '\0';   
		// si es un test no se graba la previa
		if (strncmp(pforitn->fiisocom.fiiso000,"18",2))
		{  
			// origen datafono privado
			if (strncmp(pforitn->fidatpro.fiorigen,"99",2)==0)
			{
				strncpy(auxClaPrevia, pforitn->fidatpro.fifecha0,2); //el siglo lo tomo de la fecha de proceso!!
				strncpy(auxClaPrevia+2, pforitn->fiisocom.fifechor.aa, 2);	
				strncpy(auxClaPrevia+4, pforitn->fiisocom.fifechor.mm, 2);	
				strncpy(auxClaPrevia+6, pforitn->fiisocom.fifechor.dd, 2);	
				strncpy(auxClaPrevia+8, pforitn->fiisocom.fifechor.hh, 2);	
				strncpy(auxClaPrevia+10, pforitn->fiisocom.fifechor.mi, 2);	
				strncpy(auxClaPrevia+12, pforitn->fiisocom.fifechor.ss, 2);	
				strncpy(auxClaPrevia+14, pforitn->fidatiso.fiisoaut.fiaut038, 6);	

				strncpy(reglogo.loclavpr,auxClaPrevia,20);
				reglogo.loclavpr[20]= '\0';	 
			}
			// resto (origen universal y sisma)
			else
			{
		   	claprice = (tpclav02 *) reglogo.loclavpr;
		   	retorno = fclase_tipomsg(tipomsg);
				// las repeticiones -1
		   	if (pforitn->fiisocom.fiiso000[3] == '1' || pforitn->fiisocom.fiiso000[3] == '5')
		   	{
		    	sprintf(claprice->lotipopr, "%04d", tipomsg-1);
		   	}
		   	else
		   	{
					if ((retorno == RESPUESTAS_NUCLEO) || (retorno == RESPUESTA_CIERRE))
		   			sprintf(claprice->lotipopr, "%04d", tipomsg-10);
					else
		       	sprintf(claprice->lotipopr, "%04d", tipomsg);
		   	}
		   	strncpy(claprice->lonumepr, pforitn->fiisocom.fiidtran, 6);		
		   	strncpy(claprice->lofehopr,pforitn->fiisocom.fifechor.aa, 2);	
		   	strncpy(claprice->lofehopr+2,pforitn->fiisocom.fifechor.mm, 2);
		   	strncpy(claprice->lofehopr+4,pforitn->fiisocom.fifechor.dd, 2);
		   	strncpy(claprice->lofehopr+6,pforitn->fiisocom.fifechor.hh, 2);
		   	strncpy(claprice->lofehopr+8,pforitn->fiisocom.fifechor.mi, 2);
		   	strncpy(claprice->lofehopr+10,pforitn->fiisocom.fifechor.ss,2);
		   	strncpy(claprice->loadqupr, pforitn->fiisocom.fi032094.longitud, 2);
		   	strncpy(claprice->loadqupr+2, pforitn->fiisocom.fi032094.datos, 11);
				reglogo.loclavpr[35]= '\0';	 
			}
		}
		else //PRJ0258542 - Fase 3 - Cambio 11052026 - INI
		{
			if (strncmp(pforitn->fiisocom.fiiso000,"18",2) == 0)
			{
				strncpy(reglogo.loclavpr, pforitn->fiisocom.fiiso000, 4);
				strncpy(reglogo.loclavpr + 4, pforitn->fidatiso.fiisoctr.fictr011, 6);
				strncpy(reglogo.loclavpr + 10, pforitn->fidatiso.fiisoctr.fictr012.aa, 2);
				strncpy(reglogo.loclavpr + 12, pforitn->fidatiso.fiisoctr.fictr012.mm, 2);
				strncpy(reglogo.loclavpr + 14, pforitn->fidatiso.fiisoctr.fictr012.dd, 2);
				strncpy(reglogo.loclavpr + 16, pforitn->fidatiso.fiisoctr.fictr012.hh, 2);
				strncpy(reglogo.loclavpr + 18, pforitn->fidatiso.fiisoctr.fictr012.mm, 2);
				strncpy(reglogo.loclavpr + 20, pforitn->fidatiso.fiisoctr.fictr012.ss, 2);
				strncpy(reglogo.loclavpr + 22, pforitn->fidatiso.fiisoctr.fictr094.longitud, 2);
				strncpy(reglogo.loclavpr + 24, pforitn->fidatiso.fiisoctr.fictr094.datos, atoi(pforitn->fidatiso.fiisoctr.fictr094.longitud));
				reglogo.loclavpr[35]= '\0';   
			}
		} //PRJ0258542 - Fase 3 - Cambio 11052026 - FIN
		reglogo.loeseven[0]=' '; 

		/////////////////// IMPORTES  //////////////////////////////////////////
		//////////// si la operación no es de tarjeta no grabo importes ////////
		// 20150225
		// si llega informada la tasa de conciliacion utilizamos importe conciliación
		// si no, si llega la tasa del titular, utilizamos importe titular
		// si no, utilizamos el importe en moneda original.

		if(lon_pan > 0)
		{
			if(!nspaces(pforitn->fidatiso.fiisoaut.fiaut009, 8))
		 		strncpy(reglogo.loimporte, pforitn->fidatiso.fiisoaut.fiaut005, 12);
			else
			{
		 	if(!nspaces(pforitn->fidatiso.fiisoaut.fiaut010, 8))
		   	strncpy(reglogo.loimporte, pforitn->fidatiso.fiisoaut.fiaut006, 12);
		 	else 
		   	strncpy(reglogo.loimporte, pforitn->fidatiso.fiisoaut.fiaut004, 12);
			}
#if defined (TRAZA) && (TRAZA > 10)	 
			fprintLog(nomProg, "reglogo.loimporte >%.12s<\n", reglogo.loimporte );
#endif	   
		}
		else
		{ 
			//if (strncmp(pforitn->fiisocom.fiiso000, "1644",4)== 0 && strncmp(pforitn->fiisocom.fiiso024,"892",3) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
			if (strncmp(pforitn->fiisocom.fiiso000, "1644",4)== 0) //PRJ0258542 - Fase 3 - Cambio 11052026
			{
				strncpy(reglogo.loimporte, "            ", 12); //Los mensajes administrativos no llevan importe
			}
			else
			{
				strncpy(reglogo.loimporte, "000000000000", 12);
			}
		}

	} //PRJ0258542 Fin del else
	memcpy(reglogo.loforitn, (char *)pforitn, sizeof(reglogo.loforitn));

	strncpy(clave_log1.fecha0, reglogo.lofecha0, 8);
	strncpy(clave_log1.hora00, reglogo.lohora00, 6);
	strncpy(clave_log1.numope, reglogo.lonumope, 6);
	clave_log1.final[0] = '\0';

#ifdef TIEMPOS
	tiempoL = milelong();
	fprintLog(nomProg, "* * Antes del if pforitn->fiisocom.fiiso000 >%.4s< \n", pforitn->fiisocom.fiiso000); //PRJ0257005
	if(!strncmp(pforitn->fiisocom.fiiso000, "1110", 4) ||!strncmp(pforitn->fiisocom.fiiso000, "1210", 4))
	{
	 	pforitn->fidatpro.fitienre[8]='\0';
	  fprintf(pf, "Antes de ESCRIBIR %05lu\n", tiempoL - atol(pforitn->fidatpro.fitienre));
	  fflush(pf);
	}
	fprintLog(nomProg,"fitisare >%.8s< fitienre >%.8s<\n", pforitn->fidatpro.fitisare, pforitn->fidatpro.fitienre); 
#endif
	fprintLog(nomProg, "* * Va a grabar log con operacion iso0 >%.4s< \n", pforitn->fiisocom.fiiso000); //PRJ0257005

	retorno = 0;
 	///////////// 20160411 - el filefun ya controla que si existe haga update pero tarda /////
  //PRIMERO LEE, SI EXISTE REESCRIBE, SI NO INSERTA
	//PRJ0247167

	fprintLog(nomProg,"*** *** fgrablog.c, NPTLOGO - IGUAL >%s<\n", clave_log1); 
  retorno = tratar_fich("NPTLOGO",IGUAL, &w_reglogo, &clave_log1);

  //REESCRIBIR si existe
  if(!retorno)
  {
  	if (strncmp(pforitn->fidatpro.fiorigen,"40",2)!=0)
    {
    	memcpy(reglogo.loclavpr, w_reglogo.loclavpr, sizeof(reglogo.loclavpr));
    }
    memcpy(&log_fi, &w_reglogo.loforitn, sizeof(w_reglogo.loforitn));
    //PRJ0247167
    fprintLog(nomProg,"*** *** fgrablog.c, NPTLOGO - REESCRIBIR\n"); 
    retorno = tratar_fich("NPTLOGO",REESCRIBIR,&reglogo,NULL);

#if defined (TRAZA) && (TRAZA > 1)        		
	if(retorno)
  {
		fprintLog(nomProg,"Retorno al RESCRIBIR LOG   %d\n",retorno);  
	}
#endif		  
  }
  //ESCRIBIR si no existe
  else
  {
    //ESCRIBIR
  	//PRJ0247167
		fprintLog(nomProg,"*** *** fgrablog.c, NPTLOGO - ESCRIBIR\n"); 
		//fprintLog(nomProg, "-- Variables fgrablog.c loforitn >%s< \n", reglogo.loforitn);
		//PRJ0258542-INI
		//fprintLog(nomProg,"Valores nptlogo fecha y sesion>%.3s< >%.6s<\n",reglogo.losesion,reglogo.lofecses);
		//fprintLog(nomProg,"Valores nptlogo lofecha0 >%.8s<\n",reglogo.lofecha0);
		//fprintLog(nomProg,"Valores nptlogo lohora00 >%.6s<\n",reglogo.lohora00);
		//fprintLog(nomProg,"Valores nptlogo lonumope >%.6s<\n",reglogo.lonumope);
		//fprintLog(nomProg,"Valores nptlogo lonumpan >%.19s<\n",reglogo.lonumpan);
		//fprintLog(nomProg,"Valores nptlogo locomerc >%.15s<\n",reglogo.locomerc);
		//fprintLog(nomProg,"Valores nptlogo loeseven >%.1s<\n",reglogo.loeseven);
		//fprintLog(nomProg,"Valores nptlogo loesanul >%.1s<\n",reglogo.loesanul);
		//fprintLog(nomProg,"Valores nptlogo loimporte >%.12s<\n",reglogo.loimporte);
		//fprintLog(nomProg,"Valores nptlogo loforitn >%d<\n",sizeof(reglogo.loforitn));
		//fprintLog(nomProg,"Valores nptlogo loclavpr >%.35s<\n",reglogo.loclavpr );
		//PRJ0258542-FIN
		retorno = tratar_fich("NPTLOGO",ESCRIBIR,&reglogo,NULL); 
  	fprintLog(nomProg,"Retorno al ESCRIBIR LOG %d\n",retorno); //PRJ0258542
	}

	/////// VUELVO A PONER EL PAN EN CLARO Y EL CIFRADO PARA QUE VIAJEN
	/// tanto si ha ido bien como si ha ido mal y para escribir y reescribir vuelvo a poner bien el pan
	if(lon_pan > 0)
  {
  	// PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de finumPAM ni panClr
   	strncpy(pforitn->fidatiso.fiisoaut.finumPAN.datos, panClr, lon_pan);
	  strncpy(pforitn->fidatpro.fipancif, panCfd, lon_pan);
	}
		
#ifdef TIEMPOS
  tiempoL = milelong();
  if(!strncmp(pforitn->fiisocom.fiiso000, "1110", 4) || !strncmp(pforitn->fiisocom.fiiso000, "1210", 4))
  {
  	pforitn->fidatpro.fitienre[8]='\0';
    fprintf(pf, "Despues de ESCRIBIR %05lu\n", tiempoL - atol(pforitn->fidatpro.fitienre));
    fflush(pf);
  }
#endif 
	fprintLog(nomProg,"fgrablog.c Sale de fgraba_log\n"); //PRJ0258542
  return(retorno);
} /*** fin funcion graba log ***/
