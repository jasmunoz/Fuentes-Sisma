//PRJ0247167 - Revisado ampliación P48
//BIZUM revisar llamada a traduce_accion_puc
#include "nptacto.h" 
#include "nptpara.h"
#include "procesos.h" 
typedef struct cuotas
{
     char SignoCta[1];
     char impC[12];
     char impD[12];
} cuotas;

int ftraduce_accionSIO(formato_interno *pforitn);
int fgraActo(formato_interno * pforitn);
int fenvia_cola(int colaid,formato_interno *pforitn); 
int fverfi(formato_interno fi);
void ftraducr_cuotas(formato_interno *pforitn, cuotas *varAux);

int fgenera_admin(formato_interno * pforitn); //PRJ0258542 - Fase 3 - Generacion adminstrativa
int fenvia_var(formato_interno *pfoin); //PRJ0258542 - Fase 3 - Generacion adminstrativa
int qenvio, qrtratami, tsubsis; //PRJ0258542 - Fase 3 - Generacion adminstrativa

NPSPARA  *reg_para;
char pscerrado[1]; //PRJ0258542 - Fase 3 - Cambio 11052026
/**********************************************/
/***   funcionEO: ENVIA AL ORIGEN           ***/
/**********************************************/

int funcioneo(formato_interno * pforitn)
{
	int retorno;
	int    tipomsg;
	char idcsborigen_aux[5];
	char idcpdorigen_aux[3];  
	char respusal[3]; //PRJ0331925

	double importe_copia =0;
	char respusal_copia[3];
	char firespri_copia[2];
	int ll;

	/**  Rellena los campos generales del formato interno y transforma el mensaje de peticion en respuesta ***/
 	fprintLog(nomProg,"=======================\n");
 	fprintLog(nomProg,"= Funcion EO 29072025 =\n");
 	fprintLog(nomProg,"= Envia al origen     =\n");
 	fprintLog(nomProg,"=======================\n");
	
	fprintLog(nomProg,"** Numope: %.6s **\n", pforitn->fidatpro.finumope);

	/***    transforma el iso000 de peticion en respuesta   ***/
	retorno = fconvres(pforitn);
	
  fverfi(*pforitn);

	if ((retorno = carga_para("00")) != 0) //PRJ0387060
	{
	   fprintf(stdout,"**** Error carga tabla NPTPARA ****\n");
	}
	else //PRJ0387060
	{
		strncpy(pscerrado, reg_para[ll].pscerrando, 1); //PRJ0387060
	} //PRJ0387060

	for (ll=0 ; ll<2 ; ll++) 
	{  
		if ((strncmp(reg_para[ll].psnument,pforitn->fidatpro.fiorigen,2)==0))  //entidad
		{	
			strncpy(pscerrado, reg_para[ll].pscerrando, 1); //PRJ0258542 - Fase 3 - Cambio 11052026
			retorno = ftraduce_accionprice(pforitn);
			fprintLog(nomProg,"ftraduce_accionprice retorno->%d<- CODFIN ->%.3s<-\n", retorno, pforitn->fiisocom.respusal);
			fprintLog(nomProg," --- Valor del semaforo de falseo - %s - ---\n", pscerrado); //PRJ0258542 - Fase 3 - Cambio 11052026
			if (memcmp(pscerrado,"3",1) == 0 && (memcmp(pforitn->fiisocom.fiiso000,"1814",3) == 0 || memcmp(pforitn->fiisocom.fiiso000,"1834",3) == 0)) //PRJ0258542 - Fase 3 - Cambio 11052026
			{
				fprintLog(nomProg,"-- Semaforo a 3, se simula claves en cambio, se procede al falseo --\n"); 
				fprintLog(nomProg,"ftraduce_accionprice retorno->%d<- respusal ->%.3s<- P39 ->%.3s<-\n", retorno, pforitn->fiisocom.respusal, pforitn->fidatiso.fiisocla.ficla039);
				strncpy(pforitn->fiisocom.respusal, "801", 3);
				strncpy(pforitn->fidatiso.fiisocla.ficla039, "801", 3); 
				strncpy(pforitn->fidatiso.fiisocla.ficla096.longitud,"000",3);
				strncpy(pforitn->fidatiso.fiisocla.ficla096.datos," ",360);
				fprintLog(nomProg,"ftraduce_accionprice retorno->%d<- respusal ->%.3s<- P39 ->%.3s<-\n", retorno, pforitn->fiisocom.respusal, pforitn->fidatiso.fiisocla.ficla039);
			}
			break;            
		}  
		else
		{ 
			if((strncmp(reg_para[ll].psresolu,pforitn->fidatpro.fiorigen,2)==0) ||   //resolutor
					(strncmp(reg_para[ll].psmonito,pforitn->fidatpro.fiorigen,2)==0) ||   //monitor
					(strncmp(pforitn->fidatpro.fiorigen,"50",2)==0)) //diferido
			{
				break;
			}	
		}
	}

  //PRJ0331925 - INI
  fprintLog(nomProg," -- Validacion de respusal para marcar P38 ->%.3s< >%.6s< \n", pforitn->fiisocom.respusal, pforitn->fidatiso.fiisoaut.fiaut038);
  strncpy(respusal, "   ", 3);
  if (strlen(pforitn->fiisocom.respusal) > 0)
  {
  	strncpy(respusal, pforitn->fiisocom.respusal, 3);
    if (atoi(pforitn->fiisocom.respusal) < 100 && !isspace(respusal[0]))
    {
    	fprintLog(nomProg," -- Validacion de respusal porque retorno es menor de 100 y tiene datos -- \n");
    	if (nspaces(pforitn->fidatiso.fiisoaut.fiaut038, 6))
    	{
				strncpy(pforitn->fidatiso.fiisoaut.fiaut038, pforitn->fidatpro.finumope, 6);
				fprintLog(nomProg," -- Asigna P38 >%.6s< porque va vacio --\n", pforitn->fidatiso.fiisoaut.fiaut038);
			}
			else
			{
				fprintLog(nomProg," -- Mantiene P38 >%.6s< recibido de SF --\n", pforitn->fidatiso.fiisoaut.fiaut038);										
    	}
    }
	}
  //PRJ0331925 - FIN
	
	if (reg_para[ll].psnument==NULL)//origen desconocido no es diferido, ni redsys, ni resolutor
	{
	     //Añadido para Monitorizacion SISMA 
	          fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Envio origen codigo de error 248 no encontrado origen\n");
	          fgraba_incidencia_cp("248",pforitn->fidatpro.finumope,NULL,NULL);
	          return(11);
	 }	
   // una vez traducido el código de respuesta a la red
   // si es aceptada grabamos con ficodfin = '000' aunque sea otro código de aceptación de redsys
   // para que conste ok en las estadisticas (no es contable así que el importe no afecta a la conciliacion)
   if( strncmp(pforitn->fidatpro.ficodfin , "010" ,3)==0) // ACEPTADA POR IMPORTE PARCIAL
	   strncpy(pforitn->fidatpro.ficodfin , "000" ,3);
	
  
   retorno = fgraba_log(pforitn);
	//PRJ0258542 - Fase 3
  
  
   if(retorno)
   {      
 
    //Añadido para Monitorizacion SISMA 
    fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Escritura BBDD tabla NPTLOGO codigo de error  >%.3s<\n",coderror);
    fgraba_incidencia_cp("222",pforitn->fidatpro.finumope,NULL,NULL);

   }
   	fprintLog(nomProg,"Listo para enviar mensaje >%.4s<\n",pforitn->fiisocom.fiiso000);//PRJ0258542
		if( strncmp(pforitn->fiisocom.fiiso000 , "1644" ,4)==0)
		{
		   fprintLog(nomProg,"1644 no se responde\n");
		   return(11);
		}
		else
		{    /***   solo se contestan los distintos de 1644   ***/
				fprintLog(nomProg,"  Valor semaforo falseo para cerrar semaforo >%.1s<\n", pscerrado); //PRJ0387060
				fprintLog(nomProg,"  Valores a validar >%.4s< >%.2s<\n", pforitn->fiisocom.fiiso000, pforitn->fidatiso.fiisoaut.fiaut003.dig12); //PRJ0387060
				//if (memcmp(pscerrado,"4",1) == 0 && strncmp(pforitn->fiisocom.fiiso000, "11", 2) == 0 && strncmp(pforitn->fidatiso.fiisoaut.fiaut003.dig12, "20", 2) == 0) //PRJ0387060
				if (memcmp(pscerrado,"4",1) == 0 && (strncmp(pforitn->fiisocom.fiiso000, "11", 2) == 0 || strncmp(pforitn->fiisocom.fiiso000, "12", 2) == 0)) //PRJ0387060
				{
					fprintLog(nomProg,"-- Semaforo no responder activo, no se envia a resolutor --\n"); //PRJ0387060
					return(finalizar(11)); //PRJ0387060
				} //PRJ0387060
		   retorno = fenvia_origen(pforitn);
		   if (!retorno)
		  	fprintLog(nomProg,"Enviado al origen\n");
		}
	
    if(retorno) //retorno de enviar al origen
    {
     //NO SE PONE MENSAJE DE MONITORIZACION PORQUE ESTA EN LA FUNCION fenvia_origen(pforitn)
         fgraba_incidencia_cp("266", pforitn->fidatpro.finumope, NULL, NULL);
         return(11);
    }
    return(11);
}
/*** fin funcion de envia al origen ***********************************************************/


/***********************************************************************/
/*   fgraActo : graba en nptacto (tabla de acumulados para totales)    */
/***********************************************************************/
int fgraActo(formato_interno * pforitn)
{
    NPSACTO regacto;
    int ret=0;
    int escrito =0;
    cuotas varAux;

    if (pforitn->fidatpro.finototal[0]== 'N') //repeticiones cuya previa figura contestada
        return(0);                            // y anulaciones de previas no contables  
	
    if(!strncmp(pforitn->fidatpro.ficlatab + 8, "51", 2)) //repeticion ya resuelta, aqui no lo usamos
        return(0);
    
    if(!strncmp(pforitn->fidatpro.ficlatab + 2, "30", 2)) //consulta de saldos o movimientos
        return(0);
    
    if(!strncmp(pforitn->fidatpro.ficlatab, "22", 2)) // operaciones off desde el host
        return(0);
    
    if(!strncmp(pforitn->fidatpro.ficlatab, "130000000003", 12)) // los mensajes 9304 no se envian rmonitor.
        return(0);
    
    if(!strncmp(pforitn->fidatpro.ficlatab + 6 , "24", 2)) //mensajes  620 (comunicacion mensajes inf Funcion transparente)
                                                           //mensajes  304 (cambiar estado de una tarjeta)
        return(0);
    
    if(!strncmp(pforitn->fidatpro.ficlatab, "113000005030", 12)) // los mensajes consulta de saldo y movimiento,no enviar rmonitor
        return(0);

	
	//operaciones contables
    if (strncmp(pforitn->fiisocom.fiiso000, RESP_PETI_CONT, 4) == 0 //1210
     || strncmp(pforitn->fiisocom.fiiso000, RESP_COMU_CONT, 4) == 0 //1230
     || strncmp(pforitn->fiisocom.fiiso000, RESP_ANUL_PETI, 4) == 0 //1430
	 || strncmp(pforitn->fiisocom.fiiso000, RESP_COMU_CONT_TCAP, 4) == 0 //1234
	 || strncmp(pforitn->fiisocom.fiiso000, RESP_ANUL_TCAP, 4) == 0)      //1434
    {
       if (strncmp(pforitn->fidatpro.ficodfin, "000",3)) //operaciones que no finalizan ok
       {
       // esta soperaciones totalizan, pero solo en el numero, el importe a cero
          strncpy(regacto.atnumope, pforitn->fidatpro.finumope, sizeof(regacto.atnumope));
          strncpy(regacto.atcodfin, pforitn->fidatpro.ficodfin, sizeof(regacto.atcodfin));
          strncpy(regacto.atiso000, pforitn->fiisocom.fiiso000, sizeof(regacto.atiso000));
          strncpy(regacto.atdig12, pforitn->fidatiso.fiisoaut.fiaut003.dig12, sizeof(regacto.atdig12));
          strncpy(regacto.atimporte, "000000000000", sizeof(regacto.atimporte));
          strncpy(regacto.atredes, pforitn->fidatpro.fiorigen, 2);
		  strncpy(regacto.atredes +2, pforitn->fidatpro.fisubori, 2);
          strncpy(regacto.atsesion, pforitn->fidatiso.fiisoaut.fiaut029, sizeof(regacto.atsesion));
          strncpy(regacto.atimpcuod, "000000000000", 12);
		  strncpy(regacto.atimpcuoc, "000000000000", 12);
		  strncpy(regacto.atresol, pforitn->fidatpro.firespri, sizeof(regacto.atresol));
		  strncpy(regacto.atcompe,pforitn->fidatiso.fiisoaut.fiaut048.comun01.compen, sizeof(regacto.atcompe));
		  strncpy(regacto.atfecses, pforitn->fidatiso.fiisoaut.fiaut028.aa, 2);
		  strncpy(regacto.atfecses +2, pforitn->fidatiso.fiisoaut.fiaut028.mm, 2);
		  strncpy(regacto.atfecses +4, pforitn->fidatiso.fiisoaut.fiaut028.dd, 2);

          ret = tratar_fich("NPTACTO",ESCRIBIR,&regacto,NULL);
          if(ret) 
          {
#if defined (TRAZA) && (TRAZA > 1)  			  
             fprintLog(nomProg,"Error>%d< escribir NPTACTO con sesion>%.3s< numope>%.6s<\n", ret,regacto.atsesion,pforitn->fidatpro.finumope);
             //Añadido para Monitorizacion SISMA 
             fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Escritura tabla NPTACTO codigo de error >%d< con sesion>%.3s< numope>%.6s<\n",ret,regacto.atsesion,pforitn->fidatpro.finumope);
     
			 fprintLog(nomProg,"reg NPTACTO 0>%.*s<\n", sizeof(regacto),(char *)&regacto);
#endif			 
          }
          else
          {
#if defined (TRAZA) && (TRAZA > 10)  			  		  
             fprintLog(nomProg,"reg NPTACTO 0>%.*s<\n", sizeof(regacto),(char *)&regacto);
#endif			 
          }
          return(0);
       }
	   /// las que sí finalizan ok 
       strncpy(regacto.atnumope, pforitn->fidatpro.finumope, sizeof(regacto.atnumope));
       strncpy(regacto.atcodfin, pforitn->fidatpro.ficodfin, sizeof(regacto.atcodfin));
	   strncpy(regacto.atiso000, pforitn->fiisocom.fiiso000, sizeof(regacto.atiso000));
       strncpy(regacto.atdig12, pforitn->fidatiso.fiisoaut.fiaut003.dig12, sizeof(regacto.atdig12));
       strncpy(regacto.atredes, pforitn->fidatpro.fiorigen, 2);
	   strncpy(regacto.atredes +2, pforitn->fidatpro.fisubori, 2);
       strncpy(regacto.atresol, pforitn->fidatpro.firespri, sizeof(regacto.atresol));
       strncpy(regacto.atsesion, pforitn->fidatiso.fiisoaut.fiaut029, sizeof(regacto.atsesion));
       strncpy(regacto.atcompe,pforitn->fidatiso.fiisoaut.fiaut048.comun01.compen, sizeof(regacto.atcompe));	   
       strncpy(regacto.atfecses, pforitn->fidatiso.fiisoaut.fiaut028.aa, 2);
	   strncpy(regacto.atfecses +2, pforitn->fidatiso.fiisoaut.fiaut028.mm, 2);
	   strncpy(regacto.atfecses +4, pforitn->fidatiso.fiisoaut.fiaut028.dd, 2);
	   
	   // importe 
	   // si llega informada la tasa de conciliación, importe conciliacion
	   // si no, si llega la tasa del titular, importe en moneda del titular
	   // si no, el importe en moneda original
	   if(!nspaces(pforitn->fidatiso.fiisoaut.fiaut009, 8))
	     strncpy(regacto.atimporte, pforitn->fidatiso.fiisoaut.fiaut005, sizeof(regacto.atimporte));	   
	   else
	   {
	    if(!nspaces(pforitn->fidatiso.fiisoaut.fiaut010, 8))
	      strncpy(regacto.atimporte, pforitn->fidatiso.fiisoaut.fiaut006, sizeof(regacto.atimporte));	   
        else	    
          strncpy(regacto.atimporte, pforitn->fidatiso.fiisoaut.fiaut004, sizeof(regacto.atimporte));	   
	   }

       // pendiente, puede tener cuatro importes de cuota, y por cada uno debe grabar el total en el fichero, para C y para D
	   // inicializo las cuotas a 0
        strncpy(regacto.atimpcuod, "000000000000", 12);
        strncpy(regacto.atimpcuoc, "000000000000", 12);
		
       ftraducr_cuotas( pforitn, &varAux);
       strncpy(regacto.atimpcuoc, varAux.impC, 12);   //cuotas abono, 109
       strncpy(regacto.atimpcuod, varAux.impD, 12);   //cuotas cargo, 110
	   	   
      ret = tratar_fich("NPTACTO",ESCRIBIR,&regacto,NULL);
      if(ret)
      {
#if defined (TRAZA) && (TRAZA > 1)  			  	  
         fprintLog(nomProg,"Error >%d< al escribir en NPTACTO con sesion >%.3s< numope >%.6s<\n", ret,regacto.atsesion,pforitn->fidatpro.finumope);
		 fprintLog(nomProg,"reg NPTACTO >%.*s<\n",sizeof(regacto) ,(char *) &regacto.atnumope);
#endif	
         //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Escritura tabla NPTACTO codigo de error >%d< con sesion>%.3s< numope>%.6s<\n",ret,regacto.atsesion,pforitn->fidatpro.finumope);
	 
      }
      else
      {
#if defined (TRAZA) && (TRAZA > 10)  			  	  
         fprintLog(nomProg,"reg NPTACTO >%.*s<\n",sizeof(regacto) ,regacto.atnumope);
#endif            
      }
    }
    else
    {
#if defined (TRAZA) && (TRAZA > 10)  			  	
        fprintLog(nomProg,"No tiene que grabar en nptacto numope >%.6s< \n",pforitn->fidatpro.finumope);
#endif		
        ret=0;
    }

    return(ret);
}

/**************************************************************************/
/***   traduce la respuesta de ficodfin a respusal para enviar a PUC   ***/
/**************************************************************************/
int ftraduce_accionpuc (formato_interno *pforitn)
{
#include "tbfin039.h"
int ll;

int tiponum;
int codfin;
char p_039 [4];

  codfin = atoi(pforitn->fidatpro.ficodfin);
  tiponum = atoi(pforitn->fiisocom.fiiso000);

  //DMND0036176 añadidas las preautorizaciones
   
  if (!strncmp(pforitn->fidatpro.ficodfin, "000", 3)) //si es cero.
  {
   switch(tiponum)
   {
      case 1110:
      case 1210:
         strcpy(pforitn->fiisocom.respusal,"000");
         break;
      case 1130:
      case 1230:
      case 1121:
      case 1221:
      case 1913:
      case 1916:
         strcpy(pforitn->fiisocom.respusal,"900");
         break;
      case 1430:
      case 1421:
         strcpy(pforitn->fiisocom.respusal,"400");
         break;
      case 1324:
      case 1325:
         strcpy(pforitn->fiisocom.respusal,"300");
         break;
      case 1814:
      case 1834:
         strcpy(pforitn->fiisocom.respusal,"800");
         break;
      case 1644:
         strcpy(pforitn->fiisocom.respusal,"600");
         break;
      default:
         return(1);
   }
/*   if 
   {
/*    switch(tiponum) //BIZUM controlamos el retorno para operaciones BIZUM
    {
       case 1100:
       case 1120:
       case 1121:	   
       case 1420:
          strcpy(pforitn->fiisocom.respusal,"000");
          break;

       default:
          return(1);
	}		
   }*/   
  }
  else ///////// DENEGAR
   /***     falta rellenar la tabla tbfin039 completamente     ***/
  {
      /////// por defecto 190 en autorizaciones y 909 el resto
	  //DMND0036176 añadidas preautorizaciones
      switch(tiponum)
      {
        case 1110:
        case 1210:
          strcpy(pforitn->fiisocom.respusal, "190"); //denegar sin especificar motivo
        case 1534:
          strcpy(pforitn->fiisocom.respusal, "503"); //PTASK24762 - 02/06/2026 - no se puede devolver 909 en un mensaje de totales
        default:
          strcpy(pforitn->fiisocom.respusal, "909"); //error del sistema
      }

     // a las comunicaciones se contesta siempre con "900" salvo que el emisor responda algo específico
      if (( (strncmp(pforitn->fiisocom.fiiso000,"1130",4)==0) ||
            (strncmp(pforitn->fiisocom.fiiso000,"1230",4)==0)) &&
          (strncmp(pforitn->fiisocom.respusal,"9",1)!=0))   
                strcpy(pforitn->fiisocom.respusal,"900");
      else // para el resto y comunicaciones contestadas por el emisor
      {
	     ///// consulta la tabla de conversiones codfin - p39
        for (ll=0 ; ; ll++ )
        {
           if ((datos_fin039[ll].tab_fin == codfin) ||
               (datos_fin039[ll].tab_fin == 0))
           {
               sprintf(p_039, "%03d", datos_fin039[ll].tab_039);
               strcpy(pforitn->fiisocom.respusal , p_039);
               break;
           }
        }
      }
      strncpy(pforitn->fidatpro.ficodfin,pforitn->fiisocom.respusal,3);
	  
  }

  return(0);

}

/**********************************************/
/***   envia un mensaje  a  su origen       ***/
/**********************************************/
int fenvia_origen(formato_interno * pforitn)
{
 int qorigen, retorno;
 char cola_origen[9];
 char cola_diferido[9];
 int ll;
 
   fprintLog(nomProg,"fenvia_origen fiorigen >%s<\n", pforitn->fidatpro.fiorigen); //PRJ0258542 - Fase 3 - Cambio 11052026
   fprintLog(nomProg,"fenvia_origen firespri >%s<\n", pforitn->fidatpro.firespri); //PRJ0258542 - Fase 3 - Cambio 11052026
   fprintLog(nomProg,"fenvia_origen ficodfin >%s<\n", pforitn->fidatpro.ficodfin); //PRJ0258542 - Fase 3 - Cambio 11052026
   fprintLog(nomProg,"fenvia_origen fiiso024 >%s<\n", pforitn->fiisocom.fiiso024); //PRJ0258542 - Fase 3 - Cambio 11052026 //strncmp(pfi->fiisocom.fiiso024, "816", 3) == 0)

	//PRJ0258542 - Fase 3 - Manda mensaje administrativo 1644 si algo no ha ido bien //PRJ0258542 - Fase 3 - Cambio 11052026
	if(strncmp(pforitn->fiisocom.fiiso000 , "1814", 4) ==0)
	{
		if((strncmp(pforitn->fidatpro.ficodfin, "801", 3) == 0 ||strncmp(pforitn->fidatpro.ficodfin, "802", 3) == 0 || strncmp(pforitn->fidatpro.ficodfin, "916", 3) == 0) 
			&& (strncmp(pforitn->fidatpro.firespri, "30", 2) == 0 || strncmp(pforitn->fidatpro.firespri, "40", 2) == 0 || strncmp(pforitn->fidatpro.firespri, "20", 2) == 0 || strncmp(pforitn->fidatpro.firespri, "10", 2) == 0)
			&& (strncmp(pforitn->fidatpro.fiorigen, "01", 2) == 0 || strncmp(pforitn->fidatpro.fiorigen, "02", 2) == 0 || strncmp(pforitn->fidatpro.fiorigen, "03", 2) == 0 || strncmp(pforitn->fidatpro.fiorigen, "04", 2) == 0 || strncmp(pforitn->fidatpro.fiorigen, "05", 2) == 0))
		{
			fprintLog(nomProg,"Genera mensaje administrativo para >%s<\n", pforitn->fidatpro.firespri);
			retorno = fgenera_admin(pforitn);
			fprintLog(nomProg,"Salida mensaje administrativo para >%s<\n", pforitn->fidatpro.firespri);
		}
	}
	fprintLog(nomProg,"Continua proceso normal, envia al origen\n");

   if (strncmp(pforitn->fidatpro.fiorigen, "50", 2)) //origen no 50 manda a penvio correspondiente
   {        
	   strncpy(cola_origen,PENVIO,6);	   
       strncpy(cola_origen+6, pforitn->fidatpro.fiorigen,2);
       cola_origen[8] = '\0';
       qorigen = damecola(psvm, cola_origen,NULL);
	   fprintLog(nomProg,"origen no 50 qorigen >%d<\n", qorigen);
   }
   else   //origen 50 manda a cola diferido correspondiente
   {
	  for (ll=0 ; ll<2 ; ll++) 
    { 
	     if (strncmp(pforitn->fidatpro.firespri,reg_para[ll].psnument,2)==0) //origen redsys	     
	     { 
       		strncpy(cola_diferido,RDIF,4);
          strncpy(cola_diferido+4,reg_para[ll].psnument,2);
		     	strncpy(cola_diferido+6,"02",2);
          cola_diferido[8] = '\0';		 
          qorigen=damecola(psvm,cola_diferido,NULL); 
			 		fprintLog(nomProg,"origen 50 qorigen cola diferido >%d<\n", qorigen);
		     	break;
	     }
	     else   
	     {
	    		if (strncmp(pforitn->fidatpro.firespri,reg_para[ll].psresolu,2)==0) //origen resolutor
	       	{
		      	strncpy(cola_diferido,RDIF,4);
            strncpy(cola_diferido+4,reg_para[ll].psresolu,2);
		       	strncpy(cola_diferido+6,"01",2);
            cola_diferido[8] = '\0';		   
            qorigen=damecola(psvm,cola_diferido,NULL);
			   		fprintLog(nomProg,"origen 50 qorigen cola diferido >%d<\n", qorigen);
			   		break;
		    	}
	    	}
    }
	  if (reg_para[ll].psnument==NULL)//origen desconocido no es diferido, ni redsys, ni resolutor
	  {
	     //Añadido para Monitorizacion SISMA 
	     fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Envio origen codigo de error 248 no encontrado origen\n");
	     fgraba_incidencia_cp("248",pforitn->fidatpro.finumope,NULL,NULL);
	     return(11);
	  }
   }
  
   if (qorigen == -1)
   {
      fprintLog(nomProg, "Error al obtener la cola de envio al origen\n"); 
      //Añadido para Monitorizacion SISMA 
      fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Obtener cola origen codigo de error 009 \n");
      fgraba_incidencia_cp("009",pforitn->fidatpro.finumope,NULL,NULL);
      return(2);
   }
   else
   {
      retorno  = fenvia_cola(qorigen, pforitn);
      if (retorno != 0)
      {
         //NO SE AÑADE MONITORIZACION PORQUE EL MENSAJE YA ESTA EN LA FUNCION fenvia_cola(qorigen, pforitn)
         fgraba_incidencia_cp("266",pforitn->fidatpro.finumope,NULL,NULL);
      }
      else
	  {
        retorno = fgraActo(pforitn);
        if (retorno != 0)
        {
          //NO SE AÑADE MONITORIZACION PORQUE EL MENSAJE YA ESTA EN LA FUNCION fgraActo(pforitn)
          fgraba_incidencia_cp("203",pforitn->fidatpro.finumope,NULL,"NPTACTO");
          return(11);
        }
        else
          return(0);	   
	 }
   }
   
}/* fin funcion */

//PRJ0258542 - Fase 3 - INI - Generacion 1644 - Mensaje administrativo
int fgenera_admin(formato_interno * pforitn)
{
	formato_interno foin;
	char claveP[5];
	char auxYear[5];
	int retorno, pos_sub;
	char numoper[6]; //PRJ0258542 - Fase 3 - Cambio 11052026
  char numcross[10]; //PRJ0258542 - Fase 3 - Cambio 11052026

	fprintLog(nomProg,"*** Va a generar administrativa ***\n");

  retorno = inicfori(1644,&foin);
  strcpy(foin.fiisocom.fiiso000, "1644");

  tm_dec tm;
  fechorsys(&tm);
  strcpy(auxYear,itoa(tm.tm_year)); 
	strncpy(foin.fidatpro.fifecha0,"20",2); //PRJ0258542 - Fase 3 - Cambio 11052026
	sprintf(foin.fidatpro.fifecha0+2,"%02d", tm.tm_year - 2000); //PRJ0258542 - Fase 3 - Cambio 11052026
	sprintf(foin.fidatpro.fifecha0+4,"%02d", tm.tm_mon); //PRJ0258542 - Fase 3 - Cambio 11052026
	sprintf(foin.fidatpro.fifecha0+6,"%02d", tm.tm_mday); //PRJ0258542 - Fase 3 - Cambio 11052026
	sprintf(foin.fidatpro.fihora00,"%02d", tm.tm_hour); //PRJ0258542 - Fase 3 - Cambio 11052026
	sprintf(foin.fidatpro.fihora00+2,"%02d", tm.tm_min); //PRJ0258542 - Fase 3 - Cambio 11052026
	sprintf(foin.fidatpro.fihora00+4,"%02d", tm.tm_sec); //PRJ0258542 - Fase 3 - Cambio 11052026
  strncpy(foin.fiisocom.fifechor.aa, auxYear +2, 2); 
  lpad(foin.fiisocom.fifechor.aa, '0', 2);
  strcpy(foin.fiisocom.fifechor.mm, itoa(tm.tm_mon)); 
  lpad(foin.fiisocom.fifechor.mm, '0', 2);
  strcpy(foin.fiisocom.fifechor.dd, itoa(tm.tm_mday)); 
  lpad(foin.fiisocom.fifechor.dd, '0', 2);
  strcpy(foin.fiisocom.fifechor.hh, itoa(tm.tm_hour)); 
  lpad(foin.fiisocom.fifechor.hh, '0', 2);
  strcpy(foin.fiisocom.fifechor.mi, itoa(tm.tm_min));
  lpad(foin.fiisocom.fifechor.mi, '0', 2);
  strcpy(foin.fiisocom.fifechor.ss, itoa(tm.tm_sec)); 
  lpad(foin.fiisocom.fifechor.ss, '0', 2);
	if ((retorno = fnumope(numoper, numcross, '0')) != 0) //PRJ0258542 - Fase 3 - Cambio 11052026
	{
		fprintLog(nomProg,"error al asignar numero de operacion \n");
	  fgraba_incidencia_cp("264", NULL,NULL,NULL);
		strcpy(foin.fiisocom.respusal,"909");
	  return(retorno);
	}
	else
	{
	  strncpy(foin.fidatpro.finumope,numoper,6);
	  foin.fidatpro.finumope[6] = '\0';
	} //PRJ0258542 - Fase 3 - Cambio 11052026

	strcpy(foin.fiisocom.fiidtran, foin.fiisocom.fifechor.dd);
	strcpy(foin.fiisocom.fiidtran+2, foin.fiisocom.fifechor.hh);
	strcpy(foin.fiisocom.fiidtran+4, foin.fiisocom.fifechor.mi);

	strcpy(pforitn->fidatiso.fiisocla.ficla024, pforitn->fiisocom.fiiso024);
	strcpy(pforitn->fidatiso.fiisocla.ficla033.longitud, "06");
	strcpy(foin.fidatpro.firespri, pforitn->fidatpro.firespri);
	//fprintf(stdout,"foin.fidatpro.firespri antes >%.2s< \n",foin.fidatpro.firespri);
	fprintf(stdout,"Valores pforitn 00, 11, 12, 33, 39, 93, 94, 128, 96: >%.4s<>%.6s<>%.12s<>%.2s<>%.11s<>%.3s<>%.2s<>%.11s<>%.2s<>%.11s<>%.8s<>%.3s<>%.360s<\n",
									pforitn->fiisocom.fiiso000,pforitn->fidatiso.fiisocla.ficla011,pforitn->fidatiso.fiisocla.ficla012,
									pforitn->fidatiso.fiisocla.ficla033.longitud,pforitn->fidatiso.fiisocla.ficla033.datos,pforitn->fidatiso.fiisocla.ficla039,
									pforitn->fidatiso.fiisocla.ficla093.longitud,pforitn->fidatiso.fiisocla.ficla093.datos,pforitn->fidatiso.fiisocla.ficla094.longitud,pforitn->fidatiso.fiisocla.ficla094.datos,
									pforitn->fidatiso.fiisocla.ficlamac,pforitn->fidatiso.fiisocla.ficla096.longitud,pforitn->fidatiso.fiisocla.ficla096.datos);

	//strcpy(foin.fiisocom.fiiso024 , pforitn->fiisocom.fiiso024); //AQUI
	strcpy(foin.fiisocom.fiiso024 , "892"); //Segun comunicado, se manda un 892 en el P24
	strcpy(foin.fidatiso.fiisoadm.fiadm039 , pforitn->fidatiso.fiisocla.ficla039);
	strcpy(foin.fiisocom.respusal , pforitn->fidatpro.ficodfin);

  strcpy(foin.fidatiso.fiisoadm.fiadm072.longitud , "085");
  //strncpy(foin.fidatiso.fiisoadm.fiadm072.datos , "999", 3);
  strncpy(foin.fidatiso.fiisoadm.fiadm072.datos , "001", 3); //PRJ0258542 - Fase 3 - Cambio 11-03-2025 - Nos indican que contestemos bajo este formato
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+3, pforitn->fiisocom.fiiso000, 4);
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+7, "                ", 16);
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+23, pforitn->fidatiso.fiisocla.ficla011, 6);
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+29, pforitn->fidatiso.fiisocla.ficla012, 12);
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+41, pforitn->fidatiso.fiisocla.ficla033.longitud, 2);
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+43, pforitn->fidatiso.fiisocla.ficla033.datos, 6);
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+49, pforitn->fidatiso.fiisocla.ficla039, 3);
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+52, pforitn->fidatiso.fiisocla.ficla093.longitud, 2);
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+54, pforitn->fidatiso.fiisocla.ficla093.datos, 6);
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+60, pforitn->fidatiso.fiisocla.ficla094.longitud, 2);
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+62, pforitn->fidatiso.fiisocla.ficla094.datos, 6);
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+68, pforitn->fidatiso.fiisocla.ficla096.longitud, 3);
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+71, pforitn->fidatiso.fiisocla.ficla096.datos, 6);
	memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+77, pforitn->fidatiso.fiisocla.ficlamac, 8);
	
	//fprintLog(nomProg,"fiadm072 -----------------------\n");
	//fhexdump(stdout,foin.fidatiso.fiisoadm.fiadm072.datos,atoi(foin.fidatiso.fiisoadm.fiadm072.longitud));
	//fprintLog(nomProg,"fiadm072 -----------------------\n");

	//strncpy(foin.fidatiso.fiisoadm.fiadm093.longitud, (psvm+pos_sub)->str_svm.svmpars.pacidest,2);
	//strncpy(foin.fidatiso.fiisoadm.fiadm093.datos, (psvm+pos_sub)->str_svm.svmpars.pacidest + 2,11);
	//strncpy(foin.fiisocom.fi032094.longitud, (psvm+pos_sub)->str_svm.svmpars.paciadqui,2);
	//strncpy(foin.fiisocom.fi032094.datos, (psvm+pos_sub)->str_svm.svmpars.paciadqui + 2,11);
	//strncpy(foin.fidatiso.fiisoadm.fiadm094.longitud, (psvm+pos_sub)->str_svm.svmpars.paciadqui,2);
	//strncpy(foin.fidatiso.fiisoadm.fiadm094.datos, (psvm+pos_sub)->str_svm.svmpars.paciadqui + 2,11);
	memcpy(foin.fidatiso.fiisoadm.fiadm093.longitud, pforitn->fidatiso.fiisocla.ficla093.longitud, 2);
	memcpy(foin.fidatiso.fiisoadm.fiadm093.datos, pforitn->fidatiso.fiisocla.ficla093.datos, 11);
	memcpy(foin.fidatiso.fiisoadm.fiadm094.longitud, pforitn->fidatiso.fiisocla.ficla094.longitud,2);
	memcpy(foin.fidatiso.fiisoadm.fiadm094.datos, pforitn->fidatiso.fiisocla.ficla094.datos, 11);
	memcpy(foin.fiisocom.fi032094.longitud, pforitn->fidatiso.fiisocla.ficla094.longitud, 2);
	memcpy(foin.fiisocom.fi032094.datos, pforitn->fidatiso.fiisocla.ficla094.datos, 11); //PRJ0258542 - Fase 3 - Cambio 11052026 - estaba 093 en lugar de 094

	//foin.fidatiso.fiisoadm.fiadm093.longitud[2] = '\0';
	//foin.fidatiso.fiisoadm.fiadm093.datos[11] = '\0';
	//foin.fidatiso.fiisoadm.fiadm094.longitud[2] = '\0';
	//foin.fidatiso.fiisoadm.fiadm094.datos[11] = '\0';
	//foin.fiisocom.fi032094.longitud[2] = '\0';
	//foin.fiisocom.fi032094.datos[11] = '\0';

	fprintLog(nomProg,"foin.fidatiso.fiisoadm.fiadm093.longitud >%.2s< \n",foin.fidatiso.fiisoadm.fiadm093.longitud);
	fprintLog(nomProg,"foin.fidatiso.fiisoadm.fiadm093.datos    >%.11s< \n",foin.fidatiso.fiisoadm.fiadm093.datos);
	fprintLog(nomProg,"foin.fidatiso.fiisoadm.fiadm094.longitud >%.2s< \n",foin.fidatiso.fiisoadm.fiadm094.longitud);
	fprintLog(nomProg,"foin.fidatiso.fiisoadm.fiadm094.datos    >%.11s< \n",foin.fidatiso.fiisoadm.fiadm094.datos);
	retorno = fenvia_var(&foin);
	if (retorno != 0)
	{
		fprintLog(nomProg,"error al enviar a red un 1644 de error\n");
	}
	retorno = fgraba_log(&foin); //PRJ0258542 - Fase 3 - Cambio 11052026
	return (0);
}

int fenvia_var(formato_interno *pfoin)
{
	int qorigen, retorno;
	char cola_origen[9];
	mensaje_nuc mensa1;

 	fprintLog(nomProg,"------- Voy a enviar al PENVIO%.2s -----\n",pfoin->fidatpro.firespri);
	int aux_longmensa = sizeof(mensa1.fiforitn);
	strncpy(cola_origen,PENVIO,6);	   
	strncpy(cola_origen+6, pfoin->fidatpro.firespri,2);
	cola_origen[8] = '\0';
	fprintLog(nomProg,"cola_origen proceso >%.8s< y psvm >%s<\n", cola_origen, psvm);
	qorigen = damecola(psvm, cola_origen,NULL);
	//fprintLog(nomProg,"origen numero qorigen >%d<\n", qorigen);

 	if (atoi(pfoin->fidatpro.fipidori) !=0)
		mensa1.tipo = atoi(pfoin->fidatpro.fipidori);
 	else
		mensa1.tipo=1111;

	memcpy(&mensa1.fiforitn,pfoin,sizeof(mensa1.fiforitn));
	errno=0;
	signal(SIGALRM,sigue);
 	alarm(ESSNDCOL);
 	fprintLog(nomProg,"long_mensa %d por la cola %i\n", aux_longmensa, qorigen);
 	if ((msgsnd(qorigen, &mensa1, aux_longmensa, 0))== -1)
 	{
		alarm(0);
		perror("mensaje>>>>>");
		fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LA COLA DE %.8s codigo de error >%i<\n",cola_origen, errno);
		return(-1);
	}
 	alarm(0);
 	return (0);
}  //*  fin de fenvia_var  

//PRJ0258542 - Fase 3 - FIN generacion mensaje administrativo

/**********************************************/
/***   funcion fenvia_cola                  ***/
/**********************************************/
int fenvia_cola(int colaid,formato_interno *pforitn)
{
	mensaje_nuc  mensa;
	int long_mensa = sizeof(mensa.fiforitn); //PRJ0258452 - quitado = sizeof(mensa)-sizeof(mensa.tipo);

	errno=0;

	pforitn->fidatpro.fipidori[5]='\0';
	if (atoi(pforitn->fidatpro.fipidori) !=0)
		mensa.tipo = atoi(pforitn->fidatpro.fipidori);
	else
		mensa.tipo=1111;  

	memcpy(&mensa.fiforitn,pforitn,sizeof(mensa.fiforitn));
	signal(SIGALRM,sigue);
	alarm(ESSNDCOL);
	if ((msgsnd(colaid, &mensa, long_mensa,0))== -1)
	{
        fprintLog(nomProg,"Error >%d< al enviar por la cola >%d<\n", errno, colaid);
       //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Envio PENVIO %d codigo de error >%d<\n",colaid, errno);
	
        perror("msgsnd");
        alarm(0);
        return(2);
   }
   else
   {
      alarm(0);
#if defined (TRAZA) && (TRAZA > 1)  			  	  
      fprintLog(nomProg,"Enviado por colaid >%d<\n", colaid);
#endif		  	  
      return(0);
   }
}

/**************************************************************************/
/***   traduce la respuesta de ficodfin a respusal para enviar a Price   ***/
/**************************************************************************/
int ftraduce_accionprice(formato_interno *pforitn)
{
#include "tbfin039.h"
 int ll;
 int tiponum;
 int codfin;
 char p_039 [4];

  codfin = atoi(pforitn->fidatpro.ficodfin);
  tiponum = atoi(pforitn->fiisocom.fiiso000);

//PRJ0331925 - Cambio el literal de la traza para poder ver el codfin y el P24
  fprintLog(nomProg,"--- Valor de codfin >%d< y valor de P24 >%.3s<\n", codfin, pforitn->fiisocom.fiiso024);

  if (!strncmp(pforitn->fidatpro.ficodfin, "000", 3))
  {
	//DMND0036176 añadidas las preautorizaciones
    switch(tiponum)
    {
    	case 1110:
      case 1210:
      	//RAQUEL PREAUTORIZACIONES DMND0036176 si la operación es de importe 0 y codfunc p24=150 en vez de respusal 000, es 085.
        //PRJ0331925 - Se pueden recibir operaciones de importe superior a 0 - comunicado 19/24
        fprintLog(nomProg,"-- Valor inicial Iso24 >%.3s< Iso04 >%.12s< Respusal >%.3s< \n", pforitn->fiisocom.fiiso024, pforitn->fidatiso.fiisoaut.fiaut004, pforitn->fiisocom.respusal);
        if(strncmp(pforitn->fiisocom.fiiso024, "150",3) == 0) //PRJ0331925 - operaciones de importe superior a 0
        {
        	if(strncmp(pforitn->fidatiso.fiisoaut.fiaut004, "000000000000",12) == 0)
      		{			   
      			if (strncmp(pforitn->fiisocom.respusal,"01",2) == 0)
      			{
            	strcpy(pforitn->fiisocom.respusal,"085");
            }
      			if (strncmp(pforitn->fiisocom.respusal,"85",2) == 0)
      			{
            	strcpy(pforitn->fiisocom.respusal,"085");
            }
         	}
        	if (strncmp(pforitn->fidatiso.fiisoaut.fiaut004, "000000000000",12) != 0)
      		{			   
      			if (strncmp(pforitn->fiisocom.respusal,"01",2) == 0)
      			{
            	strcpy(pforitn->fiisocom.respusal,"000");
            }
      			if (strncmp(pforitn->fiisocom.respusal,"85",2) == 0)
      			{
            	strcpy(pforitn->fiisocom.respusal,"085");
            }
         	}
       	}			  
        else
        {			   
        	strcpy(pforitn->fiisocom.respusal,"000");
        }			  
        fprintLog(nomProg,"-- Valor final Iso24 >%.3s< Iso04 >%.12s< Respusal >%.3s< \n", pforitn->fiisocom.fiiso024, pforitn->fidatiso.fiisoaut.fiaut004, pforitn->fiisocom.respusal);
        break;
       case 1130:
       case 1230:
       case 1121:
       case 1221:
       case 1913:
       case 1916:
           strcpy(pforitn->fiisocom.respusal,"900");
           break;
       case 1430:
       case 1421:
           strcpy(pforitn->fiisocom.respusal,"400");
           break;
       case 1334:
           strcpy(pforitn->fiisocom.respusal,"300");
           break;
       case 1814:
       case 1834:
           strcpy(pforitn->fiisocom.respusal,"800");
           break;
       case 1644:
           strcpy(pforitn->fiisocom.respusal,"600");
           break;
       default:
					//BIMSF-4997 - Evitar que no viaje otra cosa distinta de 500, 501 o 503 en respuestas a mensaje de totales
					if (tiponum == 1534)
					{
						fprintLog(nomProg,"1-- -- Respueta a mensaje de totales, valor P39 >%s< 1 \n", pforitn->fiisocom.respusal);
					  if (strncmp(pforitn->fiisocom.respusal,"500",3) !=0 && strncmp(pforitn->fiisocom.respusal,"501",3) !=0 && strncmp(pforitn->fiisocom.respusal,"503",3) !=0)
					  {
							fprintLog(nomProg,"-- -- Respueta a mensaje de totales se cambia a 503\n");
							strcpy(pforitn->fiisocom.respusal,"503");
					  }
						fprintLog(nomProg,"-- -- Respueta a mensaje de totales, valor P39 >%s< 2 \n", pforitn->fiisocom.respusal);
					}
					//BIMSF-4997 - Evitar que no viaje otra cosa distinta de 500, 501 o 503 en respuestas a mensaje de totales
           return(1);
    }
  }
  else
  {
/***     falta rellenar la tabla tbfin039 completamente     ***/
     //por defecto las peticiones denegadas se responden con 190
	 //si no son peticiones con 909
//No traducimos, ya viene el código traducido
// RAQUEL AÑADIDO PARA LOS 76 DEL RESPUSAL
     strncpy(pforitn->fiisocom.respusal, pforitn->fidatpro.ficodfin, 3);  
     if (strncmp(pforitn->fidatpro.ficodfin, "297", 3) ==0)
        strcpy(pforitn->fiisocom.respusal,"912");
     if (strncmp(pforitn->fidatpro.ficodfin, "225", 3) ==0)
        strcpy(pforitn->fiisocom.respusal,"942");
       
     if (strncmp(pforitn->fidatpro.ficodfin, "263", 3) ==0)
        strcpy(pforitn->fiisocom.respusal,"943");
 
     if (strncmp(pforitn->fidatpro.ficodfin, "223", 3) ==0) //PTASK0022158 - CONTROL DE RESPUESTA 223
     {
        strcpy(pforitn->fiisocom.respusal,"480");
     }
     if (strncmp(pforitn->fidatpro.ficodfin, "282", 3) ==0) //PTASK0022158 - CONTROL DE RESPUESTA 282
     {
        strcpy(pforitn->fiisocom.respusal,"916");
     }
     if (strncmp(pforitn->fidatpro.ficodfin, "261", 3) ==0) //PTASK0022158 - CONTROL DE RESPUESTA 261
     {
        strcpy(pforitn->fiisocom.respusal,"190");
     }
        
     printf("pforitn->fiisocom.respusal <%s>\n", pforitn->fiisocom.respusal); //PTASK0022158 - MUEVO LA TRAZA DE SITIO
 
   
   // el "190" solo vale para autorizaciones, el resto se contestan con "909"
   if (strncmp(pforitn->fiisocom.respusal,"190",3)==0
   &&  tiponum != 1210 && tiponum != 1110)
   {
	    strcpy(pforitn->fiisocom.respusal, "909"); // (error del sistema)
   }
  } 
	//BIMSF-4997 - Evitar que no viaje otra cosa distinta de 500, 501 o 503 en respuestas a mensaje de totales
  if (tiponum == 1534)
  {
  	fprintLog(nomProg,"2-- -- Respueta a mensaje de totales, valor P39 >%s< 1 \n", pforitn->fiisocom.respusal);
    if (strncmp(pforitn->fiisocom.respusal,"500",3) !=0 && strncmp(pforitn->fiisocom.respusal,"501",3) !=0 && strncmp(pforitn->fiisocom.respusal,"503",3) !=0)
    {
  		fprintLog(nomProg,"-- -- Respueta a mensaje de totales se cambia a 503\n");
			strcpy(pforitn->fiisocom.respusal,"503");
    }
  	fprintLog(nomProg,"-- -- Respueta a mensaje de totales, valor P39 >%s< 2 \n", pforitn->fiisocom.respusal);
  }
	//BIMSF-4997 - Evitar que no viaje otra cosa distinta de 500, 501 o 503 en respuestas a mensaje de totales

  return(0);
}

/**************************************************************************/
/***   traduce la respuesta de ficodfin a respusal para enviar al Nodo  ***/
/**************************************************************************/
int ftraduce_accionSIO(formato_interno *pforitn)
{
  #include "tbfin033.h"
  int ll;

  int tiponum;
  int codfin;
  char p_033 [4];

 codfin = atoi(pforitn->fidatpro.ficodfin);
 tiponum = atoi(pforitn->fiisocom.fiiso000);

 switch(codfin)
 {
  case 777:
    if (strncmp(pforitn->fidatiso.fiisoaut.fiaut003.dig12, "00",2)==0)
    {
      //fprintLog(nomProg,"codfin >%d<  fiaut003.dig12,>%s<\n", codfin, pforitn->fidatiso.fiisoaut.fiaut003.dig12);
      strcpy(pforitn->fiisocom.respusal, "077"); // para probar los ajustes tenemos que contestar concretamente esto en venta
    }
    else
      strcpy(pforitn->fiisocom.respusal, "076"); // para probar los ajustes tenemos que contestar concretamente esto en cajero
    break;
  case 0:
    switch(tiponum)
    {
       case 110:
       case 130:
       case 210:
       case 230:
       case 270:
       case 314:
       case 430:
       case 450:
       case 610:
       case 630:
       case 710:
       case 730:
       case 5010:
       case 5030:
       case 5110:
       case 5130:
       case 5210:
       case 5230:
       case 5310:
       case 5330:
           strcpy(pforitn->fiisocom.respusal,"001");
           break;
       default:
           return(1);
    }
    break;
  default:
/***     falta rellenar la tabla tbfin033 completamente     ***/
     strcpy(pforitn->fiisocom.respusal, "076"); // por defecto
     for (ll=0 ; ; ll++ )
     {
         if((datos_fin033[ll].tab_fin == codfin) ||
            (datos_fin033[ll].tab_fin == 0))
         {
             sprintf(p_033, "%03d", datos_fin033[ll].tab_033);
             strcpy(pforitn->fiisocom.respusal , p_033);
             break;
         }
     }

   }
   return(0);
}
/**************************************************************************/
/***   traduce el formato 046 a una variable donde estan las cuotas     ***/
/**************************************************************************/
void ftraducr_cuotas(formato_interno *pforitn, cuotas *varAux)
{
 int tam46;
 char aux[9];
 int numC, i;
 int impC=0, impD=0; 

  strncpy(aux, pforitn->fidatiso.fiisoaut.fiaut046.longitud, 2);
  aux[2]='\0';
  tam46=atoi(aux);
  numC=tam46/11;
  
  //si no hay cuota inicializo a 000000 signo C
  strncpy(varAux->SignoCta, "C", 1);
  
  for (i=0; i<numC; i++)
  { 
#if defined (TRAZA) && (TRAZA > 10)  			    
     fprintLog(nomProg,"valor de 046: %s\n", pforitn->fidatiso.fiisoaut.fiaut046.datos+i*11+2);
#endif	 
     if (!strncmp(pforitn->fidatiso.fiisoaut.fiaut046.datos+(i*11)+2,"C",1))
     {
        //fprintLog(nomProg,"estoy en C\n");
        if (!strcmp(varAux->SignoCta,"D"))
            strncpy(varAux->SignoCta, "X", 1);
        else
            strncpy(varAux->SignoCta, "C", 1);
        strncpy(aux, pforitn->fidatiso.fiisoaut.fiaut046.datos+(i*11)+2+1,8);
        aux[8]='\0';
        impC=impC+atoi(aux);
     }
     else
     {
        //fprintLog(nomProg,"estoy en D\n");
        if (!strcmp(varAux->SignoCta,"C"))
            strncpy(varAux->SignoCta, "X", 1);
        else
            strncpy(varAux->SignoCta, "D", 1);
        strncpy(aux, pforitn->fidatiso.fiisoaut.fiaut046.datos+(i*11)+2+1,8);
        aux[8]='\0';
        impD=impD+atoi(aux);
     }
  }
  sprintf(varAux->impC, "%.012d", impC);
  sprintf(varAux->impD, "%.012d", impD);
}
