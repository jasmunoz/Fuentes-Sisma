//PRJ0247167 - Revisado ampliación P48
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>
#include "max_min.h"

extern int errno;
//RAQUEL COMPROBAR ESTE MAX_TXT PARA LA MODIFICACION P48
#define MAX_TXT 4000

void fprintLog(char ficheroPlantilla[50], char* format, ...);

extern char nomProg[50];

/* 
LeerCola
parametros:int cola       ---> Cola de la que leer
           int timeout    ---> Tiempo de espera de respuesta
           char *Datosmsg ---> Puntero a char donde recibir los datos
           int Lonmsg     ---> Longitud a recibir
           int Tipomsg    ---> Tipo del mensaje a recibir
Devuelve el numero de caracteres leidos o -1 si hay error 
*/

int LeerCola(int cola,int timeout,char *Datosmsg,int Lonmsg,int Tipomsg)
{
  int retorno;
  int err;
  int leer;
  int leidos;
  typedef struct
  {
   long  tipo;
   char  datos[MAX_TXT]; 
  } mensaje;

  mensaje    msg;
  
  //PRJ0247167 - Revisado ampliación P48 se revisa variable Datosmsg que es puntero y no necesita redimensionar
  // se revisa variable MAX_TXT definida ya con el tamaño total del mensaje 4096 no
  //necesario modificar - se tiene que poner a 4000 porque no cabe mas en variable se Oracle
  
  leer=min(Lonmsg,MAX_TXT);
  fprintf(stderr,"longitud leer %i\n", leer);
  memset(Datosmsg,' ',Lonmsg);
  if (timeout  > 0)
     alarm(timeout); 
  errno=0;
  //PRJ0247167 - Revisado ampliación P48 se comprueba que msgrcv no tiene problema con el nuevo tamaño de mensaje 4000
  retorno=msgrcv(cola, &msg, leer, Tipomsg, 0);
  err=errno;
  fprintf(stderr,"errno %d  cola %d \n", errno,cola);
  alarm(0);
  if (retorno == -1)
  {
#if defined (TRAZA) && (TRAZA > 1)
   perror("msgrcv:");
#endif

  //Añadido para Monitorizacion SISMA 
  fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Lectura >%i< codigo de error>%.3s<\n",cola, err);

  }
  else
  {
    leidos=min(Lonmsg,retorno);
    memcpy(Datosmsg,msg.datos,leidos);
  }
  return(retorno);
} 

/* 
EnviaCola
parametros:int cola       ---> Socket al que enviar
           int timeout    ---> Tiempo de espera de envio    
           char *Datosmsg ---> Puntero a char donde estan los datos
           int Lonmsg     ---> Longitud a enviar
           int Tipomsg    ---> Tipo del mensaje a enviar
Devuelve 0 si ha enviado o -1 si hay error 
*/
int  EnviaCola(int cola,int timeout,char *Datosmsg,int Lenmsg,int Tipomsg)
{
  int retorno;
  int err;
  typedef struct
  {
   long  tipo;
   char  datos[MAX_TXT];
  } mensaje;

  mensaje    msg;

  msg.tipo=Tipomsg;
  
  errno=E2BIG;
  //PRJ0247167 - Revisado ampliación P48 se revisa variable Datosmsg que es puntero y no necesita redimensionar
  // se revisa variable MAX_TXT definida ya con el tamaño total del mensaje 4096 no
  //necesario modificar - se pasa a 4000
 
  if (Lenmsg > MAX_TXT)
     return(-1);
    
  memcpy(msg.datos,Datosmsg ,Lenmsg);
  
 // printf("Raquel dentro del comun_q msg.datos %s\n",msg.datos);
  
  if (timeout  > 0)
     alarm(timeout);  
  /*   envia a cola  */
  errno=0;
  
  //PRJ0247167 - Revisado ampliación P48 se comprueba que msgsnd no tiene problema con el nuevo tamaño de mensaje 4000
  retorno=msgsnd(cola, &msg, Lenmsg , 0);
  err=errno;

  alarm(0);

  if (retorno == -1)
  {
   //Añadido para Monitorizacion SISMA 
   fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Envio >%i< codigo de error>%.3s<\n",cola, err);
#if defined (TRAZA) && (TRAZA > 1)
   fprintf (stderr,"error(%d) envio a cola %d\n",err,cola); 
   perror("msgsend:");
  }
#endif
  return(retorno);        
}