//PRJ0247167 - Revisado ampliación P48
/***********************************************************************
  FUNCION   funcionTO
    
  DESCRIPCION : tratamiento de mensajes de cierre procedentes de pantalla
                rellena los campos de totales en mensajes de pantalla
***********************************************************************/

int funcionto(formato_interno *pforitn)
{
	
int fci_carga_totales(formato_interno *pforitn);
int retorno;
int tipomsg;

  fprintLog(nomProg,"=============\n");
  fprintLog(nomProg,"= funcionTO =\n");
  fprintLog(nomProg,"=============\n");

  tipomsg = atoi(pforitn->fiisocom.fiiso000); 

  switch(tipomsg)
  {
   case 1524:

    retorno = fci_carga_totales(pforitn);
   break;

   default:
    fprintLog(nomProg,"recibido mensaje no contemplado :%d\n",tipomsg);
    strcpy(coderror, "280");
    //Añadido para Monitorizacion SISMA 
    fprintLog(nomProg,"TIPO FUNCIONAL – TIPO DE MENSAJE NO CONTEMPLADO: Tipo mensaje >%d< no contemplado codigo de eror >%.3s<\n",tipomsg,coderror);

    strcpy(pforitn->fidatpro.ficodfin, "280");
    strcpy(pforitn->fidatpro.firesfin, "00");
    retorno = -1;
  }
     
   if (retorno != 0)
   {

       return (finalizar(98));
   }

   return (finalizar(11));
}


int fci_carga_totales(formato_interno * ci_foritn)

{
    int posic_subsis;
    float importeNeto;
	float importeNetoR;
	

    NPSTOTO     reg_toto;
    NPSTOTO     reg_toto1;
    char clave_toto[24];
    int retorno=0;
	char tonuabon074[11];
	char tonuanab075[11];
	char tonucarg076[11];
	char tonuanca077[11];
	char toimabon086[17];
	char toimanab087[17];
	char toimcarg088[17];
	char toimanca089[17];
	char toimabcu109[17];
	char toimcacu110[17];
	char toimpnet097[18];
	char impNeto4815[181];
	
    fprintLog (nomProg,"sesion >%.3s< fecha_sesion >%.2s%.2s%.2s< \n",
	            ci_foritn->fidatiso.fiisotot.fitot029, 
				ci_foritn->fidatiso.fiisotot.fitot028.aa,
				ci_foritn->fidatiso.fiisotot.fitot028.mm,
				ci_foritn->fidatiso.fiisotot.fitot028.dd);

	/////////// GUARDAR LOS TOTALES RECIBIDOS DE LA RED ////////////////

     memset(&reg_toto,'\0',sizeof(NPSTOTO));

	 
	/** insertamos en la tabla de totales el registro de la red ***/
     strncpy(reg_toto.tosubsis, ci_foritn->fidatpro.fiorigen, 2);
     strncpy(reg_toto.tosubapl, ci_foritn->fidatpro.fisubori, 2);
     strncpy(reg_toto.tosesion, ci_foritn->fidatiso.fiisotot.fitot029, 3);
     strncpy(reg_toto.toorides, ci_foritn->fidatpro.fiorigen,2); //redsys 30 o cajeros 07
     strncpy(reg_toto.tofecses, ci_foritn->fidatiso.fiisotot.fitot028.aa, 2);
     strncpy(reg_toto.tofecses +2, ci_foritn->fidatiso.fiisotot.fitot028.mm, 2);
     strncpy(reg_toto.tofecses +4, ci_foritn->fidatiso.fiisotot.fitot028.dd, 2);
     strncpy(reg_toto.tonuabon, ci_foritn->fidatiso.fiisotot.fitot074, 10);
     strncpy(reg_toto.tonuanab, ci_foritn->fidatiso.fiisotot.fitot075, 10);
     strncpy(reg_toto.tonucarg, ci_foritn->fidatiso.fiisotot.fitot076, 10);
     strncpy(reg_toto.tonuanca, ci_foritn->fidatiso.fiisotot.fitot077, 10);
     strncpy(reg_toto.toimabon, ci_foritn->fidatiso.fiisotot.fitot086, 16);
     strncpy(reg_toto.toimanab, ci_foritn->fidatiso.fiisotot.fitot087, 16);
     strncpy(reg_toto.toimcarg, ci_foritn->fidatiso.fiisotot.fitot088, 16);
     strncpy(reg_toto.toimanca, ci_foritn->fidatiso.fiisotot.fitot089, 16);
     strncpy(reg_toto.toimabcu, ci_foritn->fidatiso.fiisotot.fitot109, 16);
     strncpy(reg_toto.toimcacu, ci_foritn->fidatiso.fiisotot.fitot110, 16);
     
     fprintLog(nomProg, "reg_toto.tosubsis>%.2s<\n", reg_toto.tosubsis);
     fprintLog(nomProg, "reg_toto.tosubapl>%.2s<\n", reg_toto.tosubapl);
     fprintLog(nomProg, "reg_toto.tosesion>%.3s<\n", reg_toto.tosesion);
     fprintLog(nomProg, "reg_toto.toorides>%.2s<\n", reg_toto.toorides);
     fprintLog(nomProg, "reg_toto.tofecses>%.2s<\n", reg_toto.tofecses);
     fprintLog(nomProg, "reg_toto.tofecses>%.2s<\n", reg_toto.tofecses);
     fprintLog(nomProg, "reg_toto.tofecses>%.2s<\n", reg_toto.tofecses);
     fprintLog(nomProg, "reg_toto.tonuabon>%.10s<\n", reg_toto.tonuabon);
     fprintLog(nomProg, "reg_toto.tonuanab>%.10s<\n", reg_toto.tonuanab);
     fprintLog(nomProg, "reg_toto.tonucarg>%.10s<\n", reg_toto.tonucarg);
     fprintLog(nomProg, "reg_toto.tonuanca>%.10s<\n", reg_toto.tonuanca);
     fprintLog(nomProg, "reg_toto.toimabon>%.16s<\n", reg_toto.toimabon);
   	 fprintLog(nomProg, "reg_toto.toimanab>%.16s<\n", reg_toto.toimanab);
     fprintLog(nomProg, "reg_toto.toimcarg>%.16s<\n", reg_toto.toimcarg);
     fprintLog(nomProg, "reg_toto.toimanca>%.16s<\n", reg_toto.toimanca);
     fprintLog(nomProg, "reg_toto.toimabcu>%.16s<\n", reg_toto.toimabcu);
     fprintLog(nomProg, "reg_toto.toimcacu>%.16s<\n", reg_toto.toimcacu);
	  
     retorno=tratar_fich("NPTTOTO", ESCRIBIR, &reg_toto, NULL);

     if (retorno != 0)
     {
          fprintLog(nomProg, "error al escribir TOTO RETORNO :>%d<\n",retorno);
 					//PRJ288711 - INI - Lo que queremos es que se vuelva a enviar el menaje de totales a tuxedo - si es 101 es que ya ha pasado por aqui, retornamos 000
 					if (retorno == 101)
 					{
 						fprintLog(nomProg,"Ha encontrado mensaje de totales ya grabado en NPTTOTO\n");
 						retorno = 0;
 						fprintLog(nomProg,"Genero un 0 forzado a retorno = %d\n",retorno);
 						return(retorno);
 					}
 					//PRJ288711 - FIN - Lo que queremos es que se vuelva a enviar el menaje de totales a tuxedo - si es 101 es que ya ha pasado por aqui, retornamos 000
         	//Añadido para Monitorizacion SISMA 
         	fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Escritura tabla NPTTOTO codigo de error 203\n");

          fgraba_incidencia_cp("203", ci_foritn->fidatpro.finumope,"NPTTOTO", NULL);
		   		return(retorno);
     }
	 fprintLog(nomProg, "escrito toto de redsys\n");

    /******** CALCULAR LOS TOTALES PROPIOS ***************/
     strncpy(clave_toto, ci_foritn->fidatpro.fiorigen, 2);
     strncpy(clave_toto +2,ci_foritn->fidatpro.fisubori, 2);
     strncpy(clave_toto +4, ci_foritn->fidatiso.fiisotot.fitot029, 3);
     strncpy(clave_toto +7,"00",2); //es SISMA siempre en el rmonitor;
     strncpy(clave_toto +9, ci_foritn->fidatiso.fiisotot.fitot028.aa, 2);
     strncpy(clave_toto +11, ci_foritn->fidatiso.fiisotot.fitot028.mm, 2);
     strncpy(clave_toto +13, ci_foritn->fidatiso.fiisotot.fitot028.dd, 2);
     clave_toto[15]='\0';
     fprintLog(nomProg,"clave monitor >%s<\n",clave_toto);
	
    retorno=tratar_fich("NPTTOTO", MONITOR, &reg_toto1, clave_toto);
    if (retorno)
    { 
       fprintLog(nomProg,"Error en el monitor %d\n",retorno);
          //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Monitor tabla NPTTOTO\n");

       strcpy(ci_foritn->fidatpro.ficodfin, "280");
       strcpy(ci_foritn->fidatpro.firesfin, "00");
       return(retorno);
    }

	 //20150925
	 // para ver si cuadra calculamos el IMPORTE NETO EN SISMA
	 // en pruebas vemos que no nos están llegando los importes de las anulaciones
	 // y llega el importe de los cargos restando las anulaciones ya. Así que utilizo el neto para comparar
	 
	sprintf(tonuabon074,"%.10s" ,reg_toto1.tonuabon);
	tonuabon074[10]='\0';
	sprintf(tonuanab075,"%.10s" ,reg_toto1.tonuanab);
	tonuanab075[10]='\0';
	sprintf(tonucarg076,"%.10s" ,reg_toto1.tonucarg);
	tonucarg076[10]='\0';
	sprintf(tonuanca077,"%.10s" ,reg_toto1.tonuanca);
	tonuanca077[10]='\0';
	sprintf(toimabon086,"%.16s" ,reg_toto1.toimabon);
	toimabon086[16]='\0';
	sprintf(toimanab087,"%.16s" ,reg_toto1.toimanab);
	toimanab087[16]='\0';
	sprintf(toimcarg088,"%.16s" ,reg_toto1.toimcarg);
	toimcarg088[16]='\0';
	sprintf(toimanca089,"%.16s" ,reg_toto1.toimanca);
	toimanca089[16]='\0';
	sprintf(toimabcu109,"%.16s" ,reg_toto1.toimabcu);
	toimabcu109[16]='\0';
	sprintf(toimcacu110,"%.16s" ,reg_toto1.toimcacu);
	toimcacu110[16]='\0';
	
	fprintLog(nomProg,"abon086>%s< + anab087>%s< - carg088>%s< - anca089>%s< + abcu109>%s< - cacu110>%s< \n",
	           toimabon086, toimanab087, toimcarg088, toimanca089, toimabcu109, toimcacu110);
		   
	 
     importeNeto = atof(toimabon086) + atof(toimanab087) -
                   atof(toimcarg088) - atof(toimanca089) +
                   atof(toimabcu109) - atof(toimcacu110);
	
	//fprintLog(nomProg, "Importe neto calculado >%f<\n", importeNeto);
	//no utilizo la variable importeNeto porque cuando el número es grande redondea y necesito el valor exacto
	
	if((atof(toimabon086) + atof(toimanab087) - atof(toimcarg088) - 
	    atof(toimanca089) + atof(toimabcu109) - atof(toimcacu110) ) < 0)
	{
		   toimpnet097[0]='D';
		   sprintf(toimpnet097+1, "%016.0f", (-1) * (atof(toimabon086) + atof(toimanab087) - atof(toimcarg088) 
		                                           - atof(toimanca089) + atof(toimabcu109) - atof(toimcacu110)));
	}
	else
	{
		   toimpnet097[0]='C';
		   sprintf(toimpnet097+1, "%016.0f", atof(toimabon086) + atof(toimanab087) - atof(toimcarg088) 
		                                   - atof(toimanca089) + atof(toimabcu109) - atof(toimcacu110));
	}
	fprintLog(nomProg, "importe neto calculado >%.17s< float>%f<\n", toimpnet097, importeNeto);
	fprintLog(nomProg, "importe neto recibido  >%.17s<\n", ci_foritn->fidatiso.fiisotot.fitot097);
	 

	 if(   !strncmp(ci_foritn->fidatiso.fiisotot.fitot097,toimpnet097,17) 
	     ||(   !strncmp(ci_foritn->fidatiso.fiisotot.fitot097+1,"0000000000000000",16) 
		    && !strncmp(toimpnet097+1,"0000000000000000",16)))
	 { 
          strncpy(ci_foritn->fiisocom.respusal,"500",3);
          strncpy(ci_foritn->fidatpro.ficodfin, "000", 3);
          fprintLog(nomProg, "cuadra\n");
		  
		  //si no llega informado el 48.15 lo calculamos nosotros pq se necesita para contabilidad
		  fprintLog(nomProg, "48.15 >%.18s<\n", ci_foritn->fidatiso.fiisotot.fitot4815);
		  if(ci_foritn->fidatiso.fiisotot.fitot4815[0]==' ')
		  {
              retorno=tratar_fich("NPTACTO", COMPENS, impNeto4815, clave_toto);
			  if(!retorno)
			  {
			    fprintLog(nomProg, "impNeto >%s<\n", impNeto4815);
                strncpy(ci_foritn->fidatiso.fiisotot.fitot4815, impNeto4815, 180);
                ci_foritn->fidatiso.fiisotot.fitot4815[180]='\0';
			  }
			  else
			     fgraba_incidencia_cp("304", ci_foritn->fidatpro.finumope,"ftratato", "COMPENS");
          }
     }
     else
     {
              fprintLog(nomProg, "no cuadra\n");
              strncpy(ci_foritn->fiisocom.respusal,"501",3);
			  			strncpy(ci_foritn->fidatpro.ficodfin, "310", 3);
              if (strncmp(ci_foritn->fidatiso.fiisotot.fitot074,reg_toto1.tonuabon,10))
              {
                   strncpy(ci_foritn->fidatiso.fiisotot.fitot074,reg_toto1.tonuabon, 10);
                   ci_foritn->fidatiso.fiisotot.fitot074[10]='\0';
              } 
              if (strncmp(ci_foritn->fidatiso.fiisotot.fitot075,reg_toto1.tonuabon,10))
              {
                   strncpy(ci_foritn->fidatiso.fiisotot.fitot075,reg_toto1.tonuabon, 10);
                   ci_foritn->fidatiso.fiisotot.fitot075[10]='\0';
              } 
              if (strncmp(ci_foritn->fidatiso.fiisotot.fitot076,reg_toto1.tonucarg,10))
              {
                   strncpy(ci_foritn->fidatiso.fiisotot.fitot076,reg_toto1.tonucarg, 10);
                   ci_foritn->fidatiso.fiisotot.fitot076[10]='\0';
              } 
              if (strncmp(ci_foritn->fidatiso.fiisotot.fitot077,reg_toto1.tonuanca,10))
              {
                   strncpy(ci_foritn->fidatiso.fiisotot.fitot077,reg_toto1.tonuanca, 10);
                   ci_foritn->fidatiso.fiisotot.fitot077[10]='\0';
              }
              if (strncmp(ci_foritn->fidatiso.fiisotot.fitot086,reg_toto1.toimabon,16))
              {
                   strncpy(ci_foritn->fidatiso.fiisotot.fitot086,reg_toto1.toimabon, 16);
                   ci_foritn->fidatiso.fiisotot.fitot086[16]='\0';
              } 
              if (strncmp(ci_foritn->fidatiso.fiisotot.fitot087,reg_toto1.toimanab,16))
              {
                   strncpy(ci_foritn->fidatiso.fiisotot.fitot087,reg_toto1.toimanab, 16);
                   ci_foritn->fidatiso.fiisotot.fitot087[16]='\0';
              } 
              if (strncmp(ci_foritn->fidatiso.fiisotot.fitot088,reg_toto1.toimcarg,16))
              {
                   strncpy(ci_foritn->fidatiso.fiisotot.fitot088,reg_toto1.toimcarg, 16);
                   ci_foritn->fidatiso.fiisotot.fitot088[16]='\0';
              } 
              if (strncmp(ci_foritn->fidatiso.fiisotot.fitot089,reg_toto1.toimanca,16))
              {
                   strncpy(ci_foritn->fidatiso.fiisotot.fitot089,reg_toto1.toimanca, 16);
                   ci_foritn->fidatiso.fiisotot.fitot086[16]='\0';
              } 
              if (strncmp(ci_foritn->fidatiso.fiisotot.fitot109,reg_toto1.toimabcu,16))
              {
                   strncpy(ci_foritn->fidatiso.fiisotot.fitot109,reg_toto1.toimabcu, 16);
                   ci_foritn->fidatiso.fiisotot.fitot109[16]='\0';
              } 
              if (strncmp(ci_foritn->fidatiso.fiisotot.fitot110,reg_toto1.toimcacu,16))
              {
                   strncpy(ci_foritn->fidatiso.fiisotot.fitot110,reg_toto1.toimcacu, 16); 
                   ci_foritn->fidatiso.fiisotot.fitot110[16]='\0';
              } 
              strncpy(ci_foritn->fidatiso.fiisotot.fitot097, toimpnet097, 17);
              ci_foritn->fidatiso.fiisotot.fitot097[18]='\0';
			  
			  //calcula los importes netos por tipo de compensacion
			  //pq lo necesita cristina en el fi del 1534 si no se concilia
              retorno=tratar_fich("NPTACTO", COMPENS, impNeto4815, clave_toto);
			  if(!retorno)
			  {
			    fprintLog(nomProg, "impNeto >%s<\n", impNeto4815);
			    strncpy(ci_foritn->fidatiso.fiisotot.fitot4815, impNeto4815, 180); 
                ci_foritn->fidatiso.fiisotot.fitot4815[180]='\0';
			  }
			  else
			  {
			     fgraba_incidencia_cp("304", ci_foritn->fidatpro.finumope,"ftratato", "COMPENS");  
			  }			  
     } 
        
     fverfi(*ci_foritn); 

     return(0);
}
