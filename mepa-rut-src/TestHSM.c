/*******************
// Proceso para comprobar la conexión con el HSM principal, conecta y lanza test, si es ok, termina = 0
// si no es ok, conecta y lanza test al HSM secundario, si es OK, termina =0
// si no consigue conectar con el secundario o no permite cambiarlos termina 2 (para avisar error)
// si conecta con el secundario y los cambia termina 1 (para avisar revisión)
*******************/
//****************coger la fecha de sistema si lo quieren datar por dia y fecha si no solo la fecha sin hora
//****************en todo caso segun email de Juanmi mar 08/10/2019 14:06 ellos se encargan del borrado y no será necesario que sea por hora, se hara diario 
//****************se lanzara cada 10 minutos aunque pregunta si es parametrizable
//****************se incluira la hora de conexion y la IP conectada
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <sys/time.h> 
#include <time.h>
#include <netdb.h>
#include <errno.h>
#include "/sf/tux/include/utilidades.h" 


int manejhsm(char * clavehsm, char* comandohsm, char * datoshsm, long NumByte, int sfd, char * prespuestahsm); 
int conexhsm(char* codent, NPSDHSM registro);
void desconexhsm(int sock);
int fCambiaIP();
int fConexIP(int iHSM);
char Entrada_dll[4096];
char Salida_dll[4096];

//fichero de trazas
FILE * pFile;

int main(int argc, char *argv[])
{

  int retorno = 0;
  int lon_mensaje=0;
  int socket_con;
  NPSDHSM registro;
  char clave1[151];
  char clave2[151];
  int numHSM = 0;
  int nConectado = 0;
  char fichero[200];
  char* ip_conex;
  char* tiempo;


  // Montamos el nombre del fichero
   sprintf(fichero, "%s/hsm.log",DIRTRTUX);
   pFile = freopen(fichero, "w", stdout);
   fflush(pFile);
   
   struct hostent *p = gethostbyname("localhost");
   
   for (;;)
   {
       memset(registro.dhcodent, ' ', sizeof(NPSDHSM)); 

       retorno = fConexIP(1);
       
      // printf("retorno ip1 %i\n", retorno);
      // fflush(pFile);

       if (retorno < 0)
       {
       
       // si no se ha podido conectar intentamos al numHSM 2   
         retorno = fConexIP(2);
        // printf("retorno ip1 %i\n", retorno);
        // fflush(pFile);        
         if (retorno < 0)
         {
     
           fflush(pFile);
         }
         else 
         { 
           fflush(pFile);
           socket_con=retorno;
           strncpy(Entrada_dll," ",1);
           memset(Salida_dll,' ', sizeof(Salida_dll));
           
           lon_mensaje = strlen(Entrada_dll);
           retorno = (manejhsm) (clave1, "01", 0, Entrada_dll, lon_mensaje, socket_con, Salida_dll);

             if (retorno == 6)
             {        
                if (strncmp(Salida_dll, "000000",6) == 0)
                    nConectado = 2;
             }     

         }   
       }
       else
       {
         fflush(pFile);
         socket_con=retorno;
         strncpy(Entrada_dll," ",1);
         memset(Salida_dll,' ', sizeof(Salida_dll));
       
         lon_mensaje = strlen(Entrada_dll);
         
        // printf("llega a retorno ip1\n");
        // fflush(pFile);
         retorno = (manejhsm) (clave1, "01", 0, Entrada_dll, lon_mensaje, socket_con, Salida_dll);
       // printf("retorna del manehsm ip1\n");
       //  fflush(pFile);
         if (retorno == 6)
         {        
              if (strncmp(Salida_dll, "000000",6) == 0)
                  nConectado = 1;
         }     

       }

   }            
}//fin main

