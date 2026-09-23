#include "nptiden.h"  
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/opensslv.h>
#include "nptpara.h"

int fdesepin(int longi, char *sempaq, char *sdesem); 

int calculo_bloquePIN(formato_interno *pfi, char * offsetS); //PRJ0258542
//long fQuitar_Blancos (char * cadena_entrada, long long_entrada); //PRJ0258542

NPSPARA  *reg_para;
    
/***********************************************************************
  FUNCION   funcionVH

  DESCRIPCION : tratamiento de las validaciones de MAC y ARQC.
                MAC en autorizaciones y actualizacion de ficheros
				ARQC en autorizaciones con tarjeta EMV
***********************************************************************/

int funcionvh(formato_interno *pforitn)
{
 	NPSIDEN regiden; 
 	char clave_iden[5];
 	char zona_mac[3];
 	char tipo_mac[3]; //PRJ0258542
 	int tipo_macn; //PRJ0258542
 	char comando[50]; //PRJ0258542
 	// PRJ0202598 cambio bin de 6 posiciones 8 
 	char clave_bin[8];
 	int retorno =0; 

 	int ii;
 	unsigned char cc;
 	char linea[500];
 	char * addr;
 	
 	char area_machsm[500];
 	char datos_hsm[500];
 	int long_hsm = 0;
 	char mac_desempaq[16];
 	char mac_desempaq_aes[32]; //PRJ0258542
 	char mac_calculado[4096]; // tamano LONG_MSG pq en el hsm se inicializa con el total del tamano
 	char arqc_verificado[4096];  
 	char datos_criptograma[200];

 	char clave1[151];
  char clave2[151];
  char claveMac[151]; //PRJ0258542 - pasa de 50 a 151
  char claveEMV[151]; //PRJ0258542 - pasa de 50 a 151
  char offsetS[4]; //PRJ0258542
  NPSDHSM reghsm;
  char codResp[3];
  int ll;
  char clavecone[4];

  
#if defined (TRAZA) && (TRAZA > 5)     
 	fprintLog(nomProg,"=======================\n");
 	fprintLog(nomProg,"= Funcion VH 29072025 =\n");
 	fprintLog(nomProg,"= Validacion MAC      =\n");
 	fprintLog(nomProg,"=======================\n");
#endif 

//Inicializacion de variables
 	memset(mac_calculado,'\0',sizeof(mac_calculado));
  memset(area_machsm,'\0',sizeof(area_machsm));
  memset(datos_hsm,'\0',sizeof(datos_hsm));
  memset(mac_desempaq ,'\0', sizeof(mac_desempaq)); 
  memset(mac_desempaq_aes ,'\0', sizeof(mac_desempaq_aes)); //PRJ0258542
  memset(arqc_verificado,'\0',sizeof(arqc_verificado));
  memset(datos_criptograma, '\0', sizeof(datos_criptograma)); 

//Obtencion de los identificadores de claves hsm
//en este caso validamos con las claves acordadas con la red por la que entra el mensaje
  strncpy(clave_iden, pforitn->fidatpro.fiorigen, 2);
  strncpy(clave_iden + 2, pforitn->fidatpro.fisubori, 2); 
  clave_iden[4]='\0';
  
  // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a finumPAN pero si a clave_bin
 	strncpy(clave_bin, pforitn->fidatiso.fiisoaut.finumPAN.datos, 8);
 	clave_bin[8]='\0';
  
#if defined (TRAZA) && (TRAZA > 10)  
	fprintLog(nomProg,"- Numope >%.6s<\n",pforitn->fidatpro.finumope);
  fprintLog(nomProg,"Subsistema de claves hsm >%s<\n",clave_iden);
#endif
   //las claves de mac no van por bin
 	for (ll=0 ; ll<2 ; ll++) 
  {
  	if (strncmp(pforitn->fidatpro.fiorigen,reg_para[ll].psnument,2)==0)
		{
			//PRJ0258542- INI
			tipo_macn = 0;
			strncpy(tipo_mac, "  ", 2);
			tipo_mac[2] = '\0';
			strncpy(tipo_mac, pforitn->fidatiso.fiisoaut.fiaut053 + 2, 2);
			tipo_macn = atoi(tipo_mac);
			fprintLog(nomProg, "Valor de P53 completo >%.16s< y posicion 3y4 >%.2s< numero >%d< valor ll >%d<\n ",pforitn->fidatiso.fiisoaut.fiaut053,tipo_mac,tipo_macn,ll);
			//fprintLog(nomProg, "Valor de reg_para[ll].psclzaka >%.50s< y reg_para[ll].psclvhsm >%.50s<\n ",reg_para[ll].psclzaka,reg_para[ll].psclvhsm);
      switch (tipo_macn)
      {
       	case 00: //TDES sin PIN
       	case 01: //Algoritmo DES
        	strncpy(comando,reg_para[ll].psclvhsm,50);
        	comando[50]='\0';
        	break;
        case 02: //Algoritmo 3DES
        case 03: //TDES
        	strncpy(comando,reg_para[ll].psclvhsm,50);
        	comando[50]='\0';
        	break;
        case 05: //AES
        	strncpy(comando,reg_para[ll].psclzaka,50);
        	comando[50]='\0';
        	break;
 			}
    	//retorno = claveshsm(reg_para[ll].psclvhsm, clave_iden, "000000", clave1, clave2);
    	retorno = claveshsm(comando, clave_iden, "000000", clave1, clave2);
			break;
			//PRJ0258542- FIN
    }
  }
   
   if (retorno<0)
   {
#if defined (TRAZA) && (TRAZA > 1)     
	      fprintLog(nomProg, "Error al obtener la clave\n");
#endif		
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA ACCESO HSM: conexion HSM  error al obtener la clave retorno>%i<\n", retorno);
  
      strcpy(coderror, "187"); //PRJ0258542
			strcpy(reginci.innomfic,"HSM"); //PRJ0258542
      strcpy(pforitn->fidatpro.ficodfin, "187"); //PRJ0258542
      strcpy(pforitn->fidatpro.firesfin, "00"); //PRJ0258542
		  return(finalizar(98)); 
   }

  //zona mac segun indique el p53  
  if(strncmp(pforitn->fiisocom.fiiso000,"13",2)==0) //////// en operaciones de act. ficheros
  {
    strncpy(zona_mac,pforitn->fidatiso.fiisofic.fific053 + 8, 2);
    zona_mac[2]='\0';
  }
  else /////////////////////////////////////////////////// en autorizaciones
  {
    strncpy(zona_mac,pforitn->fidatiso.fiisoaut.fiaut053 + 8, 2);
    zona_mac[2]='\0';
  }
    
  // clave de mac----------------------------------------------------------------------------
  if(strncmp(zona_mac,"01",2)==0)
  {
  	strncpy(pforitn->fidatpro.fimacarq.idmac, clave1+5,5); //guardo solo el identificador en el fi
 	 	strncpy(claveMac,clave1, strlen(clave1));
	 	claveMac[strlen(clave1)]='\0';
 	}
  else
    if(strncmp(zona_mac,"02",2)==0)
		{
	   	strncpy(pforitn->fidatpro.fimacarq.idmac, clave2+5,5); //guardo solo el identificador en el fi
  	  strncpy(claveMac,clave2, strlen(clave2));
	   	claveMac[strlen(clave2)]='\0';
		}
		else
		{
#if defined (TRAZA) && (TRAZA > 1)  	
     	fprintLog(nomProg,"Zona MAC no esperada %.2s\n", zona_mac);
#endif	  
      strcpy(coderror, "916"); //PRJ0258542
			strcpy(reginci.innomfic,"HSM"); //PRJ0258542
      strcpy(pforitn->fidatpro.ficodfin, "916"); //PRJ0258542
      strcpy(pforitn->fidatpro.firesfin, "00"); //PRJ0258542
      return(finalizar(98));
		}
  

  //calculo del MAC ----------------------------------------------------------------------
#if defined (TRAZA) && (TRAZA > 10)    
  fprintLog(nomProg,"longitud de fiaremac >%d<\n",strlen(pforitn->fidatpro.fiaremac));
#endif  
  addr = pforitn->fidatpro.fiaremac; 

  for (ii=0; ii < strlen(pforitn->fidatpro.fiaremac); ii++)
  {
		cc = *(addr + ii);
		sprintf(linea+2*ii, "%02X", cc);
  }
  
  strncpy(area_machsm,linea,strlen(pforitn->fidatpro.fiaremac)*2);
  long_hsm = (strlen(pforitn->fidatpro.fiaremac)*2);
  area_machsm[long_hsm]='\0';
  
#if defined (TRAZA) && (TRAZA > 10)    
  fprintLog(nomProg,"long_hsm <%d> area_machsm >%.*s<\n",long_hsm,long_hsm,area_machsm);
#endif  

  strncpy(datos_hsm, area_machsm, long_hsm);
  datos_hsm[long_hsm]='\0';

	//PRJ0258542 - Si es AES tengo que llamar a la funcion 07 que es el 802 para CMAC en lugar de al 801 para MAC
	fprintLog(nomProg, "Valor de la nueva variable tipo_macn - ftratavh >%d<\n", tipo_macn); //PRJ0258542
	if (tipo_macn == 5)
	{
	  retorno = manejhsm(claveMac, "07", tipo_macn, area_machsm, long_hsm, sockssl, mac_calculado); //PRJ0258542 - Se manda al comando 07
	  fprintLog(nomProg, "Valor de retorno de calculo de CMAC - ftratavh >%d<\n", retorno); //PRJ0258542
	}
	else //PRJ0258542 - Si es TDES sigue calculando el MAC
	{
	  //Llamada a la libreria de hsm para calculo de MAC 
	  retorno = manejhsm(claveMac, "04", tipo_macn, area_machsm, long_hsm, sockssl, mac_calculado); //PRJ0258542 - Se anade el 3 parametro
	  fprintLog(nomProg, "Valor de retorno de calculo de MAC - ftratavh >%d<\n", retorno); //PRJ0258542
	}
  //////////// si da error por la conexión se reintenta ////////////
  if(retorno == -32 || retorno == -145)
  {
	  
#if defined (TRAZA) && (TRAZA > 1)    
     fprintLog(nomProg,"Intenta reconectar\n"); 
#endif	 
     memset(&reghsm, ' ', sizeof(NPSDHSM));

	//RAQUEL modificado para conectar por bbdd y entidad
     conexhsm_C("0001",reg_para[ll].psclvhsm, sockssl, codResp);

     if (!strcmp(codResp, "01")) 
     {
     	fprintLog(nomProg, "Valor de la nueva variable tipo_macn ftratavh >%d<\n", tipo_macn); //PRJ0258542
			//PRJ0258542 - Si es AES tengo que llamar a la funcion 07 que es el 802 para CMAC en lugar de al 801 para MAC
			if (tipo_macn == 5)
			{
			  retorno = manejhsm(claveMac, "07", tipo_macn, area_machsm, long_hsm, sockssl, mac_calculado); //PRJ0258542 - Se manda al comando 07 - CMAC
			}
			else
			{
	    	retorno = manejhsm(claveMac, "04", tipo_macn, area_machsm, long_hsm, sockssl, mac_calculado); //PRJ0258542 - Se manda al comando 04 - MAC
	    }
     }
     else
	    retorno = sockssl->sfd_ssl;
  }
  
  //////////////////////////////// si ha ido bien retorna 16 = bytes de la respuesta.
  //PRJ0258542 - si es un MAC AES tiene que retornar 32
  if (retorno != 16 && retorno != 32) 
  {
		if (tipo_macn == 5)
		{
			fprintLog(nomProg,"Error en respuesta a calculo de cmac AES\n");
		}
		else
		{
      fprintLog(nomProg,"Error en respuesta a calculo de mac TDES\n");
		}
      strcpy(pforitn->fidatpro.firesfin, "00");
      switch (retorno)
      {
        case 34: //error en la rutina de MAC
                strcpy(reginci.innomfic,"MAC"); 
                strcpy(pforitn->fidatpro.ficodfin, "909"); //PRJ0258542 - 909 es error de sistema, 916 es error de MAC
                return(finalizar(98));
        default:
                strcpy(reginci.innomfic,"HSM"); 
                strcpy(pforitn->fidatpro.ficodfin, "909"); //PRJ0258542 - 909 es error de sistema, 916 es error de MAC
                return(finalizar(98));
      }
// Fin Tratamiento de errores para el calculo del MAC
  }//fin de retorno 16 0 32;
  else
  {
		// Inicio cambios operaciones BIZUM
		fprintLog(nomProg,"Valores de pforitn->fiisocom.fiiso000 >%.4s< pforitn->fidatiso.fiisoaut.fiaut003.dig12 >%.2s< \n", pforitn->fiisocom.fiiso000, pforitn->fidatiso.fiisoaut.fiaut003.dig12);
		//fprintLog(nomProg,"Valores de pforitn->fidatiso.fiisoaut.fiaut128 >%.8s< pforitn->fidatiso.fiisoaut.fiaut064 >%.8s< \n", pforitn->fidatiso.fiisoaut.fiaut128, pforitn->fidatiso.fiisoaut.fiaut064);
		if( 
				(strncmp(pforitn->fiisocom.fiiso000,"13",2)==0) ||
	 			( 
	 				(strncmp(pforitn->fiisocom.fiiso000,"12",2)==0) && 
	     		( 
	     			(strncmp(pforitn->fidatiso.fiisoaut.fiaut003.dig12,"40",2)==0) ||
	     			(strncmp(pforitn->fidatiso.fiisoaut.fiaut003.dig12,"49",2)==0)
	     		)
	     	) ||
	     	(
	     		((strncmp(pforitn->fiisocom.fiiso000,"11",2)==0) || 
	     /*		(strncmp(pforitn->fiisocom.fiiso000,"12",2)==0)) &&   JFL - 21-08-24 Elimino esta linea para añadir el 12 y el 14*/
	     		(strncmp(pforitn->fiisocom.fiiso000,"12",2)==0) ||
	     		(strncmp(pforitn->fiisocom.fiiso000,"14",2)==0)) &&
	     		(
	     			(strncmp(pforitn->fidatiso.fiisoaut.fiaut003.dig12,"10",2)==0) ||
	     			(strncmp(pforitn->fidatiso.fiisoaut.fiaut003.dig12,"29",2)==0)
	     		)
	     	)
	     )
		// Final cambios operaciones BIZUM
	 	{
			fprintLog(nomProg,"Valores if fidatiso.fiisoaut.fiaut128 >%8s<\n", pforitn->fidatiso.fiisoaut.fiaut128);
			fprintLog(nomProg,"Valores if fidatiso.fiisoaut.fiaut064 >%8s<\n", pforitn->fidatiso.fiisoaut.fiaut064);
			//PRJ0258542 - Fase 3 - Interesa que siempre llegue un MAC, siempre se mueve el informado
			if (pforitn->fidatiso.fiisoaut.fiaut128[0] == ' ')
			{
		 		if (tipo_macn == 5) //PRJ0258542
		 		{
		 			fdesepin(16,pforitn->fidatiso.fiisoaut.fiaut064, mac_desempaq_aes);
		 		}
		 		else
		 		{
		    	fdesepin(8,pforitn->fidatiso.fiisoaut.fiaut064, mac_desempaq); 
		    }
			}
			else
			{
		 		if (tipo_macn == 5) //PRJ0258542
		 		{
		 			fdesepin(16,pforitn->fidatiso.fiisoaut.fiaut128, mac_desempaq_aes);
		 		}
		 		else
		 		{
	    		fdesepin(8,pforitn->fidatiso.fiisoaut.fiaut128, mac_desempaq); 	 
	    	}
	    }
	    //PRJ0258542 - Fase 3 - Interesa que siempre llegue un MAC, siempre se mueve el informado
	 	}
	 	else ////////////////////////////////////////////////////  en  autorizaciones
	 	{
			fprintLog(nomProg,"Valores else fidatiso.fiisoaut.fiaut128 >%8s<\n", pforitn->fidatiso.fiisoaut.fiaut128);
			fprintLog(nomProg,"Valores else fidatiso.fiisoaut.fiaut064 >%8s<\n", pforitn->fidatiso.fiisoaut.fiaut064);
			//PRJ0258542 - Fase 3 - Interesa que siempre llegue un MAC, siempre se mueve el informado
			if (pforitn->fidatiso.fiisoaut.fiaut128[0] == ' ')
			{
		 		if (tipo_macn == 5) //PRJ0258542
		 		{
		 			fdesepin(16,pforitn->fidatiso.fiisoaut.fiaut064, mac_desempaq_aes);
		 		}
		 		else
		 		{
		    	fdesepin(8,pforitn->fidatiso.fiisoaut.fiaut064, mac_desempaq); 
		    }
			}
			else
			{
		 		if (tipo_macn == 5) //PRJ0258542
		 		{
		 			fdesepin(16,pforitn->fidatiso.fiisoaut.fiaut128, mac_desempaq_aes);
		 		}
		 		else
		 		{
	    		fdesepin(8,pforitn->fidatiso.fiisoaut.fiaut128, mac_desempaq); 	 
	    	}
	    }
	    //PRJ0258542 - Fase 3 - Interesa que siempre llegue un MAC, siempre se mueve el informado
		}
		
#if defined (TRAZA) && (TRAZA > 10)  
    fprintLog(nomProg,"ftratavh.c MAC calculado en HSM : >%s<\n", mac_calculado);   //PRJ0257005 - Revisado tokenizacion
    if (tipo_macn == 5) //PRJ0258542
    {
			fprintLog(nomProg,"ftratavh.c MAC desempaquetado   : >%.32s<\n", mac_desempaq_aes); //PRJ0257005 - Revisado tokenizacion
		}
		else
    {
			fprintLog(nomProg,"ftratavh.c MAC desempaquetado   : >%.16s<\n", mac_desempaq); //PRJ0257005 - Revisado tokenizacion
		}
#endif 
     // para pruebas
	 // si el mac del fi es ceros es que no llega informado, no lo valido.

     
    if (tipo_macn == 5) //PRJ0258542
    {
			if ((memcmp(mac_desempaq_aes,"0000000000000000",16)) && (memcmp(mac_desempaq_aes,mac_calculado,16))) //Se comparan 16 en AES
	    {
				fprintLog(nomProg,"** MAC de formato interno no coincide con MAC de HSM ** \n"); //PRJ0257005 - Revisado tokenizacion
				strcpy(coderror, "916"); //PTASK0022156 - No exite 282, cambiar siempre po 916
				strcpy(reginci.innomfic,"HSM"); 
				strcpy(pforitn->fidatpro.ficodfin, "916"); //PTASK0022156 - No exite 282, cambiar siempre po 916 - {916,282},
				strcpy(pforitn->fidatpro.firesfin, "00");
				return(finalizar(98));
	  	}
    }
    else
    {
			if ((memcmp(mac_desempaq,"00000000",8)) && (memcmp(mac_desempaq,mac_calculado,8))) // para compararlo tengo en cuenta solo los 8 primeros
	    {
	         fprintLog(nomProg,"** MAC de formato interno no coincide con MAC de HSM ** \n"); //PRJ0257005 - Revisado tokenizacion
	         strcpy(coderror, "916"); //PTASK0022156 - No exite 282, cambiar siempre po 916 - {916,282},
	         strcpy(reginci.innomfic,"HSM"); 
	         strcpy(pforitn->fidatpro.ficodfin, "916"); //PTASK0022156 - No exite 282, cambiar siempre po 916 - {916,282},
	         strcpy(pforitn->fidatpro.firesfin, "00");
	         return(finalizar(98));
	     }
     }
  }
  
  // TRIODOS NO AUTORIZA EN FALLBACK
  if(( pforitn->fidatiso.fiisoaut.fiaut022[6] == 'S' || pforitn->fidatiso.fiisoaut.fiaut022[6] == 'T') //FALLBACK
    && pforitn->fiisocom.fiiso000[2] != '2' ) // NO COMUNICACION
    {
#if defined (TRAZA) && (TRAZA > 1)  	
         fprintLog(nomProg,"No se admite fallback\n");
#endif		 
         strcpy(pforitn->fidatpro.ficodfin, "195");
         strcpy(pforitn->fidatpro.firesfin, "00");
         return(finalizar(98));
     }
	  

//#if defined (TRAZA) && (TRAZA > 1)    
      fprintLog(nomProg,"no se hace validaciones criptograma\n");
//#endif	  

	//PRJ0258542 - INI
	if (strncmp(pforitn->fidatiso.fiisoaut.fiaut003.dig12,"90",2) == 0)
	{
		fprintLog(nomProg,"*** Operacion con valor 90xxxx de cambio de PIN - bloque informado ***\n");
		fprintLog(nomProg,"Valor de bloque de PIN fiiso4832 >%.32s<\n", pforitn->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832);
		fprintLog(nomProg,"Campo P53 >%.16s<\n", pforitn->fidatiso.fiisoaut.fiaut053);
		fprintLog(nomProg,"Subsis, subapl, fiori, subori, orini >%.2s< >%.2s< >%.2s< >%.2s< >%.2s<\n", pforitn->fidatpro.firespri, pforitn->fidatpro.fisubres, pforitn->fidatpro.fiorigen, pforitn->fidatpro.fisubori, pforitn->fidatpro.fioriini);
		strncpy(offsetS,"0000",4);
		offsetS[4] = '\0';
		retorno = calculo_bloquePIN (pforitn, offsetS);
		if (retorno != 0)
		{
			strncpy(offsetS, pforitn->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832, 4);
		}
		strncpy(pforitn->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832, offsetS, 4);
		strncpy(pforitn->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832 + 4, " ", 28);
		pforitn->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832[4] = '\0';
		fprintLog(nomProg,"Valor de bloque de PIN ya offset >%.32s<\n", pforitn->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832);
	}
	fprintLog(nomProg,"- Numope >%.6s<\n",pforitn->fidatpro.finumope);
	//PRJ0258542 - FIN

  return (finalizar(11));
}

//PRJ0258542 - INI
/***********************************************************************
  FUNCION   calculo_bloquePIN  //PRJ0258542

  DESCRIPCION : sacar el bloque de PIN del mensaje 90xxx del P48.32

     retorna :     	0 . todo correcto
                  !=0 . error de HSM
***********************************************************************/

int calculo_bloquePIN(formato_interno *pfi, char * offsetS)
{
 	NPSIDEN regiden; 
  NPSDHSM reghsm;
 	int tipo_pinn, long_mac, tipoC;
  int ll;
 	int retorno = 0; 
 	long long_hsm = 0;
 	long long_bla = 0;
 	int ii;
  char clave_iden[60];
  char clave_iden00[60];
  char auxComando[50];
  char auxBin[7];
	char opcion[3];
 	unsigned char cc;
 	char linea[4096];
 	char * addr;
 	char clavpin[50];
 	char clavblo[50];
 	long lbloque = 0;
 	char bloquepin[32];
 	char origen[2];
 	char num_clav[2];
 	char tipo_pin[2];
 	char formato[1];
 	char area_mac[1024];
 	char area_offset[4096];
 	char datos_hsm[4096];
 	char long_tarj[2];
 	char tarjeta[16];
 	char cadenaSal[10];
 	char arqc_verificado[4096];  
 	char datos_criptograma[200];
 	char clave1[150];
  char clave2[150];
  char clavePin[150];
  char claveBloq[150];
  char codResp[3];
  char clavecone[4];
  char doblecom[100];
  char tabla_decimal[16] = {"849F79EE1F2945A3"}; //{"0123456789012345"}; //cambiar en conexhsm.h en sf/tux/include
  fprintf(stdout,"Entrada en rutina calculo_bloquePIN\n"); 

 	memcpy(cadenaSal," ",10);
	cadenaSal[10] = '\0';
  doblecom[100] = '\0';
  strncpy(offsetS, "0000", 4); //Inicializa a ceros
  //strncpy(tabla_decimal,"849F79EE1F2945A3",16);
  strncpy(bloquepin, pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832, strlen(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832));
  lbloque = fQuitar_Blancos(bloquepin, sizeof(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832));
  bloquepin[lbloque] = '\0';
	strncpy(long_tarj, "16", 2);
  long_tarj[2] = '\0';
  strncpy(tarjeta, pfi->fidatiso.fiisoaut.finumPAN.datos, 16);	
  tarjeta[16] = '\0';
	fprintLog(nomProg, "Datos ya movidos >%.16s< >%.*s< \n", tarjeta, strlen(bloquepin), bloquepin);
	if (strncmp(pfi->fidatpro.fiorigen,"50",2)==0)  //Diferido siempre sera 50
	{
		strncpy(clave_iden,pfi->fidatpro.fioriini,2); //si el origen es 50 tenemos que mirar fioriini que dara la red (30,40,20 ,....)
		strncpy(origen,pfi->fidatpro.fioriini,2);
	}
	else 
	{
		strncpy(clave_iden,pfi->fidatpro.fiorigen,2); //si el origen no es 50 pasaremos directamente el origen
		strncpy(origen,pfi->fidatpro.fiorigen,2); //si el origen no es 50 pasaremos directamente el origen
	}
  strncpy(clave_iden + 2, pfi->fidatpro.fisubori, 2); 
  clave_iden[4]='\0';
  origen[2] = '\0';
  
 	for (ll=0 ; ll<2 ; ll++) 
  {
  	if (strncmp(origen,reg_para[ll].psnument,2)==0)
		{
			strncpy(tipo_pin, pfi->fidatiso.fiisoaut.fiaut053 + 2, 2);
			fprintLog(nomProg, "Valor de P53 algoritmo >%.2s< \n ", tipo_pin);
			tipo_pinn = atoi(tipo_pin);
      switch (tipo_pinn)
      {
       	case 00: //En claro o sin PIN
       	case 01: //Algoritmo DES
       	case 02: //Algoritmo TDES
       	case 03: //Algoritmo TDES
        	strncpy(clavblo,reg_para[ll].psclblop,50);
        	clavblo[50]='\0';
        	tipoC = 0;
        	break;
        case 05: //AES - pagina 16 del comunicado 23/24, se calcula con la ZMK activa
        	strncpy(clavblo,reg_para[ll].psclzpka,50);
        	clavblo[50]='\0';
        	tipoC = 5;
        	break;
 			}
 			strncpy(clavpin,reg_para[ll].psclclap,50);
 			clavpin[50]='\0';
			break;
    }
  }

	strncpy(doblecom, clavpin, 50);
	strncpy(doblecom + 50, clavblo, 50);
	doblecom[100] = '\0';
	//fprintLog(nomProg,"-- Claves unidas >%.100s<\n", doblecom);
	strncpy(num_clav, pfi->fidatiso.fiisoaut.fiaut053 + 6, 2);
	//fprintLog(nomProg, "Valor de P53 indice clave >%.2s< \n ", num_clav);
	//Cambio 09/04/2025 - para todas igual, ya que se va a pasar todo a AES
  fprintLog(nomProg, "Valor clave_iden para PIN-Offset >%.4s< \n ", clave_iden);
	retorno = claveshsm(clavpin, clave_iden, "000000", clave1, clave2); //Clave de bloque va con clave_iden
	if (retorno != 0)
	{
		fprintf(stdout,"Salida tratar_fich con retorno >%d<\n", retorno);
		return(-1);
	}
	if (strncmp(num_clav,"01",2)==0)
	{
		strncpy(clavePin,clave1,150);
		long_bla = fQuitar_Blancos(clavePin, sizeof(clave1));
	}
	if (strncmp(num_clav,"02",2)==0)
	{
		strncpy(clavePin,clave2,150);
		long_bla = fQuitar_Blancos(clavePin, sizeof(clave2));
	}
	clavePin[long_bla]='\0';
	retorno = claveshsm(clavblo, clave_iden, "000000", clave1, clave2);
	if (retorno != 0)
	{
		fprintf(stdout,"Salida tratar_fich con retorno >%d<\n", retorno);
		return(-1);
	}
	if (strncmp(num_clav,"01",2)==0)
	{
		strncpy(claveBloq,clave1,150);
		long_bla = fQuitar_Blancos(claveBloq, sizeof(clave1));
	}
	if (strncmp(num_clav,"02",2)==0)
	{
		strncpy(claveBloq,clave2,150);
		long_bla = fQuitar_Blancos(claveBloq, sizeof(clave2));
	}
	claveBloq[long_bla]='\0';

  //Monta mensaje para enviar  
	strncpy(datos_hsm, clavePin, strlen(clavePin)); //Clave de calculo de PIN
	long_hsm = strlen(clavePin);
	strncpy(datos_hsm + long_hsm, claveBloq, strlen(claveBloq)); //Clave de bloque de PIN
	long_hsm = long_hsm + strlen(claveBloq);
	strncpy(datos_hsm + long_hsm, tabla_decimal, 16); //Tabla de decimalizacion  //{"0123456789012345"} en conexhsm.h cambiada a 849F79EE1F2945A3
	long_hsm = long_hsm + 16;
	if (strncmp(pfi->fidatiso.fiisoaut.fiaut053 + 4, "01", 2) == 0 || strncmp(pfi->fidatiso.fiisoaut.fiaut053 + 4, "10", 2) == 0)
	{
		strncpy(formato, "0", 1);
	}
	if (strncmp(pfi->fidatiso.fiisoaut.fiaut053 + 4, "13", 2) == 0)
	{
		strncpy(formato, "3", 1);
	}
	if (strncmp(pfi->fidatiso.fiisoaut.fiaut053 + 4, "14", 2) == 0)
	{
		strncpy(formato, "7", 1);
	}
	formato[1] = '\0';
	strncpy(datos_hsm + long_hsm, formato, 1); //Formato de PIN
	long_hsm = long_hsm + 1;
	strncpy(datos_hsm + long_hsm, "1", 1); //Algoritmo
	long_hsm = long_hsm + 1;
	//fprintLog(nomProg, "Datos movidos 1 >%.*s<\n", long_hsm, datos_hsm);
	strncpy(datos_hsm + long_hsm, bloquepin, lbloque); //Bloque de PIN
	long_hsm = long_hsm + lbloque;
	strncpy(datos_hsm + long_hsm, "16", 2); //Longitud de la tarjeta
	long_hsm = long_hsm + 2;
	//fprintLog(nomProg, "Datos movidos 3 >%.*s<\n", long_hsm, datos_hsm);
	strncpy(datos_hsm + long_hsm, tarjeta, 16); //Tarjeta
	long_hsm = long_hsm + 16;
	strncpy(datos_hsm + long_hsm, "4", 1); //Longitud del PIN
	long_hsm = long_hsm + 1;
	strncpy(datos_hsm + long_hsm, tarjeta, 16); //Tarjeta
	long_hsm = long_hsm + 16;
	datos_hsm[long_hsm] = '\0';
	//fprintLog(nomProg, "Va a entrar en fConexIP\n");
	//sfd_hsm = fConexIP(1);
	//fprintLog(nomProg, "Sale de fConexIP con socket >%d<\n",sfd_hsm);
	strncpy(codResp, "   ", 3);
	codResp[3] = '\0';
	//conexhsm_C("0001"," ", sockssl, codResp);
	fprintLog(nomProg, "Cadena entrada manejhsm 11 >%.*s< >%d< >%.*s< >%d< \n", strlen(claveBloq), claveBloq, tipoC, long_hsm, datos_hsm, long_hsm);
  retorno = manejhsm(claveBloq, "11", tipoC, datos_hsm, long_hsm, sockssl, cadenaSal); 
	if (retorno != 4)
	{
		fprintf(stdout,"Salida manejhsm 11 con retorno >%d<\n", retorno);
		return(-1);
	}
	fprintf(stdout,"Salida manejhsm 11, retorno >%d<, cadenaSal >%.4s<\n", retorno, cadenaSal);
	
	strncpy(offsetS, cadenaSal, 4);
	
	fprintf(stdout,"Salida de rutina calculo_bloquePIN\n");
	return(0);
} //FIN calculo_bloquePIN

/*
// Quita los blancos y devuelve el tamaño de la cadena que se queda en cadena_entrada
long fQuitar_Blancos (char * cadena_entrada, long long_entrada)
{
	//printf(" ** ** ** Entrando rutina fQuitar_Blancos 12/2024 v1.0\n");

  long tam_entrada = 0;
  char cadena_aux[long_entrada];

  memset(cadena_aux, '\0', sizeof(cadena_aux));

  while (cadena_entrada[tam_entrada]!=' ' && tam_entrada < long_entrada)
  {
     sprintf(cadena_aux + tam_entrada, "%c", cadena_entrada[tam_entrada]); 
     tam_entrada++;
  }
  strncpy(cadena_entrada, cadena_aux, tam_entrada);
  cadena_entrada[tam_entrada] = '\0';
  
	//printf(" ** ** ** Saliendo rutina fQuitar_Blancos 12/2024 v1.0\n");
	return (tam_entrada);
}
*/
//PRJ0258542 - FIN
