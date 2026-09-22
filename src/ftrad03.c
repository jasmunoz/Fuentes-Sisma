/*
//PRJ0257037 - Liquidación doméstica
Se mete control como esta en la grabación de log y en la grabación de tacto
para que el importe que vaya en IMPORTX sea siempre el valor en euros y el valor
que vaya en IMPOROP sea el de la moneda extranjera, porque ahora este campo siempre
esta vacio, no se esta usando
Se mete informado el campo P22 en el PUN-SERV-X de la copy para que tengan el valor
Se mete informado el campo P37 en el REFADQ de la copy para que tenga el valor, aunque no
se corresponde con este dato, pero lo quieren tener en MEPA
*/
//PRJ0257005 - Revisado tokenizacion
//PRJ0247167 - Revisado ampliación P48
//PTASK000015929 - Añadido para arreglar el salto del cambio de sesión de diferido de las 1524
//PTASK000019960 - TOTALES PRICE VACIOS INVERSIS
// BIZUM COD_FUN cumplimentar con el valor fiiso024, 
//       IDE_ADQ cumplimentar con el valor fi032094.datos, 
//       REF_ADQ cumplimentar con el valor fiaut037, 
//      BIT_P_51 cumplimentar con el conjunto de datos del P48.44 
//       IDTR    cumplimentar con el fiidtran REVISAR  ********************** REVISAR ***********************
//      Se debe verificar que los campos se mueven en los dos sentidos, se verifican las funciones ftrad_haciaReso y ftrad_desdeReso
/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> 

#include "ftradreso.h"
#include "svm.h"
#include "nptpars.h"
#include "npttrms.h"
#include "nptpara.h"
#include "npttoto.h" //PTASK0022158
#include "nptiden.h" //PRJ0258542
#include "nptdhsm.h" //PRJ0258542
//sentido; 
/* 1: del formato del resolutor a formato interno */
/* 2: del formato interno al del resolutor        */


/* prototipos de funciones */
int encuentra_svm(SVM *psvm_par,char *auxsub,int tipo);
int ftrad_desdeReso( int *long_iso, char *mensaje_reso, formato_interno *mensaje_netplus, int tipMsj,char *subsistema_general);
int ftrad_haciaReso( int *long_iso, char *mensaje_reso, formato_interno *mensaje_netplus,char *subsistema_general);

//PTASK000023003 - INI
#include "nptbine.h"
typedef struct dbine tbine;
struct dbine
{
 	char   t_biclave0[8];     /*  Bin                    (clave)  */
 	char   t_birnginf[11];    /*  Rango inferior         (clave)  */
 	char   t_birngsup[11];    /*  Rango superior                             */
 	char   t_bicodent[4];     /*  Entidad                                    */
 	char   t_bitboper[2];     /*  Tabla de operaciones                       */
 	char   t_birespri[2];     /*  Resolutor primario                         */
 	char   t_biparau1[1];     /*  Parametro de autorizacion 1                */
 	char   t_biparau2[1];     /*  Parametro de autorizacion 2                */
 	tbine      *bin_siguiente;
};
tbine  *pbi_binprimero;

int fbusca_bin(tbine *pbine, char *pbinope, char *prangoope, int plongrango, char *ptboper, char *pparau1, char *pparau2,char *prespri, char *codent);
//PTASK000023003 - FIN


char codResp[3]; //PRJ0258542
int nspaces (char *c,int n);
int fcarga_tdattrms(char * red);
char signo[1];



//RAQUEL añadido para unificar las entidades por bbdd
SVM *psvm;
NPSPARA  *reg_para;

/********************************************/
/***   funcion ftrad03                    ***/
/********************************************/
int ftrad03(int sentido, int *long_iso, char *mensaje_reso, formato_interno *mensaje_netplus, int tipMsj)
{

    fprintLog(nomProg,"Entramos en ftrad03 en sentido %i\n", sentido); //PRJ0257005
    
    int cod_err;
    int retorno;
    char auxsub[5];
    char red_trms[2];
    int ll;

    if  ((sentido == 1 && mensaje_reso == NULL)
      || (sentido == 2 && mensaje_netplus == NULL)
      || (sentido != 1 && sentido != 2 ))
        return(1);
        
   sprintf(auxsub,"%.*s00",2,subsistema_general); 
  
   
//Buscamos en la trms por red
//Raquel modificado para cargar el dato por bbdd y entidad
	 
  for (ll=0 ; ll<2 ; ll++) 
  {  
    if ((strncmp(reg_para[ll].psnument,subsistema_general,2)==0) ||
	    (strncmp(reg_para[ll].psresolu,subsistema_general,2)==0))
    {	
       	strncpy(red_trms,reg_para[ll].pscotrms, 2);
	      
        if ((retorno = fcarga_tdattrms(red_trms)) != 0)
        {
#if defined (TRAZA) && (TRAZA > 1)   
          fprintLog(nomProg,"error carga tablas traduccion \n");
#endif	   
        }
		break; 
	}
  } 	
		
   if (sentido == 1 )
   {
        cod_err = ftrad_desdeReso(long_iso, mensaje_reso, mensaje_netplus, tipMsj,subsistema_general);
	}
	
    if (sentido == 2 )
	{
        cod_err = ftrad_haciaReso(long_iso, mensaje_reso, mensaje_netplus,subsistema_general);
	}
	
	return(cod_err);

} // fin funcion principal


/********************************************/
/***   funcion ftrad_haciaReso            ***/
/*** Informa la estructura formato03 con  ***/
/*** los datos del sistema o del formato  ***/
/*** interno según corresponda            ***/
/********************************************/
 
int   ftrad_haciaReso( int *long_iso, char *mensaje_reso, formato_interno *mensaje_netplus, char *subsistema_general)
{
#if defined (TRAZA) && (TRAZA > 10)   		      
   fprintLog(nomProg, "** ** ftrad03.c ---- funcion ftrad_haciaReso\n"); //PRJ0257005
#endif   
   char offsetS[4];
   int posic_subsis, i; 
   char auxsub[5];
   int retorno=0;
   char CabeceraInput[300];
   int tipomsg;
   char CODIGOPROCESO[6];
   char ide_red[1];
   char ide_sesion[4];
   char emisor_str_hacia[2]; 
   int lei;
   int err;
   NPSPARS r_parsub;
   int LenIDE_ADQ;
   char strlongitud[4];
   char strlong_ORI[4];
   int len_ORI;
   NPSTOTO reg_toto; 		//PTASK0022158
   char clave_toto[16]; //PTASK0022158
   char claveP[5];
   char keypars[5]; //indica que se va a arrancar
   
   int tam46;
   char aux[12];
   int num_car_abo;
   int importe_cargo=0;
   int importe_abono=0;
   
   // Añadido para arreglar el salto del cambio de sesión de diferido de las 1524    
   //PTASK000015929 
   int sesion_1524;
   char fecha_1524[6];
   char aux_sesion[3];
   int j;
//PTASK000023003 - INI
	char bin [8];
	char rango[16];
	int longrango;
	char clave_ope[3];
	char parautor1[2];
	char parautor2[2];
	char resolutor[3];
	char entidad[4];
//PTASK000023003 - FIN
   	 
     memset(fecha_1524, '\0', sizeof(fecha_1524));
     memset(aux_sesion, '\0', sizeof(aux_sesion));
   
   //memset((char *)&menReso, ' ', sizeof(formato03));    

   
   
///////////////////////////////////// NUEVO  /////////////////////
	// AREA DE CABECERA
	formatoNuevo menReso;
    memset((char *)&menReso, ' ', sizeof(formatoNuevo));
		strncpy(parautor1,"N",1); //PRJ0387060
//PTASK000023003 - INI
		strncpy(menReso.SIOAPL_TIPMOV,"T",1);
		if (strncmp(mensaje_netplus->fiisocom.fiiso000,"18",2) != 0)
		{
			strncpy(bin, mensaje_netplus->fidatiso.fiisoaut.finumPAN.datos, 8);
			longrango = atoi(mensaje_netplus->fidatiso.fiisoaut.finumPAN.longitud) - 8;
			strncpy(rango, mensaje_netplus->fidatiso.fiisoaut.finumPAN.datos + 8, longrango);
			retorno = fcarga_bines();
			retorno = fbusca_bin(pbi_binprimero, bin, rango, longrango, clave_ope, parautor1, parautor2, resolutor, entidad);
	    if(retorno == 0)
			{
				fprintLog(nomProg,"--- Este BIN %.8s es BIZUM Si o No -->%.1s<-- \n", bin, parautor1);
				if (strncmp(parautor1, "S", 1) == 0)
				{
					strncpy(menReso.SIOAPL_TIPMOV,"B",1);
				}
				else
				{
					strncpy(menReso.SIOAPL_TIPMOV,"T",1);
				}
			}
			else
			{
				strncpy(menReso.SIOAPL_TIPMOV,"T",1);
			}
		}
//PTASK000023003 - FIN

   //IDTR tiene que llevar el numero de operacion original de PRICE  mensaje_netplus->fiisocom.fiidtran
   strncpy(menReso.IDTR, mensaje_netplus->fiisocom.fiidtran, 6);
   
   //NUMOPER_SISMA  necesitamos guardar el numero de operacion interno de sisma 
   strncpy(menReso.NUMOPER_SISMA,mensaje_netplus->fidatpro.finumope, 6); 

  // NUMOPER_SISMA  de momento lo ponemos en TOTAL_TELEBANCO aunque no es su campo
   strncpy(menReso.TOTAL_TELEBANCO,mensaje_netplus->fidatpro.finumope, 6); 
   
   // IDE_CD
    strncpy(menReso.IDE_CD, "                ", 16); 
		strncpy(emisor_str_hacia, reg_para[j].pscotrms,2); //PRJ0387060
      //RAQUEL modificado para recoger el dato por bbdd y entidad
    for (j=0 ; j<2 ; j++) 
    { 
        if ((strncmp(reg_para[j].psnument,mensaje_netplus->fidatpro.fiorigen,2)==0) ||
	        //(strncmp(reg_para[j].psresolu,subsistema_general,2)==0)) esto no nos sirve para inversis/sefide
		    ((strncmp(mensaje_netplus->fidatpro.fiorigen,"50",2)==0) && 
			              (strncmp(mensaje_netplus->fidatpro.fioriini,reg_para[j].psnument,2)==0)))
        {	
		    strncpy(ide_red, reg_para[j].psidered,1);  
		    strncpy(emisor_str_hacia, reg_para[j].pscotrms,2);   
		    strncpy(keypars,reg_para[j].psnument,2);  
		    strncpy(keypars+2,"02",2);
		    keypars[5]='\0';
           break;            
        }  

    } 
	if (strncmp(mensaje_netplus->fidatpro.fiorigen,"50",2)==0)  //esto se puede quedar porque diferido siempre sera 50
    {
	       strncpy(keypars,mensaje_netplus->fidatpro.fioriini,2);  //si el origen es 50 tenemos que mirar fioriini que dara la red (30,40,20 ,....)
    }
	else 
	{
		   strncpy(keypars,mensaje_netplus->fidatpro.fiorigen,2); // si el origen no es 50 pasaremos directamente el origen
    }
    strncpy(keypars+2,"02",2);
    keypars[5]='\0';            
    /*     leer regpars en nptpars   */

    err = tratar_fich("NPTPARS", IGUAL, &r_parsub, keypars);  
     
	//Se coge el número de sesión del mensaje para totales para solucionar el problema de la PTASK000019221 - 01/08/2023
	//Añadido para arreglar el salto del cambio de sesión de diferido de las 1524  
	//PTASK000015929 
    strncpy(fecha_1524, mensaje_netplus->fiisocom.fifechor.aa, 2);	
    strncpy(fecha_1524+2, mensaje_netplus->fiisocom.fifechor.mm, 2);	  
    strncpy(fecha_1524+4, mensaje_netplus->fiisocom.fifechor.dd, 2);  
    fecha_1524[6]='\0'; 
	
	// Según PTASK 19959, 19960, 19961 - los números de sesiones no son correlativos - 07/12/2023
	// Cuando está entrando por el else se salta un número con respecto al último número enviado el día anterior
	fprintLog(nomProg,"* * * Antes de evaluar (ftrad_haciaReso-frtrad03) Mensaje >%.4s< fecha_1524 >%.6s< r_parsub.rdfses.pafecses >%.6s< \n",mensaje_netplus->fiisocom.fiiso000,fecha_1524,r_parsub.rdfses.pafecses);
	fprintLog(nomProg,"* * *                  (ftrad_haciaReso-frtrad03) fidatiso.fiisotot.fitot029 >%.3s< r_parsub.rdfses.panumses >%.3s< \n",mensaje_netplus->fidatiso.fiisotot.fitot029,r_parsub.rdfses.panumses);
    if((strncmp(mensaje_netplus->fiisocom.fiiso000,"15",2)==0)  && (strncmp(fecha_1524,r_parsub.rdfses.pafecses, 6)!=0))
    {
		fprintLog(nomProg,"*** por el if (ftrad_haciaReso) fecha_1524 >%.6s< y mensaje_netplus->fidatiso.fiisotot.fitot029 en ftrad03 >%.3s< dejas el que llega\n",fecha_1524,mensaje_netplus->fidatiso.fiisotot.fitot029);
		strncpy(aux_sesion,mensaje_netplus->fidatiso.fiisotot.fitot029,3);
		//// aux_sesion
		//strncpy(aux_sesion,r_parsub.rdfses.panumses,3);  //quitado 01/08/2023
		aux_sesion[3]='\0'; 
		//sesion_1524 = atoi(aux_sesion) -1;  //quitado 07/12/2023
		sesion_1524 = atoi(aux_sesion);
        memset(aux_sesion, '\0', sizeof(aux_sesion)); 
        sprintf(aux_sesion, "%03d", sesion_1524); 
        strncpy(ide_sesion,aux_sesion,3);
        ide_sesion[4]='\0';
    }
    else
    {
		fprintLog(nomProg,"*** por el else (ftrad_haciaReso) fecha_1524 >%.6s< y r_parsub.rdfses.panumses en ftrad03 >%.3s<\n",fecha_1524,r_parsub.rdfses.panumses);
        memcpy(ide_sesion,r_parsub.rdfses.panumses,3);
        ide_sesion[4]='\0';
    }
	////fin añadido para 1524 
   	// Se coge el número de sesión del mensaje para totales para solucionar el problema de la PTASK000019221 - 01/08/2023

    fprintLog(nomProg,"ide_sesion en ftrad03 despues de evaluar cual poner >%s<\n",ide_sesion);
    strncpy(menReso.IDE_CD, "                ", 16);  
    //red en la segunda posicion
    strncpy(menReso.IDE_CD+1,ide_red,1);
    //sesion con un 0 delante en la posicion 7
    strncpy(menReso.IDE_CD+6,"0",1);
    //sesion recogida de la tabla NPTPARS panumses posicion 8-10
    strncpy(menReso.IDE_CD+7,ide_sesion,3);
   
   //IDE_CD_ORI 
   strncpy(menReso.IDE_CD_ORI, "0", 1);
   strncpy(menReso.IDE_CD_ORI+1,ide_red, 1);
   strncpy(menReso.IDE_CD_ORI+2, "0", 1);
   strncpy(menReso.IDE_CD_ORI+3, ide_sesion, 3);
//   strncpy(menReso.IDE_CD_ORI+6, menReso.IDTR, 6);  
   strncpy(menReso.IDE_CD_ORI+6, mensaje_netplus->fidatpro.finumope,6);
   strncpy(menReso.IDE_CD_ORI+12, mensaje_netplus->fiisocom.fifechor.dd, 2);
   strncpy(menReso.IDE_CD_ORI+14, mensaje_netplus->fiisocom.fifechor.mm, 2);

   // OTROS DATOS DE LA CABECERA
   strncpy(menReso.CAB_CTRL_IOO, "000000", 6);
   strncpy(menReso.CAB_CTRL_IOD, "000000", 6);
   strncpy(menReso.CAB_CTRL_IAP, "0000", 4);
   
    //INF_ADIC_PET_TRANSF  
   if(((strncmp(mensaje_netplus->fiisocom.fiiso000,"1200",4)==0)||
      (strncmp(mensaje_netplus->fiisocom.fiiso000,"1220",4)==0)||
      (strncmp(mensaje_netplus->fiisocom.fiiso000,"1221",4)==0)) &&
      (strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12,"40",2)==0)||(strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12,"49",2)==0))
	{
       strncpy(menReso.INF_ADIC_PET_TRANSF, mensaje_netplus->fidatiso.fiisoaut.fiaut103.datos, 20);
       strncpy(menReso.INF_ADIC_PET_TRANSF+20,mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4833, 12);
    }       

  
   strncpy(menReso.CAB_CTRL_IOP, "000001", 6);
   strncpy(menReso.CAB_CTRL_NB, "00", 2);
   strncpy(menReso.CAUR, "00", 2);
    // PRJ0202598 cambio bin de 6 posiciones 8 ya eran de 8
   strncpy(menReso.BIN1, "00000000", 8);  //00000011 si totales
   strncpy(menReso.BIN2, "00000000", 8);
   strncpy(menReso.BIN3, "00000000", 8);
   
   //IDTM
   tipomsg = atoi(mensaje_netplus->fiisocom.fiiso000);
   //PRJ0247167
   #if defined (TRAZA) && (TRAZA > 10)   		      
      fprintLog(nomProg,"*** *** Tipo de mensaje a tratar >%i< *** *** \n",tipomsg);
   #endif 
   

  strncpy(CODIGOPROCESO,mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12,2);
  strncpy(CODIGOPROCESO+2, mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig34,2);
  strncpy(CODIGOPROCESO+4, mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig56,2); 
	fprintLog(nomProg,"*** Valores consulta  >%.4s< >%.6s< >%.2s< >%.1s< *** \n", mensaje_netplus->fiisocom.fiiso000, CODIGOPROCESO, emisor_str_hacia, parautor1); //PRJ0387060
   /*******CAMBIAMOS LOS IF POR LA CONSULTA AL REGISTRO DE LA TABLA NPTTRMS*****/
   //Raquel modificado para recoger los datos por bbdd y entidad
  int ll,salir;
  
  if ((tipomsg==1804) || (tipomsg==1524))
  {
		for (ll=0,salir=0; ll< 500 && !salir; ll++)  
		{
			if (strncmp(parautor1, "S", 1) == 0) //PRJ0387060 - INI
			{
				//fprintLog(nomProg,"*** * Operativa BIZUM * ***\n");
				if (strncmp(tabla_trms[ll].trmsidtm,mensaje_netplus->fiisocom.fiiso000,4)==0
					&& strncmp(tabla_trms[ll].trmsemis,emisor_str_hacia,2)==0
					&& strncmp(tabla_trms[ll].trmsoper,"B",1) == 0)
				{
					salir = 1;
					strncpy(menReso.IDTM, tabla_trms[ll].trmsidsi,4);
					break;
				}
			}
			if (strncmp(parautor1, "N", 1) == 0)
			{
				//fprintLog(nomProg,"*** * Operativa Tarjeta * ***\n");
				if (strncmp(tabla_trms[ll].trmsidtm,mensaje_netplus->fiisocom.fiiso000,4)==0
					&& strncmp(tabla_trms[ll].trmsemis,emisor_str_hacia,2)==0
					&& strncmp(tabla_trms[ll].trmsoper,"T",1) == 0)
				{
					salir = 1;
					strncpy(menReso.IDTM, tabla_trms[ll].trmsidsi,4);
					break;
				}
			} //PRJ0387060 - FIN
		}    /***   fin del bucle   ***/
  }    
  else
  {
		for (ll=0,salir=0; ll< 500 && !salir; ll++)  
		{
			if (strncmp(parautor1, "S", 1) == 0) //PRJ0387060 - INI
			{
				//fprintLog(nomProg,"*** * Operativa BIZUM * ***\n");
				//fprintLog(nomProg,"*** Valores tabla  >%.4s< >%.6s< >%.2s< >%.1s< *** \n", tabla_trms[ll].trmsidtm, tabla_trms[ll].trmscopr, tabla_trms[ll].trmsemis, tabla_trms[ll].trmsoper); //PRJ0387060
				if (strncmp(tabla_trms[ll].trmsidtm,mensaje_netplus->fiisocom.fiiso000,4)==0
					&& strncmp(tabla_trms[ll].trmscopr,CODIGOPROCESO,6)==0
					&& strncmp(tabla_trms[ll].trmsemis,emisor_str_hacia,2)==0
					&& strncmp(tabla_trms[ll].trmsoper,"B",1) == 0)
				{
					strncpy(menReso.IDTM, tabla_trms[ll].trmsidsi,4);
					strncpy(menReso.CODPROC, tabla_trms[ll].trmscosi,6);
					//PTASK000023003 - INI
					fprintLog(nomProg,"*** CODPROC 1 >%.6s<  IDTM >%.4s< SIOAPL_TIPMOV >%.1s< \n",menReso.CODPROC, menReso.IDTM, menReso.SIOAPL_TIPMOV);        
					if (strncmp(menReso.SIOAPL_TIPMOV,"T",1) == 0 && strncmp(menReso.IDTM,"1420",4) == 0 && strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12,"10",2) == 0)
					{
						strncpy(menReso.IDTM, "0420", 4);
						strncpy(menReso.CODPROC, "072000", 6);          
					}
					if (strncmp(menReso.SIOAPL_TIPMOV,"T",1) == 0 && strncmp(menReso.IDTM,"1420",4) == 0 && strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12,"29",2) == 0)
					{
						strncpy(menReso.IDTM, "0420", 4);
						strncpy(menReso.CODPROC, "072000", 6);          
					}
					fprintLog(nomProg,"*** CODPROC 2 >%.6s<  IDTM >%.4s< SIOAPL_TIPMOV >%.1s< \n",menReso.CODPROC, menReso.IDTM, menReso.SIOAPL_TIPMOV);        
					//PTASK000023003 - FIN
					salir = 1;
					break;
				}      
			}
			if (strncmp(parautor1, "N", 1) == 0) //PRJ0387060 - INI
			{
				//fprintLog(nomProg,"*** * Operativa Tarjeta * ***\n");
				//fprintLog(nomProg,"*** Valores tabla  >%.4s< >%.6s< >%.2s< >%.1s< *** \n", tabla_trms[ll].trmsidtm, tabla_trms[ll].trmscopr, tabla_trms[ll].trmsemis, tabla_trms[ll].trmsoper); //PRJ0387060
				if (strncmp(tabla_trms[ll].trmsidtm,mensaje_netplus->fiisocom.fiiso000,4)==0
					&& strncmp(tabla_trms[ll].trmscopr,CODIGOPROCESO,6)==0
					&& strncmp(tabla_trms[ll].trmsemis,emisor_str_hacia,2)==0
					&& strncmp(tabla_trms[ll].trmsoper,"T",1) == 0)
				{
					strncpy(menReso.IDTM, tabla_trms[ll].trmsidsi,4);
					strncpy(menReso.CODPROC, tabla_trms[ll].trmscosi,6);
					//PTASK000023003 - INI
					fprintLog(nomProg,"*** CODPROC 1 >%.6s<  IDTM >%.4s< SIOAPL_TIPMOV >%.1s< \n",menReso.CODPROC, menReso.IDTM, menReso.SIOAPL_TIPMOV);        
					if (strncmp(menReso.SIOAPL_TIPMOV,"T",1) == 0 && strncmp(menReso.IDTM,"1420",4) == 0 && strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12,"10",2) == 0)
					{
						strncpy(menReso.IDTM, "0420", 4);
						strncpy(menReso.CODPROC, "072000", 6);          
					}
					if (strncmp(menReso.SIOAPL_TIPMOV,"T",1) == 0 && strncmp(menReso.IDTM,"1420",4) == 0 && strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12,"29",2) == 0)
					{
						strncpy(menReso.IDTM, "0420", 4);
						strncpy(menReso.CODPROC, "072000", 6);          
					}
					fprintLog(nomProg,"*** CODPROC 2 >%.6s<  IDTM >%.4s< SIOAPL_TIPMOV >%.1s< \n",menReso.CODPROC, menReso.IDTM, menReso.SIOAPL_TIPMOV);        
					//PTASK000023003 - FIN
					salir = 1;
					break;
				}      
			} //PRJ0387060 - FIN
		}    /***   fin del bucle   ***/
  }
  
//si no encontramos traduccion en TRMS ponemos el valo PRICE y activamos el indicador de error
  if (menReso.IDTM[0] == ' ')
  {
     strncpy(menReso.IDTM, mensaje_netplus->fiisocom.fiiso000, 4);
     strncpy(menReso.CODPROC, CODIGOPROCESO,6);
     menReso.TOTAL_TELEBANCO[6] = 'E';
     printf("No encontrado TRMS movemos valores PRICE IDTM <%.4s> CODPROC <%.6s>\n", menReso.IDTM, menReso.CODPROC);
  } else {
     menReso.TOTAL_TELEBANCO[6] = ' ';
  } 
  
   //MBIT_P
   strncpy(menReso.MBIT_P, "0000000000000000000000000000000000000000000000000000000000000000", 64);
   
   // PAN_L Y PAN_D
   // PRJ0202598 cambio bin de 6 posiciones 8 ya eran de 8
   strncpy(menReso.PAN_L, mensaje_netplus->fidatiso.fiisoaut.finumPAN.longitud, 2);
   mensaje_netplus->fidatiso.fiisoaut.finumPAN.longitud[2]='\0';
   if(atoi(mensaje_netplus->fidatiso.fiisoaut.finumPAN.longitud)!=0){
      strncpy(menReso.PAN_D, mensaje_netplus->fidatiso.fiisoaut.finumPAN.datos, 16);	
      strncpy(menReso.PAN_D+16, "00", 2);

   }
   //PRJ0257037 - INICIO
   //original-//IMPORTX
   //original-strncpy(menReso.IMPORTX, mensaje_netplus->fidatiso.fiisoaut.fiaut004+4, 8);
   if(mensaje_netplus->fidatiso.fiisoaut.finumPAN.longitud > 0)
   {
     if(!nspaces(mensaje_netplus->fidatiso.fiisoaut.fiaut009, 8))
     {
      strncpy(menReso.IMPORTX, mensaje_netplus->fidatiso.fiisoaut.fiaut005+4, 8);
      strncpy(menReso.IMPOROP, mensaje_netplus->fidatiso.fiisoaut.fiaut004, 12);
     }
     else
     {
       if(!nspaces(mensaje_netplus->fidatiso.fiisoaut.fiaut010, 8))
       {
         strncpy(menReso.IMPORTX, mensaje_netplus->fidatiso.fiisoaut.fiaut006+4, 8);
         strncpy(menReso.IMPOROP, mensaje_netplus->fidatiso.fiisoaut.fiaut004, 12);
       }
       else 
       {
         strncpy(menReso.IMPORTX, mensaje_netplus->fidatiso.fiisoaut.fiaut004+4, 8);
         strncpy(menReso.IMPOROP, "000000000000", 12);
       }
     }

     //printf("Valor de menReso.IMPORTX >%.8s<\n", menReso.IMPORTX);
     //printf("Valor de menReso.IMPOROP >%.12s<\n", menReso.IMPOROP);

   }
   else
   {
      strncpy(menReso.IMPORTX, "00000000", 8);
      strncpy(menReso.IMPOROP, "000000000000", 12);
   }
   strncpy(menReso.PUNT_SERV, mensaje_netplus->fidatiso.fiisoaut.fiaut022, 12);

   // IDE_ADQ cumplimentar con el valor fi032094.datos 
   if (strncmp(menReso.SIOAPL_TIPMOV,"B",1)==0) // BIZUM comprobacion
   {
      //fprintLog(nomProg,"fi032094 >%s< >%.*s<\n", mensaje_netplus->fiisocom.fi032094.longitud, atoi(mensaje_netplus->fiisocom.fi032094.longitud), mensaje_netplus->fiisocom.fi032094.datos);
      strncpy(menReso.IDE_ADQ, mensaje_netplus->fiisocom.fi032094.datos,atoi(mensaje_netplus->fiisocom.fi032094.longitud));
      //fprintLog(nomProg,"IDE_ADQ >%.11s<\n", menReso.IDE_ADQ);
   }
//PTASK000023003 - INI - control del IDE_ADQ para operaciones de transferencia
	if (strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12, "10", 2) == 0 || strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12, "29", 2) == 0)
	{
		if (strncmp(menReso.SIOAPL_TIPMOV, "T", 1) == 0)
		{
			strncpy(menReso.IDE_ADQ, entidad, 4);
			strncpy(menReso.IDE_ADQ + 4, "       ", 7);
			strncpy(menReso.INF_ADIC_PET_TRANSF, entidad, 4);
			strncpy(menReso.INF_ADIC_PET_TRANSF + 4, "                            ", 28);
			fprintLog(nomProg,"Fuerza IDE_ADQ  y INF_ADIC_PET_TRANSF al ser Fast-Fund >%.11s< >%.32s<\n", menReso.IDE_ADQ, menReso.INF_ADIC_PET_TRANSF);
		}
	}
//PTASK000023003 - INI - control del IDE_ADQ para operaciones de transferencia
   // REF_ADQ cumplimentar con el valor fiaut037
   strncpy(menReso.REF_ADQ, mensaje_netplus->fidatiso.fiisoaut.fiaut037, 12);  
   if (strncmp(menReso.SIOAPL_TIPMOV,"B",1)==0) // BIZUM comprobacion
   {
      //fprintLog(nomProg,"fiaut037 >%.12s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut037);
      strncpy(menReso.REF_ADQ, mensaje_netplus->fidatiso.fiisoaut.fiaut037, 12);
      //fprintLog(nomProg,"REF_ADQ >%.12s<\n", menReso.REF_ADQ);
   }
   // IDTR BIZU  ponemos el valor de mensaje_netplus->fiisocom.fiidtran  
   if (strncmp(menReso.SIOAPL_TIPMOV,"B",1)==0) // BIZUM comprobacion
   {
      //fprintLog(nomProg,"fiidtran >%.6s<\n", mensaje_netplus->fiisocom.fiidtran);
      strncpy(menReso.IDTR, mensaje_netplus->fiisocom.fiidtran,6);
      //fprintLog(nomProg,"IDTR BIZU >%.6s<\n", menReso.IDTR);
   }
   //PRJ0257037 - FINAL

   //FECHAL
   strncpy(menReso.FECHAL, mensaje_netplus->fiisocom.fifechor.aa, 2);	
   strncpy(menReso.FECHAL+2, mensaje_netplus->fiisocom.fifechor.mm, 2);	
   strncpy(menReso.FECHAL+4, mensaje_netplus->fiisocom.fifechor.dd, 2);
   
   //CMONTI y MON_CTBLE
	//PRJ0258542 - cambio moneda extranjera - INI
  if (strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut049, "978", 3) == 0)
  {
   	strncpy(menReso.CMONTI ,"0", 1);
   	strncpy(menReso.CMONTI+1 ,mensaje_netplus->fidatiso.fiisoaut.fiaut049, 3);
   	strncpy(menReso.MON_CTBLE, "0000", 4);
   	strncpy(menReso.IDEN_BLOQUE_MOV, "000000000000", 12);
  }
  else
  {
   	strncpy(menReso.CMONTI ,"0978", 4);
   	strncpy(menReso.MON_CTBLE ,"0", 1);
   	strncpy(menReso.MON_CTBLE+1 ,mensaje_netplus->fidatiso.fiisoaut.fiaut049, 3);
		if(!nspaces(mensaje_netplus->fidatiso.fiisoaut.fiaut009, 8))
		{
			strncpy(menReso.IMPORTX, mensaje_netplus->fidatiso.fiisoaut.fiaut005+4, 8);
			strncpy(menReso.IMPOROP, "000000000000", 12);
			strncpy(menReso.IDEN_BLOQUE_MOV, mensaje_netplus->fidatiso.fiisoaut.fiaut004, 12);
		}
		else
		{
		 	if(!nspaces(mensaje_netplus->fidatiso.fiisoaut.fiaut010, 8))
		 	{
		   	strncpy(menReso.IMPORTX, mensaje_netplus->fidatiso.fiisoaut.fiaut006+4, 8);
		   	strncpy(menReso.IMPOROP, "000000000000", 12);
		   	strncpy(menReso.IDEN_BLOQUE_MOV, mensaje_netplus->fidatiso.fiisoaut.fiaut004, 12);
		 	}
		 	else 
		 	{
		   	strncpy(menReso.IMPORTX, mensaje_netplus->fidatiso.fiisoaut.fiaut004+4, 8);
		   	strncpy(menReso.IMPOROP, "000000000000", 12);
		   	strncpy(menReso.IDEN_BLOQUE_MOV, "000000000000", 12);
		 	}
		}  
	}
	if(mensaje_netplus->fidatiso.fiisoaut.finumPAN.longitud < 0)
	{
		strncpy(menReso.IMPORTX, "00000000", 8);
		strncpy(menReso.IMPOROP, "000000000000", 12);
	}
 	//PRJ0258542 - cambio moneda extranjera - FIN
 
   //HORAL
   strncpy(menReso.HORAL,    mensaje_netplus->fiisocom.fifechor.hh, 2);	
   strncpy(menReso.HORAL+2,  mensaje_netplus->fiisocom.fifechor.mi, 2);	
   strncpy(menReso.HORAL+4 , mensaje_netplus->fiisocom.fifechor.ss, 2);
   
   //FECCAD
   strncpy(menReso.FECCAD, mensaje_netplus->fidatiso.fiisoaut.fifeccad.aa, 2);
   strncpy(menReso.FECCAD+2, mensaje_netplus->fidatiso.fiisoaut.fifeccad.mm, 2);
    
   //COMINF
   strncpy(menReso.COMINF_TIPO, mensaje_netplus->fidatpro.fitipcom, 2);
   strncpy(menReso.COMINF_IMPO, mensaje_netplus->fidatpro.ficomadi, 5);
   
   //SECTOR
	if(strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut018, "0000",4)==0 && 
	    strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut026, "0000",4)!=0)
	    strncpy(menReso.SECTOR , mensaje_netplus->fidatiso.fiisoaut.fiaut026, 4);
	else
        strncpy(menReso.SECTOR , mensaje_netplus->fidatiso.fiisoaut.fiaut018, 4);

  //DMND0036176 COD_FUN donde ira el codigo de funcion p24 a 150 para las preautorizaciones    
  strncpy(menReso.COD_FUN, mensaje_netplus->fiisocom.fiiso024, 4);
   
   // COD_FUN cumplimentar con el valor fiiso024
   if (strncmp(menReso.SIOAPL_TIPMOV,"B",1)==0) // BIZUM comprobacion
   {
     strncpy(menReso.COD_FUN, mensaje_netplus->fiisocom.fiiso024, 3);
   // #if defined (TRAZA) && (TRAZA > 10)   		      
     // fprintLog(nomProg,"COD_FUN >%.4s<\n",menReso.COD_FUN);
   // #endif   
   }
  
   //CODRAZ_ANUL (PROBLEMA: cod_rzn son 4 bytes, CODRAZ_ANUL son 2 bytes, cuales copiar?)
   strncpy(menReso.CODRAZ_ANUL, mensaje_netplus->fidatiso.fiisoaut.fiaut025, 2);
   
   //NUM_MIEMB
   strncpy(menReso.NUM_MIEMB, "0", 1);
  
/* //PTASK000025901 - Se quita este control para el campo ID-REDTERM y se pone para todos el mismo 
   	if ((strncmp(mensaje_netplus->fiisocom.fiiso000,"1200",4)==0) ||
        (strncmp(mensaje_netplus->fiisocom.fiiso000,"1420",4)==0))
   	{
    	strncpy(menReso.ID_REDTERM ,"00",2);                                           
      strncpy(menReso.ID_REDTERM+2 ,mensaje_netplus->fidatiso.fiisoaut.fiaut042, 8);
      strncpy(menReso.ID_REDTERM+10 ,mensaje_netplus->fidatiso.fiisoaut.fiaut041, 2);

   	}
   	else
   	{
   		// 28/06/2024 - Para preautorizaciones se mete el comercio P42 recortado a 12 que es lo que da el ID_REDTERM
   		if ((strncmp(mensaje_netplus->fiisocom.fiiso000,"1100",4)==0) || (strncmp(mensaje_netplus->fiisocom.fiiso000,"1120",4)==0))
   		{
      	strncpy(menReso.ID_REDTERM,mensaje_netplus->fidatiso.fiisoaut.fiaut042, 12);   		
   		}
   		else
   		{
      	strncpy(menReso.ID_REDTERM ,"00",2);
      	strncpy(menReso.ID_REDTERM+2,mensaje_netplus->fiisocom.fi032094.datos+2,4);
      }
   	}
*/ //PTASK000025901 - Se quita este control para el campo ID-REDTERM y se pone para todos el mismo
		strncpy(menReso.ID_REDTERM,mensaje_netplus->fidatiso.fiisoaut.fiaut042, 12); //PTASK000025901 - Se quita este control para el campo ID-REDTERM y se pone para todos el mismo

		//PRJ0350720 - se asigna valor a la nueva variable
		strncpy(menReso.SIOAPL_CODRAZ, mensaje_netplus->fidatiso.fiisoaut.fiaut025,4);
	
		//PRJ0247167 - se asigna valor a la nueva variable
		//if (!strncmp(menReso.SIOAPL_TIPMOV,"B",1)==0)
		//{
			//strncpy(menReso.VALOR_P48_20,mensaje_netplus->fiaut048.comun01.fiiso4820,16); //Esta es la linea original
   		strncpy(menReso.VALOR_P48_20,mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4820,16);
			//PRJ0257005 - se asigna valor a las nuevas variables
			strncpy(menReso.VALOR_P48_94,mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4894,6);
		//}
		//strncpy(menReso.VALOR_P62_16,mensaje_netplus->fidatiso.fiisoaut.fiaut062.bit62_16,30); // se quita para pasar solo tarjeta y caducidad
		//printf("Valor de la variable completa mensaje_netplus->fidatiso.fiisoaut.fiaut062.bit62_16 >%.30s< \n", mensaje_netplus->fidatiso.fiisoaut.fiaut062.bit62_16); //PRJ0257005
		strncpy(menReso.SIOAPL_TARJTOKEN,mensaje_netplus->fidatiso.fiisoaut.fiaut062.bit62_16+2,16); //PRJ0257005
		strncpy(menReso.SIOAPL_FECATTOKE,mensaje_netplus->fidatiso.fiisoaut.fiaut062.bit62_16+21,4); //PRJ0257005
		//printf("Valor de la variable partida menReso.SIOAPL_TARJTOKEN >%.16s< \n", menReso.SIOAPL_TARJTOKEN); //PRJ0257005
		//printf("Valor de la variable partida menReso.SIOAPL_FECATTOKE >%.4s< \n", menReso.SIOAPL_FECATTOKE); //PRJ0257005
	
    printf("Valor de fiaut056 1  >%.30s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut056.datos);
    //Para operativa BIZUM nos piden enviar el P56 que llega directamente de Redsys, sin el P00
    if(mensaje_netplus->fidatiso.fiisoaut.fiaut056.datos[0] != ' ')
    {
      strncpy(strlong_ORI, mensaje_netplus->fidatiso.fiisoaut.fiaut056.longitud,3);
      strlong_ORI[3] = 0;
      len_ORI=atoi(strlong_ORI);
	  	strncpy(menReso.DATORI,mensaje_netplus->fidatiso.fiisoaut.fiaut056.datos+4,18);
	  	
	  	//13/05/2005 - PTASK000022975 - Se quita lo que esta montado para pasar todas las anulaciones igual - INI
	  	/*
   		// 28/06/2024 - Para preautorizaciones se mete el comercio P42 recortado a 12 en los datos de la operación original para localizar la retencion
			if ((strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut056.datos,"1100",4)==0) || (strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut056.datos,"1120",4)==0))
			{
				strncpy(menReso.DATORI+18,mensaje_netplus->fidatiso.fiisoaut.fiaut042, 12);
			}
			else
			{
      	strncpy(menReso.DATORI+18,"00",2);
	      strncpy(menReso.DATORI+20,mensaje_netplus->fidatiso.fiisoaut.fiaut056.datos+26,len_ORI-26);
    	}
			*/
			strncpy(menReso.DATORI+18,mensaje_netplus->fidatiso.fiisoaut.fiaut042, 12); //Ahora - 13/05/2024 - se monta igual en todos los casos
	  	//13/05/2005 - PTASK000022975 - Se quita lo que esta montado para pasar todas las anulaciones igual - FIN
	  	
    	if (strncmp(menReso.SIOAPL_TIPMOV, "B", 1) == 0) //PTASK000023003
    	{
    		printf("Operativa BIZUM: %.2s - Cambia P56 por original\n", mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12);
    		strncpy(menReso.DATORI,mensaje_netplus->fidatiso.fiisoaut.fiaut056.datos+4,26); //Pongo el +4 y cambio de 30 a 26
    		strncpy(menReso.DATORI+26,mensaje_netplus->fidatiso.fiisoaut.fiaut056.datos,4); //Añado el P00 al final
    	}
		}
    printf("Valor de DATORI a enviar >%.30s<\n", menReso.DATORI);


   	//NUMAUT
   	// código de autorización (debe llegar informado en anulación UyP, confirmación UyP y devoluciones P)
	
		strncpy(menReso.NUMAUT, mensaje_netplus->fidatiso.fiisoaut.fiaut038, 6);

   	//NUM_REF
   
   	// REF_TPV
   	strncpy(menReso.REF_TPV ,mensaje_netplus->fidatiso.fiisoaut.fiaut042, 15);
   
   	//IM_CUO_ABO[12]; IM-CUO-CAR[12];   p046
   	//DMND0036176 añadidas preautorizaciones 1220 las 1100 no tienen bit46 activo (fiaut046)
	
    if((strncmp(mensaje_netplus->fiisocom.fiiso000,"1200",4)==0)||
       (strncmp(mensaje_netplus->fiisocom.fiiso000,"1220",4)==0)||
       (strncmp(mensaje_netplus->fiisocom.fiiso000,"1420",4)==0))
    {
        strncpy(aux, mensaje_netplus->fidatiso.fiisoaut.fiaut046.longitud, 2);
        aux[2]='\0';
        tam46=atoi(aux);
        num_car_abo=tam46/11;  
        
         for (i=0; i<num_car_abo; i++)
         { 
            if (!strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut046.datos+(i*11)+2,"C",1))
            {
               strncpy(aux, mensaje_netplus->fidatiso.fiisoaut.fiaut046.datos+(i*11)+2+1,8);
               aux[8]='\0';
               importe_abono=importe_abono+atoi(aux);
            }
            else
            {
               strncpy(aux, mensaje_netplus->fidatiso.fiisoaut.fiaut046.datos+(i*11)+2+1,8);
               aux[8]='\0';
               importe_cargo=importe_cargo+atoi(aux);
            }
         }
         sprintf(menReso.IM_CUO_ABO, "%.012d", importe_abono);
         sprintf(menReso.IM_CUO_CAR, "%.012d", importe_cargo);

         if (strncmp(mensaje_netplus->fiisocom.fiiso000,"1420",4)==0)
         {
            strncpy(menReso.COMINF_TIPO, "02", 2);
            sprintf(menReso.COMINF_IMPO, "%.6d", importe_abono + importe_cargo);
         }
    }
      //DAT_ADIC_ENV_TRANSP: char[404]      
     // si es cambio de pin
     if (!strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12,"90",2))
     {
      printf("Offset ya sacado fiiso4832 >%.32s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832); //PRJ0258542
      offsetS[4] = '\0'; //PRJ0258542
     	strncpy(offsetS, mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832, 4); //PRJ0258542
      strncpy(menReso.SIOAPL_OFFSET, offsetS, 4); //PRJ0258542
      strncpy(menReso.DAT_ADIC_ENV_TRANSP, mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.offset, 12);
	   	strncpy(menReso.DAT_ADIC_ENV_TRANSP+14, mensaje_netplus->fidatiso.fiisoaut.fiaut048.noeco.nuevof, 12);  
     }
     else
     {
     	menReso.DAT_ADIC_ENV_TRANSP[0] = '\0'; //Bizum - limpiamos porque esta pasando basura
     }
     // si es cambio de idioma	
     if (!strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12, "91", 2)){
       strncpy(menReso.DAT_ADIC_ENV_TRANSP+12, mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.idioma, 2);
	   strncpy(menReso.DAT_ADIC_ENV_TRANSP+26, mensaje_netplus->fidatiso.fiisoaut.fiaut048.noeco.nuevid, 2);
	 }
     // cvv
	 strncpy(menReso.DAT_ADIC_ENV_TRANSP+28, mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.cvv000 + 5, 3);
	 // tipo de compensación
	 strncpy(menReso.DAT_ADIC_ENV_TRANSP+31, mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.compen, 1);
     // si es una recarga número de teléfono para recargas
     if(!nspaces(mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.telefo, 9)){
	   strncpy(menReso.DAT_ADIC_ENV_TRANSP+32, mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.telefo, 9);
     }
	//Xavi movemos los datos del recibo
     //printf("fiiso4821 <%s>\n", mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4821);
     if(strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4821, "STD014",6) == 0)
     { 
           strncpy(menReso.DAT_ADIC_ENV_TRANSP, "005111",6);
           strncpy(menReso.DAT_ADIC_ENV_TRANSP+6, mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4821+6, 30);
     }
//Xavi movemos los datos del recibo

     // bloque de movimientos
	 // si es consulta de movimientos lo informo
	 if(strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12, "31",2)==0){
	   mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.bloqmo[2] ='\0';
	   switch (atoi(mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.bloqmo)){
	   case 0:
	   case 1:
	      strncpy(menReso.DAT_ADIC_ENV_TRANSP+41, "1", 1);
		  break;
              strncpy(menReso.IDTM, "5000",4);
		  break;
	   case 2:
              strncpy(menReso.IDTM, "5200",4);
		  break;
	   case 3:
              strncpy(menReso.IDTM, "5300",4);
		  break;
	   case 4:
              strncpy(menReso.IDTM, "5400",4);
		  break;
	   case 5:
              strncpy(menReso.IDTM, "5500",4);
	       strncpy(menReso.DAT_ADIC_ENV_TRANSP+41, mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.bloqmo+1, 1);
		   break;	 
       default:
           break;	   
	   }
     }

   // 48.15
	strncpy(menReso.DAT_ADIC_ENV_TRANSP+138, mensaje_netplus->fidatiso.fiisotot.fitot4815 ,180);
//Xavi movemos los datos del pago de recibos
   
//movemos los campos de totales a los campos de menReso
   if (strncmp(menReso.IDTM, "0500",4) == 0)
   {
   	 //PTASK0022158 - INI
   	 printf(" *** *** Nuevo control de totales 04-02-2025 *** *** \n");
   	 memset(&reg_toto,'\0',sizeof(NPSTOTO));
		 if (strncmp(mensaje_netplus->fidatpro.fiorigen,"50",2)==0)  //esto se puede quedar porque diferido siempre sera 50
     {
	    	strncpy(clave_toto,mensaje_netplus->fidatpro.fioriini,2);  //si el origen es 50 tenemos que mirar fioriini que dara la red (30,40,20 ,....)
     }
		 else 
		 {
		   	strncpy(clave_toto,mensaje_netplus->fidatpro.fiorigen,2); // si el origen no es 50 pasaremos directamente el origen
     }
     strncpy(clave_toto +2,  mensaje_netplus->fidatpro.fisubori, 2);
     strncpy(clave_toto +4,  mensaje_netplus->fidatiso.fiisotot.fitot029, 3);
		 if (strncmp(mensaje_netplus->fidatpro.fiorigen,"50",2)==0)  //esto se puede quedar porque diferido siempre sera 50
     {
	    	strncpy(clave_toto+7,mensaje_netplus->fidatpro.fioriini,2);  //si el origen es 50 tenemos que mirar fioriini que dara la red (30,40,20 ,....)
     }
		 else 
		 {
		   	strncpy(clave_toto+7,mensaje_netplus->fidatpro.fiorigen,2); // si el origen no es 50 pasaremos directamente el origen
     }
     strncpy(clave_toto +9,  mensaje_netplus->fidatiso.fiisotot.fitot028.aa, 2);
     strncpy(clave_toto +11, mensaje_netplus->fidatiso.fiisotot.fitot028.mm, 2);
     strncpy(clave_toto +13, mensaje_netplus->fidatiso.fiisotot.fitot028.dd, 2);
     clave_toto[15]='\0';
		 printf(" Clave_totales   >%.15s<\n",clave_toto);
   	 err = tratar_fich("NPTTOTO",IGUAL,&reg_toto,clave_toto); 
   	 printf(" Salida lectura de NPTTOTO >%i<\n", err);
   	 if (err == 0)
   	 {
   	 	 printf(" Registro de NPTTOTO \n");
	     strncpy(menReso.ICARGOS,reg_toto.toimcarg+4,12);
	     strncpy(menReso.NCARGOS,reg_toto.tonucarg,10);
	     strncpy(menReso.IABONOS,reg_toto.toimabon+4,12);
	     strncpy(menReso.NABONOS,reg_toto.tonuabon,10);
	     strncpy(menReso.I_ANUL_CAR,reg_toto.toimanab+4,12);
	     strncpy(menReso.NUM_ANUL_CAR,reg_toto.tonuanab,10);
	     strncpy(menReso.I_ANUL_ABO,reg_toto.toimanca+4,12);
	     strncpy(menReso.NUM_ANUL_ABO,reg_toto.tonuanca,10);
	     strncpy(menReso.TOT_TRASPASOS, "000000000000",12); 
	     strncpy(menReso.NUM_TRANSPASOS,"0000000000",10);
	     strncpy(menReso.TOT_ANUL_TRASPASOS, "000000000000",12); 
	     strncpy(menReso.NUM_ANUL_TRASPASOS,"0000000000",10);
	     strncpy(menReso.FEC_REF_TOTALES,mensaje_netplus->fidatiso.fiisotot.fitot028.aa,2);
	     strncpy(menReso.FEC_REF_TOTALES+2,mensaje_netplus->fidatiso.fiisotot.fitot028.mm,2);
	     strncpy(menReso.FEC_REF_TOTALES+4,mensaje_netplus->fidatiso.fiisotot.fitot028.dd,2);
	     strncpy(menReso.IM_CUO_ABO,reg_toto.toimabcu+4,12);
	     strncpy(menReso.IM_CUO_CAR,reg_toto.toimcacu+4,12);
   	 }
   	 else
   	 {
   	 	 printf(" Registro de REDSYS \n");
	     strncpy(menReso.ICARGOS,mensaje_netplus->fidatiso.fiisotot.fitot088+4,12);
	     strncpy(menReso.NCARGOS,mensaje_netplus->fidatiso.fiisotot.fitot076,10);
	     strncpy(menReso.IABONOS,mensaje_netplus->fidatiso.fiisotot.fitot086+4,12);
	     strncpy(menReso.NABONOS,mensaje_netplus->fidatiso.fiisotot.fitot074,10);
	     strncpy(menReso.I_ANUL_CAR,mensaje_netplus->fidatiso.fiisotot.fitot087+4,12);
	     strncpy(menReso.NUM_ANUL_CAR,mensaje_netplus->fidatiso.fiisotot.fitot075,10);
	     strncpy(menReso.I_ANUL_ABO,mensaje_netplus->fidatiso.fiisotot.fitot089+4,12);
	     strncpy(menReso.NUM_ANUL_ABO,mensaje_netplus->fidatiso.fiisotot.fitot077,10);
	     strncpy(menReso.TOT_TRASPASOS, "000000000000",12); 
	     strncpy(menReso.NUM_TRANSPASOS,"0000000000",10);
	     strncpy(menReso.TOT_ANUL_TRASPASOS, "000000000000",12); 
	     strncpy(menReso.NUM_ANUL_TRASPASOS,"0000000000",10);
	     strncpy(menReso.FEC_REF_TOTALES,mensaje_netplus->fidatiso.fiisotot.fitot028.aa,2);
	     strncpy(menReso.FEC_REF_TOTALES+2,mensaje_netplus->fidatiso.fiisotot.fitot028.mm,2);
	     strncpy(menReso.FEC_REF_TOTALES+4,mensaje_netplus->fidatiso.fiisotot.fitot028.dd,2);
	     strncpy(menReso.IM_CUO_ABO,mensaje_netplus->fidatiso.fiisotot.fitot109+4,12);
	     strncpy(menReso.IM_CUO_CAR,mensaje_netplus->fidatiso.fiisotot.fitot110+4,12);
     }
   	 //PTASK0022158-FIN
	 	 printf("Mensaje 0500\n");
     strncpy(menReso.CMONTI,"0978",4);
     printf("CMONTI <%.4s> monedafac <%.4s>\n", menReso.CMONTI,mensaje_netplus->fidatiso.fiisotot.monedafac);
     printf("ICARGOS <%.12s> fitot088 <%.17s>\n",menReso.ICARGOS,mensaje_netplus->fidatiso.fiisotot.fitot088);
     printf("NCARGOS <%.10s> fitot076 <%.12s>\n", menReso.NCARGOS,mensaje_netplus->fidatiso.fiisotot.fitot076);
     printf("IABONOS <%.12s> fitot086 <%.17s>\n",menReso.IABONOS,mensaje_netplus->fidatiso.fiisotot.fitot086);
     printf("NABONOS <%.12s> fitot086 <%.12s>\n",menReso.NABONOS,mensaje_netplus->fidatiso.fiisotot.fitot074);
     printf("I_ANUL_CAR <%.12s> fitot089 <%.17s>\n",menReso.I_ANUL_CAR,mensaje_netplus->fidatiso.fiisotot.fitot089);
     printf("NUM_ANUL_CAR <%.10s> fitot077 <%.12s>\n",menReso.NUM_ANUL_CAR,mensaje_netplus->fidatiso.fiisotot.fitot077);
     printf("I_ANUL_ABO <%.12s> fitot087 <%.17s>\n",menReso.I_ANUL_ABO,mensaje_netplus->fidatiso.fiisotot.fitot089);
     printf("NUM_ANUL_ABO <%.10s> fitot075 <.12%s>\n",menReso.NUM_ANUL_ABO,mensaje_netplus->fidatiso.fiisotot.fitot077);
   }
//movemos los campos de totales a los campos de menReso

		//PRJ0258542 - Fase 3 - INI
		if (strncmp(menReso.IDTM, "0500",4) == 0)
		{
			strncpy(menReso.FEC_SESS,mensaje_netplus->fidatiso.fiisotot.fitot028.aa,2);
			strncpy(menReso.FEC_SESS+2,mensaje_netplus->fidatiso.fiisotot.fitot028.mm,2);
			strncpy(menReso.FEC_SESS+4,mensaje_netplus->fidatiso.fiisotot.fitot028.dd,2);
    }
    else
    {
			strncpy(menReso.FEC_SESS,mensaje_netplus->fidatiso.fiisoaut.fiaut028.aa,2);
			strncpy(menReso.FEC_SESS+2,mensaje_netplus->fidatiso.fiisoaut.fiaut028.mm,2);
			strncpy(menReso.FEC_SESS+4,mensaje_netplus->fidatiso.fiisoaut.fiaut028.dd,2);
    }
		//PRJ0258542 - Fase 3 - FIN

	
   //NOMEST
   strncpy(menReso.NOMEST, mensaje_netplus->fidatiso.fiisoaut.fiaut043, 40);
	 
   // TASA DE ACCESO ATM
   if (strncmp(mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4854,"  ",2) != 0)
   {
      strncpy(menReso.COMINF_TIPO, "02", 2);
      strncpy(menReso.COMINF_IMPO, mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4854+3, 6);
   } 

   //MODAUT_TERM (PROB: MODAUT_TERM=4B, bit22=12B)
   strncpy(menReso.MODAUT_TERM ,mensaje_netplus->fidatiso.fiisoaut.fiaut022, 4);

   // BIZUM BIT_P_51 cumplimentar con el conjunto de datos del P48.44 que se han creado nuevos, 
   //                  concatenados todos sin separación, pero manteniendo su estructura y orden
   if (!strncmp(menReso.SIOAPL_TIPMOV,"B",1)) 
   {
		strncpy(menReso.BIT_P_51    ,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484402,35);
		strncpy(menReso.BIT_P_51+35 ,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484403,26);
		strncpy(menReso.BIT_P_51+61 ,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484405,12);
		strncpy(menReso.BIT_P_51+73 ,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484406,13);
		strncpy(menReso.BIT_P_51+86 ,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484407,13);
		strncpy(menReso.BIT_P_51+99 ,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484408,14);
		strncpy(menReso.BIT_P_51+113,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484409,14);
		strncpy(menReso.BIT_P_51+127,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484410,35);
   	strncpy(menReso.BIT_P_51+162,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484412,70);

  	fprintLog(nomProg,"fiiso484402>%.*s<\n",35,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484402);
		fprintLog(nomProg,"fiiso484403>%.*s<\n",26,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484403);
		fprintLog(nomProg,"fiiso484405>%.*s<\n",12,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484405);
		fprintLog(nomProg,"fiiso484406>%.*s<\n",13,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484406);
		fprintLog(nomProg,"fiiso484407>%.*s<\n",13,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484407);
		fprintLog(nomProg,"fiiso484408>%.*s<\n",14,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484408);
		fprintLog(nomProg,"fiiso484409>%.*s<\n",14,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484409);
		fprintLog(nomProg,"fiiso484410>%.*s<\n",35,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484410);
		fprintLog(nomProg,"fiiso484412>%.*s<\n",70,mensaje_netplus->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484412);

    strncpy(menReso.SIOAPL_IBAN_ORDE,mensaje_netplus->fidatiso.fiisoaut.fiaut102.datos,atoi(mensaje_netplus->fidatiso.fiisoaut.fiaut102.longitud));
    strncpy(menReso.SIOAPL_IBAN_BENE,mensaje_netplus->fidatiso.fiisoaut.fiaut103.datos,atoi(mensaje_netplus->fidatiso.fiisoaut.fiaut103.longitud));

   	strncpy(menReso.INF_CTRL_SEG,mensaje_netplus->fidatiso.fiisoaut.fiaut053,16);
   }
   
   //MAC
   //CLAVE_MAC
   //INF_OP_OT_REDES
   
   //IND_SIO_PRI
   strncpy(menReso.IND_SIO_PRI , "P", 1);
   
	//PRJ0247167 - Pintamos datos de envío a MEPA
	fprintLog(nomProg,"- - - - - - - Antes de pintar todas las trazas de envio a MEPA\n");   

	fprintLog(nomProg," * * * * * * Datos enviados a Tuxedo (ftrad_haciaReso en ftrad03.c) * * * * * * *\n");
	fprintLog(nomProg,"IDE_CD       >%.16s<\n",menReso.IDE_CD);
  fprintLog(nomProg,"IDE_CD_ORI   >%.16s<\n",menReso.IDE_CD_ORI);
  fprintLog(nomProg,"CAB_CTRL_IOO >%.6s<\n",menReso.CAB_CTRL_IOO);
	fprintLog(nomProg,"CAB_CTRL_IOD >%.6s<\n",menReso.CAB_CTRL_IOD);
	fprintLog(nomProg,"CAB_CTRL_IAP >%.4s<\n",menReso.CAB_CTRL_IAP);  
	fprintLog(nomProg,"CAB_CTRL_IOP >%.6s<\n",menReso.CAB_CTRL_IOP);
	fprintLog(nomProg,"CAB_CTRL_NB  >%.2s<\n",menReso.CAB_CTRL_NB);
	fprintLog(nomProg,"CAUR         >%.2s<\n",menReso.CAUR);
	fprintLog(nomProg,"BIN1         >%.8s<\n",menReso.BIN1);
	fprintLog(nomProg,"BIN2         >%.8s<\n",menReso.BIN2);
	fprintLog(nomProg,"BIN3         >%.8s<\n",menReso.BIN3);
	fprintLog(nomProg," --> --> --> -->            IDTM         >%.4s<\n",menReso.IDTM);
	fprintLog(nomProg,"MBIT_P       >%.64s<\n",menReso.MBIT_P);
	fprintLog(nomProg,"PAN_L        >%.2s<\n",menReso.PAN_L);
	fprintLog(nomProg,"PAN_D        >%.18s<\n",menReso.PAN_D);
	fprintLog(nomProg,"CODPROC      >%.6s<\n",menReso.CODPROC);
	fprintLog(nomProg,"IMPORTX      >%.8s<\n",menReso.IMPORTX);
	fprintLog(nomProg,"IDEN_USR_L   >%.2s<\n",menReso.IDEN_USR_L);
	fprintLog(nomProg,"IDEN_USR_TIPO>%.2s<\n",menReso.IDEN_USR_TIPO);
	fprintLog(nomProg,"IDEN_USR_DATO>%.30s<\n",menReso.IDEN_USR_DATO);
	fprintLog(nomProg,"IMPOROP      >%.12s<\n",menReso.IMPOROP);
	fprintLog(nomProg," --> --> --> -->            IDTR         >%.6s<\n",menReso.IDTR);
	fprintLog(nomProg,"FECHAL       >%.6s<\n",menReso.FECHAL);
	fprintLog(nomProg,"CMONTI       >%.4s<\n",menReso.CMONTI);
	fprintLog(nomProg,"HORAL        >%.6s<\n",menReso.HORAL);
	fprintLog(nomProg,"FECCAD       >%.4s<\n",menReso.FECCAD);
	fprintLog(nomProg,"ACCION_TARJ  >%.2s<\n",menReso.ACCION_TARJ);
	fprintLog(nomProg,"DAT_DEP_EFE  >%.4s<\n",menReso.DAT_DEP_EFE);
	fprintLog(nomProg,"COMINF_TIPO  >%.2s<\n",menReso.COMINF_TIPO);
	fprintLog(nomProg,"COMINF_IMPO  >%.6s<\n",menReso.COMINF_IMPO);
	fprintLog(nomProg,"SECTOR       >%.4s<\n",menReso.SECTOR);
  fprintLog(nomProg,"ID_REDTERM   >%.12s<\n",menReso.ID_REDTERM);
	fprintLog(nomProg,"CODRAZ_ANUL  >%.2s<\n",menReso.CODRAZ_ANUL);
	fprintLog(nomProg,"NUM_MIEMB    >%.1s<\n",menReso.NUM_MIEMB);
  fprintLog(nomProg,"ICARGOS      >%.12s<\n",menReso.ICARGOS);
  fprintLog(nomProg,"NCARGOS      >%.10s<\n",menReso.NCARGOS);
  fprintLog(nomProg,"IABONOS      >%.12s<\n",menReso.IABONOS);
  fprintLog(nomProg,"NABONOS      >%.10s<\n",menReso.NABONOS);
  fprintLog(nomProg,"I_ANUL_CAR   >%.12s<\n",menReso.I_ANUL_CAR);
  fprintLog(nomProg,"NUM_ANUL_CAR >%.10s<\n",menReso.NUM_ANUL_CAR);
  fprintLog(nomProg,"I_ANUL_ABO   >%.12s<\n",menReso.I_ANUL_ABO);
  fprintLog(nomProg,"NUM_ANUL_ABO >%.10s<\n",menReso.NUM_ANUL_ABO);
  fprintLog(nomProg,"DAT_ABO_COM  >%.36s<\n",menReso.DAT_ABO_COM);
	fprintLog(nomProg,"INF_ADIC_PET_TRANSF >%.32s<\n",menReso.INF_ADIC_PET_TRANSF);
	fprintLog(nomProg," --> --> --> -->            CODRES       >%.2s<\n",menReso.CODRES);
  fprintLog(nomProg,"IDEN_BLOQUE_MOV >%.12s<\n",menReso.IDEN_BLOQUE_MOV);
	fprintLog(nomProg,"ADIC_RESP_TRANSFER >%.32s<\n",menReso.ADIC_RESP_TRANSFER);
  fprintLog(nomProg,"DATORI       >%.30s<\n",menReso.DATORI);
  fprintLog(nomProg,"NUMAUT       >%.6s<\n",menReso.NUMAUT);
	fprintLog(nomProg,"INF_SALDO    >%.110s<\n",menReso.INF_SALDO);
  fprintLog(nomProg,"INF_MOV      >%.222s<\n",menReso.INF_MOV);
  fprintLog(nomProg,"NUM_REF      >%.8s<\n",menReso.NUM_REF);
	fprintLog(nomProg,"REF_TPV      >%.16s<\n",menReso.REF_TPV);
	fprintLog(nomProg,"MON_CTBLE    >%.4s<\n",menReso.MON_CTBLE);
	fprintLog(nomProg,"TOTAL_TELEBANCO >%.218s<\n",menReso.TOTAL_TELEBANCO);
	fprintLog(nomProg,"IMP_TERMINAL >%.14s<\n",menReso.IMP_TERMINAL);
	fprintLog(nomProg,"DAT_ADIC_ENV_TRANSP >%.404s<\n",menReso.DAT_ADIC_ENV_TRANSP);
	fprintLog(nomProg,"NOMEST       >%.40s<\n",menReso.NOMEST); 
	fprintLog(nomProg,"DAT_ADIC_REC_TRANSP >%.404s<\n",menReso.DAT_ADIC_REC_TRANSP);
	fprintLog(nomProg,"MODAUT_TERM  >%.4s<\n",menReso.MODAUT_TERM);
	fprintLog(nomProg,"MAC          >%.6s<\n",menReso.MAC); 
	fprintLog(nomProg,"CLAVE_MAC    >%.2s<\n",menReso.CLAVE_MAC); 
  fprintLog(nomProg,"TOT_TRASPASOS      >%.12s<\n",menReso.TOT_TRASPASOS);
  fprintLog(nomProg,"NUM_TRANSPASOS     >%.10s<\n",menReso.NUM_TRANSPASOS);
  fprintLog(nomProg,"TOT_ANUL_TRASPASOS >%.12s<\n",menReso.TOT_ANUL_TRASPASOS);
  fprintLog(nomProg,"NUM_ANUL_TRASPASOS >%.10s<\n",menReso.NUM_ANUL_TRASPASOS);
	fprintLog(nomProg,"BIT_P_58     >%.94s<\n",menReso.BIT_P_58); 
	fprintLog(nomProg,"BIT_P_59     >%.186s<\n",menReso.BIT_P_59); 
	fprintLog(nomProg,"FEC_REF_TOTALES >%.6s<\n",menReso.FEC_REF_TOTALES); 
	fprintLog(nomProg,"IND_CUADRE_TOT  >%.2s<\n",menReso.IND_CUADRE_TOT); 
	fprintLog(nomProg,"STATUS_TELEBANCO>%.20s<\n",menReso.STATUS_TELEBANCO); 
	fprintLog(nomProg,"INF_OP_OT_REDES >%.40s<\n",menReso.INF_OP_OT_REDES); 
	fprintLog(nomProg,"PUNT_SERV    >%.12s<\n",menReso.PUNT_SERV); 
	fprintLog(nomProg,"EMISOR       >%.2s<\n",menReso.EMISOR); 
	fprintLog(nomProg,"CTA_EMISOR   >%.18s<\n",menReso.CTA_EMISOR); 
 	fprintLog(nomProg,"COD_FUN      >%.3s<\n",menReso.COD_FUN); 
	fprintLog(nomProg,"COD_ACTI     >%.4s<\n",menReso.COD_ACTI); 
	fprintLog(nomProg,"FEC_SESS     >%.8s<\n",menReso.FEC_SESS); 
	fprintLog(nomProg,"REF_ADQ      >%.23s<\n",menReso.REF_ADQ); 
	fprintLog(nomProg,"IDE_ADQ      >%.11s<\n",menReso.IDE_ADQ); 
	fprintLog(nomProg,"IM_CUO       >%.44s<\n",menReso.IM_CUO); 
	fprintLog(nomProg,"INF_CTRL_SEG >%.16s<\n",menReso.INF_CTRL_SEG); 
	fprintLog(nomProg,"IDENT_AUT    >%.11s<\n",menReso.IDENT_AUT); 
	fprintLog(nomProg,"IND_SIO_PRI  >%.1s<\n",menReso.IND_SIO_PRI); 
	fprintLog(nomProg,"IM_CUO_ABO   >%.12s<\n",menReso.IM_CUO_ABO);
	fprintLog(nomProg,"IM_CUO_CAR   >%.12s<\n",menReso.IM_CUO_CAR);
	fprintLog(nomProg,"CONCILI      >%.1s<\n",menReso.CONCILI);
	fprintLog(nomProg," --> --> --> -->            NUMOPER_SISMA>%.6s<\n",menReso.NUMOPER_SISMA);
	fprintLog(nomProg,"VALOR_P48_20 >%.16s<\n",menReso.VALOR_P48_20);   //PRJ0247167
	fprintLog(nomProg,"BIT_P_51 000 a 060 >%.60s<\n", menReso.BIT_P_51);
	fprintLog(nomProg,"BIT_P_51 061 a 120 >%.60s<\n", menReso.BIT_P_51 + 61);
	fprintLog(nomProg,"BIT_P_51 121 a 180 >%.60s<\n", menReso.BIT_P_51 + 121);
	fprintLog(nomProg,"BIT_P_51 181 a 240 >%.60s<\n", menReso.BIT_P_51 + 181);
	fprintLog(nomProg,"BIT_P_51 241 a 300 >%.60s<\n", menReso.BIT_P_51 + 241);
	fprintLog(nomProg,"BIT_P_51 301 a 360 >%.60s<\n", menReso.BIT_P_51 + 301);
	fprintLog(nomProg,"BIT_P_51 361 a 420 >%.60s<\n", menReso.BIT_P_51 + 361);
	fprintLog(nomProg,"BIT_P_51 421 a 480 >%.60s<\n", menReso.BIT_P_51 + 421);
	fprintLog(nomProg,"VALOR_P48_94 >%.6s<\n",menReso.VALOR_P48_94); 	//PRJ0257005
	//fprintLog(nomProg,"VALOR_P62_16 >%.30s<\n",menReso.VALOR_P62_16); 	//PRJ0257005
	fprintLog(nomProg,"SIOAPL_TARJTOKEN >%.16s<\n",menReso.SIOAPL_TARJTOKEN); 	//PRJ0257005
	fprintLog(nomProg,"SIOAPL_FECATTOKE >%.4s<\n",menReso.SIOAPL_FECATTOKE); 	//PRJ0257005
	fprintLog(nomProg,"SIOAPL_TIPMOV     >%.1s<\n" ,menReso.SIOAPL_TIPMOV); 	//BIZUM Valor fijo B si es Bizum - T para el resto de operativa
	fprintLog(nomProg,"SIOAPL_CODIGO_RES  >%.3s<\n" ,menReso.SIOAPL_CODIGO_RES); 	//BIZUM Respuesta a la operativa Bizum
	fprintLog(nomProg,"SIOAPL_IBAN_ORDE  >%.34s<\n",menReso.SIOAPL_IBAN_ORDE); 	//BIZUM Iban cuenta ordenante
	fprintLog(nomProg,"SIOAPL_IBAN_BENE  >%.34s<\n",menReso.SIOAPL_IBAN_BENE); 	//BIZUM Iban cuenta Beneficiario
	fprintLog(nomProg,"SIOAPL_OFFSET  >%.4s<\n",menReso.SIOAPL_OFFSET); 	//PRJ0258542
	fprintLog(nomProg,"COD_RAZ >%.4s<\n",menReso.SIOAPL_CODRAZ); //PRJ0350720
	fprintLog(nomProg,"- - - - - Despues de pintar todas las trazas de envio a MEPA\n");   
  
  memcpy(mensaje_reso, (char*) &menReso, sizeof(formatoNuevo));
  
  for (i = 0; i<sizeof(formatoNuevo); i++){
     if(mensaje_reso[i] == '\0'  || mensaje_reso[i] == '<' || mensaje_reso[i] == '>')
	   mensaje_reso[i] = ' ';
  }
   
   //fprintLog(nomProg,"traducido en ftrad_haciaReso >%.*s< ...\n", sizeof(formatoNuevo), mensaje_reso);  
      
   *long_iso = sizeof(formatoNuevo);
   
   return(0);

} // fin ftrad_haciaReso


/********************************************/
/***   funcion ftrad_desdeReso            ***/
/*** Identifica la operación y actualiza  ***/
/*** algunos datos de formato_interno con ***/
/*** la información de mensaje_reso       ***/
/********************************************/

int   ftrad_desdeReso( int *long_iso, char *mensaje_reso, formato_interno *mensaje_netplus, int tipMsj,char *subsistema_general)
{
	 char  auxMoneda[4];
	 char  infoSaldo[3]; //CDd - Credito - Debito - diario
	 int i, contMov =0;
	 int auxLongi = 0;
	 int retorno=0;
	 char tjtdescifrada[22];
	 char auxComi[6];
     char strMovimientos[38];
     char strSaldos[25];
     char CODIGOPROCESO[6];
     char emisor_str_desde[2]; 
     char ide_cd_aux[3];      
     int LenIDE_ADQ;
     char strlongitud[11];   
     int long_DATORI;    
     char strlong_ORI[4];
     
     char IDTMstr[5];
     int  IDTMint;
     int j;
    
	 fprintLog(nomProg, "Numope >%.6s< traducimos mensaje recibido MEPA >%d<>%s<\n",mensaje_reso + 22 ,sizeof(formatoNuevo),mensaje_reso); 

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// NUEVO /////////////////////////////////////
	 formatoNuevo *menReso;
	 menReso = (formatoNuevo*)mensaje_reso;
   
     //IDE_CD_ORI
   strncpy(mensaje_netplus->fidatpro.fifecha0, menReso->IDE_CD_ORI, 2);
   strncpy(mensaje_netplus->fiisocom.fifechor.aa, menReso->IDE_CD_ORI+2, 2);
   strncpy(mensaje_netplus->fiisocom.fifechor.mm, menReso->IDE_CD_ORI+4, 2);
   strncpy(mensaje_netplus->fiisocom.fifechor.dd, menReso->IDE_CD_ORI+6, 2);
   strncpy(mensaje_netplus->fiisocom.fifechor.hh, menReso->IDE_CD_ORI+8, 2);
   strncpy(mensaje_netplus->fiisocom.fifechor.mi, menReso->IDE_CD_ORI+10, 2);
   strncpy(mensaje_netplus->fiisocom.fifechor.ss, menReso->IDE_CD_ORI+12, 2);
   //PRJ0247167
   /*
   #if defined (TRAZA) && (TRAZA > 10)
	  fprintLog(nomProg,"fifecha0>%.2s<\n", mensaje_netplus->fidatpro.fifecha0);  
      fprintLog(nomProg,"fifechor>%.2s-%.2s-%.2s %.2s:%.2s:%.2s<\n"
         ,mensaje_netplus->fiisocom.fifechor.aa
		 ,mensaje_netplus->fiisocom.fifechor.mm
		 ,mensaje_netplus->fiisocom.fifechor.dd
		 ,mensaje_netplus->fiisocom.fifechor.hh
		 ,mensaje_netplus->fiisocom.fifechor.mi
		 ,mensaje_netplus->fiisocom.fifechor.ss);
   #endif
   */
	 
    // PAN_L Y PAN_D
	// PRJ0202598 cambio bin de 6 posiciones 8 ya eran de 8
   strncpy(mensaje_netplus->fidatiso.fiisoaut.finumPAN.longitud, menReso->PAN_L, 2);
   strncpy(mensaje_netplus->fidatpro.fipancif, menReso->PAN_D, 18);
   //PRJ0247167
   /*
  #if defined (TRAZA) && (TRAZA > 10)   		   
      fprintLog(nomProg,"fiisoaut finumPAN longitud>%.2s<\n", mensaje_netplus->fidatiso.fiisoaut.finumPAN.longitud);
	  fprintLog(nomProg,"fipancif>%.18s<\n", mensaje_netplus->fidatpro.fipancif);
   #endif
	*/
   //CODPROC 
 
      /*******CAMBIAMOS LOS IF POR LA CONSULTA AL REGISTRO DE LA TABLA NPTTRMS*****/
   strncpy(ide_cd_aux, menReso->IDE_CD,2);
   ide_cd_aux[3]='\0';
//cambiamos el IDTM en las consultas sucesivas de movimietos si es 5200,5300,5400, 5500, 5600 lo convertimos en 5000
   if (strncmp(menReso->IDTM,"5200",4) == 0 || strncmp(menReso->IDTM,"5300",4) == 0 ||
       strncmp(menReso->IDTM,"5400",4) == 0 || strncmp(menReso->IDTM,"5500",4) == 0 ||
       strncmp(menReso->IDTM,"5600",4) == 0 )
   {
       strncpy(menReso->IDTM,"5000",4);
   }

    //RAQUEL modificado para recoger el dato de bbdd por entidad
	
    for (j=0 ; j<2 ; j++) 
    {
      if ((strncmp(reg_para[j].psidered,ide_cd_aux,2)==0))
      {	
          strncpy(emisor_str_desde, reg_para[j].pscotrms,2);
          break;            
      }  
    } 
  int ll ,salir;
  
   if ((tipMsj==1804) || (tipMsj==0510))
   {
      for (ll=0,salir=0; ll< 500 && !salir; ll++)
       {
      
         if ((strncmp(tabla_trms[ll].trmsidsi,menReso->IDTM,4)==0)
             && (strncmp(tabla_trms[ll].trmsemis,emisor_str_desde,2)==0))
         {
             salir = 1;
             
            strncpy(mensaje_netplus->fiisocom.fiiso000, tabla_trms[ll].trmsidtv,4);
             
             break;
         }
         
       }    /***   fin del bucle   ***/ 
  
   }
   else
   {
       for (ll=0,salir=0; ll< 500 && !salir; ll++)
       {
      
         if ((strncmp(tabla_trms[ll].trmsidsi,menReso->IDTM,4)==0)
             &&  (strncmp(tabla_trms[ll].trmscosi,menReso->CODPROC,6)==0)
             && (strncmp(tabla_trms[ll].trmsemis,emisor_str_desde,2)==0))  
         {
             salir = 1;
             
            strncpy(mensaje_netplus->fiisocom.fiiso000, tabla_trms[ll].trmsidtv,4);
            strncpy(CODIGOPROCESO, tabla_trms[ll].trmscopr,6);
             
             break;
         }
         
       }    /***   fin del bucle   ***/
   }
     //CODIGOPROCESO
//Movemos los valores de IDTM y CODPROC originales, si no encontramos traducción quedan los originales
   if (mensaje_netplus->fiisocom.fiiso000[0] == ' ')
   {
      strncpy(IDTMstr, menReso->IDTM,4);
      IDTMstr[4] = '\0';
      IDTMint = atoi(IDTMstr) + 10;
      sprintf(IDTMstr, "%.4s", IDTMint);
      strncpy(mensaje_netplus->fiisocom.fiiso000, IDTMstr,4);
      strncpy(CODIGOPROCESO, menReso->CODPROC,6);
   }

   strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12, CODIGOPROCESO, 2);
   strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig34, CODIGOPROCESO+2, 2);
   strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig56, CODIGOPROCESO+4, 2); 
   
   //PRJ0247167 - INICIO
   /*
   #if defined (TRAZA) && (TRAZA > 10)  
      fprintLog(nomProg,"fiaut003>%.2s-%.2s-%.2s<\n"
     ,mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig12
	 ,mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig34
	 ,mensaje_netplus->fidatiso.fiisoaut.fiaut003.dig56);
     
     fprintLog(nomProg,"fiiso000>%.4s<\n",mensaje_netplus->fiisocom.fiiso000);
     
   #endif
    */
   //PRJ0257037 - INICIO
   //IMPORTX
   //PROB: IMPORTX=8, IMPRTE=12
   //original -- strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut004, menReso->IMPORTX, 8);
   if(mensaje_netplus->fidatiso.fiisoaut.finumPAN.longitud > 0)
   {
     if(!nspaces(mensaje_netplus->fidatiso.fiisoaut.fiaut009, 8))
     {
      strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut005, menReso->IMPORTX, 8);
      strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut004, menReso->IMPOROP, 12);
     }
     else
     {
       if(!nspaces(mensaje_netplus->fidatiso.fiisoaut.fiaut010, 8))
       {
         strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut006, menReso->IMPORTX, 8);
         strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut004, menReso->IMPOROP, 12);
       }
       else 
       {
         strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut004, menReso->IMPORTX, 8);
       }
     }
   }
   else
   {
      strncpy(menReso->IMPORTX, "00000000", 8);
      strncpy(menReso->IMPOROP, "000000000000", 12);
   }
   strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut022, menReso->PUNT_SERV, 12);
   strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut037, menReso->REF_ADQ, 12);
   //PRJ0257037 - FINAL

   //PRJ0247167 - FINAL
	/*
	#if defined (TRAZA) && (TRAZA > 10)
	  fprintLog(nomProg,"fiaut004>%.8s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut004);
	#endif
	*/
   //NUMOPER_SISMA  

   strncpy(mensaje_netplus->fidatpro.finumope,menReso->NUMOPER_SISMA,6);
   strncpy(mensaje_netplus->fidatpro.finumope,menReso->TOTAL_TELEBANCO,6); 
   
   //PRJ0247167
   /*
   #if defined (TRAZA) && (TRAZA > 10)
	  fprintLog(nomProg,"finumope>%.6s<\n",mensaje_netplus->fidatpro.finumope);
   #endif
	*/
   //FECHAL
   strncpy(mensaje_netplus->fiisocom.fifechor.aa, menReso->FECHAL, 2);	
   strncpy(mensaje_netplus->fiisocom.fifechor.mm, menReso->FECHAL+2, 2);	
   strncpy(mensaje_netplus->fiisocom.fifechor.dd, menReso->FECHAL+4, 2);
   //PRJ0247167
   /*
   #if defined (TRAZA) && (TRAZA > 10)  
      fprintLog(nomProg,"fifechor aa-mm-dd>%.2s-%.2s-%.2s<\n"
         ,mensaje_netplus->fiisocom.fifechor.aa
		 ,mensaje_netplus->fiisocom.fifechor.mm
		 ,mensaje_netplus->fiisocom.fifechor.dd);
   #endif
	*/
   //CMONTI
   //strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut049, menReso->CMONTI, 3);
   //CMONTI y MON_CTBLE
	//PRJ0258542 - cambio moneda extranjera - INI
  if (strncmp(menReso->CMONTI, "0978", 4) == 0)
  {
   	strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut049, menReso->CMONTI, 3);
  }
  else
  {
   	strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut049, menReso->MON_CTBLE, 3);
   	strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut004, menReso->IDEN_BLOQUE_MOV, 12);
		if(!nspaces(mensaje_netplus->fidatiso.fiisoaut.fiaut009, 8))
		{
			strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut005, menReso->IMPORTX, 8);
			strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut050, "978", 3);
		}
		else
		{
		 	if(!nspaces(mensaje_netplus->fidatiso.fiisoaut.fiaut010, 8))
		 	{
		   	strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut006, menReso->IMPORTX, 8);
				strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut051, "978", 3);
		 	}
		 	else 
		 	{
		   	strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut004, menReso->IMPORTX, 8);
		 	}
		}  
	}
 	//PRJ0258542 - cambio moneda extranjera - FIN

   //PRJ0247167
   /*
   #if defined (TRAZA) && (TRAZA > 10)
	  fprintLog(nomProg,"fiaut049>%.3s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut049);
   #endif
	*/
   //HORAL
   strncpy(mensaje_netplus->fiisocom.fifechor.hh, menReso->HORAL, 2);	
   strncpy(mensaje_netplus->fiisocom.fifechor.mi, menReso->HORAL+2, 2);	
   strncpy(mensaje_netplus->fiisocom.fifechor.ss, menReso->HORAL+4, 2);
   //PRJ0247167
   /*
   #if defined (TRAZA) && (TRAZA > 10)  
      fprintLog(nomProg,"fifechor hh-mi-ss>%.2s-%.2s-%.2s<\n"
        ,mensaje_netplus->fiisocom.fifechor.hh
		 		,mensaje_netplus->fiisocom.fifechor.mi
		 		,mensaje_netplus->fiisocom.fifechor.ss);
   #endif
	*/
   //FECCAD
   strncpy(mensaje_netplus->fidatiso.fiisoaut.fifeccad.aa, menReso->FECCAD, 2);
   strncpy(mensaje_netplus->fidatiso.fiisoaut.fifeccad.mm, menReso->FECCAD+2, 2);
   //PRJ0247167
   /*
   #if defined (TRAZA) && (TRAZA > 10)  
      fprintLog(nomProg,"fifeccad aa-mm>%.2s-%.2s<\n"
         ,mensaje_netplus->fidatiso.fiisoaut.fifeccad.aa
		 ,mensaje_netplus->fidatiso.fiisoaut.fifeccad.mm);   
   #endif 
	*/
   
   //RDCAJEROS comisiones
   //si es una retirada de efectivo una consulta de saldo o una consulta de movimiento puede haber comision adicional 

	//PTASK0022158 - INI - Se quita meter los dos ceros fijos, salvo que llegue informado el importe de la comision
	/*
	strncpy(mensaje_netplus->fidatpro.ficomisi,"00",2);
	strncpy(mensaje_netplus->fidatpro.ficomisi+2,menReso->COMINF_IMPO,6); // la comision se pasa siempre
	if(strncmp(mensaje_netplus->fidatpro.ficomisi, "        ",8)!=0)  //si no llega a espacios es que esta informada aunque sea cero
	{ 
		sprintf(mensaje_netplus->fidatpro.fitipcom,"%.1s","1"); //comision exacta 
	}
	*/ //PTASK0022158 - Se quita este asteriscado
	printf(" *** *** Nuevo control de comisiones 30-07-2026 *** *** \n"); //PRJ0387060
	if(strncmp(menReso->COMINF_IMPO, "      ",6) != 0)
	{
		printf("------- Indica que trae comision ---ftrad--- \n"); //PRJ0387060
		strncpy(mensaje_netplus->fidatpro.ficomisi,"00",2);
		strncpy(mensaje_netplus->fidatpro.ficomisi+2,menReso->COMINF_IMPO,6);
		sprintf(mensaje_netplus->fidatpro.fitipcom,"%.1s","1");
	}
	else //PRJ0387060
	{
		printf("------- Indica que no trae comision ---ftrad--- \n");
		strncpy(mensaje_netplus->fidatpro.ficomisi,"        ",8);
		strncpy(mensaje_netplus->fidatpro.fitipcom,"  ",2);	
	} //PRJ0387060
	//PTASK0022158 - FIN
	
  //COMISION ADICIONAL
  //operaciones 1210 codigo proceso 01 reintegro cajero con comision informada
  //PTASK000016208  PTASK000016224 
  //si es una retirada de efectivo puede haber comision adicional
   if(strncmp(menReso->CODPROC,"01",2)==0)
   { 
			printf("------- Indica que tiene que poner comision ---ftrad--- \n"); //PRJ0387060 INI
			if (strncmp(menReso->COMINF_IMPO, "      ",6) == 0)
			{
				strncpy(menReso->COMINF_IMPO, "000000",6);
			}
			if (strncmp(menReso->COMINF_TIPO, "  ",2) == 0)
			{
				strncpy(menReso->COMINF_TIPO, "00",2);
			}
			//PRJ0387060 FIN
	   sprintf(auxComi,"%.6s",menReso->COMINF_IMPO);
	   if(atoi(auxComi)>0)  //la comision adicinal solo la muevo si es mayor de cero.
       {
        strncpy(mensaje_netplus->fidatpro.fitipcom, menReso->COMINF_TIPO+1,1); 

       }
       strncpy(mensaje_netplus->fidatpro.ficomadi,menReso->COMINF_IMPO,6);
    }
	else //PRJ0387060
	{
		printf("------- Indica que no tiene que poner comision ---ftrad--- \n");
		strncpy(mensaje_netplus->fidatpro.ficomadi,"      ",6);
		strncpy(mensaje_netplus->fidatpro.fitipcom,"  ",2);	
		strncpy(mensaje_netplus->fidatpro.ficomisi,"        ",8);
	} //PRJ0387060
   
   //SECTOR
	strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut026, menReso->SECTOR, 4);
  strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut018, menReso->SECTOR, 4);
   //PRJ0247167
   /*
   #if defined (TRAZA) && (TRAZA > 10)
     fprintLog(nomProg,"fiaut018 >%.4s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut018);
     fprintLog(nomProg,"fiaut026 >%.4s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut026);
   #endif	
	*/
 
	//ID_REDTERM RAQUEL
	//strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut041, menReso->ID_REDTERM , 8);

   //PRJ0247167
   /*
      printf("REDTERM long <%.3s> datos <%.8s>\n", "008", menReso->ID_REDTERM);
      
#if defined (TRAZA) && (TRAZA > 10)   		
     fprintLog(nomProg,"fi032094 >%.12s<\n", mensaje_netplus->fiisocom.fi032094.datos);
#endif
	*/
  
   strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut025, menReso->CODRAZ_ANUL, 2);
   //PRJ0247167
   /*
   #if defined (TRAZA) && (TRAZA > 10) 
	  fprintLog(nomProg,"fiaut025>%.2s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut025);
   #endif 
	*/
    //ADIC_RESP_TRANSFER  //INF_ADIC_PET_TRANSF   

   if((strncmp(mensaje_netplus->fiisocom.fiiso000,"12",2)==0) && 
      ((strncmp(CODIGOPROCESO,"40",2)==0)||(strncmp(CODIGOPROCESO,"49",2)==0)))
   {
        strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut103.datos,menReso->INF_ADIC_PET_TRANSF,20);
       strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut103.longitud,"20",2);
      
       strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4861,menReso->ADIC_RESP_TRANSFER,10);
       
       if((strncmp(menReso->ADIC_RESP_TRANSFER+22," ",1)==0))
       {
          strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4861,"0000000000",10);
          strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut102.datos,"00000000000000000000",20);
          strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut102.longitud,"20",2);    
       }
       else 
       {           
          strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4861,menReso->ADIC_RESP_TRANSFER,10);
          strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut102.datos,menReso->ADIC_RESP_TRANSFER+22,20);
          strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut102.longitud,"20",2);  
       }          
      
   //PRJ0247167
   /*
       #if defined (TRAZA) && (TRAZA > 10)   		      
          //fprintLog(nomProg,"fiaut103.datos >%.20s<\n",mensaje_netplus->fidatiso.fiisoaut.fiaut103.datos);
          //fprintLog(nomProg,"fiaut102.datos >%.20s<\n",mensaje_netplus->fidatiso.fiisoaut.fiaut102.datos);
          fprintLog(nomProg,"fiiso4861 >%.10s<\n",mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4861);
       #endif   
	*/
    } 

  fprintLog(nomProg,"Codigo de respuesta menReso->CODRES <%c%c> IDMT <%c%c%c%c>\n", menReso->CODRES[0], menReso->CODRES[1], menReso->IDTM[0],menReso->IDTM[1],menReso->IDTM[2],menReso->IDTM[3]); 

  sprintf(mensaje_netplus->fiisocom.respusal,"%.2s", menReso->CODRES);
  sprintf(mensaje_netplus->fidatpro.firesprs,"%.2s", menReso->CODRES); //este lo guardo pq respusal se machaca con la traducción en EO   
	fprintLog(nomProg,"Valor que lleva respusal despues de mover CODRES >%.2s<\n", mensaje_netplus->fiisocom.respusal);
  fprintLog(nomProg,"Valor que lleva firesprs despues de mover CODRES >%.2s<\n", mensaje_netplus->fidatpro.firesprs);
	
   //NUMAUT // BIZUM - metemos el NUMOPE para estas operaciones
   	fprintLog(nomProg,"  -- Valor para saber si es operativa Bizum y poner NUMOPE >%.2s< >%.2s<\n", menReso->CODPROC, menReso->CODRES);
	//PRJ0331925 - INI
  	if (strncmp(menReso->SIOAPL_TIPMOV,"B",1) == 0 && strncmp(menReso->CODRES, "01", 2) == 0)//PTASK000023003
   	{
   		if (strncmp(menReso->NUMAUT, "      ", 6))
   		{
   			strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut038, menReso->NUMAUT, 6);
	   		fprintLog(nomProg," NUMAUT asignado por Bizum >%.6s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut038);
  		}
   		else
   		{
   			strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut038, mensaje_netplus->fidatpro.finumope, 6);
  	 		fprintLog(nomProg," NUMAUT asignado por Sisma >%.6s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut038);
   		}
		}
		else
		{   
   		if (strncmp(menReso->NUMAUT, "      ", 6))
   		{
	   		strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut038, menReso->NUMAUT, 6);
  	 		fprintLog(nomProg," NUMAUT asignado por SF >%.6s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut038);
   		}
   		else
   		{
   			strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut038, mensaje_netplus->fidatpro.finumope, 6);
  	 		fprintLog(nomProg," NUMAUT asignado por Sisma >%.6s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut038);
   		}
   	}
	//PRJ0331925 - FIN
   //BIZUM nuevos campos - respuesta de SF
   	if (!strncmp(menReso->SIOAPL_TIPMOV,"B",1)) 
   	{
      strncpy(mensaje_netplus->fiisocom.respusal, menReso->BIT_P_51 + 233, 3); //BIZUM Respuesta a la operativa Bizum
      fprintLog(nomProg," -- respusal asignado por Bizum >%.3s<\n", mensaje_netplus->fiisocom.respusal);
   	}
 
   	
   //PRJ0247167
   /*
   #if defined (TRAZA) && (TRAZA > 10)
	  fprintLog(nomProg,"fiaut038>%.6s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut038);
   #endif
	*/
   //NUM_REF

  // REF_TPV
   strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut042, menReso->REF_TPV , 15);
   //PRJ0247167
   /*
   #if defined (TRAZA) && (TRAZA > 10)   		
     fprintLog(nomProg,"fiaut042 >%.15s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut042);
   #endif
	*/
   // si los saldos son distintos de cero : P48.05 //////////////////////////      
   // y no es una consulta de movimientos
   //INF_SALDOS 
   if (strncmp(menReso->INF_SALDO, " ",1) != 0)
   {
     //EL P48 NUMERO CUENTAS SIOAPL-SALDO-NUM-CTAS  PIC  9(002).
	   //RAQUEL **** habra que comprobar si al aumentar el p48 estas posiciones de saldo se desplazan
       sprintf(mensaje_netplus->fidatiso.fiisoaut.fiaut048.augen.saldos.num_ctas, "%.1s" , menReso->INF_SALDO+5);
      #if defined (TRAZA) && (TRAZA > 10)
        fprintLog(nomProg,"fiaut048 cantidad de cuentas>%.1s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut048.augen.saldos.num_ctas);
      #endif 
      //EL P48 CADA UNA DE LAS CUENTAS  
      int PosINF_SAL; 
      for (i = 0; i<4; i++){
             memset(strSaldos,0,25); 
             PosINF_SAL = 6+(25*i);
             strncpy(strSaldos, menReso->INF_SALDO+PosINF_SAL+1,1);        
             strncpy(strSaldos+1, menReso->INF_SALDO+PosINF_SAL+2,6);             
             if (strncmp(menReso->INF_SALDO+(PosINF_SAL + 9),"1",1)==0)
              {
	               strncpy(strSaldos+7, "1",1);
              } 
              else 
              {
	               strncpy(strSaldos+7, "0",1); 
              }
              strncpy(strSaldos+8, menReso->INF_SALDO+(PosINF_SAL+10),12);
              strncpy(strSaldos+20, "978",3);
              strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut048.augen.saldos.saldo_dat[i], strSaldos,23);
              printf("saldos <%d> : <%s>\n", i,strSaldos);
              printf("mensaje_netplus->fidatiso.fiisoaut.fiaut048.augen.saldos.saldo_dat <%d> : <%s>\n", i,mensaje_netplus->fidatiso.fiisoaut.fiaut048.augen.saldos.saldo_dat[i]);
      }

   }

    // si se informan los movimientos: P48.06 y es una consulta de movimiento pq si no machaco el 48 para saldos.
	//RAQUEL *********comprobar si esta informacion de movimientos se desplazaria con las posiciones del p48 y p48.20
   //INF_MOV
   if (strncmp(menReso->INF_MOV, " ",1) != 0)
   {
     //EL P48 FECHA SIOAPL-UM-FEC-SALDO PIC 9(006)
      sprintf(mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.sub04806.fecha, "%.6s" , menReso->INF_MOV+4);
      #if defined (TRAZA) && (TRAZA > 10)
        fprintLog(nomProg,"fiaut048 movimiento saldo fecha  >%.6s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.sub04806.fecha);
      #endif 
      //EL P48 SIGNO SIOAPL-UM-SIG-SALDO PIC X(002).
      sprintf(mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.sub04806.signo, "%.1s" , menReso->INF_MOV+11);
      #if defined (TRAZA) && (TRAZA > 10)
        fprintLog(nomProg,"fiaut048 movimiento  saldo signo >%.1s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.sub04806.signo);
      #endif	
      //EL P48 SALDO SIOAPL-UM-IMP-SALDO PIC 9(012)V99.
      sprintf(mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.sub04806.importe, "%.12s" , menReso->INF_MOV+14);
      #if defined (TRAZA) && (TRAZA > 10)
        fprintLog(nomProg,"fiaut048 movimiento importe>%.14s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.sub04806.importe);
      #endif	
      //EL P48 MONEDA SIOAPL-UM-COD-MONE PIC X(004).
       sprintf(mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.sub04806.moneda, "%.3s" , menReso->INF_MOV+67);
      #if defined (TRAZA) && (TRAZA > 10)
        fprintLog(nomProg,"fiaut048 movimiento saldo moneda>%.3s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.sub04806.moneda);
      #endif	   
     //EL P48 NUMERO MOVIMIENTOS SIOAPL-UM-NUM-MVTOS PIC 9(002).
       sprintf(mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.sub04806.num_movi, "%.1s" , menReso->INF_MOV+31);
      #if defined (TRAZA) && (TRAZA > 10)
        fprintLog(nomProg,"fiaut048 movimiento saldo cantidad de movimientos>%.1s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.sub04806.num_movi);
      #endif 
      //EL P48 CADA UNO DE LOS MOVIMIENTOS  
      int PosINF_MOV; 
      for (i = 0; i<5; i++){
              memset(strMovimientos,0,38); 
              PosINF_MOV = 32+(38*i);
	      strncpy(strMovimientos, menReso->INF_MOV+PosINF_MOV,6);
              if (strncmp(menReso->INF_MOV+(PosINF_MOV + 7),"1",1)==0)
              {
	         strncpy(strMovimientos+6, "1",1);
              } else {
	         strncpy(strMovimientos+6, "0",1); 
              }
              strncpy(strMovimientos+7, menReso->INF_MOV+(PosINF_MOV+10),12);
              strncpy(strMovimientos+19, menReso->INF_MOV+(PosINF_MOV+22),12);
              strncpy(strMovimientos+31, menReso->INF_MOV+(PosINF_MOV+35),3);
              strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut048.movim.sub04806.movimi[i], strMovimientos,37);
              printf("movimiento <%d> : <%s>\n", i,strMovimientos);
      }

   }
   
	//PRJ0247167 - se recoge valor de la nueva variable
	strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4820,menReso->VALOR_P48_20,16);
	//PRJ0257005 - se recoge valor de las nuevas variables
	strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut048.comun01.fiiso4894,menReso->VALOR_P48_94,6);
	//strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut062.bit62_16,menReso->VALOR_P62_16,30); //PRJ0257005 - se quita
	strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut062.bit62_16+2,menReso->SIOAPL_TARJTOKEN,16); //PRJ0257005
	strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut062.bit62_16+21,menReso->SIOAPL_FECATTOKE,4); //PRJ0257005

   //NOMEST
   strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut043, menReso->NOMEST, 40);
   //PRJ0247167
   /*
   #if defined (TRAZA) && (TRAZA > 10)
	  fprintLog(nomProg,"fiaut043>%.40s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut043);
   #endif
		*/
   //MODAUT_TERM (PROB: MODAUT_TERM=4B, bit22=12B)
   strncpy(mensaje_netplus->fidatiso.fiisoaut.fiaut022, menReso->MODAUT_TERM, 4);
   //PRJ0247167
   /*
   #if defined (TRAZA) && (TRAZA > 10)
	  fprintLog(nomProg,"fiaut022>%.4s<\n", mensaje_netplus->fidatiso.fiisoaut.fiaut022);
   #endif
	*/

	//PRJ0247167 - Pintamos datos de recepcion a MEPA
	fprintLog(nomProg,"- - - - - - - Antes de pintar todas las trazas de recepcion de MEPA\n");   

	fprintLog(nomProg," * * * * * * Datos recibidos de Tuxedo (ftrad_desdeReso en ftrad03.c) * * * * * * *\n");
	fprintLog(nomProg,"IDE_CD       >%.16s<\n",menReso->IDE_CD);
	fprintLog(nomProg,"IDE_CD_ORI   >%.16s<\n",menReso->IDE_CD_ORI);
	fprintLog(nomProg,"CAB_CTRL_IOO >%.6s<\n",menReso->CAB_CTRL_IOO);
	fprintLog(nomProg,"CAB_CTRL_IOD >%.6s<\n",menReso->CAB_CTRL_IOD);
	fprintLog(nomProg,"CAB_CTRL_IAP >%.4s<\n",menReso->CAB_CTRL_IAP);  
	fprintLog(nomProg,"CAB_CTRL_IOP >%.6s<\n",menReso->CAB_CTRL_IOP);
	fprintLog(nomProg,"CAB_CTRL_NB  >%.2s<\n",menReso->CAB_CTRL_NB);
	fprintLog(nomProg,"CAUR         >%.2s<\n",menReso->CAUR);
	fprintLog(nomProg,"BIN1         >%.8s<\n",menReso->BIN1);
	fprintLog(nomProg,"BIN2         >%.8s<\n",menReso->BIN2);
	fprintLog(nomProg,"BIN3         >%.8s<\n",menReso->BIN3);
	fprintLog(nomProg," <-- <-- <-- <--            IDTM         >%.4s<\n",menReso->IDTM);
	fprintLog(nomProg,"MBIT_P       >%.64s<\n",menReso->MBIT_P);
	fprintLog(nomProg,"PAN_L        >%.2s<\n",menReso->PAN_L);
	fprintLog(nomProg,"PAN_D        >%.18s<\n",menReso->PAN_D);
	fprintLog(nomProg,"CODPROC      >%.6s<\n",menReso->CODPROC);
	fprintLog(nomProg,"IMPORTX      >%.8s<\n",menReso->IMPORTX);
	fprintLog(nomProg,"IDEN_USR_L   >%.2s<\n",menReso->IDEN_USR_L);
	fprintLog(nomProg,"IDEN_USR_TIPO>%.2s<\n",menReso->IDEN_USR_TIPO);
	fprintLog(nomProg,"IDEN_USR_DATO>%.30s<\n",menReso->IDEN_USR_DATO);
	fprintLog(nomProg,"IMPOROP      >%.12s<\n",menReso->IMPOROP);
	fprintLog(nomProg," <-- <-- <-- <--            IDTR         >%.6s<\n",menReso->IDTR);
	fprintLog(nomProg,"FECHAL       >%.6s<\n",menReso->FECHAL);
	fprintLog(nomProg,"CMONTI       >%.4s<\n",menReso->CMONTI);
	fprintLog(nomProg,"HORAL        >%.6s<\n",menReso->HORAL);
	fprintLog(nomProg,"FECCAD       >%.4s<\n",menReso->FECCAD);
	fprintLog(nomProg,"ACCION_TARJ  >%.2s<\n",menReso->ACCION_TARJ);
	fprintLog(nomProg,"DAT_DEP_EFE  >%.4s<\n",menReso->DAT_DEP_EFE);
	fprintLog(nomProg,"COMINF_TIPO  >%.2s<\n",menReso->COMINF_TIPO);
	fprintLog(nomProg,"COMINF_IMPO  >%.6s<\n",menReso->COMINF_IMPO);
	fprintLog(nomProg,"SECTOR       >%.4s<\n",menReso->SECTOR);
	fprintLog(nomProg,"ID_REDTERM   >%.12s<\n",menReso->ID_REDTERM);
	fprintLog(nomProg,"CODRAZ_ANUL  >%.2s<\n",menReso->CODRAZ_ANUL);
	fprintLog(nomProg,"NUM_MIEMB    >%.1s<\n",menReso->NUM_MIEMB);
	fprintLog(nomProg,"ICARGOS      >%.12s<\n",menReso->ICARGOS);
	fprintLog(nomProg,"NCARGOS      >%.10s<\n",menReso->NCARGOS);
	fprintLog(nomProg,"IABONOS      >%.12s<\n",menReso->IABONOS);
	fprintLog(nomProg,"NABONOS      >%.10s<\n",menReso->NABONOS);
	fprintLog(nomProg,"I_ANUL_CAR   >%.12s<\n",menReso->I_ANUL_CAR);
	fprintLog(nomProg,"NUM_ANUL_CAR >%.10s<\n",menReso->NUM_ANUL_CAR);
	fprintLog(nomProg,"I_ANUL_ABO   >%.12s<\n",menReso->I_ANUL_ABO);
	fprintLog(nomProg,"NUM_ANUL_ABO >%.10s<\n",menReso->NUM_ANUL_ABO);
	fprintLog(nomProg,"DAT_ABO_COM  >%.36s<\n",menReso->DAT_ABO_COM);
	fprintLog(nomProg,"INF_ADIC_PET_TRANSF >%.32s<\n",menReso->INF_ADIC_PET_TRANSF);
	fprintLog(nomProg," <-- <-- <-- <--            CODRES       >%.2s<\n",menReso->CODRES);
	fprintLog(nomProg,"IDEN_BLOQUE_MOV >%.12s<\n",menReso->IDEN_BLOQUE_MOV);
	fprintLog(nomProg,"ADIC_RESP_TRANSFER >%.32s<\n",menReso->ADIC_RESP_TRANSFER);
	fprintLog(nomProg,"DATORI       >%.30s<\n",menReso->DATORI);
	fprintLog(nomProg,"NUMAUT       >%.6s<\n",menReso->NUMAUT);
	fprintLog(nomProg,"INF_SALDO    >%.110s<\n",menReso->INF_SALDO);
	fprintLog(nomProg,"INF_MOV      >%.222s<\n",menReso->INF_MOV);
	fprintLog(nomProg,"NUM_REF      >%.8s<\n",menReso->NUM_REF);
	fprintLog(nomProg,"REF_TPV      >%.16s<\n",menReso->REF_TPV);
	fprintLog(nomProg,"MON_CTBLE    >%.4s<\n",menReso->MON_CTBLE);
	fprintLog(nomProg,"TOTAL_TELEBANCO >%.218s<\n",menReso->TOTAL_TELEBANCO);
	fprintLog(nomProg,"IMP_TERMINAL >%.14s<\n",menReso->IMP_TERMINAL);
	fprintLog(nomProg,"DAT_ADIC_ENV_TRANSP >%.404s<\n",menReso->DAT_ADIC_ENV_TRANSP);
	fprintLog(nomProg,"NOMEST       >%.40s<\n",menReso->NOMEST); 
	fprintLog(nomProg,"DAT_ADIC_REC_TRANSP >%.404s<\n",menReso->DAT_ADIC_REC_TRANSP);
	fprintLog(nomProg,"MODAUT_TERM  >%.4s<\n",menReso->MODAUT_TERM);
	fprintLog(nomProg,"MAC          >%.6s<\n",menReso->MAC); 
	fprintLog(nomProg,"CLAVE_MAC    >%.2s<\n",menReso->CLAVE_MAC); 
	fprintLog(nomProg,"TOT_TRASPASOS      >%.12s<\n",menReso->TOT_TRASPASOS);
	fprintLog(nomProg,"NUM_TRANSPASOS     >%.10s<\n",menReso->NUM_TRANSPASOS);
	fprintLog(nomProg,"TOT_ANUL_TRASPASOS >%.12s<\n",menReso->TOT_ANUL_TRASPASOS);
	fprintLog(nomProg,"NUM_ANUL_TRASPASOS >%.10s<\n",menReso->NUM_ANUL_TRASPASOS);
	fprintLog(nomProg,"BIT_P_58     >%.94s<\n",menReso->BIT_P_58); 
	fprintLog(nomProg,"BIT_P_59     >%.186s<\n",menReso->BIT_P_59); 
	fprintLog(nomProg,"FEC_REF_TOTALES >%.6s<\n",menReso->FEC_REF_TOTALES); 
	fprintLog(nomProg,"IND_CUADRE_TOT  >%.2s<\n",menReso->IND_CUADRE_TOT); 
	fprintLog(nomProg,"STATUS_TELEBANCO>%.20s<\n",menReso->STATUS_TELEBANCO); 
	fprintLog(nomProg,"INF_OP_OT_REDES >%.40s<\n",menReso->INF_OP_OT_REDES); 
	fprintLog(nomProg,"PUNT_SERV    >%.12s<\n",menReso->PUNT_SERV); 
	fprintLog(nomProg,"EMISOR       >%.2s<\n",menReso->EMISOR); 
	fprintLog(nomProg,"CTA_EMISOR   >%.18s<\n",menReso->CTA_EMISOR); 
	fprintLog(nomProg,"COD_FUN      >%.3s<\n",menReso->COD_FUN); 
	fprintLog(nomProg,"COD_ACTI     >%.4s<\n",menReso->COD_ACTI); 
	fprintLog(nomProg,"FEC_SESS     >%.8s<\n",menReso->FEC_SESS); 
	fprintLog(nomProg,"REF_ADQ      >%.23s<\n",menReso->REF_ADQ); 
	fprintLog(nomProg,"IDE_ADQ      >%.11s<\n",menReso->IDE_ADQ); 
	fprintLog(nomProg,"IM_CUO       >%.44s<\n",menReso->IM_CUO); 
	fprintLog(nomProg,"INF_CTRL_SEG >%.16s<\n",menReso->INF_CTRL_SEG); 
	fprintLog(nomProg,"IDENT_AUT    >%.11s<\n",menReso->IDENT_AUT); 
	fprintLog(nomProg,"IND_SIO_PRI  >%.1s<\n",menReso->IND_SIO_PRI); 
	fprintLog(nomProg,"IM_CUO_ABO   >%.12s<\n",menReso->IM_CUO_ABO);
	fprintLog(nomProg,"IM_CUO_CAR   >%.12s<\n",menReso->IM_CUO_CAR);
	fprintLog(nomProg,"CONCILI      >%.1s<\n",menReso->CONCILI);
	fprintLog(nomProg," <-- <-- <-- <--            NUMOPER_SISMA>%.6s<\n",menReso->NUMOPER_SISMA);
	fprintLog(nomProg,"VALOR_P48_20 >%.16s<\n",menReso->VALOR_P48_20);
	fprintLog(nomProg,"BIT_P_51 000 a 060 >%.60s<\n", menReso->BIT_P_51);
	fprintLog(nomProg,"BIT_P_51 061 a 120 >%.60s<\n", menReso->BIT_P_51 + 61);
	fprintLog(nomProg,"BIT_P_51 121 a 180 >%.60s<\n", menReso->BIT_P_51 + 121);
	fprintLog(nomProg,"BIT_P_51 181 a 240 >%.60s<\n", menReso->BIT_P_51 + 181);
	fprintLog(nomProg,"BIT_P_51 241 a 300 >%.60s<\n", menReso->BIT_P_51 + 241);
	fprintLog(nomProg,"BIT_P_51 301 a 360 >%.60s<\n", menReso->BIT_P_51 + 301);
	fprintLog(nomProg,"BIT_P_51 361 a 420 >%.60s<\n", menReso->BIT_P_51 + 361);
	fprintLog(nomProg,"BIT_P_51 421 a 480 >%.60s<\n", menReso->BIT_P_51 + 421);
	fprintLog(nomProg,"VALOR_P48_94 >%.6s<\n",menReso->VALOR_P48_94); 	//PRJ0257005
	//fprintLog(nomProg,"VALOR_P62_16 >%.30s<\n",menReso->VALOR_P62_16); 	//PRJ0257005
	fprintLog(nomProg,"SIOAPL_TARJTOKEN >%.16s<\n",menReso->SIOAPL_TARJTOKEN); 	//PRJ0257005
	fprintLog(nomProg,"SIOAPL_FECATTOKE >%.4s<\n",menReso->SIOAPL_FECATTOKE); 	//PRJ0257005
	fprintLog(nomProg,"SIOAPL_TIPMOV     >%.1s<\n" ,menReso->SIOAPL_TIPMOV); 	//BIZUM Valor fijo B si es Bizum - T para el resto de operativa
	fprintLog(nomProg,"SIOAPL_CODIGO_RES >%.3s<\n" ,menReso->SIOAPL_CODIGO_RES); 	//BIZUM Respuesta a la operativa Bizum
	fprintLog(nomProg,"SIOAPL_IBAN_ORDE  >%.34s<\n",menReso->SIOAPL_IBAN_ORDE); 	//BIZUM Iban cuenta ordenante
	fprintLog(nomProg,"SIOAPL_IBAN_BENE  >%.34s<\n",menReso->SIOAPL_IBAN_BENE); 	//BIZUM Iban cuenta Beneficiario
	fprintLog(nomProg,"SIOAPL_OFFSET  >%.4s<\n",menReso->SIOAPL_OFFSET); 	//PRJ0258542
	fprintLog(nomProg,"COD_RAZ >%.4s<\n",menReso->SIOAPL_CODRAZ); //PRJ0350720
	fprintLog(nomProg,"- - - - - Despues de pintar todas las trazas de recepcion de MEPA\n");   
	return(0);
}

        
 /**********************************************/
/***   funcion: carga tabla de TRMS          ***/
/**********************************************/
int fcarga_tdattrms(char * red)
{
    char clave[13];
    int ll;
    int cod_err2;

    for (ll=0 ; ll<500 ; ll++)
    {  
	    memset(tabla_trms[ll].trmsemis, ' ' , 2);
	    memset(tabla_trms[ll].trmsidtm, ' ' , 4);
	    memset(tabla_trms[ll].trmscopr, ' ' , 6);
	    memset(tabla_trms[ll].trmsidtv, ' ' , 4);
	    memset(tabla_trms[ll].trmsidsi, ' ' , 4);
	    memset(tabla_trms[ll].trmscosi, ' ' , 6);
	    memset(tabla_trms[ll].trmsoper, ' ' , 1);
    }              

    /***---------- lee el primero con red  -----------***/

    strncpy(clave     , red   , 2);
    clave[2] = '\0';

    cod_err2=tratar_fich("NPTTRMS",CARGAR,&tabla_trms, clave);
    if (cod_err2)
    {
#if defined (TRAZA) && (TRAZA > 1)				
        fprintf(stdout,"Error al cargar NPTTRMS %03d\n", cod_err2);
#endif		
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Leer NPTTRMS codigo de error >%03d< \n",cod_err2);

         return(-1);
    }
     return(0);
}

//PTASK000023003 - INI
int fbusca_bin(tbine *pbine, char *pbinope, char *prangoope, int plongrango, char *ptboper, char *pparau1, char *pparau2,char *prespri, char *codent)
{
  char auxbin[MAXBINE+1];
  char auxrng[17];
  tbine *pp;
  int numaster; /* numero de asteriscos */

 	pp = pbine;

 	strncpy(auxbin , pbinope , MAXBINE);
 	auxbin[MAXBINE] = '\0';

 	strncpy(auxrng, prangoope, plongrango);
 	auxrng[plongrango] ='\0';

 	fprintLog(nomProg,"CLAVE bin = %.*s \n", MAXBINE, pbinope);

	while (pp != NULL)
	{
      if (!strncmp(auxbin, pp->t_biclave0, MAXBINE)
      && strncmp(auxrng, pp->t_birnginf, plongrango) > 0
      && strncmp(auxrng, pp->t_birngsup, plongrango) < 0)
      {
         fprintLog(nomProg, "bin encontrado\n"); 
         strncpy (ptboper, pp->t_bitboper,sizeof(pp->t_bitboper));
         strncpy (prespri, pp->t_birespri,sizeof(pp->t_birespri));
         strncpy (codent,  pp->t_bicodent,sizeof(pp->t_bicodent));
         strncpy (pparau1, pp->t_biparau1,sizeof(pp->t_biparau1)); //PTASK000023003
         return (0);
      }
    pp = pp->bin_siguiente;
	}
	fprintLog(nomProg,"Tarjeta ajena\n");
	return (1);
}
#include "fcarbine.c"
//PTASK000023003 - FIN       
