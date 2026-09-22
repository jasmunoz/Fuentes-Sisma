//PRJ0257005 - Revisado tokenizacion
//PRJ0247167 - Revisado ampliación P48
//BIZUM      - Revisado subcampos P48
/*************************************************************************
FUNCION  ftradred

DESCRIPCION: Funcion que traduce mensajes de formato PRICE a formato
interno NETPLUS, y viceversa.

AREA DE PASO :
- caracter con el uso del traductor:
usotrad ---> '0' No usamos empaquetados  (tipo mensaje y longitudes) (recepcion y envio)
usotrad ---> '1' usamos empaquetados (recepcion y envio)
usotrad ---> '2' No usamos empaquetados (simulacion price) (tipo mensaje y longitudes)
usotrad ---> '3' usamos empaquetados (simulacion price)
- entero con el sentido de traduccion    1.- price--->netplus
2.- netplus--->price
- entero con la subaplicacion del mensaje
- puntero a entero con la longitud del mensaje formato price
- puntero a char con el mensaje en formato price
- puntero a formato_interno con el mensaje en formato netplus
- puntero a entero con codigo de error de la traduccion 
- puntero a entero con el numero de bit que ha dado el error
*************************************************************************/
#include "traeetiq.h"

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/opensslv.h>
#include "svm.h"
#include "nptpars.h"
#include "nptpara.h"

//typedef struct sfdssl {         /* estructura conjunta para socket y socket seguro */
// int sfd_ssl;                   
// SSL * ssl_ssl;                               
//} sfdssl;

NPSPARA  *reg_para;
char *dtoa(double);

//#include "pcalcmac.c"
//#include "fclasmsg.c"

char cadena_salida[2000];
int bittraza = 2; // REVISAR ***********************************************
unsigned char  caraprin;
int empaquetado;
char char04[4];
petiquetas salida;
/**************/
char  area_mac[1024];

char  *itoa(int numero); //PRJ0258542
/***************/
/**************************/
char area_machsm[1024];
/*******************************/

/***  --------- prototipos de funciones locales  ----------------  ***/

void    montaR55(formato_interno *pfi, char *p55,int *L_p55);
int     fexplbit(int opciex,char *pmapa_exp, char *pmapa_com);
int fverfi(formato_interno fi);


int     fcheqbit(int sentido,char *plantilla,
                   char *pbitmapp,char *pbitmaps,
                   formato_intermedio *mensaje_inter,
                   int *error_code,int *error_bit,int suba_iso, char * red);

int     ftrapre1(formato_interno *mensaje_netplus, formato_intermedio *mensaje_inter,char *parea_mac, char * bit48); //PRJ0258542 - Fase 3 - incluye bit48 en hexadecimal
int     ftrapre2(formato_interno *mensaje_netplus, formato_intermedio *mensaje_inter);
int     fvalcont(int sentido);
int     ftraefe1(int suba_iso,int tipomsg, char *pbitmapp,char *pbitmaps,char *plantilla, formato_intermedio *mensaje_inter,char *mensaje_price, int *error_code,int *error_bit,char *parea_mac, char * bit48); //PRJ0258542 - Fase 3 - incluye bit48 en hexadecimal

int     fdesempa_mixto(int ll,int longi,int *longimix, char *sempaq,char *sdesem);
int     ftraefe2(int suba_iso,int tipomsg,
                   char *pbitmapp,char *pbitmaps,char *plantilla,
                   formato_intermedio *mensaje_inter,char *mensaje_price,
                   int *error_code,int *error_bit, char *idmac, char subsistema_general,formato_interno *mensaje_netplus); //PRJ0258542 - se agrega formato_interno
int convierte_hex (char * vaddr, int len, char * cadsal); //PRJ0258542 - para el paso a hexadecimal de la cadena de bloque de PIN

int     fempaque_mixto(int ll,int longi,int *longimix, char *sempaq,char *sdesem);
int     bin2hex(char * pcadena, int numerico, int * longi);
int     fempaque(int longi,char *sempaq,char *sdesem);
int     fdesempa(int longi,char *sempaq,char *sdesem);
int     flee_datotbm(char tipomsg_norep[5], int sentimen,char *plantilla);
int     flee_datoapl(int ll);
int     flee_datoiso(int ll);
int     nzeroes (char *c,int n);
int     nspaces (char *c,int n);
int     ncampo_numerico (char *c,int n);
int     inicfori (int tipomsg, formato_interno *pfi);
int     finicializa_intermedio(formato_intermedio *pmt);
void    lpad(char *bufer,char c,int lon);
void    rpad(char *bufer,char c,int lon);
void    pone_saldos(char * psaldo, iso48saldos * saldos);
void    pone_movimientos(char * pmovim, iso48sb06 * movimientos);
int     fempapin(int longi,char *sempaq,char *sdesem);
int     fcifraPan(char *PanClaro, char *PanCifrado);

NPSPARA  *reg_para; //PRJ0258542
char pscerrado[1]; //PRJ0258542 - Fase 3 - Cambio 11052026
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


int     ftradred(char usotrad,int sentido,int suba_iso,int *long_iso,char *mensaje_price,formato_interno *mensaje_netplus,int *error_code,int *error_bit)
{

  int retorno, sentimen;
	char cadena48[2000];
  char tipomsg_alf[5];
  char tipomsg_norep[5];
  char idmac[6];
  struct sfdssl * sockssl;
  *error_code = 0; //PRJ0258542
  *error_bit = 0; //PRJ0258542
  
  //PRJ0258542 - Fase 3 - INI
  /*
	fprintLog(nomProg,"Entra en ftradred con >%.4s< >%.3s< \n", mensaje_netplus->fiisocom.fiiso000, mensaje_netplus->fiisocom.fiiso024);
	if (strncmp(mensaje_netplus->fiisocom.fiiso024, "811", 3) == 0 || strncmp(mensaje_netplus->fiisocom.fiiso024, "815", 3) == 0 || strncmp(mensaje_netplus->fiisocom.fiiso024, "816", 3) == 0)
	{
		fprintLog(nomProg,"  Valor del P11       >%.6s< \n", mensaje_netplus->fidatiso.fiisocla.ficla011);
		fprintLog(nomProg,"  Valor del P12       >%.12s< \n", mensaje_netplus->fidatiso.fiisocla.ficla012);
		fprintLog(nomProg,"  Valor del P24       >%.3s< \n", mensaje_netplus->fidatiso.fiisocla.ficla024);
		fprintLog(nomProg,"  Valor del P25       >%.4s< \n", mensaje_netplus->fidatiso.fiisocla.ficla025);
		fprintLog(nomProg,"  Valor del P33       >%.2s< >%.11s< \n", mensaje_netplus->fidatiso.fiisocla.ficla033.longitud, mensaje_netplus->fidatiso.fiisocla.ficla033.datos);
		fprintLog(nomProg,"  Valor del P39       >%.3s< \n", mensaje_netplus->fidatiso.fiisocla.ficla039);
		fprintLog(nomProg,"  Valor del P53       >%.2s%.8s%.2s%.4s%.2s%.6s< \n", mensaje_netplus->fidatiso.fiisocla.ficla053.control, mensaje_netplus->fidatiso.fiisocla.ficla053.identificador, mensaje_netplus->fidatiso.fiisocla.ficla053.algoritmo, 
																																							mensaje_netplus->fidatiso.fiisocla.ficla053.longitud, mensaje_netplus->fidatiso.fiisocla.ficla053.proteccion, mensaje_netplus->fidatiso.fiisocla.ficla053.resto);
		fprintLog(nomProg,"  Valor del S93       >%.2s< >%.11s< \n", mensaje_netplus->fidatiso.fiisocla.ficla093.longitud, mensaje_netplus->fidatiso.fiisocla.ficla093.datos);
		fprintLog(nomProg,"  Valor del S94       >%.2s< >%.11s< \n", mensaje_netplus->fidatiso.fiisocla.ficla094.longitud, mensaje_netplus->fidatiso.fiisocla.ficla094.datos);
		fprintLog(nomProg,"  Valor del S96       >%.3s< >%.360s< \n", mensaje_netplus->fidatiso.fiisocla.ficla096.longitud, mensaje_netplus->fidatiso.fiisocla.ficla096.datos);
		fprintLog(nomProg,"  Valor del S128      >%.8s< \n", mensaje_netplus->fidatiso.fiisocla.ficlamac);
		fprintLog(nomProg,"  Valor de ficlatab   >%.12s<\n", mensaje_netplus->fidatpro.ficlatab);
	}
	*/ //Quito las trazas
  //PRJ0258542 - Fase 3 - FIN
  //if(bittraza)
  //  fprintf(stdout,"------ BIT ----> %d  \n",bittraza);
/***  ---------      validaciones previas      ----------------  ***/

  if ((sentido != 1) && (sentido != 2))
  {
    *error_code = 24;
    *error_bit = 0;
#if defined (TRAZA) && (TRAZA > 1)	
    fprintf(stdout,"Error en sentido de traducción ->%d<-\n", sentido);
#endif	
    return(-1); // sale con error
  }
  *error_code = 0; //PRJ0258542
  *error_bit = 0; //PRJ0258542

  if((sentido == 1) && (*long_iso == 0))
  {
    *error_code = 27;
    *error_bit = 0;
#if defined (TRAZA) && (TRAZA > 1)	
    fprintf(stdout,"No viene longitud del mensaje sentido ->%d<- longitud ->%d<-\n", sentido, *long_iso);
#endif	
    return(-1); // sale con error
  }
  *error_code = 0; //PRJ0258542
  *error_bit = 0; //PRJ0258542

  memset(bitmapp , ' ' , 64);
  memset(bitmaps , ' ' , 64);
  memset(area_mac , '\0' , 1024);

// usotrad ---> '0' ó '2' No usamos empaquetados (tipo mensaje y longitudes)
  switch(usotrad)
  {
  case '0':
  case '2':
    empaquetado = 0;
    break;
  case '1':
  case '3':
    empaquetado = 1;
    break;
  default:
#if defined (TRAZA) && (TRAZA > 10)  
    fprintf(stdout,"usotrad : %c \n",usotrad);
    fflush(stdout);	
#endif	
    empaquetado = 1;
  }

  if (sentido == 1)
  {
#if defined (TRAZA) && (TRAZA > 10)  
    fprintf(stdout,"Sentido de traduccion: %d \n",sentido);
    fflush(stdout);
#endif	
    if(empaquetado)
    {
        retorno = fdesempa(4 ,mensaje_price, tipomsg_alf);
        memcpy(mapa_com , mensaje_price + 2, 8);
    }
    else
    {
        strncpy(tipomsg_alf, mensaje_price, 4);
        memcpy(mapa_com , mensaje_price + 4 , 8);
    }
    /***     desempaqueta el tipo de mensaje    ***/
    tipomsg_alf[4] = '\0';
    mapa_com[8] = '\0';
    tipomsg = atoi(tipomsg_alf);
#if defined (TRAZA) && (TRAZA > 5)
    fprintf(stdout,"tipomsg : %d  tipomsg_alf : %s\n",tipomsg, tipomsg_alf);
    fflush(stdout);
#endif	

    /***     explosiona mapa de bits recibido de semp    ***/
    memset(mapa_exp , ' ' , 64);
    mapa_exp[64] = '\0';

    if (fexplbit(sentido,mapa_exp,mapa_com) != 0)
    {
        *error_code = 28;
        *error_bit = 0;
#if defined (TRAZA) && (TRAZA > 1)		
        fprintf(stdout,"Error en fexplbit\n");
#endif		
        return(-1); // sale con error
    }
	  *error_code = 0; //PRJ0258542
	  *error_bit = 0; //PRJ0258542
    strncpy(bitmapp,mapa_exp,64);

    if (bitmapp[0] == '1')
    {
        if(empaquetado)
            memcpy(mapa_com , mensaje_price + 10 , 8);
        else
            memcpy(mapa_com , mensaje_price + 12 , 8);

        mapa_com[8] = '\0';
        memset(mapa_exp , ' ' , 64);
        mapa_exp[64] = '\0';

        if (fexplbit(sentido,mapa_exp,mapa_com) != 0)
        {
            *error_code = 28;
            *error_bit = 0;
#if defined (TRAZA) && (TRAZA > 1)				
            fprintf(stdout,"Error en fexplbit\n");
#endif			
            return(-1); // sale con error
        }
			  *error_code = 0; //PRJ0258542
			  *error_bit = 0; //PRJ0258542
        strncpy(bitmaps,mapa_exp,64);
    }
  } //Sentido = 1
  else       /***    sentido  2   ***/
  {     /***------ rellena el formato intermedio a partir del interno ------***/
#if defined (TRAZA) && (TRAZA > 10)	  
    fprintf(stdout,"Sentido de traduccion: %d \n",sentido);
    fflush(stdout);
#endif
  	fprintf(stdout," ***** Antes de llamar al fverfi ***** \n"); //PRJ0258542
#if defined (TRAZA) && (TRAZA > 5)	  	
    fverfi(*mensaje_netplus);
    fflush(stdout);
#endif	
  	fprintf(stdout," ***** Antes de llamar al ftrapre2 ***** \n"); //PRJ0258542
  	//fprintf(stdout,"Valores mensaje_netplus que va a ser pfi 00, 11, 12, 24, 25, 33, 39, 53, 93, 94, 128: >%.4s<>%.6s<>%.12s<>%.3s<>%.4s<>%.2s<>%.11s<>%.3s<>%.2s<>%.8s<>%.2s<>%.4s<>%.2s<>%.6s<>%.2s<>%.11s<>%.2s<>%.11s<>%.8s<\n",
  	//								mensaje_netplus->fiisocom.fiiso000,mensaje_netplus->fidatiso.fiisocla.ficla011,mensaje_netplus->fidatiso.fiisocla.ficla012,mensaje_netplus->fidatiso.fiisocla.ficla024,mensaje_netplus->fidatiso.fiisocla.ficla025,
  	//								mensaje_netplus->fidatiso.fiisocla.ficla033.longitud,mensaje_netplus->fidatiso.fiisocla.ficla033.datos,mensaje_netplus->fidatiso.fiisocla.ficla039,
  	//								mensaje_netplus->fidatiso.fiisocla.ficla053.control, mensaje_netplus->fidatiso.fiisocla.ficla053.identificador, mensaje_netplus->fidatiso.fiisocla.ficla053.algoritmo, 
		//								mensaje_netplus->fidatiso.fiisocla.ficla053.longitud, mensaje_netplus->fidatiso.fiisocla.ficla053.proteccion, mensaje_netplus->fidatiso.fiisocla.ficla053.resto,
  	//								mensaje_netplus->fidatiso.fiisocla.ficla093.longitud,mensaje_netplus->fidatiso.fiisocla.ficla093.datos,mensaje_netplus->fidatiso.fiisocla.ficla094.longitud,mensaje_netplus->fidatiso.fiisocla.ficla094.datos,
  	//								mensaje_netplus->fidatiso.fiisocla.ficlamac);

    if (ftrapre2(mensaje_netplus,&mensaje_inter) != 0)
    {
        *error_code = 29;
        *error_bit = 0;
#if defined (TRAZA) && (TRAZA > 1)	  			
        fprintf(stdout,"Error en ftrapre2\n");
		fflush(stdout);
#endif		
        return(-1); // sale con error
    }
  	*error_code = 0; //PRJ0258542
  	*error_bit = 0; //PRJ0258542

    strncpy(tipomsg_alf,mensaje_inter.adipre000,4);
    tipomsg_alf[4] = '\0';
    tipomsg = atoi(tipomsg_alf);

  }

  /*** para leer la tabla del bitmaps, eliminamos los codigos de repeticion ***/
  strcpy(tipomsg_norep , tipomsg_alf);
  if (tipomsg_norep[3] == '1')
    tipomsg_norep[3] = '0';

  if (tipomsg_norep[3] == '5')
    tipomsg_norep[3] = '4';
/*
    usotrad ---> '0' No usamos empaquetados  (tipo mensaje y longitudes)
    usotrad ---> '1' usamos empaquetados
    usotrad ---> '2' No usamos empaquetados simulando price  (tipo mensaje y longitudes)
    usotrad ---> '3' usamos empaquetados simulando price
*/
  if (usotrad == '0' || usotrad == '1')
    sentimen = sentido;
  else
  {
    if (sentido == 1)
        sentimen = 2;
    else
        sentimen = 1;
  }
/***------ lee la plantilla de condiciones del mensaje en las tablas ------***/
  retorno = flee_datotbm(tipomsg_norep,sentimen,plantilla);
  if (retorno != 0)
  {
    if (retorno == 2)
    {   /***  ------  el mensaje no esta permitido en ese sentido  ------  ***/
        *error_code = 26;
        *error_bit = 0;
#if defined (TRAZA) && (TRAZA > 1)	  			
        fprintf(stdout,"mensaje no permitido en este sentido ->%d<-\n", sentido);
        fflush(stdout);
#endif		
        return(-1); // sale con error
    }
    else
    {   /***  ------  el mensaje no esta en la tabla ------  ***/
        *error_code = 25;
        *error_bit = 0;
#if defined (TRAZA) && (TRAZA > 1)		
        fprintf(stdout,"mensaje no tiene mapa de bits\n");
        fflush(stdout);
#endif		
        return(-1); // sale con error
    }
  }
  *error_code = 0; //PRJ0258542
  *error_bit = 0; //PRJ0258542


/***------------ construccion y chequeo del mapa de bits --------------***/
  if(fcheqbit(sentido,plantilla,bitmapp, bitmaps, &mensaje_inter, error_code,error_bit,suba_iso,subsistema_general))
  {
#if defined (TRAZA) && (TRAZA > 1)  
    fprintf(stdout,"ERROR en fcheqbit: code=%d bit=%d\n",*error_code,*error_bit);
    fflush(stdout);
#endif	
    return(-1); // sale con error(0);
  }
  *error_code = 0; //PRJ0258542
  *error_bit = 0; //PRJ0258542

  if (sentido == 1)
  {
  	fprintf(stdout," - - - - Va a entrar en ftraefe1 con sentido: %d\n", sentido);
  
    if(ftraefe1(suba_iso,tipomsg,bitmapp,bitmaps,plantilla,&mensaje_inter,mensaje_price,error_code,error_bit,area_mac,cadena48))
    {
        return(-1); // sale con error(0);
    }
    //fprintf(stdout,"Control de salida ftraefe1\n");
	  *error_code = 0; //PRJ0258542
	  *error_bit = 0; //PRJ0258542
    
  }
  else
  {
       
    strncpy(idmac,(char *)mensaje_netplus->fidatpro.fimacarq.idmac,5);
  	fprintf(stdout," ***** Antes de llamar al ftraefe2 ***** \n"); //PRJ0258542
  	//fprintf(stdout,"Valores mensaje_netplus 00, 11, 12, 24, 25, 33, 39, 53, 93, 94, 128: >%.4s<>%.6s<>%.12s<>%.3s<>%.4s<>%.2s<>%.11s<>%.3s<>%.2s<>%.8s<>%.2s<>%.4s<>%.2s<>%.6s<>%.2s<>%.11s<>%.2s<>%.11s<>%.8s<\n",
  	//								mensaje_netplus->fiisocom.fiiso000,mensaje_netplus->fidatiso.fiisocla.ficla011,mensaje_netplus->fidatiso.fiisocla.ficla012,mensaje_netplus->fidatiso.fiisocla.ficla024,mensaje_netplus->fidatiso.fiisocla.ficla025,
  	//								mensaje_netplus->fidatiso.fiisocla.ficla033.longitud,mensaje_netplus->fidatiso.fiisocla.ficla033.datos,mensaje_netplus->fidatiso.fiisocla.ficla039,
  	//								mensaje_netplus->fidatiso.fiisocla.ficla053.control, mensaje_netplus->fidatiso.fiisocla.ficla053.identificador, mensaje_netplus->fidatiso.fiisocla.ficla053.algoritmo, 
		//								mensaje_netplus->fidatiso.fiisocla.ficla053.longitud, mensaje_netplus->fidatiso.fiisocla.ficla053.proteccion, mensaje_netplus->fidatiso.fiisocla.ficla053.resto,
  	//								mensaje_netplus->fidatiso.fiisocla.ficla093.longitud,mensaje_netplus->fidatiso.fiisocla.ficla093.datos,mensaje_netplus->fidatiso.fiisocla.ficla094.longitud,mensaje_netplus->fidatiso.fiisocla.ficla094.datos,
  	//								mensaje_netplus->fidatiso.fiisocla.ficlamac);

    if (ftraefe2(suba_iso,tipomsg, bitmapp,bitmaps,plantilla,&mensaje_inter,mensaje_price,error_code,error_bit, idmac,subsistema_general,mensaje_netplus)) //PRJ0258542 - se anade mensaje_netplus
    {
#if defined (TRAZA) && (TRAZA > 1)	
        fprintf(stdout,"ERROR en ftraefe2: code=%d bit=%d\n",*error_code,*error_bit);
        fflush(stdout);
#endif		
        return(-1); // sale con error(0);
    }
	  *error_code = 0; //PRJ0258542
	  *error_bit = 0; //PRJ0258542
  }

/***------ rellena el formato interno a partir del intermedio ------***/
  if (sentido == 1)
  {
		//fprintf(stdout,"Llamada a ftrapre1: mensaje_inter.adipre000 >%.4s< mensaje_inter.adipre003 >%.6s< mensaje_inter.adipre064 >%.8s< mensaje_inter.adipre128 >%.8s<\n", mensaje_inter.adipre000, mensaje_inter.adipre003, mensaje_inter.adipre064, mensaje_inter.adipre128); //Bizum
	
    if (ftrapre1(mensaje_netplus,&mensaje_inter,area_mac,cadena48))
    {
        *error_code = 31;
        *error_bit = 0;
#if defined (TRAZA) && (TRAZA > 1)		
        fprintf(stdout,"error en ftrapre1\n");
        fflush(stdout);
#endif		
        return(-1); // sale con error;
    }
	  *error_code = 0; //PRJ0258542
	  *error_bit = 0; //PRJ0258542
  }

  return (0); 
} //FIN de ftradred


/******************************************************************
lee de las tablas cargadas el formato de un dato iso
******************************************************************/
int flee_datoiso(int ll)
{
#if defined (TRAZA) && (TRAZA > 5)   		   
    //fprintf(stdout,"================ Saca el bit %d del mensaje ===============\n", ll); //PRJ0258542
    fflush(stdout);	
#endif	


    tipo_fs = tabla_iso[ll-1].tipo_fs;
    long_fs = tabla_iso[ll-1].long_fs;
    max_fs  = tabla_iso[ll-1].max_fs;

#if defined (TRAZA) && (TRAZA > 10)			
    fprintf(stdout,"-- flee_datoiso tipo_fs >%c< long_fs >%d< max_fs >%d< BIT %03d\n", tipo_fs, long_fs, max_fs,ll); //PRJ0258542
    fflush(stdout);
#endif	
    mac_fs  = tabla_iso[ll-1].mac_fs;

    if(!long_fs)
        return(-1);
    return (0);
}


/******************************************************************
lee de las tablas cargadas el formato de un dato aplicacion
******************************************************************/
int flee_datoapl(int ll)
{
    tipo_fc = tabla_apl[ll-1].tipo_fc;
    long_fc = tabla_apl[ll-1].long_fc;
    max_fc  = tabla_apl[ll-1].max_fc;
    off_fc  = tabla_apl[ll-1].off_fc;
#if defined (TRAZA) && (TRAZA > 10)	
    fprintf(stdout,"-- flee_datoapl tipo_fc >%c< long_fc >%d< max_fc >%d< off_fc >%d< BIT %03d\n",tipo_fc,long_fc,max_fc,off_fc,ll); //PRJ0258542
    fflush(stdout);
#endif
    if(!long_fc && !max_fc)
        return(-1);

    return (0);
}


/**************************************************************************
lee de las tablas cargadas el mensaje con su plantilla de condiciones
**************************************************************************/
int flee_datotbm(char tipomsg_norep[5],
                   int sentido,char *plantilla)
{
  int ll,salir;
  char sentialf;

#if defined (TRAZA) && (TRAZA > 10)		
 fprintf(stdout,"\n===== ftradred.c ===== flee_datotbm : %s , %d\n",tipomsg_norep,sentido);
#endif
 
  sentialf = ((sentido == 1) ? '1' : '2');

  for (ll=0,salir=0; ll< 100 && !salir; ll++)
  {

    if ((!strncmp(tabla_bim[ll].tipo_bm, tipomsg_norep, 4)) &&  (tabla_bim[ll].sent_bm == sentialf))
    {
        salir = 1;
     
        if (tabla_bim[ll].perv_bm == 'S')
        {
            strncpy(plantilla      ,tabla_bim[ll].bim1_bm,192);
            strncpy(plantilla + 192,tabla_bim[ll].bim2_bm,192);
            return(0);
        }
        else
            return(2);
    }
  }    /***   fin del bucle   ***/

  return (1);
}


/*****************************************************************
FUNCION  FTRAPRE1             llamada por ftradred

DESCRIPCION: Funcion que convierte el mensaje de una estructura
tipo formato_intermedio a otra tipo formato_interno

AREA DE PASO :
- puntero a formato_interno donde volvera el mensaje traducido
- puntero a formato_intermedio donde llega el mensaje a traducir

* devuelve los valores :
*****************************************************************/
int ftrapre1(formato_interno *pfi, formato_intermedio *pmt,  char *parea_mac, char * bit48) //PRJ0258542 - Fase 3 - incluye bit48 en hexadecimal
{

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

  int fcomunes1(formato_interno *pfi, formato_intermedio *pmt);
  int retorno,conta1,long_48,codinum,longinum,long_62; //PRJ0257005
  char cadena48[2000]; //PRJ0258542 - Fase 3
  char cad483216[16]; //PRJ0258542 - Fase 3
  char cadena4832[12] = "303031383332"; //PRJ0258542 - Fase 3
  int pos4832; //PRJ0258542 - Fase 3
  int cod4844; //Bizum - longitud del 48.44 - inicio cadena
  int lcod4844a; //Bizum - 48.44 - subcampo numerico
  int licod4844; //Bizum - longitud del 48.44 total
  int licod4844s; //Bizum - longitud del 48.44 de cada subcampo
  int conta2; //Bizum - acumulador longitud subcampo 48.44
  int conta3; //Bizum - acumulador de seguridad para no embuclar
  char cod4844a[2]; //Bizum - subcampo del 48.44
  char lcod4844[3]; //Bizum - longitud alfanumerica del subcampo 48.44 total
  char lcod4844s[2]; //Bizum - longitud alfanumerica de cada subcampo de 48.44
  char codialf[3];
  char longialf[4];
  char subcampo_48[999], subcampo_62[256]; //PRJ0257005 - campo para sacar en las trazas
  /** campos para p55 */
  int longinum_55,long_55;
  char codialf_55[4];
  char verif_cripto[50];
  int codinum_55;
  int nn;
  // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud de panClr ni panCfd
  char panClr[20];
  char panCfd[20]; 
  int lon_pan;
  int p4832 = 0; //PRJ0258542 - 0 = no viene, 1 = si viene
  char c4832_16[16]; //PRJ0258542
  char c4832_32[32]; //PRJ0258542
  int p52 = 0; //PRJ0258542 - 0 = no viene, 1 si viene
  char tPin[2]; //PRJ0258542 - tipo de PIN que llega - 00 = en claro, 01 = en DES, 02 = TDES, 03 = TDES, 05 = AES
	
  memset(verif_cripto, '\0', sizeof(verif_cripto));
	fprintf(stdout, "---------------- Entrada en rutina ftrapre1 ------------------- \n");

 // Inicializacion de variable
  codinum_55 = 0;
  memset(codialf_55, '\0', sizeof(codialf_55));

//DMND0036176 añadidas preautorizaciones

  if ((strncmp(pmt->adipre000,"11",2) == 0) ||
      (strncmp(pmt->adipre000,"12",2) == 0) ||
      (strncmp(pmt->adipre000,"14",2) == 0))
  {
  	
    if ((retorno = inicfori(1100,pfi)) != 0)
        return(1);

  	
    fcomunes1(pfi,pmt);

  // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud de finumPAN ni fipancif
 
    strncpy(pfi->fidatiso.fiisoaut.finumPAN.longitud,pmt->adi002l,2);
    pfi->fidatiso.fiisoaut.finumPAN.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.finumPAN.datos,pmt->adi002d,19);
    pfi->fidatiso.fiisoaut.finumPAN.datos[19] = '\0';

    lon_pan = atoi(pfi->fidatiso.fiisoaut.finumPAN.longitud);
    strncpy(panClr,pfi->fidatiso.fiisoaut.finumPAN.datos, lon_pan);
    panClr[lon_pan]='\0';
    strncpy(pfi->fidatpro.fipancif, panClr, lon_pan);

    strncpy(pfi->fidatiso.fiisoaut.fiaut003.dig12,pmt->adipre003     ,2);
    pfi->fidatiso.fiisoaut.fiaut003.dig12[2] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut003.dig34,pmt->adipre003 + 2 ,2);
    pfi->fidatiso.fiisoaut.fiaut003.dig34[2] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut003.dig56,pmt->adipre003 + 4 ,2);
    pfi->fidatiso.fiisoaut.fiaut003.dig56[2] = '\0';

    /*** importes   ***/

    strncpy(pfi->fidatiso.fiisoaut.fiaut004,pmt->adipre004, 12);
    pfi->fidatiso.fiisoaut.fiaut004[12] = '\0';

    strncpy(pfi->fidatiso.fiisoaut.fiaut005,pmt->adipre005, 12);
    pfi->fidatiso.fiisoaut.fiaut005[12] = '\0';
		
    strncpy(pfi->fidatiso.fiisoaut.fiaut006, pmt->adipre006,12);
    pfi->fidatiso.fiisoaut.fiaut006[12] = '\0';


    strncpy(pfi->fidatiso.fiisoaut.fiaut009,pmt->adipre009 ,8);
    pfi->fidatiso.fiisoaut.fiaut009[8] = '\0';
    
    strncpy(pfi->fidatiso.fiisoaut.fiaut010,pmt->adipre010 ,8);
    pfi->fidatiso.fiisoaut.fiaut010[8] = '\0';

    // fecha de caducidad, si no viene rellena hay que cogerla de la pista2 y si no de la pista1
    if (ncampo_numerico(pmt->adipre014,4) == 1 && strncmp(pmt->adipre014, "0000", 4) != 0)
    {
      strncpy(pfi->fidatiso.fiisoaut.fifeccad.aa,pmt->adipre014     ,2);
      pfi->fidatiso.fiisoaut.fifeccad.aa[2] = '\0';
      strncpy(pfi->fidatiso.fiisoaut.fifeccad.mm,pmt->adipre014 + 2 ,2);
      pfi->fidatiso.fiisoaut.fifeccad.mm[2] = '\0';
    }
    else
    {
      //pista 2 : pan, separador, fecha caducidad yymm
      strncpy(longialf , pmt->adi002l, 2); //longitud del pan
      longialf[2] = '\0';
	  if( ncampo_numerico(pmt->adi035d+atoi(longialf)+1, 4) == 1 
	  &&  strncmp(pmt->adi035d+atoi(longialf)+1, "0000", 4) != 0)
	  {
         strncpy(pfi->fidatiso.fiisoaut.fifeccad.aa,pmt->adi035d+atoi(longialf)+1, 2);
         pfi->fidatiso.fiisoaut.fifeccad.aa[2] = '\0';
         strncpy(pfi->fidatiso.fiisoaut.fifeccad.mm,pmt->adi035d+atoi(longialf)+1+2, 2);
         pfi->fidatiso.fiisoaut.fifeccad.mm[2] = '\0';		  
	  }
	  else
	  {
		  //pista 1: codigo de formato "B", pan, separador, nombre, separador, fecha caducidad (el nombre no siempre viaja)
		  //         con tarjetas de 16, si no viaja el nombre, longitud 50, feccad en posicion 20
		  //                             si    viaja el nombre, longitud 76, feccad en posicion 46		  
          if(strncmp(pmt->adi045l, "50", 2) == 0)
		  {
             strncpy(pfi->fidatiso.fiisoaut.fifeccad.aa, pmt->adi045d + 19, 2);
             pfi->fidatiso.fiisoaut.fifeccad.aa[2] = '\0';
             strncpy(pfi->fidatiso.fiisoaut.fifeccad.mm, pmt->adi045d + 19 + 2, 2);
             pfi->fidatiso.fiisoaut.fifeccad.mm[2] = '\0';		  
		  }
          if(strncmp(pmt->adi045l, "76", 2) == 0)
		  {
             strncpy(pfi->fidatiso.fiisoaut.fifeccad.aa, pmt->adi045d + 46, 2);
             pfi->fidatiso.fiisoaut.fifeccad.aa[2] = '\0';
             strncpy(pfi->fidatiso.fiisoaut.fifeccad.mm, pmt->adi045d + 46 + 2, 2);
             pfi->fidatiso.fiisoaut.fifeccad.mm[2] = '\0';		  
		  }	      
	   }
    }

    strncpy(pfi->fidatiso.fiisoaut.fiaut018,pmt->adipre018 ,4);
    pfi->fidatiso.fiisoaut.fiaut018[4] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut022,pmt->adipre022 ,12);
    pfi->fidatiso.fiisoaut.fiaut022[12] = '\0';


    strncpy(pfi->fidatiso.fiisoaut.fiaut023    ,pmt->adipre023 ,3);
    pfi->fidatiso.fiisoaut.fiaut023[3] = '\0';


    strncpy(pfi->fidatiso.fiisoaut.fiaut025    ,pmt->adipre025 ,4);
    pfi->fidatiso.fiisoaut.fiaut025[4] = '\0';

    strncpy(pfi->fidatiso.fiisoaut.fiaut026,pmt->adipre026,4);
    pfi->fidatiso.fiisoaut.fiaut026[5] = '\0';
    pfi->fidatiso.fiisoaut.fiaut027 = pmt->adipre027[0];
    strncpy(pfi->fidatiso.fiisoaut.fiaut028.aa,pmt->adipre028     ,2);
    pfi->fidatiso.fiisoaut.fiaut028.aa[2] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut028.mm,pmt->adipre028 + 2 ,2);
    pfi->fidatiso.fiisoaut.fiaut028.mm[2] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut028.dd,pmt->adipre028 + 4 ,2);
    pfi->fidatiso.fiisoaut.fiaut028.dd[2] = '\0';

    strncpy(pfi->fidatiso.fiisoaut.fiaut029,pmt->adipre029 ,3);
    pfi->fidatiso.fiisoaut.fiaut029[3] = '\0';

    strncpy(pfi->fidatiso.fiisoaut.fiaut030,pmt->adipre030 ,24);
    pfi->fidatiso.fiisoaut.fiaut030[24] = '\0';
	
	/* 13/06/2024
	
	//Se comenta esta parte porque es un error con las preautorizaciones sustitutivas
	// Al cambiar los datos, la respuesta y la retención que se envía a SF no es la correcta
	// y la retención no se hace bien, y la respuesta a Redsys provoca que Redsys nos genere
	// una operación 1644 de que no hemos respondido correctamente y nos genera una 1220
	
	/// si llega informado el P30 se sustituyen los importes p4 y p5!!!!
    if (strncmp(pmt->adipre030,"000000000000",12) != 0)
    {
        strncpy(pfi->fidatiso.fiisoaut.fiaut004, pmt->adipre030,12); 
        pfi->fidatiso.fiisoaut.fiaut004[12] = '\0';
    }
    if (strncmp(pmt->adipre030 + 12,"000000000000",12) != 0)
    {
        strncpy(pfi->fidatiso.fiisoaut.fiaut005, pmt->adipre030 + 12,12); 
        pfi->fidatiso.fiisoaut.fiaut005[12] = '\0';
    }
    
    13/06/2024 */
	
    strncpy(pfi->fidatiso.fiisoaut.fiaut031.longitud,pmt->adi031l,2);
    pfi->fidatiso.fiisoaut.fiaut031.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut031.datos,pmt->adi031d,23);
    pfi->fidatiso.fiisoaut.fiaut031.datos[23] = '\0';
	
	// P33 - identificador de la entidad que presenta la operación
    strncpy(pfi->fidatiso.fiisoaut.fiaut033.longitud,pmt->adi033l,2);
    pfi->fidatiso.fiisoaut.fiaut033.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut033.datos,pmt->adi033d,11);
    pfi->fidatiso.fiisoaut.fiaut033.datos[11] = '\0';
	
    strncpy(pfi->fidatiso.fiisoaut.fiaut035.longitud,pmt->adi035l,2);
    pfi->fidatiso.fiisoaut.fiaut035.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut035.datos,pmt->adi035d,37);
    pfi->fidatiso.fiisoaut.fiaut035.datos[37] = '\0';

    strncpy(pfi->fidatiso.fiisoaut.fiaut037,pmt->adipre037 ,12);
    pfi->fidatiso.fiisoaut.fiaut037[12] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut038,pmt->adipre038 ,6);
    pfi->fidatiso.fiisoaut.fiaut038[6] = '\0';

    strncpy(pfi->fidatiso.fiisoaut.fiaut040,pmt->adipre040 ,3);
    pfi->fidatiso.fiisoaut.fiaut040[3] = '\0';

    strncpy(pfi->fidatiso.fiisoaut.fiaut041,pmt->adipre041 ,8);
    pfi->fidatiso.fiisoaut.fiaut041[8] = '\0';

    strncpy(pfi->fidatiso.fiisoaut.fiaut042,pmt->adipre042 ,15);
    pfi->fidatiso.fiisoaut.fiaut042[15] = '\0';

    strncpy(pfi->fidatiso.fiisoaut.fiaut043,pmt->adipre043 ,40);
    pfi->fidatiso.fiisoaut.fiaut043[40] = '\0';

    strncpy(pfi->fidatiso.fiisoaut.fiaut044.longitud,pmt->adi044l,2);
    pfi->fidatiso.fiisoaut.fiaut044.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut044.datos,pmt->adi044d,64);
    pfi->fidatiso.fiisoaut.fiaut044.datos[64] = '\0';

    strncpy(pfi->fidatiso.fiisoaut.fiaut045.longitud,pmt->adi045l,2);
    pfi->fidatiso.fiisoaut.fiaut045.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut045.datos,pmt->adi045d,76);
    pfi->fidatiso.fiisoaut.fiaut045.datos[64] = '\0';
	
		
    strncpy(pfi->fidatiso.fiisoaut.fiaut046.longitud,pmt->adi046l,2);
    pfi->fidatiso.fiisoaut.fiaut046.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut046.datos,pmt->adi046d,44);
    pfi->fidatiso.fiisoaut.fiaut046.datos[44] = '\0';

    /***     48--   ***/
    conta1 = 0;
    strncpy(cadena_verif , pmt->adi048l , 3);
    cadena_verif[3] = '\0';
    long_48 = atoi(cadena_verif);
		subcampo_48[999] = '\0';  //PRJ0257005 - iniciliza variable
		fprintf(stdout, "\n ---------------- Saca subcampos P48 ------------------- \n");  //PRJ0257005
		strncpy(subcampo_48, pmt->adi048d, long_48);  //PRJ0257005
		fprintf(stdout," -- Valor long_48 pmt->adi048l >%i< y valor pmt->adi048d >%s< \n", long_48, subcampo_48); //PRJ0257005
    
    while(conta1<=long_48)
    {
        strncpy(longialf , pmt->adi048d + conta1 , 3);
        longialf[3] = '\0';
        longinum = atoi(longialf);
        //quitamos 2 a la long para mover solo el valor
        longinum = longinum - 2;
        strncpy(codialf , pmt->adi048d + conta1 + 3, 2);
        codialf[2] = '\0';
        codinum = atoi(codialf);
        conta1 = conta1 + 5;
        fprintf(stdout, "*** Subcampo descomprimido de 048 >%d<\n", codinum);
        switch(codinum)
        {
        case 1:
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.offset, pmt->adi048d + conta1 , longinum); 
            pfi->fidatiso.fiisoaut.fiaut048.comun01.offset[longinum] = '\0';
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.offset);
  					fhexdump(stdout,pfi->fidatiso.fiisoaut.fiaut048.comun01.offset,longinum);
  					fflush(stdout);
            break;
        case 2:
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.idioma, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.idioma[longinum] = '\0';
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.idioma);
            break;
        case 3:
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.noeco.nuevof, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.noeco.nuevof[longinum] = '\0';
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.noeco.nuevof);
            break;
        case 4:
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.noeco.nuevid, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.noeco.nuevid[longinum] = '\0';
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.noeco.nuevid);
            break;
        case 5: // saldos
            strncpy((char*)&pfi->fidatiso.fiisoaut.fiaut048.augen.saldos, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.augen.saldos.delimi[longinum] = '\0';
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.augen.saldos);
            break;
        case 6: // mov
            strncpy((char*)&pfi->fidatiso.fiisoaut.fiaut048.movim.sub04806, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.movim.sub04806.delimi[longinum] = '\0';
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.movim.sub04806);
            break;
        case 8:
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.cvv000, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.cvv000[longinum] = '\0';
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.cvv000);
            break;
        case 9:
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.termin, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.termin[longinum] = '\0';
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.termin);
            break;
        case 12: //bloque de movimientos
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.movim.bloqmo, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.movim.bloqmo[longinum] = '\0';
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.movim.bloqmo);
            break;
       case 13:  //tipo de tratamiento
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4813, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4813[longinum] = '\0';	   
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4813);
            break;
       case 16:  //fecha sesion de establecimiento
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4816, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4816[longinum] = '\0';	   
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4816);
            break;
       case 17:  //identificacion del establecimiento
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4817, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4817[longinum] = '\0';	   
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4817);
            break;
       case 20:  //calificacion de riesgo
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4820, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4820[longinum] = '\0';	   
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4820);
            break;
				//Xavi modificado para la referencia del recibo
       case 21:  //texto explicativo
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4821, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4821[longinum] = '\0';	   
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4821);
            break;
        case 24: //telefono
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.telefo, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.telefo[longinum] = '\0';
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.telefo);
            break;
        case 25: //ref autorizacion
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.rref, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.rref[longinum] = '\0';
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.rref);
            break;
        case 26: //tipo de compensacion
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.compen, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.compen[longinum] = '\0';
  					fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.compen);
            break;
				///////////// TRANSFERENCIAS Y TRASPASOS
        case 31: //comision
            strncpy(pfi->fidatpro.fitipcom, pmt->adi048d + conta1 , 2);
            pfi->fidatpro.fitipcom[2] = '\0';
		
            strncpy(pfi->fidatpro.ficomisi, pmt->adi048d + conta1+2 , 8);
            pfi->fidatpro.ficomisi[8] = '\0';
						break;
        case 32: //PRJ0258542 - se tiene que sacar el bloque de PIN
        		strncpy(cadena48, bit48, strlen(bit48));
        		strncpy(cad483216, pmt->adi048d + conta1, 16);
        		strncpy(cadena4832, "303031383332", 12);
        		cadena4832[12] = '\0';
        		fprintf(stdout,"   - Lon. dato recibido >%d< >%.*s< \n", longinum, longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832);
  					//fprintf(stdout,"Valor de bit48 en hexadecimal >%s< cadena4832 >%s< \n",cadena48, cadena4832);
  					if (cad483216[0] == 0x00)
  					{
  						//fprintf(stdout,"Cadena del 48 vacia\n");
  						char * res = strstr(cadena48, cadena4832);
  						if (res != NULL)
  						{
  							//fprintf(stdout,"Encontrado en posicion %ld \n", res - cadena48);
  							fprintf(stdout,"Encuentra %s \n", res);
  							strncpy(c4832_32, res + 12, 32);
		  					fprintf(stdout,"   - Lon. dato que sale >%d< >%.*s< \n", longinum * 2, longinum * 2, c4832_32);
		  					strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832, c4832_32, longinum * 2);
	  					}
  						else
  						{
  							fprintf(stdout,"No encontrado \n");
  						}
  					}
  					else
  					{
	  					strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832, pmt->adi048d + conta1 , longinum);
	            pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832[longinum] = '\0';
	  					//PRJ0258542 - se pasa a hexadecimal
	  					strncpy(c4832_16, pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832, longinum);
	  					c4832_16[longinum] = '\0';
	  					c4832_32[longinum*2] = '\0';
	  					retorno = convierte_hex (pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832, longinum, c4832_32);
	  					fprintf(stdout,"   - Lon. dato que sale >%d< >%.*s< \n", longinum * 2, longinum * 2, c4832_32);
	  					strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4832, c4832_32, longinum * 2);
  					}
            break;
		 		case 33:
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4833, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4833[longinum] = '\0';	   
 						fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4833);
            break;	
         // BIZUM tratamientos subcampos 44.02-03-05-06-07-08-09-10-12
         // 07/08/2024 - se tiene que tocar porque llega subcampo y luego longitud
				case 44:
            cod4844 = conta1;
            licod4844 = longinum;
            fprintf(stdout,"  --- Longitud total P4844 sin el 44: >%d<\n", licod4844);
            fprintf(stdout,"   -- Valor inicio 4844 en cod4844: >%d<\n", cod4844);
						strncpy(subcampo_48, pmt->adi048d + cod4844, licod4844);
						subcampo_48[licod4844] = '\0';
						fprintf(stdout," -- Valor long_48.44 >%i< y valor subcampo_48 \n", licod4844); 
						fprintf(stdout,"1        10        20        30        40        50        60        70        80        90        100       110       120       130       140       150\n"); 
						fprintf(stdout,"%s< \n", subcampo_48); 
            conta2 = 0;
            conta3 = 0;
            
	          //Inicializamos para que luego entren todas en la concatenacion del pase a SF
	          pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484402[0] = '\0';
	          pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484403[0] = '\0';
	          pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484405[0] = '\0';
	          pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484406[0] = '\0';
	          pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484407[0] = '\0';
	          pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484408[0] = '\0';
	          pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484409[0] = '\0';
	          pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484410[0] = '\0';
	          pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484412[0] = '\0';
            	
            while (conta2 < licod4844)
            {
            	strncpy(lcod4844s , subcampo_48 + conta2, 2);
            	lcod4844s[2] = '\0';
            	licod4844s = atoi(lcod4844s);
	            //fprintf(stdout,"   -- Valor de licod4844s: >%d<\n", licod4844s);
            	licod4844s = licod4844s - 2;  //Resta 2 para solo coger el dato
            	strncpy(cod4844a , subcampo_48 + conta2 + 2, 2);
            	cod4844a[2] = '\0';
            	lcod4844a = atoi(cod4844a);
	            //fprintf(stdout,"   -- Valor de lcod4844a: >%d<\n", lcod4844a);
	            
            	switch(lcod4844a)
        			{
        			case 2:
        				strncpy(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484402, subcampo_48 + conta2 + 4, licod4844s);
        				pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484402[licod4844s] = '\0';
        				fprintf(stdout,"   -- SubLon. dato recibido 2 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484402);
        				break;
        			case 3:
        				strncpy(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484403, subcampo_48 + conta2 + 4, licod4844s);
        				pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484403[licod4844s] = '\0';
        				fprintf(stdout,"   -- SubLon. dato recibido 3 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484403);
        				break;
        			case 5:
        				strncpy(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484405, subcampo_48 + conta2 + 4, licod4844s);
        				pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484405[licod4844s] = '\0';
        				fprintf(stdout,"   -- SubLon. dato recibido 5 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484405);
        				break;
        			case 6:
        				strncpy(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484406, subcampo_48 + conta2 + 4, licod4844s);
        				pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484406[licod4844s] = '\0';
        				fprintf(stdout,"   -- SubLon. dato recibido 6 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484406);
        				break;
        			case 7:
        				strncpy(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484407, subcampo_48 + conta2 + 4, licod4844s);
        				pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484407[licod4844s] = '\0';
        				fprintf(stdout,"   -- SubLon. dato recibido 7 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484407);
        				break;
        			case 8:
        				strncpy(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484408, subcampo_48 + conta2 + 4, licod4844s);
        				pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484408[licod4844s] = '\0';
        				fprintf(stdout,"   -- SubLon. dato recibido 8 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484408);
        				break;
        			case 9:
        				strncpy(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484409, subcampo_48 + conta2 + 4, licod4844s);
        				pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484409[licod4844s] = '\0';
        				fprintf(stdout,"   -- SubLon. dato recibido 9 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484409);
        				break;
        			case 10:
        				strncpy(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484410, subcampo_48 + conta2 + 4, licod4844s);
        				pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484410[licod4844s] = '\0';
        				fprintf(stdout,"   -- SubLon. dato recibido 10 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484410);
        				break;
        			case 12:
        				strncpy(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484412, subcampo_48 + conta2 + 4, licod4844s);
        				pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484412[licod4844s] = '\0';
        				fprintf(stdout,"   -- SubLon. dato recibido 12 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484412);
        				break;
        			case 93:
        				strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.flagImpParcial, subcampo_48 + conta2 + 4, licod4844s);
        				pfi->fidatiso.fiisoaut.fiaut048.comun01.flagImpParcial[licod4844s] = '\0';
        				fprintf(stdout,"   -- SubLon. dato recibido 93 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.comun01.flagImpParcial);
        				break;
        			}
            	conta2 = conta2 + licod4844s + 4;
	            //fprintf(stdout,"   -- Valor de conta2: >%d<\n", conta2);
	            conta3 = conta3 + 1;
	            if (conta3 == 20) //control de seguridad, quitar cuando funcione
	            {
	            	break;
	            }

            } /***  fin del 2 while  ***/
            break;
		
		
				// BIZUM - quito de aqui el 48.93 de importe parcial y lo meto en el bucle del 48.44
				//case 48:
        //    strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.flagImpParcial, pmt->adi048d + conta1 , longinum);
        //    pfi->fidatiso.fiisoaut.fiaut048.comun01.flagImpParcial[longinum] = '\0';	   
 				//		fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.flagImpParcial);
        //   	break;				
				// BIZUM - quito de aqui el 48.93 de importe parcial y lo meto en el bucle del 48
				case 54: //tasa de acceso a atm		
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4854, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4854[longinum] = '\0';
            fprintLog(nomProg,"TASAS ATM fiiso4854 <%s>\n", pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4854);
            break;
	    	case 61:
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4861, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4861[longinum] = '\0';	   
						fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4861);
            break;						
        case 63: //PRJ0258542
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4863, pmt->adi048d + conta1 , longinum); 
            pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4863[longinum] = '\0';
						fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4863);
            break;						
 		/////////////RAQUEL DECIMALES DE TASAS DE INTERCAMBIO 
        case 69: //tasa de intercambio con decimales de exactitud
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.augen.fiiso4869, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.augen.fiiso4869[longinum] = '\0';
						fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.augen.fiiso4869);
            break;
        case 73: //Contadores PSD2
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.contadores73, pmt->adi048d + conta1 , 20);
            // en lugar de longinum ponemos 20 porque por ahora solo movemos los valores de los contadores
            pfi->fidatiso.fiisoaut.fiaut048.comun01.contadores73[20] = '\0';
						fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.contadores73);
            break;
        case 94: //identificador operativas especiales  //PRJ0257005 - Añadimos para que descomprima el nuevo valor - se ponen trazas a cada uno de los 48
            strncpy(pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4894, pmt->adi048d + conta1 , longinum);
            pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4894[longinum] = '\0';
						fprintf(stdout,"   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4894);
            break;
        }
        conta1 = conta1 + longinum;
    }         /***   fin del while    ***/
    /***  Fin tratamiento 48--   ***/
    /*
    fprintf(stdout,"   -- SubLon. dato recibido 12 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484402);
    fprintf(stdout,"   -- SubLon. dato recibido 12 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484403);
    fprintf(stdout,"   -- SubLon. dato recibido 12 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484405);
    fprintf(stdout,"   -- SubLon. dato recibido 12 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484406);
    fprintf(stdout,"   -- SubLon. dato recibido 12 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484407);
    fprintf(stdout,"   -- SubLon. dato recibido 12 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484408);
    fprintf(stdout,"   -- SubLon. dato recibido 12 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484409);
    fprintf(stdout,"   -- SubLon. dato recibido 12 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484410);
    fprintf(stdout,"   -- SubLon. dato recibido 12 >%d< >%s<\n", licod4844s, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484412);
    */
    memcpy(pfi->fidatiso.fiisoaut.fiaut049,pmt->adipre049,3);
    pfi->fidatiso.fiisoaut.fiaut049[3] = '\0';

    memcpy(pfi->fidatiso.fiisoaut.fiaut050,pmt->adipre050,3);
    pfi->fidatiso.fiisoaut.fiaut050[3] = '\0';

		strncpy(pfi->fidatiso.fiisoaut.fiaut051,pmt->adipre051 ,3);
    pfi->fidatiso.fiisoaut.fiaut051[3] = '\0';
    /***   es tipo binario, y puede tener caracteres no deseados   ***/
    
    memcpy(pfi->fidatiso.fiisoaut.fiaut052,pmt->adipre052,16); //PRJ0258542 - pasa de 8 a 16
    pfi->fidatiso.fiisoaut.fiaut052[16] = '\0';

    strncpy(pfi->fidatiso.fiisoaut.fiaut053,pmt->adipre053 ,16); //PRJ0258542 - pasa de 16 a 24, pero se mantiene en este tipo mensaje
    pfi->fidatiso.fiisoaut.fiaut053[16] = '\0';									

    
	/////////////////////////////////////
    /* p55 */
    // falta montar el campo p55
    conta1 = 0; /* por el identificacion de elementos de datos */

    strncpy(cadena_verif , pmt->adi055l , 3);
    cadena_verif[3] = '\0';
    long_55 = atoi(cadena_verif);
#if defined (TRAZA) && (TRAZA > 10)
    fprintf(stdout,"Para montar p55 -- long_55 >%d<\n",long_55);
    fflush(stdout);
#endif	

    memcpy(pfi->fidatiso.fiisoaut.fiaut055_host,pmt->adi055l,3);
    memcpy(pfi->fidatiso.fiisoaut.fiaut055_host +3,pmt->adi055d,256);

    if (long_55 !=0)
    {
        //en long_55 está a longitud total, con la longitud del dato (2 caracteres), según el ejemplo de sermepa y la
        // documentacion del 2006 (Proce3.2) en esta longitud tbn esta el caracter de control (1 caracter)
        longinum_55 = long_55 - 3;

#if defined (TRAZA) && (TRAZA > 10)
        fprintf(stdout,"longinum_55 >%d<\n",longinum_55);
        fflush(stdout);
#endif

        memcpy(pfi->fidatiso.fiisoaut.fiaut055.carcontrol,pmt->adi055d,1); // identificacion de elemento de datos
		//20160202 - memcpy y no strncpy pq esta empaquetado y puede tener caracteres raros.

#if defined (TRAZA) && (TRAZA > 10)
        //fprintf(stdout,"fiaut055.carcontrol \n");
        //fhexdump(stdout,pfi->fidatiso.fiisoaut.fiaut055.carcontrol,1);
#endif

        trae_etiquetas(&salida, longinum_55, pmt->adi055d + 3);

        for(nn = 0; nn < salida.n_etiquetas; nn++)
        {
#if defined (TRAZA) && (TRAZA > 10)		
            fprintf(stdout,"ETIQUETA >%.4s<\n", salida.etiqueta[nn].etiqueta);
            fhexdump(stdout,salida.etiqueta[nn].p_campo,salida.etiqueta[nn].l_campo);
#endif			
        }

        sprintf(pfi->fidatiso.fiisoaut.fiaut055.longitud,"%03d",longinum_55);

        pfi->fidatiso.fiisoaut.fiaut055.longitud[3] = '\0';

        /* aqui se mueve a formato interno */
        fetiquetas_A_fi(pfi, &salida);

    }//fin long_55;
    /**** fin p55 ***/

    strncpy(pfi->fidatiso.fiisoaut.fiaut056.longitud,pmt->adi056l,2);
    pfi->fidatiso.fiisoaut.fiaut056.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut056.datos,pmt->adi056d,35);
    pfi->fidatiso.fiisoaut.fiaut056.datos[35] = '\0';

    strncpy(pfi->fidatiso.fiisoaut.fiaut058.longitud,pmt->adi058l,2);
    pfi->fidatiso.fiisoaut.fiaut058.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut058.datos,pmt->adi058d,11);
    pfi->fidatiso.fiisoaut.fiaut058.datos[11] = '\0';
	

    /***     62 --   ***/ //PRJ0257005 - Añadimos para que descomprima el nuevo valor del bit 62
    conta1 = 0;
    strncpy(cadena_verif , pmt->adi062l , 3);
    cadena_verif[3] = '\0';
    long_62 = atoi(cadena_verif);
		subcampo_62[256] = '\0';  //PRJ0257005 - iniciliza variable
		fprintf(stdout, " ---------------- Saca subcampos P62 ------------------- \n");  //PRJ0257005
		strncpy(subcampo_62, pmt->adi062d, long_62);  //PRJ0257005
		fprintf(stdout," -- Valor long_62  pmt->adi062l>%i< y valor pmt->adi062d >%.33s< \n", long_62, subcampo_62); //PRJ0257005
		if (long_62 > 0)
		{
			while(conta1<=long_62)
	    	{
	        strncpy(longialf , pmt->adi062d + conta1 , 3);
	        longialf[3] = '\0';
	        longinum = atoi(longialf);
	        longinum = longinum - 2;
	        strncpy(codialf , pmt->adi062d + conta1 + 3, 2);
	        codialf[2] = '\0';
	        codinum = atoi(codialf);
	        conta1 = conta1 + 5;
	#if defined (TRAZA) && (TRAZA > 10)
	        fprintf(stdout, "*** Subcampo descomprimido de 062 >%d<\n", codinum);
	#endif		
	        switch(codinum)
	        {
	        case 16:
	            strncpy(pfi->fidatiso.fiisoaut.fiaut062.bit62_16, pmt->adi062d + conta1 , longinum); 
	            pfi->fidatiso.fiisoaut.fiaut062.bit62_16[longinum] = '\0';
	            printf("   - Lon. dato recibido >%d< >%s<\n", longinum, pfi->fidatiso.fiisoaut.fiaut062.bit62_16);
				break;
	        }
	        conta1 = conta1 + longinum;
	    }         /***   fin del while    ***/
	 	}
	 	else
	 	{
	 		fprintf(stdout," -- No tiene que sacar subcampos del P62 porque es cero, no llegan datos \n");
	 	}
    /***  Fin tratamiento 62 --   ***/

    /***   es tipo binario, y puede tener caracteres no deseados   ***/
    
//PRJ0258542 - Fase 3 - Cambio 11052026  
    memcpy(pfi->fidatiso.fiisoaut.fiaut064,pmt->adipre064,8);
    pfi->fidatiso.fiisoaut.fiaut064[8] = '\0';
    fhexdump(stdout, pfi->fidatiso.fiisoaut.fiaut064, 8);
    fflush(stdout);    
    memcpy(pfi->fidatiso.fiisoaut.fiaut128,pmt->adipre128,8);
    pfi->fidatiso.fiisoaut.fiaut128[8] = '\0';
    fhexdump(stdout, pfi->fidatiso.fiisoaut.fiaut128, 8);
    fflush(stdout);    
//PRJ0258542 - Fase 3 - Cambio 11052026

    /*** paso de area mac a formato interno para validar en el HSM**/
	memcpy(pfi->fidatpro.fiaremac,parea_mac,strlen(parea_mac));
	pfi->fidatpro.fiaremac[strlen(parea_mac)] = '\0';
    
    strncpy(pfi->fiisocom.fi032094.longitud,pmt->adi032l,2);
    pfi->fiisocom.fi032094.longitud[2] = '\0';
    strncpy(pfi->fiisocom.fi032094.datos,pmt->adi032d,11);
    pfi->fiisocom.fi032094.datos[11] = '\0';
	
	//// TRANSFERENCIAS Y TRASPASOS (1200 con 2º mapa de bits)	Ahora se suma la operativa bizum
	/***--------------------------------------  identificacion de cuenta  ------adi102l longitud y adi102d datos**/

 //if ((!strncmp(pmt->adipre000,"1200",4)  ||  
 //      !strncmp(pmt->adipre000,"1220",4) ||
 //      !strncmp(pmt->adipre000,"1221",4)) && 
 //     (!strncmp(pmt->adipre003, "40", 2) ||!strncmp(pmt->adipre003, "49", 2)))  
// Inicio cambios operaciones BIZUM
 //printf(" ftradred.c - Lon. dato recibido 000 >%.4s< 003 >%.6s< 064 >%.8s< 128 >%.8s< \n", pmt->adipre000, pmt->adipre003, pmt->adipre064, pmt->adipre128); //Bizum
 if (
     (!strncmp(pmt->adipre000,"1200",4)  || !strncmp(pmt->adipre000,"1220",4)  || !strncmp(pmt->adipre000,"1221",4)) && 
     (!strncmp(pmt->adipre003, "40", 2)  || !strncmp(pmt->adipre003, "49", 2)) ||
     (!strncmp(pmt->adipre000,"1100",4)  || !strncmp(pmt->adipre000,"1120",4)  || 
      !strncmp(pmt->adipre000,"1121",4)  || !strncmp(pmt->adipre000,"1420",4)) && 
     (!strncmp(pmt->adipre003, "10", 2)  || !strncmp(pmt->adipre003, "29", 2))
    )  
// Final cambios operaciones BIZUM
  {  
		printf(" ftradred.c ftrapre1- Datos adi102l >%d< adi102d >%.*s< adi103l >%d< adi103d >%.*s<\n", 
		atoi(pmt->adi102l), atoi(pmt->adi102l),pmt->adi102d, atoi(pmt->adi103l),atoi(pmt->adi103l), pmt->adi103d);    //Bizum
    strncpy(pfi->fidatiso.fiisoaut.fiaut102.longitud,pmt->adi102l,2);
    pfi->fidatiso.fiisoaut.fiaut102.longitud[2] = '\0';
    //strncpy(pfi->fidatiso.fiisoaut.fiaut102.datos,pmt->adi102d,28);
    strncpy(pfi->fidatiso.fiisoaut.fiaut102.datos,pmt->adi102d,atoi(pfi->fidatiso.fiisoaut.fiaut102.longitud));
    pfi->fidatiso.fiisoaut.fiaut102.datos[28] = '\0';
    strncpy(pfi->fidatiso.fiisoaut.fiaut103.longitud,pmt->adi103l,2);
    pfi->fidatiso.fiisoaut.fiaut103.longitud[2] = '\0';
    //strncpy(pfi->fidatiso.fiisoaut.fiaut103.datos,pmt->adi103d,28);
    strncpy(pfi->fidatiso.fiisoaut.fiaut103.datos,pmt->adi103d,atoi(pfi->fidatiso.fiisoaut.fiaut103.longitud));
    pfi->fidatiso.fiisoaut.fiaut103.datos[28] = '\0';
		printf(" ftradred.c ftrapre1- Datos fiaut102.longitud >%d< fiaut102.datos >%.*s< fiaut103.Longitud >%d< fiaut102.datos >%.*s<\n", 
    atoi(pfi->fidatiso.fiisoaut.fiaut102.longitud),atoi(pfi->fidatiso.fiisoaut.fiaut102.longitud),pfi->fidatiso.fiisoaut.fiaut102.datos, 
    atoi(pfi->fidatiso.fiisoaut.fiaut103.longitud),atoi(pfi->fidatiso.fiisoaut.fiaut103.longitud),pfi->fidatiso.fiisoaut.fiaut103.datos);    //Bizum
	
    strncpy(pfi->fidatiso.fiisoaut.fiaut053,pmt->adipre053,16);
    pfi->fidatiso.fiisoaut.fiaut053[16] = '\0';									

    memcpy(pfi->fidatiso.fiisoaut.fiaut128,pmt->adipre128,8);  //Bizum Mac
    pfi->fidatiso.fiisoaut.fiaut128[8] = '\0'; 

    /***   es tipo binario, y puede tener caracteres no deseados   ***/
    memcpy(pmt->adipre064,pfi->fidatiso.fiisoaut.fiaut064,8);
    
   // 
	//// TRANSFERENCIAS Y TRASPASOS (1200 con 2º mapa de bits)	Ahora se suma la operativa bizum
  }
  
//PTASK000023003 - INI
		strncpy(bin, pfi->fidatpro.fipancif, 8);
		longrango = lon_pan - 8;
		strncpy(rango, pfi->fidatpro.fipancif + 8, longrango);
		retorno = fcarga_bines();
		retorno = fbusca_bin(pbi_binprimero, bin, rango, longrango, clave_ope, parautor1, parautor2, resolutor, entidad);
		if(retorno == 0)
		{
			fprintLog(nomProg,"--- Este BIN %.8s es BIZUM Si o No -->%.1s<-- \n", bin, parautor1);
			if (strncmp(parautor1, "S", 1) == 0)
			{
				printf(" Operacion Bizum se incorpora cuentas - Datos adi102l >%d< adi102d >%.*s< adi103l >%d< adi103d >%.*s<\n", 
					atoi(pmt->adi102l), atoi(pmt->adi102l),pmt->adi102d, atoi(pmt->adi103l),atoi(pmt->adi103l), pmt->adi103d);
		    strncpy(pfi->fidatiso.fiisoaut.fiaut102.longitud,pmt->adi102l,2);
		    pfi->fidatiso.fiisoaut.fiaut102.longitud[2] = '\0';
		    strncpy(pfi->fidatiso.fiisoaut.fiaut102.datos,pmt->adi102d,atoi(pfi->fidatiso.fiisoaut.fiaut102.longitud));
		    pfi->fidatiso.fiisoaut.fiaut102.datos[28] = '\0';
		    strncpy(pfi->fidatiso.fiisoaut.fiaut103.longitud,pmt->adi103l,2);
		    pfi->fidatiso.fiisoaut.fiaut103.longitud[2] = '\0';
		    strncpy(pfi->fidatiso.fiisoaut.fiaut103.datos,pmt->adi103d,atoi(pfi->fidatiso.fiisoaut.fiaut103.longitud));
		    pfi->fidatiso.fiisoaut.fiaut103.datos[28] = '\0';
				printf(" Operacion Bizum se ha metido cuentas - Datos fiaut102.longitud >%d< fiaut102.datos >%.*s< fiaut103.Longitud >%d< fiaut102.datos >%.*s<\n", 
		    	atoi(pfi->fidatiso.fiisoaut.fiaut102.longitud),atoi(pfi->fidatiso.fiisoaut.fiaut102.longitud),pfi->fidatiso.fiisoaut.fiaut102.datos, 
		    	atoi(pfi->fidatiso.fiisoaut.fiaut103.longitud),atoi(pfi->fidatiso.fiisoaut.fiaut103.longitud),pfi->fidatiso.fiisoaut.fiaut103.datos);
			}
			else
			{
				printf("*** BIN NO BIZUM, no informa bit 102 y 103 ***\n");
			}
		}
		else
		{
			printf("*** BIN NO encontrado, no informa bit 102 y 103 ***\n");
		}
//PTASK000023003 - FIN

  
 } //operaciones 11,12,14

  if (strncmp(pmt->adipre000,"13",2) == 0)
  {
    if ((retorno = inicfori(1304,pfi)) != 0)
        return(1);
    fcomunes1(pfi,pmt);

// PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud total de finumPAN ni de fipancif
    strncpy(pfi->fidatiso.fiisofic.fific002.longitud,pmt->adi002l,2);
    pfi->fidatiso.fiisofic.fific002.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisofic.fific002.datos,pmt->adi002d,19);
    pfi->fidatiso.fiisofic.fific002.datos[19] = '\0';
	///////cifrar la tarjeta ---------------
    lon_pan = atoi(pfi->fidatiso.fiisoaut.finumPAN.longitud);
    strncpy(panClr,pfi->fidatiso.fiisoaut.finumPAN.datos, lon_pan);
    panClr[lon_pan]='\0';
//guardamos el mismo pan recibido en el pan cifrado
//	strncpy(pfi->fidatpro.fipancif, panCfd, lon_pan);
    strncpy(pfi->fidatpro.fipancif, panClr, lon_pan);
	///////////////// ----------------------
	
    strncpy(pfi->fidatiso.fiisofic.fific053,pmt->adipre053,24);	
    pfi->fidatiso.fiisofic.fific053[4] = '\0';									

    strncpy(pfi->fidatiso.fiisofic.fific072.longitud,pmt->adi072l,3);
    pfi->fidatiso.fiisofic.fific072.longitud[3] = '\0';
    /***   es tipo binario, y puede tener caracteres no deseados   ***/
    memcpy(pfi->fidatiso.fiisofic.fific072.datos,pmt->adi072d,256);
    pfi->fidatiso.fiisofic.fific072.datos[256] = '\0';
    strncpy(pfi->fidatiso.fiisofic.fific093.longitud,pmt->adi093l,2);
    pfi->fidatiso.fiisofic.fific093.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisofic.fific093.datos,pmt->adi093d,11);
    pfi->fidatiso.fiisofic.fific093.datos[11] = '\0';

    strncpy(pfi->fidatiso.fiisofic.fific101.longitud,pmt->adi101l,2);
    pfi->fidatiso.fiisofic.fific101.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisofic.fific101.datos,pmt->adi101d,17);
    pfi->fidatiso.fiisofic.fific101.datos[17] = '\0';

    /***   es tipo binario, y puede tener caracteres no deseados   ***/
    memcpy(pfi->fidatiso.fiisofic.fific128,pmt->adipre128,8);
    pfi->fidatiso.fiisofic.fific128[8] = '\0';
		fprintf(stdout,"pmt->adipre128 >%.8s<\n",pmt->adipre128);
		fprintf(stdout,"pfi->fidatiso.fiisofic.fific128 >%.8s<\n",pfi->fidatiso.fiisofic.fific128);
	

    strncpy(pfi->fiisocom.fi032094.longitud,pmt->adi094l,2);
    pfi->fiisocom.fi032094.longitud[2] = '\0';
    strncpy(pfi->fiisocom.fi032094.datos,pmt->adi094d,11);
    pfi->fiisocom.fi032094.datos[11] = '\0';
	
    /*** paso de area mac a formato interno para validar en el HSM**/
	memcpy(pfi->fidatpro.fiaremac,parea_mac,strlen(parea_mac));
	pfi->fidatpro.fiaremac[strlen(parea_mac)] = '\0';

  } //fin ficheros

  if (strncmp(pmt->adipre000,"1644",4) == 0) //PRJ0258542 - se anaden los nuevos campos
  {
    if ((retorno = inicfori(1644,pfi)) != 0)
        return(1);
    fcomunes1(pfi,pmt);

  	//PRJ0258542 - Fase 3 - Cambio 11052026 - INI
  	fprintf(stdout,"Campos mensaje de entrada>%.4s< \n", pmt->adipre000);
  	fprintf(stdout,"  Valor del P11          >%.6s< \n", pmt->adipre011);
  	fprintf(stdout,"  Valor del P12          >%.12s< \n", pmt->adipre012);
  	fprintf(stdout,"  Valor del P24          >%.3s< \n", pmt->adipre024);
  	fprintf(stdout,"  Valor del P39          >%.3s< \n", pmt->adipre039);
  	fprintf(stdout,"  Valor del S72          >%.3s< >%.7s< \n", pmt->adi072l, pmt->adi072d);
  	fprintf(stdout,"  Valor del S93          >%.2s< >%.11s< \n", pmt->adi093l, pmt->adi093d);
  	fprintf(stdout,"  Valor del S94          >%.2s< >%.11s< \n", pmt->adi094l, pmt->adi094d);
		//PRJ0258542 - Fase 3 - Cambio 11052026 - FIN

    strncpy(pfi->fidatiso.fiisoadm.fiadm011,pmt->adipre011,6);
    pfi->fidatiso.fiisoadm.fiadm011[6] = '\0';
    strncpy(pfi->fidatiso.fiisoadm.fiadm012,pmt->adipre012,12);
    pfi->fidatiso.fiisoadm.fiadm012[12] = '\0';
    strncpy(pfi->fidatiso.fiisoadm.fiadm024,pmt->adipre024,3);
    pfi->fidatiso.fiisoadm.fiadm024[3] = '\0';
    strncpy(pfi->fidatiso.fiisoadm.fiadm039,pmt->adipre039,3);
    pfi->fidatiso.fiisoadm.fiadm039[3] = '\0';

    strncpy(pfi->fidatiso.fiisoadm.fiadm072.longitud,pmt->adi072l,3);
    pfi->fidatiso.fiisoadm.fiadm072.longitud[3] = '\0';
    memcpy(pfi->fidatiso.fiisoadm.fiadm072.datos,pmt->adi072d,7); //PRJ0258542 - Fase 3 - Cambio 11052026
    memcpy(pfi->fidatiso.fiisoadm.fiadm072.datos + 7,pmt->adi072d + 23,atoi(pfi->fidatiso.fiisoadm.fiadm072.longitud)-23); //PRJ0258542 - Fase 3 - Cambio 11052026
    pfi->fidatiso.fiisoadm.fiadm072.datos[256] = '\0';

    strncpy(pfi->fidatiso.fiisoadm.fiadm093.longitud,pmt->adi093l,2);
    pfi->fidatiso.fiisoadm.fiadm093.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisoadm.fiadm093.datos,pmt->adi093d,11);
    pfi->fidatiso.fiisoadm.fiadm093.datos[11] = '\0';
    
    strncpy(pfi->fidatiso.fiisoadm.fiadm094.longitud,pmt->adi094l,2);
    pfi->fidatiso.fiisoadm.fiadm094.longitud[2] = '\0';
    strncpy(pfi->fidatiso.fiisoadm.fiadm094.datos,pmt->adi094d,11);//PRJ0258542 - Fase 3 - Cambio 11052026 - estaba 093 en lugar de 094
    pfi->fidatiso.fiisoadm.fiadm094.datos[11] = '\0';

    strncpy(pfi->fiisocom.fi032094.longitud,pmt->adi094l,2);
    pfi->fiisocom.fi032094.longitud[2] = '\0';
    strncpy(pfi->fiisocom.fi032094.datos,pmt->adi094d,11);
    pfi->fiisocom.fi032094.datos[11] = '\0';

  }

  if (strncmp(pmt->adipre000,"18",2) == 0)
  {
  	//PRJ0258542 - INI
  	fprintf(stdout,"Dentro de ftradred mensa >%.4s< \n", pmt->adipre000);
  	fprintf(stdout,"  Valor del P11          >%.6s< \n", pmt->adipre011);
  	fprintf(stdout,"  Valor del P12          >%.12s< \n", pmt->adipre012);
  	fprintf(stdout,"  Valor del P24          >%.3s< \n", pmt->adipre024);
  	fprintf(stdout,"  Valor del P25          >%.4s< \n", pmt->adipre025);
  	fprintf(stdout,"  Valor del P33          >%.2s< >%.11s< \n", pmt->adi033l, pmt->adi033d);
  	fprintf(stdout,"  Valor del P39          >%.3s< \n", pmt->adipre039);
  	fprintf(stdout,"  Valor del P53          >%.24s< \n", pmt->adipre053);
  	fprintf(stdout,"  Valor del S93          >%.2s< >%.11s< \n", pmt->adi093l, pmt->adi093d);
  	fprintf(stdout,"  Valor del S94          >%.2s< >%.11s< \n", pmt->adi094l, pmt->adi094d);
  	fprintf(stdout,"  Valor del S96          >%.3s< >%.360s< \n", pmt->adi096l, pmt->adi096d);
//PRJ0258542 - Fase 3 - Cambio 11052026  
		fprintf(stdout,"  Valor del adipre128    >\n");
    fhexdump(stdout, pmt->adipre128, 8);
    fflush(stdout);    
//PRJ0258542 - Fase 3 - Cambio 11052026
		//PRJ0258542 - FIN
    if ((retorno = inicfori(1804,pfi)) != 0)
        return(1);
    fcomunes1(pfi,pmt);
    //PRJ0258542 - lo nuevo va en el IF
		if (strncmp(pmt->adipre024, "811", 3) == 0 || strncmp(pmt->adipre024, "815", 3) == 0 || strncmp(pmt->adipre024, "816", 3) == 0) 
		{
			fprintf(stdout," Mensaje de claves 1>%.4s< con P24 >%.3s<\n", pmt->adipre000, pmt->adipre024);
	    strncpy(pfi->fidatiso.fiisocla.ficla011,pmt->adipre011,6);
	    pfi->fidatiso.fiisocla.ficla011[6] = '\0';
	    strncpy(pfi->fidatiso.fiisocla.ficla012,pmt->adipre012,12);
	    pfi->fidatiso.fiisocla.ficla012[12] = '\0';
	    strncpy(pfi->fidatiso.fiisocla.ficla024,pmt->adipre024,3);
	    pfi->fidatiso.fiisocla.ficla024[3] = '\0';
	    strncpy(pfi->fidatiso.fiisocla.ficla025,pmt->adipre025,4);
	    pfi->fidatiso.fiisocla.ficla025[4] = '\0';
    	strncpy(pfi->fidatiso.fiisocla.ficla033.longitud,pmt->adi033l,2);
    	pfi->fidatiso.fiisocla.ficla033.longitud[2] = '\0';
    	strncpy(pfi->fidatiso.fiisocla.ficla033.datos,pmt->adi033d,11);
    	pfi->fidatiso.fiisocla.ficla033.datos[11] = '\0';
	    strncpy(pfi->fidatiso.fiisocla.ficla039,pmt->adipre039,3);
	    pfi->fidatiso.fiisocla.ficla039[3] = '\0';
	    
	    strncpy(pfi->fidatiso.fiisocla.ficla053.control,			 pmt->adipre053			,2);
	    strncpy(pfi->fidatiso.fiisocla.ficla053.identificador, pmt->adipre053 + 2	,8);
	    strncpy(pfi->fidatiso.fiisocla.ficla053.algoritmo,		 pmt->adipre053 + 10	,2);
	    strncpy(pfi->fidatiso.fiisocla.ficla053.longitud,			 pmt->adipre053 + 12	,4);
	    strncpy(pfi->fidatiso.fiisocla.ficla053.proteccion,		 pmt->adipre053 + 16	,2);
	    strncpy(pfi->fidatiso.fiisocla.ficla053.resto,				 pmt->adipre053 + 18	,6);
	    
    	strncpy(pfi->fidatiso.fiisocla.ficla093.longitud,pmt->adi093l,2);
    	pfi->fidatiso.fiisocla.ficla093.longitud[2] = '\0';
    	strncpy(pfi->fidatiso.fiisocla.ficla093.datos,pmt->adi093d,11);
    	pfi->fidatiso.fiisocla.ficla093.datos[11] = '\0';
    	strncpy(pfi->fidatiso.fiisocla.ficla094.longitud,pmt->adi094l,2);
    	pfi->fidatiso.fiisocla.ficla094.longitud[2] = '\0';
    	strncpy(pfi->fidatiso.fiisocla.ficla094.datos,pmt->adi094d,11);
			pfi->fidatiso.fiisocla.ficla094.datos[11] = '\0';

    	strncpy(pfi->fidatiso.fiisocla.ficla096.longitud,pmt->adi096l,3);
    	pfi->fidatiso.fiisocla.ficla096.longitud[3] = '\0';
   		strncpy(pfi->fidatiso.fiisocla.ficla096.datos,pmt->adi096d,360);
   		pfi->fidatiso.fiisocla.ficla096.datos[360] = '\0';
			
//PRJ0258542 - Fase 3 - Cambio 11052026  
//		fprintf(stdout,"****** Valor del ficlamac 1 ******\n");
//  	strncpy(pfi->fidatiso.fiisocla.ficlamac,pmt->adipre128,8);
//		pfi->fidatiso.fiisocla.ficlamac[8] = '\0';
//    fhexdump(stdout, pfi->fidatiso.fiisocla.ficlamac, 8);
//    fflush(stdout);    
//PRJ0258542 - Fase 3 - Cambio 11052026  
		fprintf(stdout,"****** Valor del ficlamac 2 ******\n");
    memcpy(pfi->fidatiso.fiisocla.ficlamac,pmt->adipre128,8);
    pfi->fidatiso.fiisocla.ficlamac[8] = '\0';
    fhexdump(stdout, pfi->fidatiso.fiisocla.ficlamac, 8);
    fflush(stdout);    
//PRJ0258542 - Fase 3 - Cambio 11052026
			
		}
		else if (strncmp(pmt->adipre000, "1814", 4) == 0 && (strncmp(pmt->adipre039, "800", 3) == 0 || strncmp(pmt->adipre039, "801", 3) == 0 || strncmp(pmt->adipre039, "802", 3) == 0) && (strncmp(pmt->adi033l, "00", 2) != 0))//PRJ0258542 - nuevo ELSE para recepcionar 1814
		{
			fprintf(stdout," Mensaje de claves 2>%.4s< con P39 >%.3s<\n", pmt->adipre000, pmt->adipre039);
	    strncpy(pfi->fidatiso.fiisocla.ficla011,pmt->adipre011,6);
	    pfi->fidatiso.fiisocla.ficla011[6] = '\0';
	    strncpy(pfi->fidatiso.fiisocla.ficla012,pmt->adipre012,12);
	    pfi->fidatiso.fiisocla.ficla012[12] = '\0';
	    strncpy(pfi->fidatiso.fiisocla.ficla024,pmt->adipre024,3);
	    pfi->fidatiso.fiisocla.ficla024[3] = '\0';
	    strncpy(pfi->fidatiso.fiisocla.ficla025,pmt->adipre025,4);
	    pfi->fidatiso.fiisocla.ficla025[4] = '\0';
    	strncpy(pfi->fidatiso.fiisocla.ficla033.longitud,pmt->adi033l,2);
    	pfi->fidatiso.fiisocla.ficla033.longitud[2] = '\0';
    	strncpy(pfi->fidatiso.fiisocla.ficla033.datos,pmt->adi033d,11);
    	pfi->fidatiso.fiisocla.ficla033.datos[11] = '\0';
	    strncpy(pfi->fidatiso.fiisocla.ficla039,pmt->adipre039,3);
	    pfi->fidatiso.fiisocla.ficla039[3] = '\0';
	    
	    strncpy(pfi->fidatiso.fiisocla.ficla053.control,			 pmt->adipre053			,2);
	    strncpy(pfi->fidatiso.fiisocla.ficla053.identificador, pmt->adipre053 + 2	,8);
	    strncpy(pfi->fidatiso.fiisocla.ficla053.algoritmo,		 pmt->adipre053 + 10	,2);
	    strncpy(pfi->fidatiso.fiisocla.ficla053.longitud,			 pmt->adipre053 + 12	,4);
	    strncpy(pfi->fidatiso.fiisocla.ficla053.proteccion,		 pmt->adipre053 + 16	,2);
	    strncpy(pfi->fidatiso.fiisocla.ficla053.resto,				 pmt->adipre053 + 18	,6);
	    
    	strncpy(pfi->fidatiso.fiisocla.ficla093.longitud,pmt->adi093l,2);
    	pfi->fidatiso.fiisocla.ficla093.longitud[2] = '\0';
    	strncpy(pfi->fidatiso.fiisocla.ficla093.datos,pmt->adi093d,11);
    	pfi->fidatiso.fiisocla.ficla093.datos[11] = '\0';
    	strncpy(pfi->fidatiso.fiisocla.ficla094.longitud,pmt->adi094l,2);
    	pfi->fidatiso.fiisocla.ficla094.longitud[2] = '\0';
    	strncpy(pfi->fidatiso.fiisocla.ficla094.datos,pmt->adi094d,11);
			pfi->fidatiso.fiisocla.ficla094.datos[11] = '\0';

    	strncpy(pfi->fidatiso.fiisocla.ficla096.longitud,pmt->adi096l,3);
    	pfi->fidatiso.fiisocla.ficla096.longitud[3] = '\0';
   		strncpy(pfi->fidatiso.fiisocla.ficla096.datos,pmt->adi096d,360);
   		pfi->fidatiso.fiisocla.ficla096.datos[360] = '\0';
			
    	strncpy(pfi->fidatiso.fiisocla.ficlamac,pmt->adipre128,8);
			pfi->fidatiso.fiisocla.ficlamac[8] = '\0';		
		}
		else //PRJ0258542 - lo que habia va en el ELSE
		{
			fprintf(stdout," Mensaje de control >%.4s< con P24 >%.3s<\n", pmt->adipre000, pmt->adipre024); //PRJ0258542
    	//strncpy(pfi->fidatiso.fiisoctr.fictr028.aa,pmt->adipre028     ,2); //PRJ0258542
    	//pfi->fidatiso.fiisoctr.fictr028.aa[2] = '\0'; //PRJ0258542
    	//strncpy(pfi->fidatiso.fiisoctr.fictr028.mm,pmt->adipre028 + 2 ,2); //PRJ0258542
    	//pfi->fidatiso.fiisoctr.fictr028.mm[2] = '\0'; //PRJ0258542
    	//strncpy(pfi->fidatiso.fiisoctr.fictr028.dd,pmt->adipre028 + 4 ,2); //PRJ0258542
    	//pfi->fidatiso.fiisoctr.fictr028.dd[2] = '\0'; //PRJ0258542
    	//strncpy(pfi->fidatiso.fiisoctr.fictr029,pmt->adipre029 ,3); //PRJ0258542
    	//pfi->fidatiso.fiisoctr.fictr029[3] = '\0'; //PRJ0258542

	  	//PRJ0258542 - INI
	    strncpy(pfi->fidatiso.fiisoctr.fictr011,pmt->adipre011,6);
	    pfi->fidatiso.fiisoctr.fictr011[6] = '\0';
	    strncpy(pfi->fidatiso.fiisoctr.fictr012.aa,pmt->adipre012     ,2);
	    pfi->fidatiso.fiisoctr.fictr012.aa[2] = '\0';
	    strncpy(pfi->fidatiso.fiisoctr.fictr012.mm,pmt->adipre012 + 2 ,2);
	    pfi->fidatiso.fiisoctr.fictr012.mm[2] = '\0';
	    strncpy(pfi->fidatiso.fiisoctr.fictr012.dd,pmt->adipre012 + 4 ,2);
	    pfi->fidatiso.fiisoctr.fictr012.dd[2] = '\0';
	    strncpy(pfi->fidatiso.fiisoctr.fictr012.hh,pmt->adipre012 + 6 ,2);
	    pfi->fidatiso.fiisoctr.fictr012.hh[2] = '\0';
	    strncpy(pfi->fidatiso.fiisoctr.fictr012.mi,pmt->adipre012 + 8 ,2);
	    pfi->fidatiso.fiisoctr.fictr012.mi[2] = '\0';
	    strncpy(pfi->fidatiso.fiisoctr.fictr012.ss,pmt->adipre012 + 10,2);
	    pfi->fidatiso.fiisoctr.fictr012.ss[2] = '\0';
	    strncpy(pfi->fidatiso.fiisoctr.fictr024,pmt->adipre024,3);
	    pfi->fidatiso.fiisoctr.fictr024[3] = '\0';
    	strncpy(pfi->fidatiso.fiisoctr.fictr093.longitud,pmt->adi093l,2);
    	pfi->fidatiso.fiisoctr.fictr093.longitud[2] = '\0';
    	strncpy(pfi->fidatiso.fiisoctr.fictr093.datos,pmt->adi093d,11);
    	pfi->fidatiso.fiisoctr.fictr093.datos[11] = '\0';
    	strncpy(pfi->fiisocom.fi032094.longitud,pmt->adi094l,2);
    	pfi->fiisocom.fi032094.longitud[2] = '\0';
    	strncpy(pfi->fiisocom.fi032094.datos,pmt->adi094d,11);
    	pfi->fiisocom.fi032094.datos[11] = '\0';

	  	fprintf(stdout,"CAMPO MOVIDOS DEL CTRNL   \n");
	  	fprintf(stdout,"  Valor del P11          >%.6s< \n", pfi->fidatiso.fiisoctr.fictr011);
	  	fprintf(stdout,"  Valor del P12          >%.12s< \n", pfi->fidatiso.fiisoctr.fictr012);
	  	fprintf(stdout,"  Valor del P24          >%.3s< \n", pfi->fidatiso.fiisoctr.fictr024);
	  	fprintf(stdout,"  Valor del S93          >%.2s< >%.11s< \n", pfi->fidatiso.fiisoctr.fictr093.longitud, pfi->fidatiso.fiisoctr.fictr093.datos);
	  	fprintf(stdout,"  Valor del S94          >%.2s< >%.11s< \n", pfi->fidatiso.fiisoctr.fictr094.longitud, pfi->fidatiso.fiisoctr.fictr094.datos);
			//PRJ0258542 - FIN
		}
  }

  if (strncmp(pmt->adipre000,"15",2) == 0) 
  {
        if ((retorno = inicfori(1524,pfi)) != 0)
        return(1);
        fcomunes1(pfi,pmt);

    strncpy(pfi->fidatiso.fiisotot.fitot028.aa,pmt->adipre028     ,2);
    pfi->fidatiso.fiisotot.fitot028.aa[2] = '\0';
    strncpy(pfi->fidatiso.fiisotot.fitot028.mm,pmt->adipre028 + 2 ,2);
    pfi->fidatiso.fiisotot.fitot028.mm[2] = '\0';
    strncpy(pfi->fidatiso.fiisotot.fitot028.dd,pmt->adipre028 + 4 ,2);
    pfi->fidatiso.fiisotot.fitot028.dd[2] = '\0';

    strncpy(pfi->fidatiso.fiisotot.fitot029,pmt->adipre029,3);
    pfi->fidatiso.fiisotot.fitot029[3] = '\0';

    /***     48      ***/
    conta1 = 0;
    strncpy(cadena_verif , pmt->adi048l , 3);
    cadena_verif[3] = '\0';
    long_48 = atoi(cadena_verif); // longitud total p48
    while(conta1<=long_48)
    {
        strncpy(longialf , pmt->adi048d + conta1 , 3); // longitud tag
        longialf[3] = '\0';
        longinum = atoi(longialf);
        //quitamos 2 a la long para mover solo el valor
        longinum = longinum - 2;   // longitud del dato del tag
        strncpy(codialf , pmt->adi048d + conta1 + 3, 2); // código del tag
        codialf[2] = '\0';
        codinum = atoi(codialf); // número de tag
        conta1 = conta1 + 5;  // 3 de la long del tag + 2 del código del tag
		
#if defined (TRAZA) && (TRAZA > 10)
        fprintf(stdout, "subcampo de 048 >%d<\n", codinum);
#endif		
        switch(codinum)
        {
		case 15:
		   strncpy(pfi->fidatiso.fiisotot.fitot4815, pmt->adi048d + conta1, longinum); 
		   pfi->fidatiso.fiisotot.fitot4815[longinum] = '\0';
		   break;
        }
        conta1 = conta1 + longinum;
    }         /***   fin del while    ***/
			
    strncpy(pfi->fidatiso.fiisotot.fitot074,pmt->adipre074,10);
    pfi->fidatiso.fiisotot.fitot074[10] = '\0';
    strncpy(pfi->fidatiso.fiisotot.fitot075,pmt->adipre075,10);
    pfi->fidatiso.fiisotot.fitot075[10] = '\0';
    strncpy(pfi->fidatiso.fiisotot.fitot076,pmt->adipre076,10);
    pfi->fidatiso.fiisotot.fitot076[10] = '\0';
    strncpy(pfi->fidatiso.fiisotot.fitot077,pmt->adipre077,10);
    pfi->fidatiso.fiisotot.fitot077[10] = '\0';
    strncpy(pfi->fidatiso.fiisotot.fitot086,pmt->adipre086,16);
    pfi->fidatiso.fiisotot.fitot086[16] = '\0';
    strncpy(pfi->fidatiso.fiisotot.fitot087,pmt->adipre087,16);
    pfi->fidatiso.fiisotot.fitot087[16] = '\0';
    strncpy(pfi->fidatiso.fiisotot.fitot088,pmt->adipre088,16);
    pfi->fidatiso.fiisotot.fitot088[16] = '\0';
    strncpy(pfi->fidatiso.fiisotot.fitot089,pmt->adipre089,16);
    pfi->fidatiso.fiisotot.fitot089[16] = '\0';
    strncpy(pfi->fidatiso.fiisotot.fitot097,pmt->adipre097,17);
    pfi->fidatiso.fiisotot.fitot097[17] = '\0';
    strncpy(pfi->fidatiso.fiisotot.fitot109,pmt->adipre109,16);
    pfi->fidatiso.fiisotot.fitot109[16] = '\0';
    strncpy(pfi->fidatiso.fiisotot.fitot110,pmt->adipre110,16);
    pfi->fidatiso.fiisotot.fitot110[16] = '\0';

    strncpy(pfi->fiisocom.fi032094.longitud,pmt->adi032l,2);
    pfi->fiisocom.fi032094.longitud[2] = '\0';
    strncpy(pfi->fiisocom.fi032094.datos,pmt->adi032d,11);
    pfi->fiisocom.fi032094.datos[11] = '\0';

  } //cierres
  fprintf(stdout," Salida de la rutina ftrapre1\n"); //PRJ0258542
  return(0);
} //FIN ftrapre1


/*****************************************************************
FUNCION  FCOMUNES1             llamada por ftrapre1

DESCRIPCION: Funcion que informa los campos del formato_interno
comunes a todos los tipos de mensaje

AREA DE PASO :
- puntero a formato_interno donde volvera el mensaje traducido
- puntero a formato_intermedio donde llega el mensaje a traducir

* devuelve los valores :
*****************************************************************/
int fcomunes1(formato_interno *pfi, formato_intermedio *pmt)
{
    strncpy(pfi->fiisocom.fiiso000,pmt->adipre000,4);
    pfi->fiisocom.fiiso000[4] = '\0';

    strncpy(pfi->fiisocom.fiidtran,pmt->adipre011,6);
    pfi->fiisocom.fiidtran[6] = '\0';

    strncpy(pfi->fiisocom.fifechor.aa,pmt->adipre012     ,2);
    pfi->fiisocom.fifechor.aa[2] = '\0';
    strncpy(pfi->fiisocom.fifechor.mm,pmt->adipre012 + 2 ,2);
    pfi->fiisocom.fifechor.mm[2] = '\0';
    strncpy(pfi->fiisocom.fifechor.dd,pmt->adipre012 + 4 ,2);
    pfi->fiisocom.fifechor.dd[2] = '\0';
    strncpy(pfi->fiisocom.fifechor.hh,pmt->adipre012 + 6 ,2);
    pfi->fiisocom.fifechor.hh[2] = '\0';
    strncpy(pfi->fiisocom.fifechor.mi,pmt->adipre012 + 8 ,2);
    pfi->fiisocom.fifechor.mi[2] = '\0';
    strncpy(pfi->fiisocom.fifechor.ss,pmt->adipre012 + 10,2);
    pfi->fiisocom.fifechor.ss[2] = '\0';

    strncpy(pfi->fiisocom.fiiso024,pmt->adipre024,3);
    pfi->fiisocom.fiiso024[3] = '\0';

    strncpy(pfi->fiisocom.respusal,pmt->adipre039,3);
    pfi->fiisocom.respusal[3] = '\0';

    return(0);
}


/*****************************************************************
FUNCION  FTRAPRE2             llamada por ftradred

DESCRIPCION: Funcion que convierte el mensaje de una estructura
tipo formato_interno a otra tipo formato_intermedio

AREA DE PASO :
- puntero a formato_interno donde llega el mensaje a traducir
- puntero a formato_intermedio donde volvera el mensaje traducido

* devuelve los valores :
*****************************************************************/
int ftrapre2(formato_interno *pfi, formato_intermedio *pmt)
{
    int fcomunes2(formato_interno *pfi, formato_intermedio *pmt);

    int mm,conta1,conta2,conta3;
    char longialf[4];
    int  Long_R55, tam=0;
    char ch_aux[1024];
    char Long_R55alf[4];
    char longalf_55[4];
		char auxComi[6];
    int num_cuentas;
    int num_movimientos;
    int i;
    char cantidad[2];
    char cantidad_saldos[2];
    int ll;
	

  /***  inicializa el formato intermedio a los valores correspondientes ftrapre2 ***/
  finicializa_intermedio(pmt);
  
 //DMND0036176 añadidas preautorizaciones
 
  if ((strncmp(pfi->fiisocom.fiiso000,"11",2) == 0) ||
      (strncmp(pfi->fiisocom.fiiso000,"12",2) == 0) ||
      (strncmp(pfi->fiisocom.fiiso000,"14",2) == 0))
  {
	   
    fcomunes2(pfi,pmt);

// PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud total de finumPAN
    strncpy(pmt->adi002l  ,pfi->fidatiso.fiisoaut.finumPAN.longitud,2);
    memcpy(pmt->adi002d  ,pfi->fidatiso.fiisoaut.finumPAN.datos,19);

    /***   se ha cambiado el P-3 en el proceso en el netplus   ***/
    if ((strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig34,"01",2) == 0) ||
            (strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig34,"03",2) == 0))
    {
        strncpy(pfi->fidatiso.fiisoaut.fiaut003.dig34,"00",2);
    }

    // fprintf(stdout,"dig12 s>>>%.*s<<<",2,pfi->fidatiso.fiisoaut.fiaut003.dig12);

    strncpy(pmt->adipre003     ,pfi->fidatiso.fiisoaut.fiaut003.dig12,2);
    strncpy(pmt->adipre003 + 2 ,pfi->fidatiso.fiisoaut.fiaut003.dig34,2);
    strncpy(pmt->adipre003 + 4 ,pfi->fidatiso.fiisoaut.fiaut003.dig56,2);

    // fprintf(stdout,"adipre003 >>>%.*s<<<",2,pmt->adipre003);

    strncpy(pmt->adipre004,pfi->fidatiso.fiisoaut.fiaut004,12);
    strncpy(pmt->adipre005,pfi->fidatiso.fiisoaut.fiaut005,12);
    strncpy(pmt->adipre006,pfi->fidatiso.fiisoaut.fiaut006,12);
    
    strncpy(pmt->adipre009,pfi->fidatiso.fiisoaut.fiaut009,8);
		strncpy(pmt->adipre010,pfi->fidatiso.fiisoaut.fiaut010,8);

    strncpy(pmt->adipre014     ,pfi->fidatiso.fiisoaut.fifeccad.aa,2);
    strncpy(pmt->adipre014 + 2 ,pfi->fidatiso.fiisoaut.fifeccad.mm,2);

    strncpy(pmt->adipre018,pfi->fidatiso.fiisoaut.fiaut018,4);

    strncpy(pmt->adipre022,pfi->fidatiso.fiisoaut.fiaut022,12);
    memcpy(pmt->adipre025,pfi->fidatiso.fiisoaut.fiaut025,4);

    strncpy(pmt->adipre026,pfi->fidatiso.fiisoaut.fiaut026,4);
    pmt->adipre027[0] = pfi->fidatiso.fiisoaut.fiaut027;

    strncpy(pmt->adipre028     ,pfi->fidatiso.fiisoaut.fiaut028.aa,2);
    strncpy(pmt->adipre028 + 2 ,pfi->fidatiso.fiisoaut.fiaut028.mm,2);
    strncpy(pmt->adipre028 + 4 ,pfi->fidatiso.fiisoaut.fiaut028.dd,2);

    strncpy(pmt->adipre029,pfi->fidatiso.fiisoaut.fiaut029,3);

    //en operaciones no autorizadas (DMND0036176 las preautorizaciones estan añadidas)
    if ( (( strcmp(pfi->fiisocom.fiiso000,"1110") == 0 ||
           strcmp(pfi->fiisocom.fiiso000,"1210") == 0 
		  )&&
           (strncmp(pfi->fiisocom.respusal,"0",1) != 0)  //las autorizaciones aceptadas tienen p39 que empieza por 0
          )||
         (( strcmp(pfi->fiisocom.fiiso000,"1130") == 0 || 
          strcmp(pfi->fiisocom.fiiso000,"1230") == 0 ||
		  strcmp(pfi->fiisocom.fiiso000,"1134") == 0 ||
		  strcmp(pfi->fiisocom.fiiso000,"1234") == 0
		  ) &&
          strcmp(pfi->fiisocom.respusal,"900")  !=0 
		 ) ||
         ( strcmp(pfi->fiisocom.fiiso000,"1430") == 0 || 
		   strcmp(pfi->fiisocom.fiiso000,"1434") == 0
         )&&
         strcmp(pfi->fiisocom.respusal,"400") !=0)
    {
        strncpy(pmt->adipre030,pmt->adipre004,12);
		strncpy(pmt->adipre030 + 12,pmt->adipre005,12);
        memset(pmt->adipre004 , '0' , 12 );
		memset(pmt->adipre005 , '0' , 12 );
        memset(pmt->adipre038 , ' ' , 6 );
    }
    else
    {
        memset(pmt->adipre030 , '0' , 24 );
        strncpy(pmt->adipre038,pfi->fidatiso.fiisoaut.fiaut038,6);
    }

    strncpy(pmt->adi031l,pfi->fidatiso.fiisoaut.fiaut031.longitud,2);
    memcpy(pmt->adi031d,pfi->fidatiso.fiisoaut.fiaut031.datos,37);

		/// respuestas a la red (somos emisor y resolutor) 
		/// INFORMAMOS EL P-33 CON csb de la entidad resolutora 
		//DMND0036176 se añaden preautorizaciones
    if(!strcmp(pfi->fiisocom.fiiso000,"1110") ||
       !strcmp(pfi->fiisocom.fiiso000,"1130") ||
       !strcmp(pfi->fiisocom.fiiso000,"1210") ||
       !strcmp(pfi->fiisocom.fiiso000,"1230") ||
       !strcmp(pfi->fiisocom.fiiso000,"1430"))
    {
        strcpy(pmt->adi033l, "06");
        memset(pmt->adi033d, ' ', 11);
       
       //RAQUEL se recuperan los datos por bbdd y entidad
     	for (ll=0 ; ll<2 ; ll++) 
     	{  
       	if ((strncmp(reg_para[ll].psnument,pfi->fidatpro.fiorigen,2)==0))
       	{	
           strncpy(pmt->adi033d, "02", 2);
           strncpy(pmt->adi033d+2,reg_para[ll].pscsb000, 6);	
           break;            
       	}  
     	} 
  	}
		///// peticiones a la red desde nuestros cajeros (somos adquirentes)
		//// DMND0036176 se añaden preautorizaciones
		////  NO INFORMAMOS EL P-33 EN V.2 DE PRICE YA QUE COINCIDE CON EL P-32
    if(!strcmp(pfi->fiisocom.fiiso000,"1100") ||
       !strcmp(pfi->fiisocom.fiiso000,"1200") ||
       !strcmp(pfi->fiisocom.fiiso000,"1120") ||
       !strcmp(pfi->fiisocom.fiiso000,"1220") ||
       !strcmp(pfi->fiisocom.fiiso000,"1420"))
    {
        strcpy(pmt->adi033l, "00");
        memset(pmt->adi033d, '0', 11);
    }

		/// adquirente merchant respuesta a la comunicación 
		/// P33 = Adquieriente
		// DMND0036176 se añaden preautorizaciones
    if(!strcmp(pfi->fiisocom.fiiso000,"1134") ||
       !strcmp(pfi->fiisocom.fiiso000,"1234") ||
       !strcmp(pfi->fiisocom.fiiso000,"1434"))
    {
    	strcpy(pmt->adi033l, "06");
      memset(pmt->adi033d, ' ', 11);

	   //RAQUEL se modifica para recoger el dato por bbdd y entidad

      for (ll=0 ; ll<2 ; ll++) 
     	{  
       	if ((strncmp(reg_para[ll].psnument,pfi->fidatpro.fiorigen,2)==0))
       	{	
        	strncpy(pmt->adi033d, "01", 2); //01-resolutor
					strncpy(pmt->adi033d+2, reg_para[ll].pscsb000, 4); //cierre de entidad por tabla
					break;
	   		}    	
	 		}
		}	   
    strncpy(pmt->adi035l  ,pfi->fidatiso.fiisoaut.fiaut035.longitud,2);
    memcpy(pmt->adi035d  ,pfi->fidatiso.fiisoaut.fiaut035.datos,37);


    if(!strcmp(pfi->fidatpro.fisubres, "08"))
    {
        memset(pmt->adipre037, ' ', 12);
        memcpy(pmt->adipre037,pfi->fidatiso.fiisoaut.fiaut037 + 8, 4);
    }
    else
    {
        memcpy(pmt->adipre037,pfi->fidatiso.fiisoaut.fiaut037,12);
    }

  	memcpy(pmt->adipre040,pfi->fidatiso.fiisoaut.fiaut040,3);

    memcpy(pmt->adipre041,pfi->fidatiso.fiisoaut.fiaut041,8);

    strncpy(pmt->adipre042,pfi->fidatiso.fiisoaut.fiaut042, 15);

    strncpy(pmt->adipre043,pfi->fidatiso.fiisoaut.fiaut043, 40);

    strncpy(pmt->adi044l  ,pfi->fidatiso.fiisoaut.fiaut044.longitud,2);
    memcpy(pmt->adi044d  ,pfi->fidatiso.fiisoaut.fiaut044.datos,64);

    strncpy(pmt->adi045l  ,pfi->fidatiso.fiisoaut.fiaut045.longitud,2);
    memcpy(pmt->adi045d  ,pfi->fidatiso.fiisoaut.fiaut045.datos,76);

    strncpy(pmt->adi046l  ,pfi->fidatiso.fiisoaut.fiaut046.longitud,2);
    memcpy(pmt->adi046d  ,pfi->fidatiso.fiisoaut.fiaut046.datos,44);  

    /***   48--   ***/
    /***---DATOS P-48 COGER LOS CAMPOS CORRESPONDIENTES Y MONTAR
     ***---EL CAMPO A ENVIAR SEGUN LA OPERACION DE QUE SE TRATE
    */
  
        //fprintf(stdout, "fiaut048:\n");
    conta1 = 0;
    /*-- 01 OFFSET **/ //cambio de pin
		// DMND0036176 se añaden preautorizaciones
		
    if((!strcmp(pfi->fiisocom.fiiso000,"1100") ||
        !strcmp(pfi->fiisocom.fiiso000,"1200"))   &&
        strncmp(pfi->fidatiso.fiisoaut.fiaut048.comun01.offset,"            ", 12) &&
        !strncmp(pfi->fidatiso.fiisoaut.fiaut053+4,"25",2)) 
    { 
        strncpy(pmt->adi048d + conta1, "014", 3);
        strncpy(pmt->adi048d + conta1 + 3, "01", 2);
        strncpy(pmt->adi048d + conta1 + 5, pfi->fidatiso.fiisoaut.fiaut048.comun01.offset, 12);
        conta1 += 17;
    }

    /*-- 02 IDIOMA **/ //camibio de idioma
    // DMND0036176 se añaden preautorizaciones
    if((!strcmp(pfi->fiisocom.fiiso000,"1100") ||
        !strcmp(pfi->fiisocom.fiiso000,"1200"))   &&
        strncmp(pfi->fidatiso.fiisoaut.fiaut048.comun01.idioma,"  ", 2) &&
        !strncmp(pfi->fidatiso.fiisoaut.fiaut053+4,"25",2)) 
    { 
        strncpy(pmt->adi048d + conta1, "004", 3);
        strncpy(pmt->adi048d + conta1 + 3, "02", 2);
        strncpy(pmt->adi048d + conta1 + 5, pfi->fidatiso.fiisoaut.fiaut048.comun01.idioma, 2);
        conta1 += 7;
    }

    /*-- 03 NEW OFFSET **/ //cambio pin
	// DMND0036176 se añaden preautorizaciones
    if(!strcmp(pfi->fiisocom.fiiso000,"1100") ||
       !strcmp(pfi->fiisocom.fiiso000,"1200") ||
       !strcmp(pfi->fiisocom.fiiso000,"1120") ||
       !strcmp(pfi->fiisocom.fiiso000,"1220") ||
       !strcmp(pfi->fiisocom.fiiso000,"1121") ||
       !strcmp(pfi->fiisocom.fiiso000,"1221")) 
    { 
       if (!strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig12, "90", 2))
       {
         if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.noeco.nuevof, "            ", 12))
         //nuevo offset relleno
        {
           strncpy(pmt->adi048d + conta1, "014", 3);
           strncpy(pmt->adi048d + conta1 + 3, "03", 2);
           strncpy(pmt->adi048d + conta1 + 5, pfi->fidatiso.fiisoaut.fiaut048.noeco.nuevof, 12);
           conta1 += 17;
        }
        else
        {
        //puede ocurrir que el nuevo offset este en el bloque de pin, por ser teclado seguro, asi que hay que 
        //hacer previamente un calculo de pin, pero solo si la tarjeta es nuestra, pero mejor hacerlo previamente
        // y aqui llegar con el dato calculado
        //calculo offset
           strncpy(pmt->adi048d + conta1, "014", 3);
           strncpy(pmt->adi048d + conta1 + 3, "03", 2);
           strncpy(pmt->adi048d + conta1 + 5, pfi->fidatiso.fiisoaut.fiaut048.noeco.nuevof, 12);
           conta1 += 17;
        }
      }
    }

    /*-- 04 NEW IDIOMA **/ //cambio de idioma
		// DMND0036176 se añaden preautorizaciones
    if (!strcmp(pfi->fiisocom.fiiso000,"1100") ||
        !strcmp(pfi->fiisocom.fiiso000,"1200") ||
        !strcmp(pfi->fiisocom.fiiso000,"1120") ||
        !strcmp(pfi->fiisocom.fiiso000,"1220") ||
        !strcmp(pfi->fiisocom.fiiso000,"1121") ||
        !strcmp(pfi->fiisocom.fiiso000,"1221"))
    {
       if (!strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig12, "90", 2) &&
            strncmp(pfi->fidatiso.fiisoaut.fiaut048.noeco.nuevid, "  ", 2))
       {
        strncpy(pmt->adi048d + conta1, "004", 3);
        strncpy(pmt->adi048d + conta1 + 3, "04", 2);
        strncpy(pmt->adi048d + conta1 + 5, pfi->fidatiso.fiisoaut.fiaut048.noeco.nuevid, 2);
        conta1 += 7;
       }
    }

    /*--05 SALDOS ***/ //consulta de saldos

      strncpy(cantidad_saldos,pfi->fidatiso.fiisoaut.fiaut048.augen.saldos.num_ctas,1);
      cantidad_saldos[1]='\0';
      num_cuentas = atoi(cantidad_saldos); 
      fprintf(stdout,"SALDOS Numctas <%i>\n", num_cuentas);  

    if((!strcmp(pfi->fiisocom.fiiso000,"1110") ||
        !strcmp(pfi->fiisocom.fiiso000,"1210"))   &&
       (!strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig12, "30", 2) || (num_cuentas==1)))
     {
#if defined (TRAZA) && (TRAZA > 10)	
        fprintf(stdout, "conta1 >%d<\n", conta1);  
#endif	

        pone_saldos(pmt->adi048d + conta1, &pfi->fidatiso.fiisoaut.fiaut048.augen.saldos);
        conta1 += 29;
#if defined (TRAZA) && (TRAZA > 10)		
        fprintf(stdout, "pmt->adi048d:\n");
        fhexdump(stdout, pmt->adi048d, sizeof(pmt->adi048d));
#endif	
 
     }
    
    /*--06 MOVIMIENTOS ***/ //consulta de movimientos
		//	// DMND0036176 se añaden preautorizaciones
    if((!strcmp(pfi->fiisocom.fiiso000,"1110") ||
        !strcmp(pfi->fiisocom.fiiso000,"1210"))   &&
       !strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig12, "31", 2) &&
       strncmp(pfi->fidatiso.fiisoaut.fiaut048.movim.sub04806.moneda, "   " , 3))
    {
#if defined (TRAZA) && (TRAZA > 10)	
        fprintf(stdout, "conta1 >%d<\n", conta1);
#endif		
        
        pone_movimientos(pmt->adi048d + conta1, &pfi->fidatiso.fiisoaut.fiaut048.movim.sub04806);
#if defined (TRAZA) && (TRAZA > 10)		
        fprintf(stdout, "pmt->adi048d:\n");
        fhexdump(stdout, pmt->adi048d, sizeof(pmt->adi048d));
#endif	
  
      conta1 += 28;
   
      strncpy(cantidad,pfi->fidatiso.fiisoaut.fiaut048.movim.sub04806.num_movi,1);
       cantidad[1]='\0';
      num_movimientos = atoi(cantidad); 
     
      for (i = 0;i<num_movimientos;i++)
      {
        conta1 += 34; 
      }	
     
    }

    /*-- 09 TERMINAL -- */
		// DMND0036176 se añaden preautorizaciones
    if((!strcmp(pfi->fiisocom.fiiso000,"1100") ||
        !strcmp(pfi->fiisocom.fiiso000,"1200") ||
        !strcmp(pfi->fiisocom.fiiso000,"1120") ||
        !strcmp(pfi->fiisocom.fiiso000,"1220") ||
        !strcmp(pfi->fiisocom.fiiso000,"1121") ||
        !strcmp(pfi->fiisocom.fiiso000,"1221"))   &&
       (!strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig12,"00",2) &&
        !strncmp(pfi->fidatpro.fiorigen, "07", 2)))//cajero
    {
        strncpy(pmt->adi048d + conta1, "014", 3);
        strncpy(pmt->adi048d + conta1 + 3, "09", 2);
        strncpy(pmt->adi048d + conta1 + 5, pfi->fidatiso.fiisoaut.fiaut048.comun01.termin, 12);
        conta1 += 17;
    }
 
    /*-- 12 BLOQUE DE MOVIMIENTOS --*/
		// DMND0036176 se añaden preautorizaciones
    if ((!strcmp(pfi->fiisocom.fiiso000,"1100") ||
         !strcmp(pfi->fiisocom.fiiso000,"1200")) &&
        !strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig12,"31",2))
    {
        strncpy(pmt->adi048d + conta1, "004", 3);
        strncpy(pmt->adi048d + conta1 + 3, "12", 2);
        strncpy(pmt->adi048d + conta1 + 5, pfi->fidatiso.fiisoaut.fiaut048.movim.bloqmo, 2);
        conta1 += 7;
    }

    /*-- 24 RECARGAS TELEFONICAS  --*/
	// DMND0036176 se añaden preautorizaciones
    if ((!strcmp(pfi->fiisocom.fiiso000,"1100") ||
         !strcmp(pfi->fiisocom.fiiso000,"1200") ||
         !strcmp(pfi->fiisocom.fiiso000,"1120") ||
         !strcmp(pfi->fiisocom.fiiso000,"1220") ||
         !strcmp(pfi->fiisocom.fiiso000,"1121") ||
         !strcmp(pfi->fiisocom.fiiso000,"1221")) &&
        (!strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig12,"00",2) &&
         !strncmp(pfi->fidatpro.fiorigen, "07", 2)))//cajero
    {
        strncpy(pmt->adi048d + conta1, "011", 3);
        strncpy(pmt->adi048d + conta1 + 3, "24", 2);
        strncpy(pmt->adi048d + conta1 + 5, pfi->fidatiso.fiisoaut.fiaut048.comun01.telefo, 9);
        conta1 += 14;
    }
	
	    /*-- 26 TIPO DE COMPENSACION -- **/
		// en transacciones contables (respuesta a peticion ok)
    if ( !strncmp(pfi->fiisocom.fiiso000,"1200",4) ||
	    (!strncmp(pfi->fiisocom.fiiso000,"1210",4) && !strncmp(pfi->fidatpro.ficodfin,"000",3)) || 
		 !strncmp(pfi->fiisocom.fiiso000,"1220",4) ||   		
		 !strncmp(pfi->fiisocom.fiiso000,"1221",4) ||  		 
		 !strncmp(pfi->fiisocom.fiiso000,"1224",4) ||  
		 !strncmp(pfi->fiisocom.fiiso000,"1225",4) ||	 
		 !strncmp(pfi->fiisocom.fiiso000,"1230",4) ||		 
         !strncmp(pfi->fiisocom.fiiso000,"1420",4) ||  
         !strncmp(pfi->fiisocom.fiiso000,"1421",4) ||  
         !strncmp(pfi->fiisocom.fiiso000,"1424",4) ||  
         !strncmp(pfi->fiisocom.fiiso000,"1425",4) ||  
         !strncmp(pfi->fiisocom.fiiso000,"1430",4))
    {	   
        strncpy(pmt->adi048d + conta1, "003", 3);
        strncpy(pmt->adi048d + conta1 + 3, "26", 2);
        strncpy(pmt->adi048d + conta1 + 5, pfi->fidatiso.fiisoaut.fiaut048.comun01.compen, 1);
        conta1 += 6;
    }
    

   	/*-- 31 COMISION --*/
   	//añadido para corregir comisiones 
   	//PTASK000015994   PTASK000016608  PTASK000015967   		
		if(strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig12, "01",2)==0) //PRJ0387060 INI
		{
			printf("* * * Operacion de cajero, informar p48.31 * * * Comision %.8s\n",pfi->fidatpro.ficomisi);
	  	if(strncmp(pfi->fidatpro.ficomisi, "        ",8)!=0) // nos tienen que haber pasado la comisión
			{   
				if(strncmp(pfi->fidatpro.fitipcom,"1",1)==0 || strncmp(pfi->fidatpro.fitipcom,"2",1)==0)
				{
				  strncpy(pmt->adi048d + conta1, "014",3);
				  strncpy(pmt->adi048d + conta1 + 3, "31",2);
				  strncpy(pmt->adi048d + conta1 + 5, pfi->fidatpro.fitipcom,1);
				  strncpy(pmt->adi048d + conta1 + 6, pfi->fidatpro.ficomisi,8);
				  //por ahora metemos a pelo la moneda de la comision
				  strncpy(pmt->adi048d + conta1 + 14, "978",3);
	        conta1 += 17;
				}
				else
				{
					strncpy(pmt->adi048d + conta1, "014",3);
					strncpy(pmt->adi048d + conta1 + 3, "31",2);
					strncpy(pmt->adi048d + conta1 + 5, "0", 1);
					strncpy(pmt->adi048d + conta1 + 6, "00000000",8);
					strncpy(pmt->adi048d + conta1 + 14,"978",3);
					conta1 += 17;
				}
			}    
			else
			{
				strncpy(pmt->adi048d + conta1, "014",3);
				strncpy(pmt->adi048d + conta1 + 3, "31",2);
				strncpy(pmt->adi048d + conta1 + 5, "0", 1);
				strncpy(pmt->adi048d + conta1 + 6, "00000000",8);
				strncpy(pmt->adi048d + conta1 + 14,"978",3);
				conta1 += 17;
			}
		} //PRJ0387060 FIN
		
		/*--44 BIZUM adi103 adi102 adipre48.44  */
   	if (( !strncmp(pfi->fiisocom.fiiso000,"1100",4)  || 
         !strncmp(pfi->fiisocom.fiiso000,"1110",4)  || 
         !strncmp(pfi->fiisocom.fiiso000,"1120",4)  || 
	     !strncmp(pfi->fiisocom.fiiso000,"1130",4)  || 
	     !strncmp(pfi->fiisocom.fiiso000,"1121",4)  || 
	     !strncmp(pfi->fiisocom.fiiso000,"1420",4)  || 
	     !strncmp(pfi->fiisocom.fiiso000,"1430",4)) && 
         (!strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig12, "10", 2) || !strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig12, "29", 2)))       
    {
				printf(" ftradred.c ftrapre2- Datos fiaut102 >%d<  >%.*s< fiaut103 >%d< >%.*s<\n",
				atoi(pfi->fidatiso.fiisoaut.fiaut102.longitud),
				atoi(pfi->fidatiso.fiisoaut.fiaut102.longitud),
				pfi->fidatiso.fiisoaut.fiaut102.datos,
				atoi(pfi->fidatiso.fiisoaut.fiaut103.longitud),
				atoi(pfi->fidatiso.fiisoaut.fiaut103.longitud),
				pfi->fidatiso.fiisoaut.fiaut103.datos );        //Bizum

				strncpy(pmt->adi102l,pfi->fidatiso.fiisoaut.fiaut102.longitud,2);
				pmt->adi102l[2] = '\0';
				//memcpy(pmt->adi103d,pfi->fidatiso.fiisoaut.fiaut102.datos,28);
				memcpy(pmt->adi102d,pfi->fidatiso.fiisoaut.fiaut102.datos,atoi(pfi->fidatiso.fiisoaut.fiaut102.longitud));
				pmt->adi102d[28] = '\0';
				strncpy(pmt->adi103l,pfi->fidatiso.fiisoaut.fiaut103.longitud,2);
				pmt->adi103l[2] = '\0';
				//memcpy(pmt->adi103d,pfi->fidatiso.fiisoaut.fiaut103.datos,28);
				memcpy(pmt->adi103d,pfi->fidatiso.fiisoaut.fiaut103.datos,atoi(pfi->fidatiso.fiisoaut.fiaut103.longitud));
				pmt->adi103d[28] = '\0';
				printf(" ftradred.c ftrapre2- Datos adi102l >%d< adi102d >%.*s< adi103l >%d< adi103d >%.*s<\n", 
	         atoi(pmt->adi102l), atoi(pmt->adi102l), pmt->adi102d,
	         atoi(pmt->adi103l), atoi(pmt->adi103l), pmt->adi103d);    //Bizum
	         
	      // Primero calcula la longitud de los campos que han llegado
	      conta2 = 0;
	      conta3 = 0;
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484402, "    ", 4))
	      {
	      	conta2 = conta2 + sizeof(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484402) + 3;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484403, "    ", 4))
	      {
		      conta2 = conta2 + sizeof(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484403) + 3;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484405, "    ", 4))
	      {
		      conta2 = conta2 + sizeof(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484405) + 3;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484406, "    ", 4))
	      {
		      conta2 = conta2 + sizeof(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484406) + 3;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484407, "    ", 4))
	      {
		      conta2 = conta2 + sizeof(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484407) + 3;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484408, "    ", 4))
	      {
	      	conta2 = conta2 + sizeof(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484408) + 3;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484409, "    ", 4))
	      {
		      conta2 = conta2 + sizeof(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484409) + 3;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484410, "    ", 4))
	      {
	      	conta2 = conta2 + sizeof(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484410) + 3;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484412, "    ", 4))
	      {
		      conta2 = conta2 + sizeof(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484412) + 3; 
		    }
		    conta2 = conta2 + 2; //Se suman 2 del 44 inicial
		    
	      fprintLog(nomProg,"BIZUM 48.44 subcampos 02-03-05-06-07-08-09-10-12 - long conta2 %d\n", conta2);        
	      strncpy(pmt->adi048d + conta1, itoa(conta2), 3); // BIZUM longitud        
	      strncpy(pmt->adi048d + conta1 + 3, "44", 2);// BIZUM valor subcampo 44 ???
	      conta3 = 5;
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484402, "    ", 4))
	      {
	      	strncpy(pmt->adi048d + conta1 + conta3, "3702", 4);
	      	strncpy(pmt->adi048d + conta1 + conta3 + 4, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484402, 35);
	      	conta3 = conta3 + 39;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484403, "    ", 4))
	      {
	      	strncpy(pmt->adi048d + conta1 + conta3, "2803", 4);
		      strncpy(pmt->adi048d + conta1 + conta3 + 4, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484403, 26);
	        conta3 = conta3 + 30;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484405, "    ", 4))
	      {
	      	strncpy(pmt->adi048d + conta1 + conta3, "1405", 4);
		    	strncpy(pmt->adi048d + conta1 + conta3 + 4, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484405, 12);
	      	conta3 = conta3 + 16;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484406, "    ", 4))
	      {
	      	strncpy(pmt->adi048d + conta1 + conta3, "1506", 4);
	      	strncpy(pmt->adi048d + conta1 + conta3 + 4, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484406, 13);
	      	conta3 = conta3 + 17;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484407, "    ", 4))
	      {
	      	strncpy(pmt->adi048d + conta1 + conta3, "1507", 4);
	      	strncpy(pmt->adi048d + conta1 + conta3 + 4, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484407, 13);
	      	conta3 = conta3 + 17;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484408, "    ", 4))
	      {
	      	strncpy(pmt->adi048d + conta1 + conta3, "1608", 4);
	      	strncpy(pmt->adi048d + conta1 + conta3 + 4, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484408, 14);
	      	conta3 = conta3 + 18;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484409, "    ", 4))
	      {
	      	strncpy(pmt->adi048d + conta1 + conta3, "1609", 4);
	      	strncpy(pmt->adi048d + conta1 + conta3 + 4, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484409, 14);
	      	conta3 = conta3 + 18;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484410, "    ", 4))
	      {
	      	strncpy(pmt->adi048d + conta1 + conta3, "3710", 4);
	      	strncpy(pmt->adi048d + conta1 + conta3 + 4, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484410, 35);
	      	conta3 = conta3 + 39;
	      }
	      if (strncmp(pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484412, "    ", 4))
	      {
	      	strncpy(pmt->adi048d + conta1 + conta3, "7212", 4);
	      	strncpy(pmt->adi048d + conta1 + conta3 + 4, pfi->fidatiso.fiisoaut.fiaut048.fiiso4844sub.fiiso484412, 70);
	       	conta3 = conta3 + 74;
	      }
	     
	      conta1 = conta1 + conta2;
			}
    
        /*--33 TRANSFERENCIAS   adi103 adi102 adipre48.61 adipre018 adipre026***/ 
   		if (( !strncmp(pfi->fiisocom.fiiso000,"1210",4)  || !strncmp(pfi->fiisocom.fiiso000,"1230",4)) && 
         (!strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig12, "40", 2) ||!strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig12, "49", 2)))       
    	{
    
        strncpy(pmt->adi102l,pfi->fidatiso.fiisoaut.fiaut102.longitud,2);
        pmt->adi102l[2] = '\0';
        memcpy(pmt->adi102d,pfi->fidatiso.fiisoaut.fiaut102.datos,28);
        pmt->adi102d[28] = '\0';
        strncpy(pmt->adi103l,pfi->fidatiso.fiisoaut.fiaut103.longitud,2);
        pmt->adi103l[2] = '\0';
        memcpy(pmt->adi103d,pfi->fidatiso.fiisoaut.fiaut103.datos,28);
        pmt->adi103d[28] = '\0';
        
        memcpy(pmt->adipre128,pfi->fidatiso.fiisoaut.fiaut128,8); 
        pmt->adipre128[8] = '\0';
       
        memcpy(pmt->adipre018,pfi->fidatiso.fiisoaut.fiaut018,4); 
        pmt->adipre018[4] = '\0';
        
        memcpy(pmt->adipre026,pfi->fidatiso.fiisoaut.fiaut026,4); 
        pmt->adipre026[4] = '\0';

        strncpy(pmt->adi048d + conta1, "012", 3);
        strncpy(pmt->adi048d + conta1 + 3, "61", 2);
        strncpy(pmt->adi048d + conta1 + 5, pfi->fidatiso.fiisoaut.fiaut048.comun01.fiiso4861,10);
        conta1 += 15;

    	}
	

	    /*-- 93 IMPORTE DE AUTORIZACION PARCIAL --*/
			// DMND0036176 se añaden preautorizaciones
	    if (!strncmp(pfi->fiisocom.fiiso000,"1110",4) &&
	        !strncmp(pfi->fidatiso.fiisoaut.fiaut003.dig12,"00",2) &&
			!strncmp(pfi->fidatiso.fiisoaut.fiaut048.comun01.flagImpParcial, "1", 1) &&
			!strncmp(pfi->fiisocom.respusal, "010",3) &&
			ncampo_numerico(pfi->fidatiso.fiisoaut.fiaut048.comun01.importeParcial,12)==1)
	    {
	        fprintLog(nomProg,"IMPORTE AUTORIZACION PARCIAL 48.93\n");
	        strncpy(pmt->adi048d + conta1, "014", 3);
	        strncpy(pmt->adi048d + conta1 + 3, "93", 2);
	        strncpy(pmt->adi048d + conta1 + 5, pfi->fidatiso.fiisoaut.fiaut048.comun01.importeParcial, 12);
	        conta1 += 17;
	    }
		

	    if(conta1)
	    {
	        sprintf(char04, "%03d", conta1);
	        strncpy(pmt->adi048l, char04, 3);
	    }

	    strncpy(pmt->adipre049,pfi->fidatiso.fiisoaut.fiaut049,3);
	    strncpy(pmt->adipre050,pfi->fidatiso.fiisoaut.fiaut050,3);
	    memcpy(pmt->adipre051,pfi->fidatiso.fiisoaut.fiaut051,3);
	    /***   es tipo binario, y puede tener caracteres no deseados   ***/
	    memcpy(pmt->adipre052,pfi->fidatiso.fiisoaut.fiaut052,16); //PRJ0258542 - pasa de 8 a 16

	    strncpy(pmt->adipre053, pfi->fidatiso.fiisoaut.fiaut053,16); //PRJ0258542 - pasa de 16 a 24, pero se mantiene a 16 en estos mensajes
	    strncpy(pmt->adi056l, pfi->fidatiso.fiisoaut.fiaut056.longitud,2);
	    memcpy(pmt->adi056d, pfi->fidatiso.fiisoaut.fiaut056.datos,35);
	    strncpy(pmt->adi058l, pfi->fidatiso.fiisoaut.fiaut058.longitud,2);
	    memcpy(pmt->adi058d, pfi->fidatiso.fiisoaut.fiaut058.datos,11);

	    /***   es tipo binario, y puede tener caracteres no deseados   ***/
	    memcpy(pmt->adipre064,pfi->fidatiso.fiisoaut.fiaut064,8);

	    strncpy(pmt->adi032l  ,pfi->fiisocom.fi032094.longitud,2);
	    memcpy(pmt->adi032d  ,pfi->fiisocom.fi032094.datos,11);
		
	
		  /***     55--    ***/
			pfi->fidatiso.fiisoaut.fiaut055.longitud[3]='\0';
	    if(((pfi->fidatiso.fiisoaut.fiaut022[6] == '5') ||
				(pfi->fidatiso.fiisoaut.fiaut022[6] == 'M')   )&&//tarjeta chip emv
	      (atoi(pfi->fidatiso.fiisoaut.fiaut055.longitud) != 0))
	    {
	      memset(ch_aux, '0', sizeof(ch_aux));
		  
		  	montaR55(pfi, ch_aux, &Long_R55);
	    	//fprintf(stdout,"ch_aux en ftradred de longitud %d\n", Long_R55);
	      //fhexdump(stdout, ch_aux , Long_R55);
	      //fflush(stdout);
		  

	    if (Long_R55)
      {
				//20160202 memcpy en vez de strncpy pq esta empaquetado
				memcpy(pmt->adi055d,pfi->fidatiso.fiisoaut.fiaut055.carcontrol, 1);		
				pmt->adi055d[0]=0x02;  ///////// CARCONTROL deberia ser 02 pero en la traza vemos que tenemos 20 (espacios)
				///////////////////////////////  pongo 02 hasta que averigue por que.
        memset(Long_R55alf,'\0',4);

        //esta longitud es la longitud en hexadecimal y empaquetada.
        bin2hex(Long_R55alf, Long_R55,&tam);

        memset(longalf_55, '0', 4);
        memcpy(longalf_55 + 4 - tam, Long_R55alf, tam);

        fempapin(4, pmt->adi055d + 1, longalf_55);

				//20160216 - cambio strncpy por memcpy pq al estar empaquetado puede haber caracteres que corten la cadena
				memcpy(pmt->adi055d + 3, ch_aux, Long_R55);
		
				//fprintf(stdout,"p55 en ftradred\n");
        //fhexdump(stdout,pmt->adi055d ,Long_R55 +3);
        //fflush(stdout);

        //Long total del campo.
        Long_R55 = Long_R55 + 3;

        memset(ch_aux,'0',3);
        sprintf(ch_aux, "%03d", Long_R55);
        strncpy(pmt->adi055l,ch_aux ,3);
    	}
   	}
  } // fin iso000 11 12 14
  

  if (strncmp(pfi->fiisocom.fiiso000,"13",2) == 0)
  {
    fcomunes2(pfi,pmt);
    strncpy(pmt->adi002l  ,pfi->fidatiso.fiisofic.fific002.longitud,2);
    memcpy(pmt->adi002d  ,pfi->fidatiso.fiisofic.fific002.datos,19);
    strncpy(pmt->adipre053,pfi->fidatiso.fiisofic.fific053,16);
    strncpy(pmt->adi072l  ,pfi->fidatiso.fiisofic.fific072.longitud,3);
    memcpy(pmt->adi072d  ,pfi->fidatiso.fiisofic.fific072.datos,256);
    strncpy(pmt->adi093l  ,pfi->fidatiso.fiisofic.fific093.longitud,2);
    memcpy(pmt->adi093d  ,pfi->fidatiso.fiisofic.fific093.datos,11);
    strncpy(pmt->adi101l  ,pfi->fidatiso.fiisofic.fific101.longitud,2);
    memcpy(pmt->adi101d  ,pfi->fidatiso.fiisofic.fific101.datos,17);

    /***   es tipo binario, y puede tener caracteres no deseados   ***/
    memcpy(pmt->adipre128,pfi->fidatiso.fiisofic.fific128,8);

    strncpy(pmt->adi094l  ,pfi->fiisocom.fi032094.longitud,2);
    memcpy(pmt->adi094d  ,pfi->fiisocom.fi032094.datos,11);
  }

  if (strncmp(pfi->fiisocom.fiiso000,"1644",4) == 0)
  {
    fcomunes2(pfi,pmt);
    strncpy(pmt->adi072l  ,pfi->fidatiso.fiisoadm.fiadm072.longitud,3);
    memcpy(pmt->adi072d  ,pfi->fidatiso.fiisoadm.fiadm072.datos,256);
    strncpy(pmt->adi093l  ,pfi->fidatiso.fiisoadm.fiadm093.longitud,2);
    memcpy(pmt->adi093d  ,pfi->fidatiso.fiisoadm.fiadm093.datos,11);
    strncpy(pmt->adi094l  ,pfi->fiisocom.fi032094.longitud,2);
    memcpy(pmt->adi094d  ,pfi->fiisocom.fi032094.datos,11);
  }

	//PRJ0258452 - Meter If para controlar mensaje de control de claves, el else sigue haciendo lo que hacía
	
  if (strncmp(pfi->fiisocom.fiiso000,"18",2) == 0 && (strncmp(pfi->fiisocom.fiiso024,"811",3) == 0 || strncmp(pfi->fiisocom.fiiso024,"815",3) == 0 || strncmp(pfi->fiisocom.fiiso024,"816",3) == 0))
  {
  	fprintf(stdout,"ftrapre2 . Valor fiiso000 y fiiso024 >%.4s< y >%.3s<\n", pfi->fiisocom.fiiso000, pfi->fiisocom.fiiso024);
  	//fcomunes2(pfi,pmt); //PRJ0258542
  	fprintf(stdout,"Valores pfi 00, 11, 12, 24, 25, 33, 39, 53, 93, 94: >%.4s<>%.6s<>%.12s<>%.3s<>%.4s<>%.2s<>%.11s<>%.3s<>%.2s<>%.8s<>%.2s<>%.4s<>%.2s<>%.6s<>%.2s<>%.11s<>%.2s<>%.11s<>%.8s<\n",
  									pfi->fiisocom.fiiso000,pfi->fidatiso.fiisocla.ficla011,pfi->fidatiso.fiisocla.ficla012,pfi->fidatiso.fiisocla.ficla024,pfi->fidatiso.fiisocla.ficla025,
  									pfi->fidatiso.fiisocla.ficla033.longitud,pfi->fidatiso.fiisocla.ficla033.datos,pfi->fidatiso.fiisocla.ficla039,
  									pfi->fidatiso.fiisocla.ficla053.control, pfi->fidatiso.fiisocla.ficla053.identificador, pfi->fidatiso.fiisocla.ficla053.algoritmo, 
										pfi->fidatiso.fiisocla.ficla053.longitud, pfi->fidatiso.fiisocla.ficla053.proteccion, pfi->fidatiso.fiisocla.ficla053.resto,
  									pfi->fidatiso.fiisocla.ficla093.longitud,pfi->fidatiso.fiisocla.ficla093.datos,pfi->fidatiso.fiisocla.ficla094.longitud,pfi->fidatiso.fiisocla.ficla094.datos,pfi->fidatiso.fiisocla.ficlamac);
    strncpy(pmt->adipre000,pfi->fiisocom.fiiso000,4); //PRJ0258542
    strncpy(pmt->adipre011, pfi->fidatiso.fiisocla.ficla011,6); //PRJ0258542
    strncpy(pmt->adipre012, pfi->fidatiso.fiisocla.ficla012,12); //PRJ0258542
    strncpy(pmt->adipre024,pfi->fidatiso.fiisocla.ficla024,3); //PRJ0258542
    strncpy(pmt->adipre025,pfi->fidatiso.fiisocla.ficla025,4); //PRJ0258542
    strncpy(pmt->adi033l, pfi->fidatiso.fiisocla.ficla033.longitud,2); //PRJ0258542
    memcpy(pmt->adi033d, pfi->fidatiso.fiisocla.ficla033.datos,11); //PRJ0258542
    strncpy(pmt->adipre039, pfi->fidatiso.fiisocla.ficla039,3); //PRJ0258542 
    if (strncmp(pfi->fiisocom.fiiso000,"1814",4) != 0) //PRJ0258542 - Saco el P53 del envio en la respuesta del 1814
    {
	    strncpy(pmt->adipre053,    pfi->fidatiso.fiisocla.ficla053.control,2); //PRJ0258542
	    strncpy(pmt->adipre053+2,  pfi->fidatiso.fiisocla.ficla053.identificador,8); //PRJ0258542
	    strncpy(pmt->adipre053+10, pfi->fidatiso.fiisocla.ficla053.algoritmo,2); //PRJ0258542
	    strncpy(pmt->adipre053+12, pfi->fidatiso.fiisocla.ficla053.longitud,4); //PRJ0258542
	    strncpy(pmt->adipre053+16, pfi->fidatiso.fiisocla.ficla053.proteccion,2); //PRJ0258542
	    strncpy(pmt->adipre053+18, pfi->fidatiso.fiisocla.ficla053.resto,6); //PRJ0258542
    } //PRJ0258542 - Saco el P53 del envio en la respuesta del 1814
    strncpy(pmt->adi093l,	pfi->fidatiso.fiisocla.ficla093.longitud,2); //PRJ0258542
    memcpy(pmt->adi093d,	pfi->fidatiso.fiisocla.ficla093.datos,11); //PRJ0258542
    strncpy(pmt->adi094l,	pfi->fidatiso.fiisocla.ficla094.longitud,2); //PRJ0258542
    memcpy(pmt->adi094d,	pfi->fidatiso.fiisocla.ficla094.datos,11); //PRJ0258542
    if (strncmp(pfi->fiisocom.fiiso024,"816",3) == 0)
    {
    	strncpy(pmt->adi096l,	pfi->fidatiso.fiisocla.ficla096.longitud,3); //PRJ0258542
    	memcpy(pmt->adi096d,	pfi->fidatiso.fiisocla.ficla096.datos,360); //PRJ0258542
    }
    strncpy(pmt->adipre128,	pfi->fidatiso.fiisocla.ficlamac, 8); //PRJ0258542 - se inicia a ceros para luego montarlo
  	fprintf(stdout,"Valores pmt 00, 11, 12, 24, 25, 33, 39, 53, 93, 94, 128: >%.4s<>%.6s<>%.12s<>%.3s<>%.4s<>%.2s<>%.11s<>%.3s<>%.24s<>%.2s<>%.11s<>%.2s<>%.11s<>%.16s<\n",
  									pmt->adipre000,pmt->adipre011,pmt->adipre012,pmt->adipre024,pmt->adipre025,pmt->adi033l,pmt->adi033d,pmt->adipre039,pmt->adipre053,pmt->adi093l,pmt->adi093d,pmt->adi094l,pmt->adi094d,pmt->adipre128);
  }
  else
  {
  	if (strncmp(pfi->fiisocom.fiiso000,"18",2) == 0)
  	{
	    fcomunes2(pfi,pmt);
	    strncpy(pmt->adipre028     ,pfi->fidatiso.fiisoctr.fictr028.aa,2);
	    strncpy(pmt->adipre028 + 2 ,pfi->fidatiso.fiisoctr.fictr028.mm,2);
	    strncpy(pmt->adipre028 + 4 ,pfi->fidatiso.fiisoctr.fictr028.dd,2);
	    strncpy(pmt->adipre029,pfi->fidatiso.fiisoctr.fictr029,3);
	    conta1=0;
	    // para mensajes de control de cambio de características de dialogo
	    // Y SNN de aplicacion 02
	    if(!strncmp(pfi->fiisocom.fiiso000, "1804", 4))
	    {
	        if(!strncmp(pfi->fiisocom.fiiso024, "890", 3) ||
	           !strncmp(pfi->fiisocom.fiiso024, "801", 3) && suba_iso == 2)
	        {
	            conta1 = 8;
	            memcpy(pmt->adi048d  ,pfi->fidatiso.fiisoctr.fictr048.contr.cardia,8);
	        }
	    }

	    sprintf(cadena_verif, "%03d", conta1);
	    strncpy(pmt->adi048l , cadena_verif , 3);

			//      fprintf(stdout, "************ %s\n", pmt->adi048l);

			// fin para mensajes de control cambio de características de dialogo

	    strncpy(pmt->adi093l  ,pfi->fidatiso.fiisoctr.fictr093.longitud,2);
	    memcpy(pmt->adi093d  ,pfi->fidatiso.fiisoctr.fictr093.datos,11);
	    strncpy(pmt->adi094l  ,pfi->fiisocom.fi032094.longitud,2);
	    memcpy(pmt->adi094d  ,pfi->fiisocom.fi032094.datos,11);
		}
  }

  if ((strncmp(pfi->fiisocom.fiiso000,"1604",4) == 0) ||
      (strncmp(pfi->fiisocom.fiiso000,"1614",4) == 0) ||
      (strncmp(pfi->fiisocom.fiiso000,"15",2) == 0))
  {
    fcomunes2(pfi,pmt);

    strncpy(pmt->adipre028     ,pfi->fidatiso.fiisotot.fitot028.aa,2);
    strncpy(pmt->adipre028 + 2 ,pfi->fidatiso.fiisotot.fitot028.mm,2);
    strncpy(pmt->adipre028 + 4 ,pfi->fidatiso.fiisotot.fitot028.dd,2);

    strncpy(pmt->adipre029,pfi->fidatiso.fiisotot.fitot029,3);

    /*
    fprintf(stdout, "if 15xx cueabo %s\n", pfi->fidatiso.fiisotot.fitot048.cueabo);
    */
    strncpy(pmt->adipre074,pfi->fidatiso.fiisotot.fitot074,10);
    strncpy(pmt->adipre075,pfi->fidatiso.fiisotot.fitot075,10);
    strncpy(pmt->adipre076,pfi->fidatiso.fiisotot.fitot076,10);
    strncpy(pmt->adipre077,pfi->fidatiso.fiisotot.fitot077,10);
    strncpy(pmt->adipre086,pfi->fidatiso.fiisotot.fitot086,16);
    strncpy(pmt->adipre087,pfi->fidatiso.fiisotot.fitot087,16);
    strncpy(pmt->adipre088,pfi->fidatiso.fiisotot.fitot088,16);
    strncpy(pmt->adipre089,pfi->fidatiso.fiisotot.fitot089,16);
    strncpy(pmt->adipre097,pfi->fidatiso.fiisotot.fitot097,17);
    strncpy(pmt->adipre109,pfi->fidatiso.fiisotot.fitot109,16);
    strncpy(pmt->adipre110,pfi->fidatiso.fiisotot.fitot110,16);

    strncpy(pmt->adi032l  ,pfi->fiisocom.fi032094.longitud,2);
    memcpy(pmt->adi032d  ,pfi->fiisocom.fi032094.datos,11);
// aqui hay que meter el codigo del banco
    strcpy(pmt->adi033l, "06");
    memset(pmt->adi033d, ' ', 11);
 //   strncpy(pmt->adi033d, "021491", 6); // ciers triodos ORIGINAL

    //RAQUEL HAY QUE CAMBIAR POR COLUMNA TABLA NPTPARA
      for (ll=0 ; ll<2 ; ll++) 
     { 
       if ((strncmp(reg_para[ll].psnument,pfi->fidatpro.fiorigen,2)==0))
       {   
	       strncpy(pmt->adi033d, "02", 2); //02-resolutor
	       strncpy(pmt->adi033d+2, reg_para[ll].pscsb000, 4); //cierre de entidad por tabla
	   	   break;
	   }    	
  
	 }
  }

  return(0);
}


/*****************************************************************
FUNCION  FCOMUNES2             llamada por ftrapre2

DESCRIPCION: Funcion que informa los campos del formato_intermedio
comunes a todos los tipos de mensaje

AREA DE PASO :
- puntero a formato_interno donde llega el mensaje a traducir
- puntero a formato_intermedio donde volvera el mensaje traducido

* devuelve los valores :
*****************************************************************/
int fcomunes2(formato_interno *pfi, formato_intermedio *pmt)

{
    strncpy(pmt->adipre000,pfi->fiisocom.fiiso000,4);

    if (strncmp(pfi->fidatpro.fiorigen,"07",2) == 0)
        strncpy(pmt->adipre011,pfi->fidatpro.finumope,6);
    else
        strncpy(pmt->adipre011,pfi->fiisocom.fiidtran,6);

    strncpy(pmt->adipre012     ,pfi->fiisocom.fifechor.aa,2);
    strncpy(pmt->adipre012 + 2 ,pfi->fiisocom.fifechor.mm,2);
    strncpy(pmt->adipre012 + 4 ,pfi->fiisocom.fifechor.dd,2);
    strncpy(pmt->adipre012 + 6 ,pfi->fiisocom.fifechor.hh,2);
    strncpy(pmt->adipre012 + 8 ,pfi->fiisocom.fifechor.mi,2);
    strncpy(pmt->adipre012 + 10,pfi->fiisocom.fifechor.ss,2);

    strncpy(pmt->adipre024,pfi->fiisocom.fiiso024,3);
    memcpy(pmt->adipre039,pfi->fiisocom.respusal,3);

    return(0);
}


/*****************************************************************
FUNCION  FINICIALIZA_INTERMEDIO       llamada por

DESCRIPCION: Funcion que

AREA DE PASO :
-
*****************************************************************/
int finicializa_intermedio(formato_intermedio *pmt)
{

    /***  inicializa el formato intermedio a los valores correspondientes  ***/
    memset(pmt->adipre000 , '0' , 4  ); 
    memset(pmt->adi002l   , '0' , 2  );
    memset(pmt->adi002d   , ' ' , 19 );
    memset(pmt->adipre003 , ' ' , 6  );
    memset(pmt->adipre004 , '0' , 12 );
    memset(pmt->adipre005 , '0' , 12 );
    memset(pmt->adipre006 , '0' , 12 );
    memset(pmt->adipre009 , ' ' , 8 );
    memset(pmt->adipre010 , ' ' , 8  );
    memset(pmt->adipre011 , ' ' , 8  );
    memset(pmt->adipre012 , '0' , 12 );
    memset(pmt->adipre014 , ' ' , 4  );
    memset(pmt->adipre016 , '0' , 4  );
    memset(pmt->adipre017 , '0' , 4  );
    memset(pmt->adipre018 , '0' , 4  );
    memset(pmt->adipre019 , '0' , 3  );
    memset(pmt->adipre022 , ' ' , 12 );
    memset(pmt->adipre023 , ' ' , 3  );
    memset(pmt->adipre024 , ' ' , 3  );
    memset(pmt->adipre025 , ' ' , 4  );
    memset(pmt->adipre026 , ' ' , 4  ); //PRJ0258542
    memset(pmt->adipre027 , ' ' , 1  ); //PRJ0258542
    memset(pmt->adipre028 , '0' , 6  );
    memset(pmt->adipre029 , ' ' , 3  );
    memset(pmt->adipre030 , '0' , 24 ); 
    memset(pmt->adi031l   , '0' , 2  ); //PRJ0258542
    memset(pmt->adi031d   , ' ' , 23 ); //PRJ0258542
    memset(pmt->adi032l   , '0' , 2  );
    memset(pmt->adi032d   , ' ' , 11 );
    memset(pmt->adi033l   , '0' , 2  ); //PRJ0258542
    memset(pmt->adi033d   , ' ' , 11 ); //PRJ0258542
    memset(pmt->adi035l   , '0' , 2  );
    memset(pmt->adi035d   , ' ' , 37 );
    memset(pmt->adipre037 , ' ' , 12 );
    memset(pmt->adipre038 , ' ' , 6  );
    memset(pmt->adipre039 , ' ' , 3  );
    memset(pmt->adipre040 , ' ' , 3 );
    memset(pmt->adipre041 , ' ' , 8 );
    memset(pmt->adipre042 , ' ' , 15 );
    memset(pmt->adipre043 , ' ' , 40 );
    memset(pmt->adi044l   , '0' , 2  );
    memset(pmt->adi044d   , ' ' , 64 );
    memset(pmt->adi045l   , '0' , 2  );
    memset(pmt->adi045d   , ' ' , 76 );
    memset(pmt->adi048l   , '0' , 3  );
	//PRJ0247167 se modifica tamaño p48
    memset(pmt->adi048d   , ' ' , 999);
    memset(pmt->adipre050 , ' ' , 3  ); //PRJ0258542
    memset(pmt->adipre051 , ' ' , 3  ); //PRJ0258542
    memset(pmt->adipre052 , ' ' , 16 ); //PRJ0258542 - pasa a 16
    memset(pmt->adipre053 , ' ' , 24 ); //PRJ0258542 - pasa a 24
    memset(pmt->adi055l   , '0' , 3  );
    memset(pmt->adi055d   , ' ' , 256); //PRJ0258542 - se baja de 999 a 256 por BBDD
    memset(pmt->adi056l   , '0' , 2  );
    memset(pmt->adi056d   , ' ' , 35 );
    memset(pmt->adipre057 , ' ' , 3  ); //PRJ0258542
    memset(pmt->adi058l   , '0' , 2  ); //PRJ0258542
    memset(pmt->adi058d   , ' ' , 11 ); //PRJ0258542
		memset(pmt->adi062l   , '0' , 3  ); //PRJ0257005 - Inicialización
		memset(pmt->adi062d   , ' ' , 256); //PRJ0257005 - Inicialización
    memset(pmt->adipre064 , ' ' , 8  );
    memset(pmt->adipre071 , ' ' , 8  ); //PRJ0258542
    memset(pmt->adi072l   , '0' , 3  );
    memset(pmt->adi072d   , ' ' , 256);
    memset(pmt->adipre074 , ' ' , 10 );
    memset(pmt->adipre075 , ' ' , 10 );
    memset(pmt->adipre076 , ' ' , 10 );
    memset(pmt->adipre077 , ' ' , 10 );
    memset(pmt->adipre080 , ' ' , 10 ); //PRJ0258542
    memset(pmt->adipre081 , ' ' , 10 ); //PRJ0258542
    memset(pmt->adipre085 , ' ' , 10 ); //PRJ0258542
    memset(pmt->adipre086 , ' ' , 16 );
    memset(pmt->adipre087 , ' ' , 16 );
    memset(pmt->adipre088 , ' ' , 16 );
    memset(pmt->adipre089 , ' ' , 16 );
    memset(pmt->adi093l   , ' ' , 2  );
    memset(pmt->adi093d   , ' ' , 11 );
    memset(pmt->adi094l   , '0' , 2  );
    memset(pmt->adi094d   , ' ' , 11 );
		memset(pmt->adi095l   , '0' , 2  ); //PRJ0258542 - Inicialización
		memset(pmt->adi095d   , ' ' , 56 ); //PRJ0258542 - Inicialización
		memset(pmt->adi096l   , '0' , 3  ); //PRJ0258542 - Inicialización
		memset(pmt->adi096d   , ' ' , 360); //PRJ0258542 - Inicialización
    memset(pmt->adipre097 , ' ' , 17 );
    memset(pmt->adi101l   , '0' , 2  );
    memset(pmt->adi101d   , ' ' , 17 );
    memset(pmt->adi102l   , '0' , 2  );
    memset(pmt->adi102d   , ' ' , 28 ); //BIZUM ampliamos de 29 a 35 -- Dejamos a 28 25/04/2024
    memset(pmt->adi103l   , '0' , 2  );
    memset(pmt->adi103d   , ' ' , 28 );	//BIZUM ampliamos de 29 a 35 -- Dejamos a 28 25/04/2024
		memset(pmt->adi104l   , '0' , 3  ); //PRJ0258542 - Inicialización
		memset(pmt->adi104d   , ' ' , 256); //PRJ0258542 - Inicialización
    memset(pmt->adipre105 , ' ' , 16 ); //PRJ0258542
    memset(pmt->adipre106 , ' ' , 16 ); //PRJ0258542
    memset(pmt->adipre107 , ' ' , 10 ); //PRJ0258542
    memset(pmt->adipre108 , ' ' , 10 ); //PRJ0258542
    memset(pmt->adipre109 , ' ' , 16 );
    memset(pmt->adipre110 , ' ' , 16 );
    memset(pmt->adipre128 , ' ' , 8  );

    return(0);
}


/*****************************************************************
FUNCION  FVALCONT             llamada por ftradred

DESCRIPCION: Funcion que

AREA DE PASO :
-
*****************************************************************/
int fvalcont(int sentido)
{
    return(0);
}

/*****************************************************************
FUNCION  FTRAEFE1             llamada por ftradred

DESCRIPCION: traduccion efectiva(price --->> formato intermedio)

AREA DE PASO :
-
*****************************************************************/
int     ftraefe1(int suba_iso,int tipomsg,char *pbitmapp,char *pbitmaps,char *plantilla,formato_intermedio *pmt,char *ppr,int *error_code,int *error_bit,char *parea_mac, char * bit48) //PRJ0258542 - Fase 3 - Meto bit48 en hexadecimal
{
    int index_semp;
		char cadena48[2000]; //PRJ0258542 - Fase 3
    int   ll,tope_bm,retorno,longimix;
    int   resto,bytes_longitud,long_efec2,bytes_datos,index_mac,long_mac;
    char  codigo_021,codigo_313,codigo_314;
    char  bitmapt[129];
    //char  area_mac[1024];
    char  campo_test1[999];
    char  mac_calculado[8];
    char  mac_mensaje[8];
    char *pmtchar = (char*) pmt;
    char tipomsg_alf[2]; //PRJ0258542
    int p52 = 0; //PRJ0258542 - 0 = no ha restado, 1 si ha restado - para pasar de 16 a 8
    char pimP53[1]; //PRJ0258542 - primera posicion del P53 para saber si es de 8 o 16
    int p53 = 0; //PRJ0258542 - 0 = no ha restado, 1 si ha restado - para pasar de 24 a 16

  /***  inicializa el formato intermedio a los valores correspondientes ftraefe1  ***/
  finicializa_intermedio(pmt);
	
  /*---se construye asi para que coincida el bit con la posicion del array---*/
  bitmapt[0] = '-';
  strncpy(bitmapt+1,bitmapp,64);
  strncpy(bitmapt+65,bitmaps,64);

  //memset(area_mac , '\0' , 1024);
  memset(mac_calculado,'\0',8);

  tope_bm    = ((bitmapt[1]) == '1') ? 129 : 65;
  index_semp = ((bitmapt[1]) == '1') ? 18  : 10;
  long_mac = 0;

  if(!empaquetado)
    index_semp += 2;

  //fprintf(stdout," Valor de entrada para pbitmapp >%s<\n",pbitmapp); //PRJ0258542
  //fprintf(stdout," Valor de entrada para pbitmaps >%s<\n",pbitmaps); //PRJ0258542
  //fprintf(stdout," Valor de entrada para plantilla >%s<\n",plantilla); //PRJ0258542
  //fprintf(stdout," Valor de entrada para pmtchar >%s<\n",pmtchar); //PRJ0258542
  //fprintf(stdout," Valor de entrada para ppr >%s<\n",ppr); //PRJ0258542

  for (ll=2 ; ll<tope_bm ; ll++)     /***   bucle por todos los bits   ***/
  {
    if(ll==bittraza)
    {
    		fprintf(stdout,"\n================ Bucle principal ftraefe1 -- tope_bm %d bit[%d] = %c< =========================\n",tope_bm, ll, bitmapt[ll]);
    		
        fflush(stdout);
    }
    /***   informamos los flags para procesar las condiciones despues   ***/
    if ((strncmp("021",plantilla + ((ll-1)*3),3) == 0))
        codigo_021 = 'S';
    else
        codigo_021 = 'N';

    if ((strncmp("313",plantilla + ((ll-1)*3),3) == 0))
        codigo_313 = 'S';
    else
        codigo_313 = 'N';

    if ((strncmp("314",plantilla + ((ll-1)*3),3) == 0))
        codigo_314 = 'S';
    else
        codigo_314 = 'N';

    /***   400 : obligatorio. si el P-38 tenia condicion '303', comprobar
    que el P-39 contiene '000', y que el P-38 existe.       ***/

    if ((strncmp("400",plantilla + ((ll-1)*3),3) == 0))
    {
        printf("salida con cond 400\n");
        if(empaquetado)
            retorno = fdesempa(3, ppr + index_semp , cadena_salida);
        else
            memcpy(cadena_salida, ppr + index_semp, 3);

        if (!strncmp ||
            !strncmp("004", cadena_salida ,3) ||
            !strncmp("060", cadena_salida ,3) )
        {
            if (bitmapt[38] == '0')
            {
                *error_code = 7;
                *error_bit = ll;
                return(-1);
            }
        }
        else
        {
            if (bitmapt[38] == '1')
            {
                *error_code = 7;
                *error_bit = ll;
                return(-1);
            }
        }
    }

    /***   solo trata los bits presentes   ***/
    if (bitmapt[ll] != '1')
        continue;

    resto = 0;
    bytes_longitud = 0;
    fprintf(stdout,"\n================ Saca el bit %d del mensaje ===============\n", ll); //PRJ0258542

    /***   lee datos del bit en formatos iso y aplicacion  ftraefe1  ***/
    if ((retorno = flee_datoapl(ll)) != 0)
    {
        *error_code = 3;
        *error_bit = ll;
        return(-1);
    }
    /***   lee datos del bit en formato iso ftraefe1  ***/
    if ((retorno = flee_datoiso(ll)) != 0)
    {
        *error_code = 12;
        *error_bit = ll;
        return(-1);
    }

    /***   test-long   ***/
    if(ll==bittraza)
    {
        fprintf(stdout,"  ----->test-long  \n");
        fflush(stdout);
    }
    
		//PRJ0258542 - INI
		if (ll==52)
		{
			strncpy(pimP53, ppr + index_semp + 8, 1);
			pimP53[1] = '\0';
     	fprintf(stdout,"long_fs y long_fc y off_fc antes index_semp>%d< >%d< >%d< >%d< \n", long_fs, long_fc, off_fc, index_semp);
			fprintf(stdout,"BIT >%d< se le asigna longitud 8 a P52, si posicion 9 es igual a 2  >%.1s< \n", ll, pimP53);
			if (strncmp(pimP53, "2", 1) == 0)
			{
    		long_fs = 8;
    		long_fc = 8;
    		p52 = 1;
    	}
    	fprintf(stdout,"long_fs y long_fc y off_fc despues >%d< >%d< >%d<\n", long_fs, long_fc, off_fc);
		}
		
		if (ll==53) 
		{
			//strncpy(tipomsg_alf, itoa(tipomsg),2);
			if (tipomsg < 1800)
			{
				fprintf(stdout,"BIT >%d< se le asigna longitud 16 a P53 para tipo mensajes %d \n", ll, tipomsg);
      	fprintf(stdout,"long_fs y long_fc y off_fc antes >%d< >%d< >%d<\n", long_fs, long_fc, off_fc);
      	long_fs = 16;
      	long_fc = 16;
      	//off_fc -= 8;
      	p53 = 1;
      	fprintf(stdout,"long_fs y long_fc y off_fc despues >%d< >%d< >%d<\n", long_fs, long_fc, off_fc);
      }
		}
		
		if ((ll > 53) && (p53 == 1))
		{
    	fprintf(stdout,"long_fs y long_fc y off_fc antes >%d< >%d< >%d<\n", long_fs, long_fc, off_fc);
    	//off_fc -= 8;
    	fprintf(stdout,"long_fs y long_fc y off_fc despues >%d< >%d< >%d<\n", long_fs, long_fc, off_fc);
		}
		//PRJ0258542 - FIN

#if defined (TRAZA) && (TRAZA > 10)
        fprintf(stdout,"long_fs->%d<-\n", long_fs);
        fflush(stdout);
#endif
    if(long_fs > 1000)
    {
        resto     = long_fs - 1000;
        // 092008 como suponemos que el bit55 viene empaquetado, y en principio es el único que viene empaquetado, lo
        // tratamos individualmente
        // if(empaquetado)
        if((empaquetado) || (ll==55))
        {		
            bytes_longitud = (resto/2) + (resto%2);
            retorno = fdesempa(resto, ppr + index_semp , cadena_salida);
        }
        else
        {
            bytes_longitud = resto;
            strncpy(cadena_salida, ppr + index_semp, resto);
#if defined (TRAZA) && (TRAZA > 10)			
            //fprintf(stdout,"cadena_salida >%.40s<\n",cadena_salida);
            //fflush(stdout);
#endif			
        }

        if (ncampo_numerico(cadena_salida,resto) == 0)
        {
            fprintf(stdout, "longitud\n");
            fhexdump(stdout, ppr + index_semp, bytes_longitud);
            fflush(stdout);
            *error_code = 20;
            *error_bit = ll;
            return(-1);
        }
        else
        {
            strncpy(cadena_verif,cadena_salida,resto);
            cadena_verif[resto] = '\0';
#if defined (TRAZA) && (TRAZA > 10)						
            fprintf(stdout,"cadena_verif >%.*s<\n",resto,cadena_verif);
            fflush(stdout);
#endif			
            long_efec2 = atoi(cadena_verif);
            index_semp = index_semp + bytes_longitud;
        }
    }
    else
    {
        long_efec2 = long_fs;
    }
    /***   fin if(long_fs > 1000)   ***/

    if (tipo_fs == 'E')
        bytes_datos = (long_efec2/2) + (long_efec2%2);
    else
        bytes_datos = long_efec2;

#if defined (TRAZA) && (TRAZA > 10)   		   
        fprintf(stdout,"valor bytes_datos:->%d<-\n", bytes_datos);
        fprintf(stdout,"valor bytes_longitud:->%d<-\n", bytes_longitud);
        fprintf(stdout,"valor index_semp:->%d<-\n", index_semp);
        fflush(stdout);
#endif		
#if defined (TRAZA) && (TRAZA > 5)   		   
        fhexdump(stdout, ppr + index_semp - bytes_longitud, bytes_longitud + bytes_datos);
        fflush(stdout);
#endif
    if (ll == 48) //PRJ0258542 - Fase 3
    {
    	retorno = convierte_hex (ppr + index_semp - bytes_longitud, bytes_datos + 3, cadena48);
    	strncpy(bit48, cadena48, bytes_datos * 2);
    	//fprintf(stdout,"Valor cadena48 hexadecimal>%s< \n", cadena48);
    }

    /*** OJO tiene que coincidir el tipo de datos? ***/
    /***   test-pic   ***/
    if((tipo_fc == 'X' || tipo_fc == 'B') &&
        tipo_fs != 'X' && tipo_fs != 'B' &&
        tipo_fs != 'E' && tipo_fs != 'M' &&
        tipo_fs != '9')
    {
#if defined (TRAZA) && (TRAZA > 1)				
    fprintf(stdout, "tipo_dap2 ->%c<-  tipo_diso ->%c<-\n", tipo_fc, tipo_fs);
		fflush(stdout);
#endif		
        *error_code = 11;
        *error_bit = ll;
        return(-1);
    }

    if(empaquetado) /* si empaquetado los numericos no pueden ir desempaquetados */
    {
        if(tipo_fc == '9' &&
          (tipo_fs == 'X' || tipo_fs == 'B' || tipo_fs == 'M'))
      
	  		{
#if defined (TRAZA) && (TRAZA > 1)				  
      fprintf(stdout, "tipo_dap2 ->%c<-  tipo_diso ->%c<-\n", tipo_fc, tipo_fs);
			fflush(stdout);
#endif			
            *error_code = 11;
            *error_bit = ll;
            return(-1);
        }
    }
    /***   fin-test-pic   ***/

    if (mac_fs == 'S') 
    {
        if (resto != 0)
        {
            index_mac = index_semp - bytes_longitud;
            memcpy(area_mac + long_mac , ppr + index_mac , bytes_longitud);
            index_mac = index_mac + bytes_longitud;
            long_mac = long_mac + bytes_longitud;
        }
        else
        {
            index_mac = index_semp;
        }

        /***   el campo S-72 interviene en el calculo de mac,
        y deberia tener tratamiento especial   ***/
        if (tipo_fs == 'B')
        {
            memcpy(area_mac + long_mac , ppr + index_mac , long_efec2 );
            long_mac = long_mac + long_efec2;
        }
        else
        {
            if (tipo_fs == 'E')
            {
                memcpy(area_mac + long_mac , ppr + index_mac , bytes_datos );
                long_mac = long_mac + bytes_datos;
            }
            else
            {
                memcpy(area_mac + long_mac , ppr + index_mac , long_efec2 );
                long_mac = long_mac + long_efec2;
            }
        }
    }//fin mac_fs == 'S';
    //fprintf(stdout,"\n MAC formado \n");
		//fhexdump(stdout,area_mac,long_mac);
    //fprintf(stdout,"----- \n");
#if defined (TRAZA) && (TRAZA > 10)			
    //fprintf(stdout,"long_fc >%d<\n",long_fc); //PRJ0258542
    fflush(stdout);
#endif	
    if (long_fc < 1000)
    {
        if (tipo_fs == 'B' || tipo_fs == 'X' || tipo_fs == '9')
            memcpy(pmtchar + off_fc , ppr + index_semp , long_efec2);
        else
        {
            if (tipo_fs == 'E')
            {
                retorno = fdesempa(long_fs , ppr + index_semp , cadena_salida);
                strncpy(pmtchar + off_fc , cadena_salida , long_efec2);
            }
            else
            {
                retorno = fdesempa_mixto(ll,long_efec2,&longimix, ppr + index_semp , cadena_salida);
                strncpy(pmtchar + off_fc , cadena_salida , long_efec2);
                bytes_datos = longimix;
            }
        }
        if(ll==bittraza)
            fprintf(stdout,"campo fijo>>>%.*s<<<\n",long_efec2, ppr + index_semp );
        /***
        ***/
    }
    else
    {
        memset(campo_test1,'\0',999);
        if(tipo_fs == 'B' || tipo_fs == 'X' || tipo_fs == '9')
        {
            memcpy(campo_test1 , ppr + index_semp , long_efec2);
            //if (ll == 48) //PRJ0258542 - Fase 3
            //{
            //	fprintf(stdout,"Valor 1 campo_test1 >%s< ppr >%s< index_semp >%d< long_efec2 >%d< \n", campo_test1, ppr, index_semp, long_efec2);
            //}
            //strncpy(campo_test1 , ppr + index_semp , long_efec2); //PRJ0258542 - Fase 3
            //if (ll == 48) //PRJ0258542 - Fase 3
            //{
            //	fprintf(stdout,"Valor 2 campo_test1 >%s< ppr >%s< index_semp >%d< long_efec2 >%d< \n", campo_test1, ppr, index_semp, long_efec2);
            //}
        }
        else
        {
            if (tipo_fs == 'E')
            {
                retorno = fdesempa(long_efec2 , ppr + index_semp , cadena_salida);
                strncpy(campo_test1 , cadena_salida , long_efec2);
            }
            else
            {
                retorno = fdesempa_mixto(ll,long_efec2,&longimix, ppr + index_semp , cadena_salida);
                strncpy(campo_test1 , cadena_salida , long_efec2);
                bytes_datos = longimix;
            }
        }

        off_fc = off_fc - resto;
        if((empaquetado)||(ll==55))
            retorno = fdesempa(resto , ppr + index_semp - bytes_longitud , cadena_salida);
        else
            strncpy(cadena_salida, ppr + index_semp - bytes_longitud, resto);

        strncpy(cadena_verif, cadena_salida , resto);
        cadena_verif[resto] = '\0';

        if (atoi(cadena_verif) > max_fc)
        {
#if defined (TRAZA) && (TRAZA > 1)					
            fprintf(stdout,"longitud ->%s<- maximo ->%d<-\n", cadena_verif, max_fc);
            fflush(stdout);
#endif			
            *error_code = 21;
            *error_bit = ll;
            return(-1);
        }
        else
        {
            if ((resto == 3) || (resto == 2))
            {                          /***   cabecera-de-3 o de-2  ***/
                if (!long_efec2)
                {
                    *error_code = 19;
                    *error_bit = ll;
                    return(-1);
                }
                if (long_efec2 > max_fs)
                {
#if defined (TRAZA) && (TRAZA > 1)							
                    fprintf(stdout,"max_fs >%d< long_efec2 >%d<\n",max_fs,long_efec2);
                    fflush(stdout);
#endif										
                    *error_code = 16;
                    *error_bit = ll;
                    return(-1);
                }
                strncpy(pmtchar + off_fc , cadena_salida , resto);
                off_fc = off_fc + resto;
                memcpy(pmtchar + off_fc , campo_test1 , long_efec2);
							
            }                          /***   fin-cabecera-de-3 o de-2   ***/
            else
            {
                *error_code = 21;
                *error_bit = ll;
                return(-1);
            }
        }// fin else
        
    }
    
    /***   fin if (long_fc < 1000)   ***/

    if ((strncmp("994",plantilla + ((ll-1)*3),3) == 0))
    {   /***   validar 994   ***/
    }
    if ((ll == (tope_bm - 1)) && (bitmapt[ll] == '1'))
        memcpy(mac_mensaje , ppr + index_semp , 8);

    index_semp = index_semp + bytes_datos;
		
//  }      /***   fin del bucle principal   ***/ //PRJ0258542 - Quitado temporalmente
 	
		//PRJ0258542 - INI
  	switch (ll)
    {
    	case 0:
				fprintf(stdout,"  Valor del P00  >%d< \n", tipomsg);
				break;
			case 2:
				fprintf(stdout,"  Valor del P02  >%.2s< >%.19s< \n", pmt->adi002l, pmt->adi002d);
				break;
			case 3:
				fprintf(stdout,"  Valor del P03  >%.6s< \n", pmt->adipre003);
				break;
			case 4:
				fprintf(stdout,"  Valor del P04  >%.12s< \n", pmt->adipre004);
				break;
			case 5:
				fprintf(stdout,"  Valor del P05  >%.12s< \n", pmt->adipre005);
				break;
			case 6:
				fprintf(stdout,"  Valor del P06  >%.12s< \n", pmt->adipre006);
				break;
			case 9:
				fprintf(stdout,"  Valor del P09  >%.8s< \n", pmt->adipre009);
				break;
			case 10:
				fprintf(stdout,"  Valor del P10  >%.8s< \n", pmt->adipre010);
				break;
			case 11:
				fprintf(stdout,"  Valor del P11  >%.6s< \n", pmt->adipre011);
				break;
			case 12:
				fprintf(stdout,"  Valor del P12  >%.12s< \n", pmt->adipre012);
				break;
			case 14:
				fprintf(stdout,"  Valor del P14  >%.4s< \n", pmt->adipre014);
				break;
			case 16:
				fprintf(stdout,"  Valor del P16  >%.4s< \n", pmt->adipre016);
				break;
			case 17:
				fprintf(stdout,"  Valor del P17  >%.4s< \n", pmt->adipre017);
				break;
			case 18:
				fprintf(stdout,"  Valor del P18  >%.4s< \n", pmt->adipre018);
				break;
			case 19:
				fprintf(stdout,"  Valor del P19  >%.3s< \n", pmt->adipre019);
				break;
			case 22:
				fprintf(stdout,"  Valor del P22  >%.12s< \n", pmt->adipre022);
				break;
			case 23:
				fprintf(stdout,"  Valor del P23  >%.3s< \n", pmt->adipre023);
				break;
			case 24:
				fprintf(stdout,"  Valor del P24  >%.3s< \n", pmt->adipre024);
				break;
			case 25:
				fprintf(stdout,"  Valor del P25  >%.4s< \n", pmt->adipre025);
				break;
			case 26:
				fprintf(stdout,"  Valor del P26  >%.4s< \n", pmt->adipre026);
				break;
			case 32:
				fprintf(stdout,"  Valor del P32  >%.2s< >%.11s< \n", pmt->adi032l, pmt->adi032d);
				break;
			case 33:
				fprintf(stdout,"  Valor del P33  >%.2s< >%.11s< \n", pmt->adi033l, pmt->adi033d);
				break;
			case 37:
				fprintf(stdout,"  Valor del P37  >%.12s< \n", pmt->adipre037);
				break;
			case 41:
				fprintf(stdout,"  Valor del P41  >%.8s< \n", pmt->adipre041);
				break;
			case 42:
				fprintf(stdout,"  Valor del P42  >%.15s< \n", pmt->adipre042);
				break;
			case 43:
				fprintf(stdout,"  Valor del P43  >%.40s< \n", pmt->adipre043);
				break;
			case 48:
				fprintf(stdout,"  Valor del P48  >%.3s< >%.999s< \n", pmt->adi048l, pmt->adi048d);
				break;
			case 49:
				fprintf(stdout,"  Valor del P49  >%.3s< \n", pmt->adipre049);
				break;
			case 52:
				fprintf(stdout,"  Valor del P52  >%.16s< \n", pmt->adipre052);
				break;
			case 53:
				fprintf(stdout,"  Valor del P53  >%.24s< \n", pmt->adipre053);
				break;
			case 58:
				fprintf(stdout,"  Valor del P58  >%.2s< >%.11s< \n", pmt->adi058l, pmt->adi058d);
				break;
			case 64:
				fprintf(stdout,"  Valor del P64  >%.8s< \n", pmt->adipre064);
				break;
			case 71:
				fprintf(stdout,"  Valor del S71  >%.8s< \n", pmt->adipre071);
				break;
			case 72:
				fprintf(stdout,"  Valor del S72  >%.3s< >%.256s< \n", pmt->adi072l, pmt->adi072d);
				break;
			case 74:
				fprintf(stdout,"  Valor del S74  >%.10s< \n", pmt->adipre074);
				break;
			case 75:
				fprintf(stdout,"  Valor del S75  >%.10s< \n", pmt->adipre075);
				break;
			case 76:
				fprintf(stdout,"  Valor del S76  >%.10s< \n", pmt->adipre076);
				break;
			case 77:
				fprintf(stdout,"  Valor del S77  >%.10s< \n", pmt->adipre077);
				break;
			case 80:
				fprintf(stdout,"  Valor del S80  >%.10s< \n", pmt->adipre080);
				break;
			case 81:
				fprintf(stdout,"  Valor del S81  >%.10s< \n", pmt->adipre081);
				break;
			case 85:
				fprintf(stdout,"  Valor del S85  >%.10s< \n", pmt->adipre085);
				break;
			case 86:
				fprintf(stdout,"  Valor del S86  >%.16s< \n", pmt->adipre086);
				break;
			case 87:
				fprintf(stdout,"  Valor del S87  >%.16s< \n", pmt->adipre087);
				break;
			case 88:
				fprintf(stdout,"  Valor del S88  >%.16s< \n", pmt->adipre088);
				break;
			case 89:
				fprintf(stdout,"  Valor del S89  >%.16s< \n", pmt->adipre089);
				break;
			case 93:
				fprintf(stdout,"  Valor del S93  >%.2s< >%.11s< \n", pmt->adi093l, pmt->adi093d);
				break;
			case 94:
				fprintf(stdout,"  Valor del S94  >%.2s< >%.11s< \n", pmt->adi094l, pmt->adi094d);
				break;
			case 95:
				fprintf(stdout,"  Valor del S95  >%.2s< >%.56s< \n", pmt->adi095l, pmt->adi095d);
				break;
			case 96:
				fprintf(stdout,"  Valor del S96  >%.3s< >%.360s< \n", pmt->adi096l, pmt->adi096d);
				break;
			case 97:
				fprintf(stdout,"  Valor del S97  >%.17s< \n", pmt->adipre097);
				break;
			case 101:
				fprintf(stdout,"  Valor del S101 >%.2s< >%.17s< \n", pmt->adi101l, pmt->adi101d);
				break;
			case 102:
				fprintf(stdout,"  Valor del S102 >%.2s< >%.28s< \n", pmt->adi102l, pmt->adi102d);
				break;
			case 103:
				fprintf(stdout,"  Valor del S103 >%.2s< >%.28s< \n", pmt->adi103l, pmt->adi103d);
				break;
			case 104:
				fprintf(stdout,"  Valor del S104 >%.2s< >%.256s< \n", pmt->adi104l, pmt->adi104d);
				break;
			case 105:
				fprintf(stdout,"  Valor del S105 >%.16s< \n", pmt->adipre105);
				break;
			case 106:
				fprintf(stdout,"  Valor del S106 >%.16s< \n", pmt->adipre106);
				break;
			case 107:
				fprintf(stdout,"  Valor del S107 >%.10s< \n", pmt->adipre107);
				break;
			case 108:
				fprintf(stdout,"  Valor del S108 >%.10s< \n", pmt->adipre108);
				break;
			case 109:
				fprintf(stdout,"  Valor del S109 >%.16s< \n", pmt->adipre109);
				break;
			case 110:
				fprintf(stdout,"  Valor del S110 >%.16s< \n", pmt->adipre110);
				break;
			case 128:
				fprintf(stdout,"  Valor del S128 >%.8s< \n", pmt->adipre128);
				break;
		}
		//PRJ0258542 - FIN
  
  }      /***   fin del bucle principal   ***/
 	fprintf(stdout,"============= Fin bucle principal ftraefe1 ================= \n");
  
  if (codigo_021 == 'S')
  {                          /***   tratar-021   ***/
   	// DMND0036176 añadidas preautorizaciones
    if ((tipomsg == 1110) || (tipomsg == 1210))
    {
        if (pmt->adipre039[0] == '0')
        {
            if (bitmapt[30] == '1')
            {
#if defined (TRAZA) && (TRAZA > 1)						
                fprintf(stdout, "Existe bit 30 y no existe bit 39\n");
#endif				
                *error_code = 7;
                *error_bit = 30;
                return(-1);
            }
        }
        else
        {
            if (bitmapt[30] == '0')
            {
#if defined (TRAZA) && (TRAZA > 1)						
                fprintf(stdout, "No existe bit 30 y existe bit 39\n");
#endif				
                *error_code = 7;
                *error_bit = 30;
                return(-1);
            }
        }
    }//fin if 1110
  }                          /***   fin-tratar-021   ***/

/***   tratar-320   ***/
/***   fin-tratar-320   ***/
	//fprintf(stdout,"\nVerificacion del valor cadena_salida %s\n", cadena_salida);
  if(empaquetado)
  {
    retorno = fdesempa(4 , ppr , cadena_salida);
    strncpy(pmt->adipre000 , cadena_salida , 4);
  }
  else
    strncpy(pmt->adipre000 , ppr, 4);
	
  if (bitmapt[tope_bm - 1] == '1')
  {
    fprintf(stdout,"\nVerificacion del valor de MAC de longitud %d\n", long_mac);
    area_mac[long_mac]='\0';
    memcpy(parea_mac,area_mac,sizeof(long_mac));
    fhexdump(stdout,parea_mac,long_mac);
    fflush(stdout);

  }//fin bitmap;
  /********************************************************************/
	fprintf(stdout,"Control de salida ftraefe1\n");
  return(0);
}


/*****************************************************************
FUNCION  FDESEMPA_MIXTO        llamada por ftraefe1

DESCRIPCION: Funcion que trata los campos con formato mixto, cuyo
tratamiento debe ser particularizado

AREA DE PASO :
-
*****************************************************************/
int     fdesempa_mixto(int ll,int longi,int *longimix,
                         char *sempaq,char *sdesem)
{
    int retorno;
    char cadena_in[999];
    char cadena_ou[999];
	
    switch (ll)
    {
    case 48:
    case 55:
        /*   los dos primeros caracteres se desempaquetan   */
        memcpy(cadena_in , sempaq, 2);
        retorno = fdesempa(3,cadena_in,cadena_ou);
        strncpy(sdesem, cadena_ou , 3);
        memcpy(sdesem+3, sempaq + 2 , longi - 2);

        *(longimix) = longi + 1;
        break;
    case 72:
        /*   los dos primeros caracteres se desempaquetan   */
        memcpy(cadena_in , sempaq, 2);
        retorno = fdesempa(3,cadena_in,cadena_ou);
        strncpy(sdesem, cadena_ou , 3);
        memcpy(sdesem+3, sempaq + 2 , longi - 2);

        *(longimix) = longi + 1;
        break;
    case 97:
        /*   el primer caracter no se desempaqueta, y los 8 siguientes si   */
        *(sdesem) = *(sempaq);
        memcpy(cadena_in , sempaq + 1 , 8);
        retorno = fdesempa(16,cadena_in,cadena_ou);
        strncpy(sdesem + 1, cadena_ou , 16);

        *(longimix) = 9;

        break;
    }

    return(0);
}

/*****************************************************************
FUNCION  FTRAEFE2             llamada por ftradred

DESCRIPCION: traduccion efectiva(price <<--- formato intermedio)

AREA DE PASO :
//PRJ0258542 - se anade mensaje_netplus al final de la declaracion

*****************************************************************/
int     ftraefe2(int suba_iso,int tipomsg,char *pbitmapp,char *pbitmaps,char *plantilla,formato_intermedio *pmt,char *ppr,int *error_code,int *error_bit, char *idmac, char subsistema_general,formato_interno *mensaje_netplus) //PRJ0258542 - anadido mensaje_netplus
{
  int   mm,ll,tope_bm,pp,opciex;
  int   resto,bytes_longitud,lon_dasem_emp,long_calc,long_calcm,long_mac,long96; //PRJ0258542 - pongo long96
  int   lon_damac,lon_dasem,off_long,area_long999;
  char  area_bitmap[17];
  char  bitmapt[129];
	char long96s[3]; //PRJ0258542 - nueva variable
  char  area_sem[2000];
  char  area_mac[1024];
  char  mac_calculado[8];
  char  mac_calculado_aes[16]; //PRJ0258542
  char  mac_calculado_hsm[4096]; //del tamaño maximo de la respuesta hsm

  char  dato_sem[999];
  char  dato_mac[999];

  int longimix;
  int puntero_clavemac;

  char *pmtchar = (char*) pmt;
  int lon_bitmap;
  int lon_tipomsg;
  int restoMac = 0;
  int ii;
  unsigned char cc;
  char linea[500];

  char clave_iden[5]; //PRJ0258542
	char claveMac[151]; //PRJ0258542 pasa de 16 a 151
	char zona_mac[3]; //PRJ0258542
	char tipo_cambio[2]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char tipo_mac[3]; //PRJ0258542
	int origen; //PRJ0258542
	int tipo_macn; //PRJ0258542
	char comando[50]; //PRJ0258542
 	char clave1[151]; //PRJ0258542
	char clave2[151]; //PRJ0258542

	char clavecone[4];
  char * addr;
	NPSDHSM reghsm;
  char codResp[3];
  int j;
	int retorno =0;


  /*---se construye asi para que coincida el bit con la posicion del array---*/
  bitmapt[0] = '-';
  strncpy(bitmapt+1,bitmapp,64);
  strncpy(bitmapt+65,bitmaps,64);

  memset(area_sem , '\0' , 2000);
  memset(area_mac , '\0' , 1024);
  memset(mac_calculado , '\0' , sizeof(mac_calculado));
  memset(mac_calculado_aes , '\0' , sizeof(mac_calculado_aes));
  memset(mac_calculado_hsm , '\0' , sizeof(mac_calculado_hsm));

  tope_bm    = ((bitmapt[1]) == '1') ? 129 : 65;
  long_calc  = 0;
  long_calcm = 0;
  long_mac   = 0;
  puntero_clavemac = 0;
  fprintf(stdout,"\n================ Bucle principal ftraefe2 -- tope_bm %d bit[%d] = %c< =========================\n",tope_bm, 2, bitmapt[2]);

  for (ll=2 ; ll<tope_bm ; ll++)     /***   bucle por todos los bits   ***/
  {
    if (bitmapt[ll] == '1') //PRJ0258542
		{
    	fprintf(stdout,"\n================ Saca el bit %d del mensaje ===============\n", ll);
    }

    /*** solo trata bits presentes o que todavia tienen condicion pendiente ***/
    if (bitmapt[ll] != '1' && bitmapt[ll] != '3')
        continue;

    resto = 0;

    /***   lee datos del bit en formato aplicacion  ftraefe2 ***/
    if ((retorno = flee_datoapl(ll)) != 0)
    {
        *error_code = 3;
        *error_bit = ll;
        return(-1);
    }

    if (ll == (tope_bm - 1))
    {  /***     calcular mac     ***/
			fprintf(stdout," * * * Va a calcular MAC de salida * * *\n"); //PRJ0258542
			if ((retorno = flee_datoiso(ll)) != 0) //PRJ0258542
			{
				*error_code = 12;
				*error_bit = ll;
				fprintf(stdout, "\n>>%03d<< No existe en datos iso\n", ll);
				fflush(stdout);
				return(-1);
			}
      addr = area_mac;
      for (ii=0; ii < long_mac; ii++)
      {
          cc = *(addr + ii);
          sprintf(linea+2*ii, "%02X", cc);
      }
      strncpy(area_machsm,linea,long_mac*2);
      long_mac = long_mac*2;
      area_machsm[long_mac]='\0';
      
			//PRJ0258542- INI
			//strncpy(clave_iden, mensaje_netplus->fidatpro.fiorigen, 2); //PRJ0258542
  		//strncpy(clave_iden + 2, mensaje_netplus->fidatpro.fisubori, 2); //PRJ0258542
  		if (strncmp(mensaje_netplus->fidatpro.fiorigen, "10", 2) == 0 ||
  				strncmp(mensaje_netplus->fidatpro.fiorigen, "20", 2) == 0 ||
  				strncmp(mensaje_netplus->fidatpro.fiorigen, "30", 2) == 0 ||
  				strncmp(mensaje_netplus->fidatpro.fiorigen, "40", 2) == 0) //PRJ0258542
  		{
				strncpy(clave_iden, mensaje_netplus->fidatpro.fiorigen, 2);
  			strncpy(clave_iden + 2, mensaje_netplus->fidatpro.fisubori, 2);
  		}
  		else //PRJ0258542
  		{
  			strncpy(clave_iden, mensaje_netplus->fidatpro.firespri, 2);
  			strncpy(clave_iden + 2, "02", 2); //PRJ0258542
  		}
  		//clave_iden[4]='\0'; //AQUI
			if (strncmp(mensaje_netplus->fiisocom.fiiso000, "18", 2) == 0 && (strncmp(mensaje_netplus->fidatiso.fiisocla.ficla024,"811",3) == 0 || strncmp(mensaje_netplus->fidatiso.fiisocla.ficla024,"815",3) == 0 || strncmp(mensaje_netplus->fidatiso.fiisocla.ficla024,"816",3) == 0))
			{
				fprintf(stdout,"* - * - * Entro en zona MAC para mensaje claves en salida * - * - *\n");
				//Si estan presentes, el MAC se tiene que montar con el 11-6 12-12 24-3 25-4 33-11 39-3 53-24 96-variable con la longitud
				//1804 y 1805 usan 11, 12, 24, 25, 33, 53
				//1814 usa 11, 12, 33, 39, 53 si esta, 96 si esta
				//1824 y 1825 usan 11, 12, 24, 25, 33, 53, 96
				//1834 usa 11, 12, 33, 39
				fflush(stdout);
				long_mac = 0;
				memcpy(area_mac, " ", 58);
				memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla011, 6);
				long_mac = long_mac + 6;
				memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla012, 12);
				long_mac = long_mac + 12;
				if (strncmp(mensaje_netplus->fiisocom.fiiso000, "1804", 4) == 0 || strncmp(mensaje_netplus->fiisocom.fiiso000, "1805", 4) == 0 || strncmp(mensaje_netplus->fiisocom.fiiso000, "1824", 4) == 0 || strncmp(mensaje_netplus->fiisocom.fiiso000, "1825", 4) == 0)
				{
					memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla024, 3);
					long_mac = long_mac + 3;
					memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla025, 4);
					long_mac = long_mac + 4;
				}
				//PRJ0258542 - El 33 va en todos
				memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla033.longitud, 2); //PRJ0258542 - se incluye la longitud para el calculo del CMAC
				long_mac = long_mac + 2; //PRJ0258542 - se incluye la longitud para el calculo del CMAC
			 	memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla033.datos, atoi(mensaje_netplus->fidatiso.fiisocla.ficla033.longitud));
				long_mac = long_mac + atoi(mensaje_netplus->fidatiso.fiisocla.ficla033.longitud);
				//PRJ0258542 - El 33 va en todos
				if (strncmp(mensaje_netplus->fiisocom.fiiso000, "1814", 4) == 0 || strncmp(mensaje_netplus->fiisocom.fiiso000, "1834", 4) == 0)
				{
					memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla039, 3);
					long_mac = long_mac + 3;
				}
				if (strncmp(mensaje_netplus->fiisocom.fiiso000, "1804", 4) == 0 || strncmp(mensaje_netplus->fiisocom.fiiso000, "1805", 4) == 0 || strncmp(mensaje_netplus->fiisocom.fiiso000, "1824", 4) == 0 || strncmp(mensaje_netplus->fiisocom.fiiso000, "1825", 4) == 0)
				{
					memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla053.control, 2);
					long_mac = long_mac + 2;
					memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla053.identificador, 8);
					long_mac = long_mac + 8;
					memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla053.algoritmo, 2);
					long_mac = long_mac + 2;
					memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla053.longitud, 4);
					long_mac = long_mac + 4;
					memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla053.proteccion, 2);
					long_mac = long_mac + 2;
					memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla053.resto, 6);
					long_mac = long_mac + 6;
				}
				//fprintf(stdout,"Datos 96 y longitud >%s< >%s< \n",mensaje_netplus->fidatiso.fiisocla.ficla096.datos,mensaje_netplus->fidatiso.fiisocla.ficla096.longitud);
				if (strncmp(mensaje_netplus->fiisocom.fiiso000, "1814", 4) == 0 || strncmp(mensaje_netplus->fiisocom.fiiso000, "1824", 4) == 0 || strncmp(mensaje_netplus->fiisocom.fiiso000, "1825", 4) == 0)
				{
					if (strncmp(mensaje_netplus->fidatiso.fiisocla.ficla096.longitud, "000", 3) != 0)
					{
				 		memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla096.longitud, 3);
						long_mac = long_mac + 3;
						//fprintf(stdout,"area_mac y longitud >%.*s< >%d< \n",long_mac,area_mac,long_mac);
						strncpy(long96s,mensaje_netplus->fidatiso.fiisocla.ficla096.longitud,3);
						long96 = atoi(long96s);
				 		memcpy(area_mac + long_mac, mensaje_netplus->fidatiso.fiisocla.ficla096.datos, long96);
						long_mac = long_mac + long96;
						//fprintf(stdout,"Datos 96 y longitud >%.*s< >%d< \n",long96,mensaje_netplus->fidatiso.fiisocla.ficla096.datos,long96);
					}
				}
				//fprintf(stdout,"Area_mac y longitud >%.*s< >%d< \n",long_mac,area_mac,long_mac);
	      addr = area_mac;
	      for (ii=0; ii < long_mac; ii++)
	      {
	          cc = *(addr + ii);
	          sprintf(linea+2*ii, "%02X", cc);
	      }
	      strncpy(area_machsm,linea,long_mac*2);
	      long_mac = long_mac*2;
	      area_machsm[long_mac]='\0';
				strncpy(zona_mac,"01", 2);
	    	zona_mac[2]='\0';
				//strncpy(tipo_mac,mensaje_netplus->fidatiso.fiisocla.ficla053.algoritmo, 2);
				strncpy(tipo_mac, "05", 2);
				tipo_mac[2]='\0';

				//fprintf(stdout,"Valor de P53 en partes >%.2s %.8s %.2s %.4s %.2s %.6s<\n ",mensaje_netplus->fidatiso.fiisocla.ficla053.control, mensaje_netplus->fidatiso.fiisocla.ficla053.identificador, mensaje_netplus->fidatiso.fiisocla.ficla053.algoritmo, 
				//						mensaje_netplus->fidatiso.fiisocla.ficla053.longitud, mensaje_netplus->fidatiso.fiisocla.ficla053.proteccion, mensaje_netplus->fidatiso.fiisocla.ficla053.resto);
				fprintf(stdout,"*>*>*<*<* Valor de nument zona_mac >%.2s< y tipo_mac >%.2s< fidatpro.fiorigen >%.2s< y clave_ide >%.4s<\n ",zona_mac,tipo_mac,mensaje_netplus->fidatpro.fiorigen,clave_iden);
				
				if ((retorno = carga_para("00")) != 0)
				{
				   fprintf(stdout,"**** Error carga tabla NPTPARA ****\n");
				}

				//return(0); //AQUI
	  		if (strncmp(mensaje_netplus->fidatpro.fiorigen, "10", 2) == 0 ||
	  				strncmp(mensaje_netplus->fidatpro.fiorigen, "20", 2) == 0 ||
	  				strncmp(mensaje_netplus->fidatpro.fiorigen, "30", 2) == 0 ||
	  				strncmp(mensaje_netplus->fidatpro.fiorigen, "40", 2) == 0)
	  		{
					if (strncmp(mensaje_netplus->fidatpro.fiorigen, reg_para[0].psnument, 2) == 0)
					{
						origen = 0;
					}	
					else
					{
						origen = 1;
					}
	  		}
	  		else
	  		{
					if (strncmp(mensaje_netplus->fidatpro.firespri, reg_para[0].psnument, 2) == 0)
					{
						origen = 0;
					}	
					else
					{
						origen = 1;
					}
				}
			}
			else
			{
				strncpy(zona_mac,mensaje_netplus->fidatiso.fiisoaut.fiaut053 + 8, 2);
	    	zona_mac[2]='\0';
				strncpy(tipo_mac,mensaje_netplus->fidatiso.fiisoaut.fiaut053 + 2, 2);
				fprintf(stdout,"Valor de P53 completo >%.16s< y posicion 3y4 >%.2s< y clave_ide >%.4s<\n ",mensaje_netplus->fidatiso.fiisoaut.fiaut053,tipo_mac,clave_iden);
			  if(strncmp(mensaje_netplus->fiisocom.fiiso000,"13",2)==0)
			  {
			    strncpy(zona_mac,mensaje_netplus->fidatiso.fiisofic.fific053 + 8, 2);
			    zona_mac[2]='\0';
			    strncpy(tipo_mac,mensaje_netplus->fidatiso.fiisofic.fific053 + 2, 2);
			  }
				tipo_mac[2]='\0';
				fprintf(stdout,"Valor de nument 0 >%.2s< y nument 1 >%.2s< y fiorigen >%.2s<\n ",reg_para[0].psnument,reg_para[1].psnument,mensaje_netplus->fidatpro.fiorigen);
				if (strncmp(mensaje_netplus->fidatpro.fiorigen, reg_para[0].psnument, 2) == 0)
				{
					origen = 0;
				}	
				else
				{
					origen = 1;
				}
			}
			strncpy(pscerrado, reg_para[origen].pscerrando, 1); //PRJ0258542 - Fase 3 - Cambio 11052026
			strncpy(tipo_cambio, mensaje_netplus->fidatiso.fiisocla.ficla053.identificador + 6, 2); //PRJ0258542 - Fase 3 - Cambio 11052026
			fprintf(stdout,"Valor de P53 identificador tipo clave >%.2s<\n", tipo_cambio); //PRJ0258542 - Fase 3 - Cambio 11052026
			tipo_macn = atoi(tipo_mac);
      switch (tipo_macn)
      {
       	case 00: //TDES sin PIN
       	case 01: //Algoritmo DES
        	strncpy(comando,reg_para[origen].psclvhsm,50);
        	comando[50]='\0';
        	break;
        case 02: //Algoritmo 3DES
        case 03: //TDES
        	strncpy(comando,reg_para[origen].psclvhsm,50);
        	comando[50]='\0';
        	break;
        case 05: //AES
					if (strncmp(mensaje_netplus->fiisocom.fiiso000, "18", 2) == 0) //La renovacion de una ZMK, la respuesta va con la anterior, con la inactiva
					{
						if ((strncmp(tipo_cambio, "22", 2) == 0 || strncmp(tipo_cambio, "00", 2) == 0) && (strncmp(mensaje_netplus->fiisocom.fiiso000, "1834", 4) == 0) && (strncmp(mensaje_netplus->fidatiso.fiisocla.ficla039, "800", 3) == 0))
						{
							fprintf(stdout,"Cambio con la inactiva por valor %.2s y mensaje %.4s con bit39 %.3s\n", tipo_cambio, mensaje_netplus->fiisocom.fiiso000, mensaje_netplus->fidatiso.fiisocla.ficla039); //PRJ0258542 - Fase 3 - Cambio 11052026
							strncpy(comando,reg_para[origen].psclziki,50);
						}
						else
						{
							fprintf(stdout,"Realiza con activa por valor %.2s y mensaje %.4s con bit39 %.3s\n", tipo_cambio, mensaje_netplus->fiisocom.fiiso000, mensaje_netplus->fidatiso.fiisocla.ficla039); //PRJ0258542 - Fase 3 - Cambio 11052026
							strncpy(comando,reg_para[origen].psclzika,50);
						}
					}
					else //PRJ0258542 - Fase 3 - Meto control, los 18xx va el MAC con la ZIK, no va con la ZAK
					{
	        	strncpy(comando,reg_para[origen].psclzaka,50);
	        }
        	comando[50]='\0';
        	break;
 			}
 			
    	retorno = claveshsm(comando, clave_iden, "000000", clave1, clave2);
			
			fflush(stdout);
			
		  if(strncmp(zona_mac,"01",2)==0)
		  {
		  	//strncpy(mensaje_netplus->fidatpro.fimacarq.idmac, clave1+5,5); //guardo solo el identificador en el fi
		 	 	strncpy(claveMac,clave1, 150);
			 	//claveMac[strlen(clave1)]='\0';
		 	}
		  else if(strncmp(zona_mac,"02",2)==0)
			{
		   	//strncpy(mensaje_netplus->fidatpro.fimacarq.idmac, clave2+5,5); //guardo solo el identificador en el fi
	  	  strncpy(claveMac,clave2, 150);
		   	//claveMac[strlen(clave2)]='\0';
			}
			else
			{
	     	fprintf(stdout,"Zona MAC no esperada %.2s\n", zona_mac);
			}
			//PRJ0258542- FIN

      fprintf(stdout,"clave de mac >%s<\n",claveMac);
      fflush(stdout);
      fprintf(stdout,"area_machsm  >%s<\n",area_machsm);
      fflush(stdout);
      fprintf(stdout,"long_mac     >%d<\n",long_mac); 
      fflush(stdout);

			fprintf(stdout, "Valor de la nueva variable tipo_macn ftrared >%d<\n", tipo_macn); //PRJ0258542 - ftraefe2
			fflush(stdout);
			//PRJ0258542 - Si es AES tengo que llamar a la funcion 07 que es el 802 para CMAC en lugar de al 801 para MAC
			if (tipo_macn == 5)
			{
			  retorno = manejhsm(claveMac, "07", tipo_macn, area_machsm, long_mac, sockssl, mac_calculado_hsm); //PRJ0258542 - Se manda al comando 07
			}
			else
			{
				retorno = manejhsm(claveMac, "04", tipo_macn, area_machsm, long_mac, sockssl, mac_calculado_hsm); //PRJ0258542 - se añade 3 campo para saber si es AES o TDES
			}
			
			fflush(stdout);
 
      //////////// SI FALLA POR ERROR DE CONEXION VUELVO A CONECTAR Y LO INTENTO DE NUEVO
			if(retorno == -32 || retorno == -145)
			{				
				fprintLog(nomProg,"Intenta reconectar HSM \n");
  			memset(&reghsm, ' ', sizeof(NPSDHSM)); 

				conexhsm_C("0001",reg_para[0].psclvhsm , sockssl, codResp);
	   
  			if (!strcmp(codResp, "01"))
 				{
					//PRJ0258542 - Si es AES tengo que llamar a la funcion 07 que es el 802 para CMAC en lugar de al 801 para MAC
					if (tipo_macn == 5)
					{
					  retorno = manejhsm(claveMac, "07", tipo_macn, area_machsm, long_mac, sockssl, mac_calculado_hsm); //PRJ0258542 - Se manda al comando 07
					}
					else
					{
    				retorno = manejhsm(claveMac, "04", tipo_macn, area_machsm, long_mac, sockssl, mac_calculado_hsm);
    			}
	    
          if(retorno == -32 || retorno == -145)
          {
           	//Añadido para Monitorizacion SISMA 
           	fprintLog(nomProg,"TIPO SISTEMA – ACCESO HSM: conexion HSM claves codigo de error >%i<\n", codResp);
          }  			
       	}
        else 
	  		{
		  		//todo en el else es nuevo solo estaba el retono = sockssl
		 			conexhsm_C("0001",reg_para[1].psclvhsm , sockssl, codResp);
	   			if (!strcmp(codResp, "01"))
         	{
						//PRJ0258542 - Si es AES tengo que llamar a la funcion 07 que es el 802 para CMAC en lugar de al 801 para MAC
						if (tipo_macn == 5)
						{
						  retorno = manejhsm(claveMac, "07", tipo_macn, area_machsm, long_mac, sockssl, mac_calculado_hsm); //PRJ0258542 - Se manda al comando 07
						}
						else
						{
	           	retorno = manejhsm(claveMac, "04", tipo_macn, area_machsm, long_mac, sockssl, mac_calculado_hsm);
	          }
		 				if(retorno == -32 || retorno == -145)
           	{
           		//Añadido para Monitorizacion SISMA 
           		fprintLog(nomProg,"TIPO SISTEMA – ACCESO HSM: conexion HSM claves codigo de error >%i<\n", codResp);
           	}  		
	   			}
	   			else
	   			{
          	retorno = sockssl->sfd_ssl;//original
	   			}
	  		}		
			} 
	
			fprintLog(nomProg," --- Valor del semaforo de falseo - %s - ---\n", pscerrado); //PRJ0258542 - Fase 3 - Cambio 11052026
    	if (memcmp(pscerrado,"2",1) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
    	{
    		fprintLog(nomProg,"-- MAC generado pero se falsea, se procede al falseo --\n"); 
    		strncpy(mac_calculado_hsm,"00000000000000000000000000000000", 32);
    	}
			if(retorno == 16) //si retorna > 0 son datos de la respuesta y el mac deben ser 16
			{
    		fempapin(16, mac_calculado, mac_calculado_hsm);
	  		memset(mac_calculado + 4, ' ', 4);
    		fprintf(stdout,"mac_desemp >>> \n");
    		fhexdump(stdout,mac_calculado, sizeof(mac_calculado));
    		memcpy(pmtchar + off_fc , mac_calculado, 8);			
			}
			else if (retorno == 32) //PRJ0258542 - si el cifrado ha sido AES
			{
    		fempapin(32, mac_calculado_aes, mac_calculado_hsm);
	  		memset(mac_calculado_aes + 8, ' ', 8);
    		fprintf(stdout,"mac_desemp >>> \n");
    		fhexdump(stdout,mac_calculado_aes, sizeof(mac_calculado_aes));
    		memcpy(pmtchar + off_fc , mac_calculado_aes, 16);			
			}
			else
			{	  			
      	fprintf(stdout,"error en el calculo de mac>%d<\n",retorno);
        fflush(stdout);
        *error_code = 22;
        *error_bit = ll;
        return(-1);
     	} 			
    }

    if (bitmapt[ll] == '3')         /***   mirar-condicion   ***/
    {
        /*** 002 : obligatorio si esta presente y no viene el P-35 ***/
        if ((strncmp("002",plantilla + ((ll-1)*3),3) == 0))
        {
            if (nspaces(pmtchar + off_fc,max_fc) != 0)
            {
                if (nspaces(pmt->adi035d,37) != 0)
                {
                    *error_code = 7;
                    *error_bit = ll;
                    return(-1);
                }
                else
                {
                    if ((strncmp("994",plantilla + ((35-1)*3),3) == 0))
                    {    /***   validar-994   ***/
                    }
                }
            }
            else
            {
                if (nspaces(pmt->adi035d,37) != 0)
                {
                    bitmapt[ll] = '1';
                }
                else
                {
                    bitmapt[ll] = '0';
                }
            }
        }

        /*** 303 : obligatorio en respuestas si se aprobo la transaccion ***/
        if ((strncmp("303",plantilla + ((ll-1)*3),3) == 0))
        {
            if(nspaces(pmt->adipre038,6))
            {
                if(pmt->adipre039[0] == '0')
                {
                    *error_code = 7;
                    *error_bit = ll;
                    return(-1);
                }
                else
                {
                    bitmapt[ll] = '0';
                }
            }
            else
            {
                if (pmt->adipre039[0] != '0')
                {
                    *error_code = 7;
                    *error_bit = ll;
                    return(-1);
                }
                else
                {
                    bitmapt[ll] = '1';
                }
            }
        }

        /*** 021 : obligatorio si se denego la peticion  ***/
        if (!strncmp("021",plantilla + ((ll-1)*3),3))
        {                                     /***   comprueba-021   ***/
            sprintf(cadena_verif , "%d", tipomsg);
            if (cadena_verif[1] == '1' || cadena_verif[1] == '2')
            {
                if (pmt->adipre039[0] == '0')
                {
                    if (bitmapt[ll] == '1')
                    {
                        *error_code = 7;
                        *error_bit = ll;
                        return(-1);
                    }
                }
                else
                {
                    if (bitmapt[ll] == '0')
                    {
                        *error_code = 7;
                        *error_bit = ll;
                        return(-1);
                    }
                }
            }
        }                                     /***   fin-comprueba-021   ***/
    }                               /***   fin-mirar-condicion   ***/

    if (bitmapt[ll] == '1')         /***     mover-a-semp     ***/
    {
            /***
            ***/
        memset(dato_sem , '\0' , 999);
        memset(dato_mac , '\0' , 999);
        lon_dasem = 1;
        lon_damac = 1;

        /***   lee datos del bit en formato iso ftraefe2  ***/
        if ((retorno = flee_datoiso(ll)) != 0)
        {
            *error_code = 12;
            *error_bit = ll;
#if defined (TRAZA) && (TRAZA > 1)						
      fprintf(stdout, "\n>>%03d<< No existe en datos iso\n", ll);
			fflush(stdout);
#endif			
            return(-1);
        }

        if (long_fc > 1000)
        {                             /***   mover-long-variable   ***/
            resto = long_fc - 1000;
            if((empaquetado)||(ll==55))
                bytes_longitud = (resto/2) + (resto%2);
            else
                bytes_longitud = resto;

            off_long = off_fc - resto;

            strncpy(cadena_verif , pmtchar + off_long , resto);
            cadena_verif[resto] = '\0';

            area_long999 = atoi(cadena_verif);
            if(area_long999 > max_fc)
            {
#if defined (TRAZA) && (TRAZA > 1)						
                fprintf(stdout,"longitud >%d< > maximo >%d<\n", area_long999, max_fc);
#endif				
                *error_code = 21;
                *error_bit = ll;
                return(-1);
            }

            lon_damac = area_long999;
            lon_dasem = area_long999;

            if((empaquetado) || (ll==55))
            {
#if defined (TRAZA) && (TRAZA > 1)						
                fprintf(stdout,"si el campo esta empaquetado\n");
                fflush(stdout);
#endif				
                retorno = fempaque(resto , cadena_salida , pmtchar + off_long);
            }
            else
                strncpy(cadena_salida, pmtchar + off_long, resto);

        		    memcpy(dato_sem , cadena_salida , bytes_longitud);
            		memcpy(dato_mac , cadena_salida , bytes_longitud);

            /*
                fprintf(stdout, "tipo_fs ->%c<- longitud ->%.*s<- \n",
                tipo_fs, bytes_longitud, cadena_salida);
                */
            if (tipo_fs == 'B' || tipo_fs == 'X' || tipo_fs == '9')
            {
                memcpy(dato_mac + bytes_longitud , pmtchar + off_fc , lon_damac);
                memcpy(dato_sem + bytes_longitud , pmtchar + off_fc , lon_dasem);
                lon_damac = lon_damac + bytes_longitud;
                lon_dasem = lon_dasem + bytes_longitud;
            }
            else
            {
                if (tipo_fs == 'E')
                {
                    retorno = fempaque(lon_dasem , cadena_salida , pmtchar + off_fc);
                    lon_dasem_emp = (lon_dasem/2) + (lon_dasem%2);
                    memcpy(dato_sem + bytes_longitud , cadena_salida , lon_dasem_emp);
                    memcpy(dato_mac + bytes_longitud , cadena_salida , lon_dasem_emp);
                    lon_damac = lon_dasem_emp + bytes_longitud;
                    lon_dasem = lon_dasem_emp + bytes_longitud;
                }
                else
                {
                    retorno=fempaque_mixto(ll,lon_dasem,&longimix,
                        cadena_salida,pmtchar + off_fc);
                    lon_dasem_emp = longimix;
                    memcpy(dato_mac + bytes_longitud, cadena_salida , lon_dasem_emp);
                    memcpy(dato_sem + bytes_longitud, cadena_salida , lon_dasem_emp);
                    lon_damac = lon_dasem_emp + bytes_longitud;
                    lon_dasem = lon_dasem_emp + bytes_longitud;
                }
            }

#if defined (TRAZA) && (TRAZA > 5)
            fhexdump(stdout, dato_sem, lon_dasem);
            fflush(stdout);
#endif

        }                             /***   fin-mover-long-variable   ***/
        else
        {                             /***   mover-long-fija   ***/

#if defined (TRAZA) && (TRAZA > 5)
                fhexdump(stdout, pmtchar + off_fc , long_fc);
                fflush(stdout);
#endif

            if ((tipo_fc == 'X' || tipo_fc == 'B') &&
                ((retorno = nspaces(pmtchar + off_fc , long_fc)) != 0) &&
                (ll != 42))
            {
#if defined (TRAZA) && (TRAZA > 10)						
                fprintf(stdout, "traefe2 >%.*s<\n", long_fc, pmtchar + off_fc);
#endif				
                *error_code = 4;
                *error_bit = ll;
                return(-1);
            }

            if (tipo_fc == '9' && (nzeroes(pmtchar + off_fc , long_fc) != 0))
            {
                if ((ll == 4 || ll == 30) &&
                    strncmp("00",pmt->adipre003,2) &&
                    strncmp("01",pmt->adipre003,2) &&
                    strncmp("20",pmt->adipre003,2))
                {  }
                else
                {
                    if (ll == 4 &&
                        !strncmp("997" , plantilla + ((30-1)*3) , 3) &&
                        !nzeroes(pmt->adipre030,24))
                    {  }
                    else
                    {           //11.feb.09 parece que estos campos si pueden ser 0. Se incluye el campo 4, para que soporte importe 0.
                        if (ll!=4 && ll!=30 && ll!=74 && ll!=75 && ll!=76 && ll!=77 &&
                            ll != 80 && ii !=81 && ll!=86 && ll!=87 && ll!=88 && ll!=89  && ll!=97 &&
                            11 != 105 && ll != 106 && ll != 107 && ll != 108 && ll != 109 && ll != 110)
                        {
                            *error_code = 5;
                            *error_bit = ll;
                            return(-1);
                        }
                    }
                }
            }

            if (ll == 38 && !strncmp("303" , plantilla + ((ll-1)*3) , 3))
            {
                for(mm=0,pp=0 ; mm<6 ; mm++)
                    if (pmt->adipre038[mm] = ' ')  pp++;
                long_fc = 6 - pp;
            }

            if (tipo_fs == 'B' || tipo_fs == 'X' || tipo_fs == '9')
            {
                memcpy(dato_mac , pmtchar + off_fc , long_fc);
                memcpy(dato_sem , pmtchar + off_fc , long_fc);
                lon_damac = long_fc;
                lon_dasem = long_fc;
            }
            else
            {
                if (tipo_fs == 'E')
                {
                    retorno = fempaque(long_fc , cadena_salida , pmtchar + off_fc);
                    lon_dasem_emp = (long_fc/2) + (long_fc%2);
                    memcpy(dato_mac , cadena_salida , lon_dasem_emp);
                    memcpy(dato_sem , cadena_salida , lon_dasem_emp);
                    lon_damac = lon_dasem_emp;
                    lon_dasem = lon_dasem_emp;
                }
                else
                {
                    retorno=fempaque_mixto(ll,lon_dasem,&longimix,cadena_salida,pmtchar + off_fc);
                    lon_dasem_emp = longimix;
                    memcpy(dato_mac , cadena_salida , lon_dasem_emp);
                    memcpy(dato_sem , cadena_salida , lon_dasem_emp);
                    lon_damac = lon_dasem_emp;
                    lon_dasem = lon_dasem_emp;
                }
            }

        }                             /***   fin-mover-long-fija   ***/

        memcpy(area_sem + long_calc , dato_sem , lon_dasem);

        if (ll == 53)
        {   /***   para 4b pondremos un campo de p-53 a "00" (SIN MAC)  ***/
            puntero_clavemac = long_calc + 4;
        }
        long_calc = long_calc + lon_dasem;

        if (mac_fs == 'S')
        /***   la caixa no usa los campos 25 y 38 para calcular el mac   ***/
        /*if (mac_fs == 'S' &&
            (suba_iso != 1 || ll != 25 && ll != 38))*/
        {
            memcpy(area_mac + long_calcm , dato_mac , lon_damac);
            long_calcm = long_calcm + lon_damac;
            long_mac = long_mac + lon_damac;
        }
    }    /***     fin-mover-a-semp     ***/

  }      /***   fin del bucle principal   ***/

/***     comprime el mapa de bits a enviar a semp    ***/
  opciex = 2;
  strncpy(mapa_exp , bitmapt + 1 , 64);
  mapa_exp[64] = '\0';
  memset(mapa_com , ' ' , 8);
  mapa_com[8] = '\0';


  if (fexplbit(opciex,mapa_exp,mapa_com) != 0)
  {
    *error_code = 28;
    *error_bit = 0;
    return(-1);
  }

  memcpy(area_bitmap,mapa_com,8);
  if (bitmapt[1] == '1')
  {
    strncpy(mapa_exp , bitmapt + 65 , 64);
    mapa_exp[64] = '\0';
    memset(mapa_com , ' ' , 8);
    mapa_com[8] = '\0';

    if (fexplbit(opciex,mapa_exp,mapa_com) != 0)
    {
        *error_code = 28;
        *error_bit = 0;
        return(-1);
    }

    memcpy(area_bitmap + 8 ,mapa_com,8);
  }

/***   empaquetamos el tipo de mensaje   ***/
  if(empaquetado)
    retorno = fempaque( 4 , cadena_salida , pmt->adipre000 );
  else
    strncpy(cadena_salida , pmt->adipre000, 4);

  if (bitmapt[1] == '0')
    lon_bitmap = 8;
  else
    lon_bitmap = 16;
  if(empaquetado)
    lon_tipomsg = 2;
  else
    lon_tipomsg = 4;

  memcpy(ppr      , cadena_salida   , lon_tipomsg);
  memcpy(ppr + lon_tipomsg, area_bitmap, lon_bitmap);
  memcpy(ppr + lon_tipomsg + lon_bitmap, area_sem, long_calc);
  long_iso = long_calc + lon_tipomsg + lon_bitmap;

	fprintf(stdout,"Control de salida ftraefe2\n"); //PRJ0258542
  return(0);
}


/*****************************************************************
FUNCION  FEMPAQUE_MIXTO        llamada por ftraefe2

DESCRIPCION: Funcion que trata los campos con formato mixto, cuyo
tratamiento debe ser particularizado

AREA DE PASO :
-
*****************************************************************/
int     fempaque_mixto(int ll,int longi,int *longimix,
                         char *sempaq,char *sdesem)
{
    int retorno;
    char codigo[3];
    char cadena_in[999];
    char cadena_ou[999];

  
  switch (ll)
  {
  case 48:
  case 55:
    codigo[2]='\0';
    strncpy(codigo, sdesem + 3, 2);
    switch(atoi(codigo))
    {
    case 2:
            /*  no empaquetamos ninguno  */

        strncpy(sempaq, sdesem , longi);
        *(longimix) = longi;

        break;
    }
    break;
  case 72:
    /*   se empaquetan los 3 primeros    */

    strncpy(cadena_in , sdesem, 3);
    retorno = fempaque(3,cadena_ou,cadena_in);
    memcpy(sempaq, cadena_ou , 2);
    memcpy(sempaq+2, sdesem + 3, longi-3);

    *(longimix) = longi-1;

    break;
  case 97:
    /*   el primer caracter no se empaqueta, y los 16 siguientes si   */
    *(sempaq) = *(sdesem);

    strncpy(cadena_in , sdesem + 1 , 16);
    retorno = fempaque(16,cadena_ou,cadena_in);

    memcpy(sempaq + 1, cadena_ou , 8);

    *(longimix) = 9;

    break;
  }
  return(0);
}


/*****************************************************************
FUNCION  FEXPLBIT             llamada por ftradred

DESCRIPCION: Funcion que convierte un mapa de bits de 8 bytes a
una cadena de 64 ceros y unos, y viceversa

AREA DE PASO :
- entero con el sentido de la expansion :
1.- expandir     (traducion price-->netplus)
2.- comprimir    (traducion netplus-->price)

- puntero a char con mapa de bits expandido (64 bytes)

- puntero a char con mapa de bits comprimido (8 bytes)
*****************************************************************/
int fexplbit(int opciex, char *pmapa_exp, char *pmapa_com)
{

    /***   el orden de los bits aparece aqui de derecha a izquierda (SCO),  de izquierda a derecha (HPUX),
    pero esto es algo variable dependiente de la maquina   ***/

//#if defined (SCO_NETPLUS) || defined (EC_NT)
    typedef struct ochobits
    {  unsigned  int bit_7  :  1;
    unsigned  int bit_6  :  1;
    unsigned  int bit_5  :  1;
    unsigned  int bit_4  :  1;
    unsigned  int bit_3  :  1;
    unsigned  int bit_2  :  1;
    unsigned  int bit_1  :  1;
    unsigned  int bit_0  :  1;
    } ochobits;
//#else
//    typedef struct ochobits
//    {  unsigned  int bit_0  :  1;
//    unsigned  int bit_1  :  1;
//    unsigned  int bit_2  :  1;
//    unsigned  int bit_3  :  1;
//    unsigned  int bit_4  :  1;
//    unsigned  int bit_5  :  1;
//    unsigned  int bit_6  :  1;
//    unsigned  int bit_7  :  1;
//    } ochobits;
//#endif

    char caracter;
    /***   convierte el tipo char de la variable caracter
    a tipo ochobits mediante la variable workbyte   ***/

    ochobits *workbyte = (ochobits*) &caracter;
    ochobits workbyte2;

    /***   convierte el tipo ochobits de la variable workbyte2
    a tipo char  mediante la variable caracter2   ***/

    char *caracter2 = (char*) &workbyte2;


    int ll;
//#ifdef TRAZABIT
    int mm;
//#endif


  switch (opciex)
  {
  case 1:
    for(ll=0;ll<8;ll++)
    {
        caracter = pmapa_com [ll];
       // fprintf(stdout,"caracter nuevo %s\n", (char*)workbyte); //si quito esta traza da error!!!!!
       // fflush(stdout);	

        strncpy(pmapa_exp + 8*ll + 0, itoa(workbyte->bit_0), 1); 
        strncpy(pmapa_exp + 8*ll + 1, itoa(workbyte->bit_1), 1); 
        strncpy(pmapa_exp + 8*ll + 2, itoa(workbyte->bit_2), 1); 
        strncpy(pmapa_exp + 8*ll + 3, itoa(workbyte->bit_3), 1); 
        strncpy(pmapa_exp + 8*ll + 4, itoa(workbyte->bit_4), 1); 
        strncpy(pmapa_exp + 8*ll + 5, itoa(workbyte->bit_5), 1); 
        strncpy(pmapa_exp + 8*ll + 6, itoa(workbyte->bit_6), 1); 
        strncpy(pmapa_exp + 8*ll + 7, itoa(workbyte->bit_7), 1); 
#if 0
            strncpy((pmapa_exp + 8*ll + 0 ),(itoa_ux(workbyte->bit_0)),1);
            strncpy((pmapa_exp + 8*ll + 1 ),(itoa_ux(workbyte->bit_1)),1);
            strncpy((pmapa_exp + 8*ll + 2 ),(itoa_ux(workbyte->bit_2)),1);
            strncpy((pmapa_exp + 8*ll + 3 ),(itoa_ux(workbyte->bit_3)),1);
            strncpy((pmapa_exp + 8*ll + 4 ),(itoa_ux(workbyte->bit_4)),1);
            strncpy((pmapa_exp + 8*ll + 5 ),(itoa_ux(workbyte->bit_5)),1);
            strncpy((pmapa_exp + 8*ll + 6 ),(itoa_ux(workbyte->bit_6)),1);
            strncpy((pmapa_exp + 8*ll + 7 ),(itoa_ux(workbyte->bit_7)),1);
#endif
    }
    break;
  case 2:
    for(ll=0;ll<8;ll++)
    {
        workbyte2.bit_0 = ((pmapa_exp [ 8*ll + 0 ] ) == '1') ? 1 : 0;
        workbyte2.bit_1 = ((pmapa_exp [ 8*ll + 1 ] ) == '1') ? 1 : 0;
        workbyte2.bit_2 = ((pmapa_exp [ 8*ll + 2 ] ) == '1') ? 1 : 0;
        workbyte2.bit_3 = ((pmapa_exp [ 8*ll + 3 ] ) == '1') ? 1 : 0;
        workbyte2.bit_4 = ((pmapa_exp [ 8*ll + 4 ] ) == '1') ? 1 : 0;
        workbyte2.bit_5 = ((pmapa_exp [ 8*ll + 5 ] ) == '1') ? 1 : 0;
        workbyte2.bit_6 = ((pmapa_exp [ 8*ll + 6 ] ) == '1') ? 1 : 0;
        workbyte2.bit_7 = ((pmapa_exp [ 8*ll + 7 ] ) == '1') ? 1 : 0;
        pmapa_com[ll] = *caracter2;
    }
    break;
  }

//#ifdef TRAZABIT
    fprintf(stdout,"--------------------------------------------------\n");

    for (ll=0;ll<8;ll++)
    {
        for (mm=0;mm<8;mm++)
            fprintf(stdout,"%c",*(pmapa_exp + (ll*8) + mm));
        fprintf(stdout," ");
    }

    fprintf(stdout,"\n");
    fflush(stdout);
    for (ll=0;ll<8;ll++)
    {
        caraprin = *(pmapa_com + ll);
        if(caraprin < 16)
            fprintf(stdout,">>>0%X<<< ", caraprin);
        else
            fprintf(stdout,">>>%X<<< ", caraprin);
    }
    fprintf(stdout,"\n--------------------------------------------------\n");
    fflush(stdout);
//#endif

  return (0);
}


/**********************************************/
/***   funcion: carga tablas de traduccion  ***/
/**********************************************/
int fcarga_tablas(char * red)
{
    int     fcarga_tbitmap(char * red);
    int     fcarga_tdatiso(char * red);
    int     fcarga_tdatapl(char * red);


    if (fcarga_tbitmap(red))
    {
#if defined (TRAZA) && (TRAZA > 1)				
        fprintf(stdout,"error carga tabla bitmap \n");
#endif		
        return(-1);
    }
    else
    {
        if (fcarga_tdatiso(red))
        {
#if defined (TRAZA) && (TRAZA > 1)					
            fprintf(stdout,"error carga tabla datiso \n");
#endif			
            return(-1);
        }
        else
        {
            if (fcarga_tdatapl(red))
            {
#if defined (TRAZA) && (TRAZA > 1)						
                fprintf(stdout,"error carga tabla datapl \n");
#endif				
                return(-1);
            }
            else
            {
                return(0);
            }
        }
    }
}  /*** fin de la funcion fcarga_tablas ***/



/**********************************************/
/***   funcion: carga tabla de datiso       ***/
/**********************************************/
int fcarga_tdatiso(char * red)
{

#include "nptdiso.h"

    NPSDISO regdiso;

    char clave[13];
    char ch05[5];
    char ch04[4];

    int ll,indice,salir;
    int cod_err2;


    for (ll=0 ; ll<128 ; ll++)
    {  tabla_iso[ll].tipo_fs = ' ';
    tabla_iso[ll].long_fs = 0;
    tabla_iso[ll].max_fs = 0;
    tabla_iso[ll].mac_fs = ' ';
    }

    ch04[3]='\0';
    ch05[4]='\0';
    /***---------- lee el primero con red  -----------***/
    rpad(clave , '\0' , 12);
    strncpy(clave     , red   , 2);
    strncpy(clave + 2 , "000"  , 3);


    salir=0;
    
    cod_err2=tratar_fich("NPTDISO",CARGAR,&tabla_iso, clave); 
    
   
    if (cod_err2)
    {
#if defined (TRAZA) && (TRAZA > 1)				
        fprintf(stdout,"Error al cargar NPTDISO %03d\n", cod_err2);
#endif	
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Leer NPTDISO codigo de error >%03d< \n",cod_err2);
	
         return(-1);
    }
    return(0);
}



/**********************************************/
/***   funcion: carga tabla de datapl       ***/
/**********************************************/
int fcarga_tdatapl(char * red)
{
#include "nptdap2.h"
    NPSDAP2 regdap2;

    char clave[13];

    int ll,indice,salir;
    int cod_err2;


    for (ll=0 ; ll<128 ; ll++)
    {
        tabla_apl[ll].tipo_fc = ' ';
        tabla_apl[ll].long_fc = 0;
        tabla_apl[ll].max_fc = 0;
        tabla_apl[ll].off_fc = 0;
    }

    /***---------- lee el primero con red  -----------***/
    rpad(clave , '\0' , 12);
    strncpy(clave     , red   , 2);
    strncpy(clave + 2 , "000"  , 3);


    salir=0;
    cod_err2=tratar_fich("NPTDAP2",CARGAR,&tabla_apl, clave);
    if (cod_err2 )
    {
#if defined (TRAZA) && (TRAZA > 1)				
        fprintf(stdout,"Error NPTDAP2 con %03d\n", cod_err2);
#endif	
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Leer NPTDAP2 codigo de error >%%03d< \n",cod_err2);
	
        return(-1);
    }   /***   fin del bucle   ***/
    return(0);
}


/**********************************************/
/***   funcion: carga tabla de bitmaps      ***/
/**********************************************/
int fcarga_tbitmap(char * red)
{
#include "nptbima.h"
    NPSBIMA regbima;


    char clave[13];


    int ll,salir;
    int cod_err2;


    for (ll=0 ; ll<100 ; ll++)
    {
        memset(tabla_bim[ll].tipo_bm , ' ', 4);
        tabla_bim[ll].sent_bm = ' ';
        tabla_bim[ll].perv_bm = ' ';
        memset(tabla_bim[ll].bim1_bm , ' ' , 192);
        memset(tabla_bim[ll].bim2_bm , ' ' , 192);
    }

    /***---------- lee el primero con red 20 (PRICE) -----------***/
    rpad(clave , '\0' , 12);
    strncpy(clave     , red  , 2);
    strncpy(clave + 2 , "0000" , 4);
    strncpy(clave + 6 , " "    , 1); 
    salir=0;
    cod_err2=tratar_fich("NPTBIMA",CARGAR,&tabla_bim,clave);
	
    if (cod_err2)
    {
#if defined (TRAZA) && (TRAZA > 1)				
        fprintf(stdout,"Error %03d en NPTBIMA\n", cod_err2);
#endif	
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Leer NPTBIMA codigo de error >%03d< \n",cod_err2);
	
        return(-1);
    }   /***   fin del bucle   ***/

    return(0);
}


/******************************************************************
pone los saldos en la posicion psaldo
******************************************************************/
void pone_saldos(char * psaldo, iso48saldos * saldos)
{
    int offset = 0;
    int i;
    int num_cuentas;
    char cantidad[2];
	

    /***---VERIFICACION DE LA INFORMACION EN SALDOS***/

    strncpy(psaldo, "026", 3);
    offset += 3;
    strncpy(psaldo + offset, "05", 2);
    offset += 2;
    strncpy(psaldo + offset, saldos->num_ctas, 1);
    offset += 1;
    memcpy(psaldo + offset, (char *)&saldos->saldo_dat, 110); 
    offset += 110;
}

/******************************************************************
pone los movimientos en la posicion pmovim
******************************************************************/
void pone_movimientos(char * pmovim, iso48sb06 * movimientos)
{
    int offset = 0;
    int num_mov;
    int i;
    char cantidad[2];
    char Longitud[4];

//    strncpy(pmovim, "227" , 3); 
    offset += 3;
    strncpy(pmovim + offset, "06", 2);
    offset += 2;
    strncpy(pmovim + offset, movimientos->fecha, 6);
    offset += 6;
    strncpy(pmovim + offset, movimientos->signo, 1);
    offset += 1;
    strncpy(pmovim + offset, movimientos->importe,12);
    offset += 12;
    strncpy(pmovim + offset, movimientos->moneda, 3);
    offset += 3;
    strncpy(pmovim + offset, movimientos->num_movi,1);
    offset += 1;
    strncpy(cantidad,movimientos->num_movi,1);

    cantidad[1]='\0';
    num_mov = atoi(cantidad);
  
  
    for (i = 0;i<num_mov;i++)
    {
      memcpy(pmovim + offset, movimientos->movimi[i], 34);
      offset += 34;
      printf("movimientos <%i> <%s>\n", i, movimientos->movimi[i]);
    }
    sprintf(Longitud, "%03d", offset-3);
    strncpy(pmovim, Longitud, 3);

#if defined (TRAZA) && (TRAZA > 10)				
    //fprintf(stdout, "en pone_movimientos:\n");
    //fhexdump(stdout, &movimientos->movimi, 227 - offset);  
#endif	
}

//PRJ02585542 - INI
int convierte_hex (char * vaddr, int len, char * cadsal)
{
 	int lonHexa = 0;
 	int i = 0; 
 	int numCodAsci=0;
 	char auxHexa8[8];
	//fprintf(stdout,"Entra >%.*s< \n",len,vaddr);
  for(i = 0; i<len; i++)
  {          
  	numCodAsci=vaddr[i];
    if(numCodAsci<0)
    {
    	sprintf(auxHexa8, "%02X", vaddr[i]);  //1 caracter ascii lo convierte en 8 cifras hexadecimales
                                                   //de las que las 2 ultimas son las significativas
      sprintf(cadsal+i*2, "%.2s", auxHexa8+6);  //convierte 1 caracter ascii en 2 hexadecimales
		}                                                     
    else
    {
    	sprintf(cadsal+i*2, "%02X", vaddr[i]);  //convierte 1 caracter ascii en 2 hexadecimales
    }
	}
  lonHexa = 2 * len;
  cadsal[lonHexa]='\0';
  //fprintf(stdout,"Sale >%.*s< \n",lonHexa,cadsal);
}
//PRJ02585542 - FIN

#include "etiquetas_A_fi.c"
#include "fsemaforo_red.c"
#include "chkbit.c"
#include "traeetiq.c"
#include "montaR55.c"
//#include "fcifraPan.c"

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
