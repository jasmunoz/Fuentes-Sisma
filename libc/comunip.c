//PRJ0247167 - Revisado ampliación P48
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <time.h>  
#include <errno.h>
#include <signal.h>
#include <sys/ipc.h>  
#include <sys/shm.h>
#include <sys/msg.h>
#include "nptpara.h"

#define USURBUFSIZE 20 

void fprintLog(char ficheroPlantilla[50], char* format, ...); 
int chartoi(char *str, int len);
int Hex2Int(char *szHexString);
void unpack(void *psource, void *ptarget, unsigned int length);
char toUpper(char myChar);
int recvFromRedsys(int fd, char *buffer, int to);

NPSPARA * reg_para;
extern int errno;
/* 
LeerSock
parametros:int sfd     ---> Socket del que leer
           int timeout ---> Tiempo de espera de respuesta, 
                            si es negativo espera eternamente
           char msg    ---> Puntero a char donde recibir los datos
           int len     ---> Longitud a recibir
Devuelve el numero de caracteres leidos o -1 si hay error 
*/

int  LeerSock(int sfd,int timeout,char *msg,int len)
{
  int retorno;
  int err;

  if (timeout > 0)
     alarm(timeout);
  errno=0;
  memset(msg, '\0',len);
  
  //PRJ0247167 - Revisado ampliación P48 no hay problema con la funcion recv para tamaño 4000
  retorno=recv(sfd,msg,len,0);
  err=errno;
  alarm(0);

//#if defined (TRAZA) && (TRAZA > 1)
  if (retorno == -1)
    {
#if defined (TRAZA) && (TRAZA > 1) 			
     fprintf (stderr,"error en recv linea : %d\n",err);
     perror("LeerSock");
#endif	 
    }
//#endif

  return(retorno);
}

/* 
EnviaSock
parametros:int sfd     ---> Socket al que enviar
           int timeout ---> Tiempo de espera de respuesta
           char msg    ---> Puntero a char donde estan los datos
           int len     ---> Longitud a enviar
Devuelve el numero de caracteres enviados o -1 si hay error 
*/
int  EnviaSock(int sfd,int timeout,char *msg,int len)
{
  int retorno=0;
  int err=0;
  

  if (timeout > 0)
     alarm(timeout);
  errno=0;
  //PRJ0247167 - Revisado ampliación P48 no hay problema con la funcion send para tamaño 4000
  retorno=send(sfd, msg, len,0);

  err=errno;
  alarm(0);
#if defined (TRAZA) && (TRAZA > 10) 		  
  fprintf(stdout,"hacemos el send con error>%i< bytes enviados>%d<\n", err, retorno);
#endif  

  if (retorno == -1)
  {
#if defined (TRAZA) && (TRAZA > 1)  
   fprintf(stderr,"error send linea : %d\n",err);
   perror("EnviaSock");
#endif   
  }

  return(retorno);        
}

/*****************************************************************************/
/*                                                                           */
/* Convierte la longitud del mensaje de entero a hex.      */
/*                                                                           */
/*****************************************************************************/

void int2hex(char *output, unsigned int n)
{
    static const char hex_digits[] = "0123456789abcdef";

    output[0] = hex_digits[(n >> 12) & 0xf];
    output[1] = hex_digits[(n >> 8) & 0xf];
    output[2] = hex_digits[(n >> 4) & 0xf];
    output[3] = hex_digits[n & 0xf];
    output[4] = '\0';
   
#if defined (TRAZA) && (TRAZA > 10)     
    fprintf(stdout,"Valor en hexadecimal de la len: %04s\n", output); 
#endif	

}

/*****************************************************************************/
/*                                                                           */
/* Convierte el array en hex a binario.                    */
/*                                                                           */
/*****************************************************************************/
void Hex2Bin( char *szHexString, char *pBinary)
{
 	int size = strlen( szHexString ) / 2, i;
 	char *p = &szHexString[0];
 
 	for( i = 0; i < size; i++, p += 2 ){
         sscanf( p, "%2hhx", &pBinary[i] );
     }
#if defined (TRAZA) && (TRAZA > 10)  
  fprintf(stdout,"Valor en binario de la len: %b\n",pBinary); 
#endif  
       
     
}

/*****************************************************************************/
/*                                                                           */
/* Envia los datos de usuario antes de enviar datos aplic. */
/*                                                                           */
/* Es necesario enviar a Redsys los datos de usuario para identificar        */
/* el socket.                                                                */
/* Es necesario dar de alta un campo de 18 en la tabla NPTPARS               */
/* Fijamos los datos fijos para la prueba 18021491000000000100               */
/*                                        LLAACCCC00000000NNT0               */               
/* LL Dos octetos en los cuales se indique en binario la                     */
/* longitud del registro lógico. Tendrá el valor 18.                         */
/* AA Número Identificativo de la Aplicación a enlazar.                      */
/* CCCC Código C.S.B.                                                        */
/* NN Número de Enlace. Su valor será fijado en el momento de comenzar       */
/* la certificación y será un número secuencial para las sesiones asociadas  */
/* a una misma aplicación.                                                   */
/* T Atributo de enlace. Su valor indicará el sentido del tráfico.           */ 
/* El valor será:                                                            */
/* ‘1': Tráfico de salida de la ENTIDAD y entrada a SERMEPA                  */
/* ‘2’. Tráfico de entrada de la ENTIDAD y salida de SERMEPA                 */
/* ‘0’. Tráfico Mixto (entrada de la ENTIDAD y salida de SERMEPA             */ 
/* por el mismo socket).                                                     */  
/*                                                                           */
/*****************************************************************************/

//int sendUserData(int sfd, char *msg, char *aplic, char *sesion, char *envRec) //ORIGINAL
int sendUserData(int sfd, char *msg, char*sistema, char *aplic, char *sesion, char *envRec) //CON SUBSISTEMA
{
    char send_buffer[USURBUFSIZE];
    char ch_05[5];
    char ch_03[3];
    char ch_02[2];
    int return_write;
	int ll;
    
    memset(send_buffer,0X00,USURBUFSIZE);
    memset(ch_05,0X00,5); 
    memset(ch_03,0X00,3);
 
    int2hex(ch_05,USURBUFSIZE-2);
    Hex2Bin(ch_05,ch_03); 
        
    memcpy(send_buffer,ch_03,2);     // longitud mensaje 
    //memcpy(send_buffer+2,aplic,2);   // aplicacion mensaje   ********REVISAR PORQUE APLI *******
    memcpy(send_buffer+2,"02",2);   // aplicacion mensaje

     //RAQUEL UNIFICANDO ENTIDADES

    if ((strncmp(reg_para[0].psnument,sistema,2)==0))
    {	
        memcpy(send_buffer+4,reg_para[0].pscsb000, 4);
        memcpy(send_buffer+8, "00000000", 8);   
    }  
	else 
	{
        memcpy(send_buffer+4,reg_para[1].pscsb000, 4);
		memcpy(send_buffer+8, "00000000", 8);
    }		
 // }     
	memcpy(send_buffer+16, sesion, 2);  // sesion (un secuencial por socket) 01 o 02
	memcpy(send_buffer+18, envRec, 1);  // enviar 1 recibir 2
	memcpy(send_buffer+19, "0", 1);  // enviar 1 recibir 2
    memcpy(msg,send_buffer,USURBUFSIZE);
    return_write = send (sfd, send_buffer, USURBUFSIZE,0);

#if defined (TRAZA) && (TRAZA > 1) 
    fprintf(stdout,"ENVIADO DATOS USUARIO return_write >%d<\n",return_write);
#endif   

    return(return_write);
}

/*****************************************************************************/
/*                                                                           */
/* 14/08/15 Alberto. Convierte el array en hex a binario.                    */
/*                                                                           */
/*****************************************************************************/
void getLenBin(char *pBinary, int len)
{
	
	char ch_05[5];
		
	int2hex(ch_05,len);
	Hex2Bin(ch_05,pBinary);	
	
}
/*****************************************************************************/
/*                                                                           */
/* 14/08/15 Alberto. Obtiene el valor de la longitud recibida en binario.    */
/*                                                                           */
/*****************************************************************************/
int getLenInt(char *pBinary, int len)
{
	int lng;
	int longitudMsg=0;
	char pdesemp[5];
	char arrayDec[6];
	int lenDec=0;
		
	unpack(pBinary,&pdesemp[0],len);
	
	pdesemp[4]= '\0';
	
	longitudMsg=Hex2Int(pdesemp);
	
#if defined (TRAZA) && (TRAZA > 10)  	
	fprintf(stdout,"Valor decimal de la longitudMsg: %d\n",longitudMsg); 
#endif	

	return(longitudMsg);
	
}
/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
char toUpper(char myChar) 
{

 if(myChar>91) myChar-=32;
 return myChar;

}
/*****************************************************************************/
/*                                                                           */
/* 11/08-15 Alberto. Convierte el array en hex a un entero.                  */
/*                                                                           */
/*****************************************************************************/
int Hex2Int(char *szHexString)
{

  int tempInt =0;
  int tempArray[4];
        
  
  for(int i=0; i<4; i++){
    if(szHexString[i]>'9') 
    {
      szHexString[i] = toUpper(szHexString[i]);
      // convierte letra a numero
      tempArray[i] = (szHexString[i]-55);
   
      // si es un caracter numero obtenemos el numero
    } else {
      // else we got a char of a number, make it a real number
      tempArray[i]=(szHexString[i]-48);
      
    }
  }

  // obtenemos la len
  tempInt = tempArray[3];
  tempInt += (tempArray[2]*16);
  tempInt += (tempArray[1]*16*16);
  tempInt += (tempArray[0]*16*16*16);

  return tempInt;
    
}
void unpack(void *psource, void *ptarget, unsigned int length)
{
   char *source, *target;
   unsigned int i, pos;
   unsigned char nibble;
   source = (char *)psource;
   target = (char *)ptarget;

   pos = 0;
   for(i=0; i<length; i++)
   {
      nibble = (unsigned char)((unsigned char) source[i] / 16);
      if (nibble<10)
         nibble = (unsigned char)(nibble + 0x30);
      else
         nibble = (unsigned char)(nibble + 'A' - 10);
      target[pos + 2*i] = nibble;

      nibble = (unsigned char)((unsigned char) source[i] % 16);
      if (nibble<10)
         nibble = (unsigned char)(nibble + 0x30);
      else
         nibble = (unsigned char)(nibble + 'A' - 10);
      target[pos + 2*i + 1] = nibble;
      
   }

}
/*****************************************************************************/
/*                                                                           */
/*   Params:                                                                 */
/*      fd     - (int) socket file descriptor                                */
/*      buffer - (char*) buffer to hold data                                 */
/*      len    - (int) maximum number of bytes to recv()                     */
/*      flags  - (int) flags (as the fourth param to recv() )                */
/*      to     - (int) timeout in milliseconds                               */
/*   Results:                                                                */
/*      int    - (int) resultado:                                            */
/*                  -1 = Error                                               */
/*                   0 = Timeout                                             */
/*                  >0 = OK                                                  */
/*   Notes:                                                                  */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
int recvFromRedsys(int fd, char *buffer, int to) {

   fd_set readset;
   int result, iof = -1;
   struct timeval tv;
   int bytesLeidos=0;
   int indx=0;
   int nBytesMsg=0;
   char ch_03[3];
   //char ch_50[50];//prueba
   time_t start_t, end_t;
   double diff_t;
   
   
   //PRJ0247167 - Revisado ampliación P48 el tamaño del mensaje ya era 4000 si cambia en futuros desarrollos
   //cambiar aqui en mensaet - estaba a 4096
   //Control de tiempos
   memset(buffer,0X00,4000);  // para que imprima correctamente.


  //1.-Obtenemos la longitud del mensaje a recibir

   memset(ch_03,0X00,3);
   //fprintf(stdout,"antes de recv\n");
    //DMND0037271 no hay problema con la funcion send para tamaño 4096
   bytesLeidos=recv(fd, ch_03, 2, 0);
   
#if defined (TRAZA) && (TRAZA > 5)     
   fprintf(stdout,"recibidos >%d< bytes\n",bytesLeidos );
#endif   
   
   if(bytesLeidos!=2)
     return bytesLeidos;  //ojo revisar si tengo que cerrar o no el socket.   	
   
   
 	 nBytesMsg=getLenInt(ch_03,2);

   do{
   	
   	  FD_ZERO(&readset);
      FD_SET(fd, &readset);
   
   // Initialize time out struct
      tv.tv_sec = to;
      tv.tv_usec = 0; //to * 1000
   	

   // select()
      result = select(fd+1, &readset, NULL, NULL, &tv); 

   	  if (result<0)  //error
   	  {
#if defined (TRAZA) && (TRAZA > 1)  	  
   		  fprintf(stdout,"Error de RECV:%d\n", result);
	      fflush(stdout);
#endif		  
	      return result;
      }
      
      if(result==0)  //timeout
      {
#if defined (TRAZA) && (TRAZA > 1)  	  
   		  fprintf(stdout,"Alcanzado el timeout de:%d seg.\n", to);
	      fflush(stdout);
#endif		  
	      return result;
      }
      	
      if (result > 0 && FD_ISSET(fd, &readset)) { //todo ok
      
         // Set non-blocking mode
        if ((iof = fcntl(fd, F_GETFL, 0)) != -1)
           fcntl(fd, F_SETFL, iof | O_NONBLOCK);	
   		  
		    //printf("valor fd antes del recv   :>%d<\n", fd);
		    
		    //SOLO PARA PRUEBAS
	      //Control de tiempos
	      if(indx==0)
	      {	
   
          time(&start_t);
          }
		     //DMND0037271 no hay problema con la funcion recv para tamaño 4096
		    bytesLeidos = recv(fd, buffer+indx, nBytesMsg-indx, 0);
			
#if defined (TRAZA) && (TRAZA > 5)  
   		  fprintf(stdout,"leidos >%i< Datos recibidos:<%s>\n",bytesLeidos, buffer+indx);
	      fflush(stdout);
#endif		  

        indx += bytesLeidos;
        
           // set as before
       if (iof != -1)
         fcntl(fd, F_SETFL, iof);   
       
     }else{
#if defined (TRAZA) && (TRAZA > 1)  	 
     	  fprintf(stdout,"Error en FD_ISSET");
	      fflush(stdout);
#endif		  
	      return -1;
      }
     	
   }while(indx < nBytesMsg);

   buffer[nBytesMsg]='\0';
   
   if(indx>0) //SOLO PRUEBAS
   {
     time(&end_t);
     diff_t = difftime(end_t, start_t);
     fprintf(stdout,"TOTAL DE TIEMPO ejecucion en recvFromRedsys = %f\n", diff_t);
     fflush(stdout);     
   }   
   result = indx;       
   return result;

}


