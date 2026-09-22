//PRJ0247167 - Revisado ampliación P48
#include<sys/shm.h>
#include<sys/ipc.h>
#include<stdio.h> 
#include <errno.h> 
#include "svm.h"
#include "llaves1.h"
#include "max_min.h"
#include "deffich.h"
#include "nptpara.h" 

key_t dame_llave(char clave);
void  amplia_Q(int cola,int tamano);


//añadido para arrancar la configuracion por bbdd

NPSIPCS  *reg_ipcs;

//crear el SVM para todas las entidades unificadas
int crearSVM(void **gen_psvm,int *shmid,int operacion,char *nombre)
{
  key_t llave;
  int tam,flag;
  int tamsvm;
  int numbin;
  int numreg;
  int numero;
  int indice;
  void *p;
  SVM *Psvm;
  char nomSVMBINE[5];
  char nombreSVM[5];
  char *prueba;
  int err;
  char letra[2];
  char  clave;
  int tamanoSVM;
  #define SVM_PARS (Psvm->str_svm.svmpars)
  
  // PRJ0202598 cambio bin de 6 posiciones 8 se cambia TAM_BIN y cambia el tamaño total de tbbine
  tbbine *psvmbine;
  
  //recorro la tabla de elementos que hay que arrancar, para encontrar el que voy a levantar
    indice=0;
	
    while (strncmp(tabla_ipcs[indice].ipcsnomb,"     ",5)!=0)
   {
	
    if (strcmp(nombre,tabla_ipcs[indice].ipcsnomb)==0)  
    {	          
       strncpy(nombreSVM, tabla_ipcs[indice].ipcsnomb,4);
	   nombreSVM[5]='\0';
	   tamanoSVM=((MAX_REG_SVM+1)*sizeof(SVM));
	   tabla_ipcs[indice].ipcstama=tamanoSVM;  	   
       break;    
    }	   
    else                                                                                                            
       indice++;
   }

  if(tabla_ipcs[indice].ipcsnomb ==NULL)
  {
     return (E_NOIPC);
  }
   //RAQUEL añadido para arranque de entidades por bbdd
	clave=*(tabla_ipcs[indice].ipcsclav);
	
 /* Me devuelve una llave que siempre es la misma para la misma clave*/
    if ((llave=dame_llave(clave)) ==(size_t) -1)
    {
     return (E_NOKEY);
	}
	
#if 0

//tamaño de los SVM
  if (operacion=='0')
    {
	//RAQUEL recorremos la tabla para buscar el nombre del smvbine

      for (i=0 ; i<50 ; i++)
      {
	    if ((strncmp(tabla_ipcs[i].ipcsnsvm,"SVMBINE",7)==0))
         {		
          strncpy(nomSVMBINE, tabla_ipcs[i].ipcsnomb,4);  
	      nomSVMBINE[5]='\0';
          break;            
         }   
      }
      
     numbin=cuenta_reg("NPTBINE") + 100; //numero de registro de bines + 100
     tam=0;
    // if (strcmp(nomsvm,N_SVMBINE)==0)
	//   if (strcmp(nomsvm,nomSVMBINE)==0)
	   if (strcmp(nombre,nomSVMBINE)==0)
       {
        tam=sizeof(tbbine)*numbin;
       }
    // tamsvm=max(tam,tabla_ipcs[indice].ipcstama);
	   tamsvm=max(tam,((MAX_REG_TBBINE + 1)*sizeof(tbbine)));
    }
#else
     tamsvm=tabla_ipcs[indice].ipcstama;
     numbin=0;
#endif
  tamsvm=tabla_ipcs[indice].ipcstama;

  if (tamsvm < 1)
   return (-1);
  
  switch (operacion)
  {
    case 0:  flag = CREAR_IPC;
             break;
    case 1:  flag = CONEC_IPC;
             break;
    default: flag = CONEC_IPC;
             break;
  }
  
  //**************para prueba****
  
  // tamsvm=40;  
  // fprintf(stderr,"TamSvm=%d\n",tamsvm);
   
 /* Creo la SVM */
  //BIMSF-3087
  fprintf(stdout,"---- Va a crear el area de memoria ----\n");
 
  if ((*shmid = shmget(llave,tamsvm,flag)) == -1) //Alloca memoria compartida en UNIX y devuelve un identificadro de esta memoria
                        //A new shared memory segment, with size equal to the value of size rounded up to a multiple of PAGE_SIZE,
                        //is created if key has the value IPC_PRIVATE or key isn't IPC_PRIVATE, no shared memory segment corresponding
                        //to key exists, and IPC_CREAT is specified in shmflg. 
                        //If shmflg specifies both IPC_CREAT and IPC_EXCL and a shared memory segment already exists for key, 
                        //then shmget() fails with errno set to EEXIST. 
    {
	//BIMSF-3087
	fprintf(stdout,"Error devuelto por shmget: %d\n", errno);
	fprintf(stdout,"Error devuelto por shmget: %s\n", strerror(errno));
#if defined (TRAZA) && (TRAZA > 1)
   //  fprintf(stderr,"ERROR(%d): Imposible conseguir la S.V.M.\n",errno);
     fprintf(stderr,"retorna E_NOSVM\n");
#endif
     //BIMSF-3087
	 fprintf(stdout,"Retorna valor E_NOSVM: %i\n", E_NOSVM);
     return (E_NOSVM);
    }
  //BIMSF-3087
  fprintf(stdout,"Segmento asignado shmid %d\n", shmid);
  fprintf(stdout,"---- Termina de crear el area de memoria ----\n");
  
 /* Me conecto a la SVM, mapeo el segmento de memoria compartida en el area de datos, el sistema seleciona 
          la primera direccion disponible*/
  if ((p = shmat(*shmid,0,0)) == NULL) //Lo conecta, crea el puntero que apunta a la memoria compartida.
    {
/*#if defined (TRAZA) && (TRAZA > 1)
     fprintf(stderr,"ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
#endif */
     return (E_CONSVM);
    }
    //inicializo si la creo de nuevas

  if (operacion==0)
    {
     memset(p, '\0', tamsvm);  // inicializa los 957008 bytes a NULOS
	 //if (strcmp(nomsvm,N_SVM)==0)
     if (strcmp(nombre,nombreSVM)==0)
       {
        Psvm=p;
        tam=0;
        while (tam<MAX_REG_SVM+1)
           {
            Psvm[tam++].svmtip=NO_TIPO;
           }
        Psvm[0].svmtip=TIP_SUPER;
        Psvm[0].str_svm.svmsupe.pid=-1;
        Psvm[0].str_svm.svmsupe.IDsvmsock=-1;
        Psvm[0].str_svm.svmsupe.IDsvmbine=-1;
        Psvm[0].NBinesSvm=numbin;
       }
    }

  *gen_psvm = p;

  return (0);
  
}

 char carga_colas(char *cdcola,int *tamano)
 {
  int indice;        
  char letra; 
  char aux_clave[1];  
  int result=0;
  
  indice=0;  
  
  while ((tabla_ipcs[indice].ipcsnomb != NULL) &&
         (strcmp(tabla_ipcs[indice].ipcsnomb,cdcola)!=0))
 {
    indice++;
  }
 
 #if defined (TRAZA) && (TRAZA > 10)
  fprintf(stderr,"Pedido %s encontrado %s retornado %s\n",cdcola,
         tabla_ipcs[indice].ipcsnomb, tabla_ipcs[indice].ipcsclav);
 #endif
  *tamano=tabla_ipcs[indice].ipcstama;
  strncpy(aux_clave, tabla_ipcs[indice].ipcsclav, 1);
  letra=*(aux_clave);
  
  return(letra);
 }


int trata_colas(char cola,int tamano,int *idcola)
{
  int err;
  int    q_id;
  key_t llave1;
  

   err=0;
   if ((llave1=dame_llave(cola)) ==(key_t) -1)
       {
#if defined (TRAZA) && (TRAZA > 1)
          fprintf(stderr,"ERROR(%d): Imposible crear la llave ..%c..\n",
                  errno,cola);
#endif
          return(1);
       }

   if ((q_id = msgget(llave1,CREAR_IPC)) == -1) //crea la cola
     {
     if (errno == ENOSPC) 
       {
#if defined (TRAZA) && (TRAZA > 1)
        fprintf(stderr,"Imposible crear mas colas en el sistema\n"); 
#endif
        return(ENOSPC);
       }
#if defined (TRAZA) && (TRAZA > 1)
      fprintf(stderr,"Imposible crear cola %d \n",errno); 
#endif
     }
   else
	{ 
     *idcola=q_id;
	}

   amplia_Q(q_id,tamano);
 
   return(0);

}

//borra el SVM segun para, uno u todos
int borrarSVM(char *nomsvm,char *pars)
{
  int error;
  int indice;
  int shmid;
  key_t llave;
  struct shmid_ds buf;
  char letra;

#if defined (TRAZA) && (TRAZA > 1)
      fprintf(stderr,"BorrarSVM(%s, %s)\n",nomsvm,pars);
#endif
//solo borro cuando es para todos o para el NUCLEO
  if (!strcmp(pars,NUCLEO) && !strcmp(pars,TODO))
     return (0);
  
  indice=0;
  while (tabla_ipcs[indice].ipcsnomb != NULL) //leo todos los elementos, colas y SVM, y recupero el que busco
   { 
    if (strcmp(nomsvm,tabla_ipcs[indice].ipcsnomb)==0) //para la SVM de entrada
       break;
    else                                                                                                  
       indice++;                                                                                          
   }                                                                                                      

  if (tabla_ipcs[indice].ipcsnomb == NULL)  // no lo encuentra
     return (E_NOIPC);

 /* Me devuelve una llave */
 // strncpy(letra, reg_ipcs[indice].ipcsclav,1);
 //   strcpy (letra, reg_ipcs[indice].ipcsclav);
   letra=*(tabla_ipcs[indice].ipcsclav);
   if ((llave=dame_llave(letra)) ==(size_t) -1) //llave del SVM
    return (E_NOKEY);

  if ((shmid = shmget(llave,tabla_ipcs[indice].ipcstama,CONEC_IPC)) == -1) //no existe
    {
     return (E_NOSVM);
    }

  if ((error = shmctl(shmid,IPC_RMID,&buf)) == -1) //borra el SVM
         {
          return (E_NOSVM);
         }
  return (0);
}
