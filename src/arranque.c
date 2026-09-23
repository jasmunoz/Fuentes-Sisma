#include <stdio.h>
#include <sys/types.h>  
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#ifdef MDIGITAL
#include <sys/uswitch.h>
int VariableEntera = 0;
#endif /* MDIGITAL */
#ifdef DUAL
#include <sys/socket.h>
#include <sys/utsname.h>
#include "/i_addr.h"
#endif /* DUAL */

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


void  rpad(char *bufer,char c,int lon);

#define movmem(src, dst, count) memmove ((dst), (src), (count))
extern int errno;
int tam_fich();
int leer_pars(char *keypars,NPSPARS *reg_parsub,int *numreg);
int carga_svm(SVM *p1svm,NPSPARS *reg,NPSCONF *pconfi,int numreg,
              char tproce[NUM_PROCES][11]);
int busca_conf(NPSCONF *pconf,char *reg,int pos);
int calcula_esp(void); 
int lee_conf(NPSCONF *st_conf,char *keypars); 
int monta_bines(int *shmidbine);
int carga_bines(tbbine *pun_svmbine); 
int monta_socks(int *shmidsock);
int carga_socks(tbsock *pun_svmsock); 
int crea_proc(SVM *p1svm,NPSCONF *pconf);
int trata_colas(char cola,int tamano,int *idcola);
char carga_colas(char *colas,int *tamano);
int inst_proc(char *dsejecut,char *tproce);
int lanzar_proc(char proce[NUM_PROCES][11], SVM *psvm);
int haz_demonio(char * proceso, int numero, char *fecha);
int todo_bien(SVM *p1svm,char proce[NUM_PROCES][11],int mialarm);
int despierta(SVM *p1svm,char proce[NUM_PROCES][11]);
void sigue();
int espera_respuesta(SVM *p1svm,char proce[NUM_PROCES][11]);
void borrar_svm( int shmid);
void salir(char *para,int tipo,int error,int shmid,SVM *psvm, char * nombresvm);
int encuentra_svm(SVM *psvm_par,char *aux,int tipo);
int desconec_svm(void *pun_svm);
void carga_subs(SVM *Psvm,NPSPARS *reg);
int Pkill(int Ppid,int Psenal);
int ConecSVMaux(void **gen_psvm,int shmid);
int crear_svmsock(void **gen_psvm,int *shmid,int operacion, char *nomsock);
int crear_svmbine(tbbine **gen_psvm,int *shmid,int operacion, char *nombine);

void fechorsys(struct tm_dec *tmPtr);
int alarma=0;
SVM        *psvm;

//RAQUEL añadido para arrancar la configuracion por bbdd
char nomProg[50];
NPSIPCS  *reg_ipcs;
NPSCONF  *reg_conf;
NPSPARA  *reg_para;

char nombre[5];
char super[8];
int main(int argc,char *argv[])
{
  char       tproce[NUM_PROCES][11];
  char       keypars[5]; //indica que se va a arrancar
  char       claveCONE[13]; //indica que se va a arrancar
  char       fechanum[20];
  char       fechaAct[20];
  char       fechaB[06];
  int        anyo;
  char       comando[200];
  char       literal[400];
  int        retorno;
  int        err,err1,shmid;
  int        shmidbine;
  int        num_reg;
  NPSPARS    *reg_pars;

  int        shmidsock;
  int        num_conf;
  NPSCONF     *pconfi;
  
  int result=0;
  int clave_para[2];  
  char tipo_SVM[6];
  
  //RAQUEL añadido para arranque por bbdd
   char indice_ipcs[2]; //siempre sera 00 para cargar toda la tabla NPTIPCS
   int i; //para recorrer la tabla IPCS y buscar
   char indice_conf[2]; //siempre sera 00 para cargar toda la tabla NPTCONF 
   int ll; //para recorrer la tabla CONF y buscar
 

#ifdef DUAL
  char nombre_host[MAXHOSTNAMELEN];
  struct hostent * host_red;
  struct utsname stname;
  struct sockaddr_in ser_addr;
  int sfd, retorno;
  struct linger opcion;
#endif /* DUAL */

// incluido para recuperar la variable de entorno y crear las trazas en su entorno correspondiente
// segun el usuario que arranque

  char* HOME;
  HOME = getenv ("SF_HOME");

  strncpy(nomProg, argv[0], strlen(argv[0]));


//conexion ficheros
  shmid = 0;
  
   //RAQUEL añadimos la carga de NPTIPCS para unificar el arranque por bbdd
   //para cargar la tabla entera usamos indice 00 para poder usar la funcion CARGAR y no añadir 
   //mas procedimientos sino reutilizar los que ya existen modificados   
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
  
  //calculo de numero de subsistemas
  num_reg = tam_fich()/sizeof(NPSPARS);
  reg_pars = (NPSPARS *) malloc((num_reg+1) * sizeof(NPSPARS));
  if(reg_pars == (NPSPARS *) NULL)
   exit(1);

   //según el parametro de "arranque", vemos que tenemos que arrancar.
   //si no tiene parametros, se arranca TODO
  memset(keypars,0,sizeof(keypars));
  if (argc < 2)
   strcpy(keypars,TODO); /* Para arrancar todos los subsistemas */
  else
   {
    num_reg = 1;
    switch (strlen(argv[1])) //lo que ocupa, ya que se arranca por numero del subsistema 
                             //y hay que completarlo pq e sun char
     {
      case 1: /* Para arrancar el subsistema especificado */
           strcpy(keypars,"0");
           strcat(keypars,argv[1]);
           break;
      case 2:
      case 3:
      case 4:
           strcpy(keypars,argv[1]);
           break;
      default:
           salir(keypars,1,9,shmid,psvm,nombre);
     }
   }
  rpad(keypars,'0',4); //introducir ceros por la derecha para formar el dato keypars

  fprintf(stderr,"Voy a leer %s con num_reg %d\n",keypars,num_reg);
//lee el subistemas si solo es uno, o los subsistema si es TODOS. (NUCLEO = 0000 y TODOS = ????)
//y lo meto en reg_pars
  err=leer_pars(keypars,reg_pars,&num_reg);
  if (err)
  {
   if (err==6)
     exit(2);
   else
     exit(3);
  }

  fprintf(stderr,"intento conectarme a SVM\n");

 //RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
    err=conecSVM((void *) &psvm); //creamos el IPC
  switch (err)
  {
   case E_NOSVM: //no hay espacio
     fprintf(stderr,"Estoy en E_NOSVM\n");
     if (!strcmp(keypars,TODO) || !strcmp(keypars,NUCLEO))
     {
      fprintf(stderr,"creando SVM \n");
	  

	  //RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
         err=crearSVM((void *) &psvm,&shmid,0,nombre); //ORIGINAL
      
      if (err)
       exit(4);
      fprintf(stderr,"SVM creada \n");
     }
     else
     {
      fprintf(stderr,"Arrancando Kernel\n");
      sprintf(comando,"%s%s/%s %s", HOME,DIREXECS,argv[0],NUCLEO);

      err1=system(comando); 
      if (err1)
       exit(err1>>8);
      //RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
	    err1=conecSVM((void *)&psvm);
      if (err1)
       exit(4);
     }
     break;
   case 0: /* Ver lo que hay arrancado y lo que queda en caso de TODO */
     if (!strcmp(keypars,TODO) || !strcmp(keypars,NUCLEO))
     {
      fprintf(stderr,"SISTEMA YA ARRANCADO %s\n",keypars);
      exit(5);
     }
     else
     {
      err=encuentra_svm(psvm,keypars,TIP_SUBSIS); //busca si hay subsistema
      fprintf(stderr,"Posicion de %s---> %d\n",keypars,err);
      if (err >= 0)
      {
       fprintf(stderr,"SUBSISTEMA YA ARRANCADO %s\n",keypars);
       fprintf(stderr,"Posicion                %d\n",err);
       exit(6);
      }
     }
     break;
   default:
     fprintf(stderr,"ERROR AL CONSEGUIR LA SVM %d\n",err);
     exit(4);
  }

/***   para guardar las trazas   ***/
#ifndef TRAZAXXXX

  tm_dec tm;
  fechorsys(&tm);
  
   /***   coger fecha del sistema   ***/

 sprintf(fechanum, "%.*d%.*d%.*d%.*d%.*d%",2,tm.tm_year,2,tm.tm_mon,2, tm.tm_mday,2, tm.tm_hour,2, tm.tm_min);
 sprintf(fechaAct, "_%02d%02d%d_%02d",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour);
 anyo=tm.tm_year % 100;
 sprintf(fechaB, "%02d%02d%02d",tm.tm_mon,tm.tm_mday,anyo);

  sprintf(comando,"%s%s/tr%s",HOME,DIRDATRZ,fechanum);
  errno=0;
  retorno=mkdir(comando,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
  #if defined (TRAZA) && (TRAZA > 1)
    if (retorno)
      {
       sprintf(literal,"Error (%d) al crear %s:",errno,comando);
       perror(literal);
      }
  #endif /* TRAZA */
  
  if(!strcmp(keypars,TODO) || !strcmp(keypars,NUCLEO)) 
    {  
      sprintf(comando,"mv %s%s* %s%str%s",HOME,DIRTRAZA,HOME,DIRDATRZ,fechanum);
  #if defined (TRAZA) && (TRAZA > 1)
      fprintf(stderr,"Ejecuto %s\n",comando);
  #endif /* TRAZA */
   //  printf(" RAQUEL COMANDO LINEA 246 %s \n", comando);
      system(comando);
    }
   else
    {
	  sprintf(comando,"mv %s%s*%.*s* %s%str%s",
                      HOME,DIRTRAZA,2,keypars,HOME,DIRDATRZ,fechanum);
  #if defined (TRAZA) && (TRAZA > 1)
      fprintf(stderr,"Ejecuto %s\n",comando);
  #endif /* TRAZA */
      system(comando);
     }

   //borro los acces que se han creado
     sprintf(comando,"for i in `ls %s%s/access* | grep -v %s%s`; do rm -fr $i; done",HOME,DIREXECS, fechaB);
     fprintf(stderr,"Ejecuto %s\n",comando);
     system(comando);

#endif
/***********************************/
  
   //RAQUEL añadimos la carga de NPTCONF para unificar el arranque por bbdd
   //para cargar la tabla entera usamos indice 00 para poder usar la funcion CARGAR y no añadir 
   //mas procedimientos sino reutilizar los que ya existen modificados   
   
     strncpy(indice_conf, "00",2);
     indice_conf[2]='\0';
     
     if ((err = carga_conf(indice_conf)) != 0)
     {
      #if defined (TRAZA) && (TRAZA > 1)   
         fprintf(stderr,"error carga tabla NPTCONF \n");
      #endif	   
     }
  //se carga todo y se crea lo que corresponda
  err=lee_conf(reg_conf,keypars); //relleno la variable pcofi con la par_conf definida en arranque.h
  if (err)
  {
   fprintf(stderr,"ERROR(%d): No se puede leer la configuracion "
    "del sistema.\n",err);
       //RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
	     salir(keypars,0,7,shmid,psvm,nombre);
  }

  
  err=carga_svm(psvm,reg_pars,reg_conf,num_reg,tproce);

  if (err)
   {
    if (err==NO_EXIS_SVM)
     {
     
      fprintf(stderr,"ERROR(%d): No mas entradas en la S.V.M.\n",err);
	  //RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
         salir(keypars,1,8,shmid,psvm,nombre);
      
     }
    else
     if (err==NO_EXIS_CONF)
      {
       fprintf(stderr,
                  "ERROR(%d): No existe configuracion para el subsistema\n",
                  err);       
	   //RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
         salir(keypars,1,7,shmid,psvm,nombre);
      }
     else
      {
       fprintf(stderr,"ERROR(%d): No se ha completado la carga\n",err);
	   //RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
          salir(keypars,1,9,shmid,psvm,nombre);
      }
   }
  else
    fprintf(stderr,"(%d): Todo se ha cargado \n",err);

  if(!strcmp(keypars,TODO) || !strcmp(keypars,NUCLEO))
  {

     err=monta_socks(&shmidsock);
     if (err)
     {
       fprintf(stderr,"Error en monta_socks..... %d\n",err);
       borrar_svm(shmidsock);
	   	 //RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
          salir(keypars,1,9,shmid,psvm,nombre);     
     }
     err=monta_bines(&shmidbine);
     if (err)
     {
       fprintf(stderr,"Error en monta_bines..... %d\n",err);
       borrar_svm(shmidsock);
       borrar_svm(shmidbine);
	   	//RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
          salir(keypars,1,9,shmid,psvm,nombre);  
     }
  }

  fprintf(stderr, "A lanzar_proc >%.11s<\n", tproce);
  err=lanzar_proc(tproce, psvm);
  strncpy(claveCONE, "0001", 4);
  if (!strcmp(keypars,TODO))
     strncpy(claveCONE+4, "TODOS", 5);
  else
     strncpy(claveCONE+4, keypars, 5);
  strncpy(claveCONE+9, "   ", 3);
  claveCONE[13]='\0';
  //desactivamos todos los modulos, si acaba bien, se activan
  err = tratar_fich("NPTCONE", DESACTIVAR, NULL, claveCONE);

  if (err)
    {
     fprintf(stderr,"En fin err= %d\n",err);
     if(!strcmp(keypars,TODO) || !strcmp(keypars,NUCLEO))
      {
       borrar_svm(shmidsock);
       borrar_svm(shmidbine);
      }
	  	//RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
          salir(keypars,2,12,shmid,psvm,nombre);
    }


#ifdef DUAL
/* antes de continuar comprobamos que no este la aplicacion */
/* arrancada en el otro servidor        */

  err = uname(&stname);
  if(err<0)
  {
   fprintf(stderr,"Error(%d):Imposible conseguir el nombre del servidor\n", errno);
   borrar_svm(shmidsock);
   borrar_svm(shmidbine);
   	//RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
       salir(keypars,2,12,shmid,pun_svm,nombre);
  }

  strcpy(nombre_host, stname.nodename);
  if(nombre_host[strlen(nombre_host)-1] == '1')
   nombre_host[strlen(nombre_host)-1] = '2';
  else
   nombre_host[strlen(nombre_host)-1] = '1';
  if ((host_red = gethostbyname(nombre_host)) == 0)
  {
/***   TRAZAPING   ***/
#ifdef TRAZA
   fprintf(stderr,"GETHOST= %d\n",errno);
   fprintf(stderr,"family= %d\n",host_red->h_addrtype);
   fprintf(stderr,"sin_port= %d\n",PORTNUM);
   fprintf(stderr,"sin_addr= %d\n",host_red->h_length);
#endif
   fprintf(stderr,"Imposible conseguir direccion del servidor %s\n", nombre_host);
   borrar_svm(shmidsock);
   borrar_svm(shmidbine);
   //RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
     salir(keypars,2,12,shmid,pun_svm,nombre);
  }
#ifdef TRAZA
  fprintf(stderr,"family= %d\n",host_red->h_addrtype);
      fprintf(stderr,"sin_port= %d\n",PORTNUM);
      fprintf(stderr,"sin_addr= %d\n",host_red->h_length);
#endif
   bzero(&ser_addr, sizeof(ser_addr));
   ser_addr.sin_family=host_red->h_addrtype;

   ser_addr.sin_port=PORTNUM;

   if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
    fprintf(stderr,"SOCKET = %d\n",errno);
    borrar_svm(shmidsock);
    borrar_svm(shmidbine);
	//RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
    salir(keypars,2,12,shmid,pun_svm,nombre);
   }

   opcion.l_onoff=1;
   opcion.l_linger=0;

   err=setsockopt(sfd,SOL_SOCKET,SO_LINGER,(char *) &opcion,sizeof(opcion));
   fprintf(stderr,"err %d\n", err);

   retorno= connect(sfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));

   err=close(sfd);
  
   if (!retorno) 
   {
    if(errno == ECONNREFUSED)
     fprintf(stderr,"Aplicacion duplicada en servidor %s\n",nombre_host);
    else
     fprintf(stderr,"Error(%d) comprobando duplicidad en servidor %s\n",
      errno, nombre_host);
    borrar_svm(shmidsock);
    borrar_svm(shmidbine);
   //RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
    salir(keypars,2,12,shmid,psvm,nombre);
   }
#endif /* ifdef DUAL */


   err=espera_respuesta(psvm,tproce);

   if (err)
      {
        fprintf(stderr,"En espera dentro del if err= %d\n",err);
        /* El SIGALRM no se ha disparado por mi despertador */
        fprintf(stderr,
           "No es mi despertador,alguien me ha levantado antes de tiempo\n");
        err=todo_bien(psvm,tproce,0);
        if (err)
           {
             fprintf(stderr,"En todo_bien err= %d\n",err);
             if(!strcmp(keypars,TODO) || !strcmp(keypars,NUCLEO))
              {
               borrar_svm(shmidsock);
               borrar_svm(shmidbine);
              }
			     //RAQUEL UNIFICAMOS PARA ARRANQUE POR BBDD 
                 salir(keypars,2,13,shmid,psvm,nombre);
           }
      }

     //BIMSF-3087  JFL
     fechorsys(&tm);
     printf("Esperamos a los hijos. %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
     signal(SIGALRM,sigue);
     alarm(10);
     pause();
     fechorsys(&tm);
     printf("Fin de espera        . %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
     //BIMSF-3087  JFL

   fprintf(stderr,"Voy a despierta err= %d\n",err);
   err=despierta(psvm,tproce);

   if (err)
      {
        fprintf(stderr,"Vuelvo de despierta err= %d\n",err);
        if(!strcmp(keypars,TODO) || !strcmp(keypars,NUCLEO))
         {
          err=espera_respuesta(psvm,tproce);
          borrar_svm(shmidsock);
          borrar_svm(shmidbine);
         }
        salir(keypars,2,14,shmid,psvm,nombre);
      }

   //si llega aqui es que se ha levantado correctamente, modulos activos en nptcone.h
   err = tratar_fich("NPTCONE", ACTIVAR, NULL, claveCONE);
/* ARRANCAMOS EL SUPERVISOR */
//  

	  if (strncmp(nombre,"SVMP",4)==0)
	  {
		  strncpy(super,"superviI", 8);
          super[8]='\0';
	  }
	  else 
	  {   if (strncmp(nombre,"SVAP",4)==0)
	     {
		  strncpy(super,"superviA", 8);
          super[8]='\0';
	     }
		 else 
	     {
		   strncpy(super,"superviW", 8);
           super[8]='\0';
		 }  
	  }
	  
       if(!strcmp(keypars,TODO) || !strcmp(keypars,NUCLEO))
        {
          strcpy(psvm->pasubsis,"0000");
       //   strcpy(psvm->str_svm.svmproc.proceso,"supervis");
	      strcpy(psvm->str_svm.svmproc.proceso,super);
          psvm->svmtip=TIP_SUPER;
          psvm->str_svm.svmproc.idcolaIn= -1;
          psvm->str_svm.svmproc.idcolaOut= -1;
          psvm->str_svm.svmsupe.pid= -1;
          psvm->estado='0';
          AcSvmBine='1';
          AcSvmSock='0'; 
  #ifndef SRV_TIENDA
          AcSvmBine='0';
  #endif

	  err=haz_demonio(super, 1, fechaAct);
      fprintf(stderr,"Vuelvo de lanzar supervis\n");
        }
  
   /* Me desconecto de la SVM */
     err = desconec_svm(psvm);
    
     exit(0);

}

int carga_svm(SVM *p1svm,NPSPARS *reg,NPSCONF *pconf,int numreg,
                char tproce[NUM_PROCES][11])
{
 char aux[13];
 int  num=0,est_svm,est_conf,pos=0,err,proc=0,n;
 int cont,c;
 

 while (num < numreg) //para todos los susbsistemas
 {
   strncpy(aux,reg->pasubsis,4);
   aux[4]='\0';
   est_svm=encuentra_svm(p1svm,aux,TIP_SUBSIS);  //Busco donde se encuentra el svm en PSVM
   if (est_svm == NO_EXIS_SVM)
        return(est_svm);

   if (est_svm < 0) est_svm *=-1;
   est_conf=busca_conf(pconf,aux,pos); //busco la configuracion para este sistema

   if ((est_conf==NO_EXIS_CONF) && (pos == 0))
   {
        fprintf(stderr,"NO EXISTE CONFIGURACION %s en CONF\n", aux);
        return(est_conf);
   }
   
   carga_subs(p1svm+est_svm,reg); //cargo los datos que le corresponden al subsistema

   est_svm++;

   while (est_conf !=NO_EXIS_CONF)
   {

      if ((est_svm+(pconf+est_conf)->confnmpr) > MAX_REG_SVM)
            return (NO_EXIS_SVM);

      err=crea_proc(p1svm+est_svm,pconf+est_conf); //crea las colas
      if (err) return (err);

      for (n=0; n<(pconf+est_conf)->confcapr; n++)
      { 
        printf("arranque.c carga_svm proceso %s\n", (pconf+est_conf)->confdsej); //BIMSF-3087  JFL
        fflush(stdout);
        proc+=inst_proc((pconf+est_conf)->confdsej,tproce[proc]);
         *tproce[proc] = 0;
      }
      
      est_svm=encuentra_svm(p1svm,"",NO_TIPO);

      if (est_svm == NO_EXIS_SVM)
           return(est_svm);

      pos=est_conf+1;

      est_conf=busca_conf(pconf,aux,pos);

    }

    reg++;
    num++;
    pos=0;
 }

  return (0);
}

//funcion, en funcion del campo numreg lee el subsistema en concreto o todos.
int leer_pars(char *keypars,NPSPARS *reg_parsub,int *numreg)
{
  int lei;
  int err;
  NPSPARS r_parsub;
  char claveP[5];

  fprintf(stderr,"numreg =%d\n",*numreg);

  if (*numreg == 1)
    {
     keypars[5]='\0';
     err = tratar_fich("NPTPARS", IGUAL, &r_parsub, keypars);
     memcpy(reg_parsub,&r_parsub,sizeof(NPSPARS));
     memset(reg_parsub+1, '\0', sizeof(NPSPARS)); 
 
     return(err);
    }
  else
    {
     err = tratar_fich("NPTPARS", PRIMERO, &r_parsub, NULL);
     lei=0;
     while (err ==0)
      {
        memcpy(reg_parsub+lei,&r_parsub,sizeof(NPSPARS)); 
        lei++;
        memset(reg_parsub+lei, '\0', sizeof(NPSPARS)); 
        strncpy(claveP, r_parsub.pasubsis , 2);
        strncpy(claveP+2, r_parsub.pasubapl, 2);
        claveP[4]='\0';
        err = tratar_fich("NPTPARS", SIGUIENTE, &r_parsub, claveP);
      }
    }

  return (0); 
}

int tam_fich()
{
 int long_fich=0;
 int err;
 NPSPARS r_parsub;
 char claveP[5];
 
 fprintf(stderr,"Inicio tam_fich \n");
 err = tratar_fich("NPTPARS", PRIMERO, &r_parsub, NULL);
 fprintf(stderr,"PRIMERO: long_fich=%d  err=%d\n",long_fich,err);
 while (err ==0)
   {
     long_fich++;
     strncpy(claveP, r_parsub.pasubsis	, 2);
     strncpy(claveP+2, r_parsub.pasubapl, 2);
     claveP[4]='\0';
     err = tratar_fich("NPTPARS", SIGUIENTE, &r_parsub, claveP);
   }
 fprintf(stderr,"Fin tam_fich \n");
 return (long_fich*sizeof(NPSPARS));
}

/*
   Funcion busca_conf:
    Devuelve: NO_EXIS_CONF=-32001 -> Si no encuentra el valor
              int >= 0 -> La primera posicion donde esta el valor buscado 
*/
int busca_conf(NPSCONF *pconf,char *aux,int pos)
{
  int num=pos;
  int ll;
  NPSCONF *auxconfig;
 
  auxconfig=pconf+num;
  

  while (auxconfig->confnmpr != 0)
  {
    if (strcmp(auxconfig->confcdsu,aux) == 0)
     {

          return (num); 
     }
     num++;
     auxconfig++;
  }
  return(NO_EXIS_CONF);

}

 int lee_conf(NPSCONF *st_conf,char *keypars)
 {
   int i=0,j=0;
 
     if (strcmp(keypars,TODO)==0)
 	//RAQUEL modificado para buscar los procesos por bbdd y entidad

 	 while (tabla_conf[i].confnmpr != 0) //recorro los procesos
       {
        movmem(&tabla_conf[i],st_conf+i,sizeof(NPSCONF));  //mueve un bloque de tamaño sizeof (NPSCONF) desde el 1 al 2
        i++;
        j++;
       }
     else

      while (tabla_conf[i].confnmpr != 0) //recorro los procesos
       {	  
        if (strncmp(keypars,tabla_conf[i].confcdsu,2)==0) //solo muevo el que que coincida con keypars
            {
             movmem(&tabla_conf[i],st_conf+j,sizeof(NPSCONF));
             j++;
            }
        i++;
       }
 
     movmem(&tabla_conf[i],st_conf+j,sizeof(NPSCONF));
 return(0);
 
 }

int crea_proc(SVM *p1svm,NPSCONF *pconfig)
{
 int num=0,err=0,pos=0;
 int qid, qidO;
 int tamano;
 char clave;
 
 if (strcmp(pconfig->confcola,"    ") !=0)
 {
	 clave = carga_colas(pconfig->confcola,&tamano);

     if (clave == ' ')
      {
       fprintf(stderr,"Error carga_colas\n");
       return (1);
      }
     err=trata_colas(clave,tamano,&qid); //crear colas

     if (err) return (err);
 }
 else
 {
     qid=-1;
	 
 }
 
 if (strcmp(pconfig->confcsal,"    ") !=0)
 //if(pconfig->confcsal != NULL)
 {
	 clave = carga_colas(pconfig->confcsal,&tamano);
     if (clave == ' ')
      {
       fprintf(stderr,"Error carga_colas\n");
       return (1);
      }

     err=trata_colas(clave,tamano,&qidO); //crear colas

     if (err) return (err);
 }
 else
 {
     qidO=-1;
 }


 while (pconfig->confnmpr > num)
  {
   pos=encuentra_svm(p1svm,"",NO_TIPO);
   if (pos == NO_EXIS_SVM) return(pos);

   strcpy((p1svm+pos)->pasubsis,pconfig->confcdsu);
   printf("arranque.c crea_proc Cargando >%s< en >%p<\n", pconfig->confdsej, p1svm+pos); //BIMSF-3087  JFL
   strcpy((p1svm+pos)->str_svm.svmproc.proceso,pconfig->confdsej);
   (p1svm+pos)->str_svm.svmproc.idcolaIn=qid;
   (p1svm+pos)->str_svm.svmproc.idcolaOut=qidO;
   (p1svm+pos)->str_svm.svmproc.pid= -1;
   (p1svm+pos)->svmtip=TIP_PROCES;
   (p1svm+pos)->estado='0';
   (p1svm+pos)->tippar=pconfig->conftipp;
   (p1svm+pos)->senal=0;
   num++;
  }
return (0);
}

int inst_proc(char *dsejecut,char *tproce)
{
/* este if esta puesto para el caso de ctm */

  if (strcmp(dsejecut,"pantalla") == 0) return(0);
  if (strncmp(dsejecut,"ptst",4) == 0) return(0);

  if ((strncmp(dsejecut,"pm",2) == 0) &&
      (strncmp(dsejecut+6,"re",2) == 0)) return(0);
  if (strcmp(dsejecut,"") == 0) return(0);
  if (dsejecut == NULL) return(0);


  strcpy(tproce,dsejecut);
  return(1);
}

int espera_respuesta(SVM *p1svm,char proce[NUM_PROCES][11])
{
 int queda,yaesta;
 
   fprintf(stderr,"espera_respuesta\n");
   signal(SIGALRM,sigue);
   alarm(TIMEWAIT);
   alarma=0;
   while ((yaesta=todo_bien(p1svm,proce,alarma)) !=0)
   {
/*
    fprintf(stderr,"Dentro del While yaesta->%d  alarma->%d\n", yaesta, alarma);
*/
   }
   queda=alarm(0);
   if(yaesta)
    return(queda);

   return (0);
}

void sigue() {

fprintf(stderr,"===========sigue===========\n"); 
   alarma=1;
}

int todo_bien(SVM *p1svm,char proce[NUM_PROCES][11],int mialarm)
{
 int donde,ii;

  ii=0;
  donde = 0;

   while ((proce[ii][1]==0) && (mialarm == 0))
   {

    if(ii>0 && !strcmp(proce[ii],proce[ii-1]))
       donde ++;
    else
       donde = encuentra_svm(p1svm,proce[ii],TIP_PROCES);

    if (donde < 0) 
          return(-1);


    if ((p1svm+donde)->str_svm.svmproc.pid == -1)
          return(-1);
    ii++;
   }
 return(0);
}

int despierta(SVM *p1svm,char proce[NUM_PROCES][11])
{
 int donde,ii;

  ii=0;
  donde = 0;

     
   while (proce[ii][0]!= 0)
   {
/*
     fprintf(stderr,"busco %s ",proce[ii]);
*/
     if (ii>0 && !strcmp(proce[ii-1],proce[ii]))
     {
         donde ++;  
     }
     else
     {
		 printf("busco encuentra_svm proceso %s \n",proce[ii]); 
         printf("busco encuentra_svm TIP_PROCES %i \n",TIP_PROCES);
         donde = encuentra_svm(p1svm,proce[ii],TIP_PROCES);
     }
/*
    fprintf(stderr,"donde %d con pid %d\n",donde,
                                   (p1svm+donde)->str_svm.svmproc.pid);
*/
		 
    if (donde < 0) 
      {
#if defined (TRAZA) && (TRAZA > 1)
        fprintf(stderr,"No encuentro %s\n",proce[ii]);
#endif
          return(-1);
      }
    if ((p1svm+donde)->str_svm.svmproc.pid == -1) 
      {
#if defined (TRAZA) && (TRAZA > 1)
        fprintf(stderr,"El proceso %s tiene pid -1\n",proce[ii]);
#endif
          return(-1);
      }
    else
      {
#if defined (TRAZA) && (TRAZA > 1)
        fprintf(stderr,"Despierto a %s\n",proce[ii]);
#endif
        Pkill((p1svm+donde)->str_svm.svmproc.pid,SIGALRM);
      }
/*
*/
    ii++;
   }
 return(0);
}

int monta_socks(int *shmidsock)
{
int err;
int err1;
int i;
tbsock     *pun_svmsock;

char nomsock[5];

 
  for (i=0 ; i<50 ; i++)
   {
	  if ((strncmp(tabla_ipcs[i].ipcsnsvm,"SVMSOCK",7)==0))
      {		
       strncpy(nomsock, tabla_ipcs[i].ipcsnomb,5);  
       break;            
      }   
   }

   if (IdSvmSock >= 0)
      err=ConecSVMaux((void *)&pun_svmsock,IdSvmSock);
   else
       err=crear_svmsock((void **)&pun_svmsock,shmidsock,0,nomsock);
   if (err)
     {
      fprintf(stderr,"Error en conseguir_svmsock..... %d\n",err);
      return(4);
     }
  err=carga_socks(pun_svmsock);
  if (err)
     err1 = desconec_svm(pun_svmsock);
  else
     IdSvmSock=*shmidsock;
 
 return(err);  
}


int monta_bines(int *shmidbine)
{
int err;
int err1;
tbbine     *pun_svmbine;

int i;
char nombine[5];

  for (i=0 ; i<50 ; i++)
   {
	  if ((strncmp(tabla_ipcs[i].ipcsnsvm,"SVMBINE",7)==0))
      {		
       strncpy(nombine, tabla_ipcs[i].ipcsnomb,5);  
       break;            
      }   
   }

   fprintf(stderr,"_svmbine..... %d\n",IdSvmBine);
   if (IdSvmBine >= 0)
     err=ConecSVMaux((void *)&pun_svmbine,IdSvmBine);
   else
     err=crear_svmbine(&pun_svmbine,shmidbine,0,nombine);
   if (err)
     {
      fprintf(stderr,"Error en conseguir_svmbine..... %d\n",err);
      return(4);
     }
  err=carga_bines(pun_svmbine);
  if (err)
     err1 = desconec_svm(pun_svmbine);
  else
     IdSvmBine=*shmidbine;
  return(err);
}