#include <stdio.h>
#include <errno.h> 
#include "cnetplus.h"
#include "datagen.h"
#include "svm.h"
#include "svmsock.h"
#include "colas.h"
#include "deffich.h"
#include "nptpara.h"
#include "nptcone.h"
#include "nptpars.h"
#include "npttrms.h"
#include "arranque.h"
#include "llaves1.h"

SVM *Psvm;
#define SVM_PARS (Psvm->str_svm.svmpars)

char nomMemo[1];
char nombre[5];
NPSIPCS  *reg_ipcs;
int carga_ipcs(char * indice);
NPSPARA  *reg_para;
int carga_para(char * indice_para);
NPSCONF *reg_conf;
int carga_conf(char * indice_conf);


// RAQUEL     En vez de pasar el nomsvm podemos pasar el svm_pars montado

int crearSVM(void **gen_psvm,int *shmid,int operacion,char *nombre);

// RAQUEL    pasamos nombre como variable global conseguida en IPCS bbdd
 int conecSVM(void **gen_psvm)
 {
 
 int err=0;
 int shmid;
 char indice_ipcs[2]; //siempre sera 00 para cargar toda la tabla NPTIPCS
 int i; //para recorrer la tabla IPCS y buscar
 int result=0;
 char indice_para[2];
 int resultado=0;
 char indice_conf[2];
 
 //RAQUEL SOLO PARA COMPROBAR, QUITAR DESPUES
 
 strncpy(indice_para, "00",2);
 indice_para[2]='\0';
 i=0;
 
 if ((result = carga_para(indice_para)) != 0)
 {
  #if defined (TRAZA) && (TRAZA > 1)   
     fprintf(stderr,"error carga tabla NPTPARA \n");
  #endif	   
 }
   strncpy(indice_ipcs, "00",2);
   indice_ipcs[2]='\0'; 
   i=0;
   
   if ((err = carga_ipcs(indice_ipcs)) != 0)
   {
    #if defined (TRAZA) && (TRAZA > 1)   
       fprintf(stderr,"error carga tabla NPTIPCS \n");
    #endif	   
   }
   for (i=0 ; i<30 ; i++)
   {
	  if ((strncmp(tabla_ipcs[i].ipcsnsvm,"NSVM",4)==0))
      {		
       strncpy(nombre, tabla_ipcs[i].ipcsnomb,5);  
       break;            
      }   
   }
 
 fprintf(stdout,"nombre al crear conecSVM %s\n", nombre);
 fflush(stdout);
 err=crearSVM(gen_psvm,&shmid,1,nombre); 
 fprintf(stdout,"crear conecSVM err %i\n", err);

 return(err);
 }

 int crear_svmbine(tbbine **gen_psvm,int *shmid,int operacion,char *nombine)
 {
 int err;

 fprintf(stdout,"nombre al crear svmbine %s\n", nombine);
 fflush(stdout);
 
 err=crearSVM((void **) gen_psvm,shmid,operacion,nombine);
 return(err);
 }

// RAQUEL     En vez de pasar el nomsvm podemos pasar el svm_pars montado
 int crear_svmsock(void **gen_psvm,int *shmid,int operacion,char *nomock)
 {
 int err; 

 fprintf(stdout,"nombre al crear svmsock %s\n", nomock);
 fflush(stdout);
 err=crearSVM((void **) gen_psvm,shmid,operacion,nomock);
 return(err);
 }

int ConecSVMaux1(void *gen_psvm,int shmid) 
{

 /* Me conecto a la SVM */
  if ((gen_psvm = shmat(shmid,0,0)) == NULL)
    {
/*#if defined (TRAZA) && (TRAZA > 1)
     fprintf(stderr,"ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
#endif*/
     return (E_CONSVM);
    }

  return (0);
}

int ConecSVMaux(void **gen_psvm,int shmid)
{

 /* Me conecto a la SVM */
  if ((*gen_psvm = shmat(shmid,0,0)) == NULL)
    {
/*#if defined (TRAZA) && (TRAZA > 1)
     fprintf(stderr,"ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
#endif */
     return (E_CONSVM);
    }

  return (0);
}

int carga_ipcs(char * indice)
{
  int err;
  int ll;
  char clave[2];
  
    for (ll=0 ; ll<50 ; ll++)
    {  
    memset(tabla_ipcs[ll].ipcsindi, ' ' , 3);
    memset(tabla_ipcs[ll].ipcsnomb, ' ' , 5);
    memset(tabla_ipcs[ll].ipcsclav, ' ' , 1);
    tabla_ipcs[ll].ipcstama =0;
    }  
      /***---------- lee el primero con indice  -----------***/

    strncpy(clave     , indice   , 2);
    clave[2] = '\0';

//RAQUEL añadido para cargar la configuracion por bbdd 
    printf("opcion de llamar en conseguir_svm %i\n",CARGAR);
    fflush(stdout);
  err=tratar_fich("NPTIPCS",CARGAR,&reg_ipcs,clave); 
  
  if (err)
  {
#if defined (TRAZA) && (TRAZA > 1)				
       fprintf(stdout,"Error %03d en NPTIPCS\n", err);
#endif	
       //Añadido para Monitorizacion SISMA 
      fprintf(stderr,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Leer NPTIPCS codigo de error >%03d< \n",err);
	
        return(-1);
  }  

   return(0);
}

int carga_para(char * indice_para)
{
	int err;
  int ll;
  char clave[2];
   
  for (ll=0 ; ll<3 ; ll++)
  {  
		memset(tabla_para[ll].psindice,' ',3);
	  memset(tabla_para[ll].pscsb000,' ',5);         
	  memset(tabla_para[ll].psnoment,' ',21);        
	  memset(tabla_para[ll].psnumope,' ',7);         
	  memset(tabla_para[ll].psnument,' ',3);         
	  memset(tabla_para[ll].pshcamse,' ',7);         
	  memset(tabla_para[ll].psfeinse,' ',9);         
	  memset(tabla_para[ll].pshoinse,' ',7);         
	  memset(tabla_para[ll].psmodocs,' ',2);         
	  memset(tabla_para[ll].pstimesp,' ',7);         
	  memset(tabla_para[ll].psnumcross,' ',11);      
	  memset(tabla_para[ll].pscerrando,' ',2);       
	  memset(tabla_para[ll].psresolu,' ',3);   
	  memset(tabla_para[ll].psmonito,' ',3);   
	  memset(tabla_para[ll].pscotrms,' ',3);  
	  memset(tabla_para[ll].psidered,' ',2);  
	  memset(tabla_para[ll].pscltrat,' ',9);  
	  memset(tabla_para[ll].psclenvi,' ',5);  
	  memset(tabla_para[ll].psclrece,' ',9);         
	  memset(tabla_para[ll].psclvhsm,' ',51);		
	  //PRJ0258542 - anadidos los 12 nuevos campos
	  memset(tabla_para[ll].psclzmka,' ',51);		
	  memset(tabla_para[ll].psclzmki,' ',51);		
	  memset(tabla_para[ll].psclzika,' ',51);		
	  memset(tabla_para[ll].psclziki,' ',51);		
	  memset(tabla_para[ll].psclzpka,' ',51);		
	  memset(tabla_para[ll].psclzpki,' ',51);		
	  memset(tabla_para[ll].psclzaka,' ',51);		
	  memset(tabla_para[ll].psclzaki,' ',51);		
	  memset(tabla_para[ll].psclzeka,' ',51);		
	  memset(tabla_para[ll].psclzeki,' ',51);		
	  memset(tabla_para[ll].psclclap,' ',51);		
	  memset(tabla_para[ll].psclblop,' ',51);		
	}  
	
  /***---------- lee el primero con indice  -----------***/
 
  strncpy(clave, indice_para, 2);
  clave[2] = '\0';
 
 	//RAQUEL añadido para cargar la configuracion por bbdd 
  err=tratar_fich("NPTPARA",CARGAR,&reg_para,clave); 
   
 	//fprintf(stdout,"Despues de CARGAR NPTPARA en arranque error %d\n", err); //PRJ0258542
  if (err)
  {
 #if defined (TRAZA) && (TRAZA > 1)				
        fprintf(stdout,"Error %03d en NPTPARA\n", err);
 #endif	
  	//Añadido para Monitorizacion SISMA 
    fprintf(stderr,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Leer NPTPARA codigo de error >%03d< \n",err);
 	
    return(-1);
 	}  
   
  reg_para=tabla_para;
  return(0);
}

//int leer_conf(char * indice, NPSCONF *tabla_conf)
int carga_conf(char * indice_conf)
{
  int err;
  int ll;
  char clave[2];
  
 	for (ll=0 ; ll<30 ; ll++)
  {  
  	memset(tabla_conf[ll].confcdsu, ' ' , 5);
    memset(tabla_conf[ll].confdsej, ' ' , 11);
    memset(tabla_conf[ll].confpara, ' ' , 21);
		memset(tabla_conf[ll].confcola, ' ' , 5);
		memset(tabla_conf[ll].conftipp, ' ' , 2);
		tabla_conf[ll].confnmpr =0;
		tabla_conf[ll].confcapr =0;
		memset(tabla_conf[ll].confcsal, ' ' , 5);
 	}  
  /***---------- lee el primero con indice  -----------***/

  strncpy(clave     , indice_conf   , 2);
  clave[2] = '\0';

	//RAQUEL añadido para cargar la configuracion por bbdd 
  err=tratar_fich("NPTCONF",CARGAR,&reg_conf,clave); 
  if (err)
  {
#if defined (TRAZA) && (TRAZA > 1)				
       fprintf(stdout,"Error %03d en NPTCONF\n", err); 
#endif	
  	//Añadido para Monitorizacion SISMA 
    fprintf(stderr,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Leer NPTCONF codigo de error >%03d< \n",err);
	
    return(-1);
 	} 
  reg_conf=tabla_conf; 
  
  return(0);
}
