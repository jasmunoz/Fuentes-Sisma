/*******************
*******************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <netdb.h>
#include "/sf/tux/include/utilidades.h"

int manejhsm(char * clavehsm, char* comandohsm, int tipcla, char * datoshsm, long NumByte, int sfd, char * prespuestahsm); 
long manejhsm_C(char * clavehsm, char* comandohsm, char * datoshsm, char *NumByte, char *sfd, char * prespuestahsm); 
int conexhsm(char* codent, NPSDHSM registro);
void desconexhsm(int sock);
int claveshsm(char * comando, char * subsistema, char * bin, char * clave1, char * clave2);

int funHexa_ascii (char * cadena, char * resultado);
int funHex_to_int(char c);
int funHex_to_ascii(char c, char d);

char Entrada_dll[4096];
char Salida_dll[4096];


int main(int argc, char *argv[])
{

  int retorno = 0;
  int lon_mensaje=0;
  int lonAscii=0;
  int lonHexa = 0;
  int opc, socket_con;
  char claveL[2];
  char datoEn[4096];
  char cadAscii[4096];
  char cadHexa[4096];
  char comando[4096];
  NPSDHSM registro;
  int lon_clave;
  char subsis[5];
  char clave1[151];
  char clave2[151];
  int i=0;
  char conviertAscii[4096];
  char auxLong[5];
  char auxSock[9];
  int numCodAsci=0;
  char auxHexa8[8];
  char lmk[3];
  int tamClave;
  char clvImp[33];
  char clvTr[49];
  char bin[7];
  
  FILE * pFile;
  char * Vent;

   struct hostent *p = gethostbyname("localhost");
   //printf("direccion es %s\n",  inet_ntoa( *((struct in_addr *)p->h_addr) ));
   // lo comento porque no compila

   printf ("iniciamos programa de conexion con HSM\n");  

   memset(&registro, ' ', sizeof(NPSDHSM));
   memset(clave1, '\0', sizeof(clave1));
   memset(clave2, '\0', sizeof(clave2));
   memset(comando, '\0', sizeof(comando));

   retorno=conexhsm("0001", registro);
   if (retorno < 0)
   {
      printf("error al conectar el socket\n");
      exit(-1);
   }
   socket_con=retorno;

  printf("¿Que operación quieres hacer?\n");
  printf("0:listado\n");
  printf("1:test\n");
  printf("2:cifrar pin\n");
  printf("3:pin en claro\n");
  printf("14:cifra pan\n");
  printf("15:descifra pan\n");
  printf("16:cargar clave DES de 32 o 16\n");
  printf("17:cifra cadena\n");
  printf("18:descifra cadena\n");
  printf("21:obtener KCV de una clave guardada en hsm\n");
  printf("30: obtener Hash\n");
  printf("31: obtener RSA\n");
  printf("26:ejecuta comando\n");


   //recogemos el datos
   scanf("%d", &opc); 

  switch(opc)
  {
   case 0:
   // listado
   {
     strncpy(Entrada_dll," ",1);
     memset(Salida_dll,' ', sizeof(Salida_dll));

     lon_mensaje = strlen(Entrada_dll);
     retorno = (manejhsm) (clave1, "00", 0, Entrada_dll, lon_mensaje, socket_con, Salida_dll);

     break;
   }
   case 1:
// Prueba de envio de cadena para TEST  y EXTRAER_CLAVES
   {
     strncpy(Entrada_dll," ",1);
     memset(Salida_dll,' ', sizeof(Salida_dll));

     lon_mensaje = strlen(Entrada_dll);
     retorno = (manejhsm) (clave1, "01", 0, Entrada_dll, lon_mensaje, socket_con, Salida_dll);
     break;
   }

   case 2:
 //Prueba de envio de cadena para cifrar PIN  
 //   //pan(16), offset(4)
     
      printf("introducir pan de 16 en claro  y PIN: ");
      scanf("%s", &datoEn);
      strncpy(Entrada_dll, datoEn, 16+4);
      
      strncpy(bin, "000000", 6);
      bin[6]='\0';

      sprintf(subsis, "%.4s", "0000");
      retorno = (claveshsm)("03", subsis, bin ,clave1, clave2);
      if (retorno<0)
      {
          printf("Error al obtener la clave\n");
      }
      else
      {
        Entrada_dll[20]='\0';
        memset(Salida_dll,' ', sizeof(Salida_dll));
        lon_mensaje = 20;
        retorno = (manejhsm) (clave1, "2", 0, Entrada_dll, lon_mensaje,socket_con, Salida_dll);
      }
    break;  
    
      case 3:
 //Prueba de envio de cadena para calculo de PIN  
   //pan(16), offset(4)
      printf("introducir pan de 16 en claro: ");
      scanf("%s", &datoEn);
      strncpy(Entrada_dll, datoEn, 16);
	  
	  strncpy(bin, datoEn, 6);
	  bin[6]='\0';
 
	  sprintf(subsis, "%.4s", "0000");
 	  retorno = (claveshsm)("PIN_SEFIDE_601_PETPIN                             ", subsis, "0000" ,clave1, clave2);
	  if (retorno<0)
	  {
	      printf("Error al obtener la clave\n");
      }
	  else
	  {
        printf("introducir el offset de 4 - si es pin nativo 0000: ");
        scanf("%s", &datoEn);
        printf("en llama_hsm opcion 3 despues del scanf\n ");
		strncpy(Entrada_dll + 16, datoEn, 4);
		
		Entrada_dll[20]='\0';
        memset(Salida_dll,' ', sizeof(Salida_dll));
        lon_mensaje = 20;
        printf("en llama_hsm opcion 3 antes de enviar a manejhsm\n ");
        retorno = (manejhsm)(clave1, "3", 0, Entrada_dll, lon_mensaje,socket_con, Salida_dll);
      }
	break;
   
     
    case  10:
   // obtención del bloque de pin
	  sprintf(subsis, "%.4s", "0000");
	  retorno = (claveshsm)("PIN_SEFIDE_601_PETPIN                             ", subsis, "000000",clave1, clave2);
	  if (retorno<0)
	  {
	      printf("Error al obtener la clave\n");
      }
	  else
	  {
        strncpy(Entrada_dll, "4539430004512086", 16); 
		strncpy(Entrada_dll + 16, "0000", 4);
		Entrada_dll[20]='\0';
        memset(Salida_dll,' ', sizeof(Salida_dll));
        lon_mensaje = 20;
        printf("en llama_hsm opcion 10 antes de enviar a manejhsm\n ");
        retorno = (manejhsm) (clave1, "10", 0, Entrada_dll, lon_mensaje,socket_con, Salida_dll);
      }

   case  14:
  //cifrado de pin (datos cortos que quepan en pantalla y hexadecimales)
   {
      printf("datos a cifrar: ");
      scanf("%s", &datoEn);
      
      sprintf(subsis, "%.4s", "0000");
      retorno = (claveshsm)("PIN_IDEMIA                                        ", subsis, "000000" ,clave1, clave2);
      if (retorno<0)
      {
          printf("Error al obtener la clave\n");
      }
      else
      {
        clave1[strlen(clave1)]='\0';      
        strncpy(Entrada_dll, datoEn, strlen(datoEn));
        memset(Salida_dll,' ', sizeof(Salida_dll));
        lon_mensaje = strlen(Entrada_dll);
        //printf ("clave1," "%s\n",clave1);
        //manejhsm(char * clavehsm, char* comandohsm, char * datoshsm, long NumByte, int sfd, char * prespuestahsm) 
        retorno = (manejhsm) (clave1, "14", 0, Entrada_dll, lon_mensaje,socket_con, Salida_dll);
      }
      break;
   }
   case  15:
  //descifrado de pin - hexadecimales
   {
      printf("datos a descifrar: ");
      scanf("%s", &datoEn);
      
      sprintf(subsis, "%.4s", "0000");
      retorno = (claveshsm)("PIN_IDEMIA                                        ", subsis,"000000" ,clave1, clave2);
      if (retorno<0)
      {
          printf("Error al obtener la clave\n");
      }
      else
      {
         clave1[strlen(clave1)]='\0';      
         strncpy(Entrada_dll, datoEn, strlen(datoEn));
         memset(Salida_dll,' ', sizeof(Salida_dll));
         lon_mensaje = strlen(Entrada_dll);
         retorno = (manejhsm) (clave1, "15", 0, Entrada_dll, lon_mensaje ,socket_con, Salida_dll);
      }
      break;
   }
   case 16:
   // cargar clave
   {
   
     printf("lmk:");
     scanf("%s", &lmk);     
     if(strlen(lmk) != 2)
     {
        printf("lmk debe ser de 2 posiciones\n");
        return(0);
     }
     
     printf("longitud de la clave (16 0 32): ");
     scanf("%d", &tamClave);     
     if(tamClave != 16 && tamClave != 32)
     {
        printf("la longitud de la clave debe ser de 16 o 32\n");
        return(0);
     }
     
     printf("introducir la clave a importar cifrada con la de transporte: \n");
     scanf("%s", &clvImp);
     if(tamClave == 16 && strlen(clvImp) != 16)
     {
        printf("la clave a importar debe ser de 16 posiciones\n");
        return(0);
     }

     if(tamClave == 32 && strlen(clvImp) != 32)
     {
        printf("la clave a importar debe ser de 32 posiciones\n");
        return(0);
     }
     
     printf("introducir la clave de transporte en formato HSM:\n");
     scanf("%s", &clvTr);
     printf("clvTr: %s\n",clvTr);
     
     if(strlen(clvTr) != 48)
     {
        printf("la clave de transporte en formato HSM debe ocupar 48 posiciones\n");
        return(0);
     }
     
     
     //comando clave de 32
     //000102CAB02020500D00321DD1C91D99422AF91DD1C91D99422AF9D0000D00327DB88E7025BE25647DB88E7025BE2564L0206757434S
          
     //comando clave de 16
     //000086CAB02020400D001632CB62B0DB9718BED0000D0032EB4C4A9E8806D72DEB4C4A9E8806D72DL0206757434S
     
     if(tamClave == 16)
     {
       strncpy(comando,"000086CAB0202", 13);
       strncpy(comando + 13, lmk, 2);
       strncpy(comando + 15, "00D0016", 7);
       strncpy(comando + 22, clvImp, 16);
       strncpy(comando + 38, "D0000", 5);
       strncpy(comando + 43, clvTr, 48);
       strncpy(comando + 91, "S",1);
       comando[92]='\0';          
     }
     if(tamClave == 32)
     {
       strncpy(comando,"000102CAB0202", 13);
       strncpy(comando + 13, lmk, 2);
       strncpy(comando + 15, "00D0032", 7);
       strncpy(comando + 22, clvImp, 32);
       strncpy(comando + 54, "D0000", 5);
       strncpy(comando + 59, clvTr, 48);
       strncpy(comando + 107, "S",1);
       comando[108]='\0';               
     }
     
     printf("comando >%s<\n", comando);
          
     strncpy(Entrada_dll,comando,strlen(comando));       
     memset(Salida_dll,' ', sizeof(Salida_dll));

     lon_mensaje = strlen(Entrada_dll);
     //monto el comando y ejecuto el 26, no he preparado conexhsm para el 16
     retorno = (manejhsm) (clave1, "26", 0, Entrada_dll, lon_mensaje, socket_con, Salida_dll);     

     break;
   }
 
   case 21:
   {
     printf("escriba el identificador de la clave de 5 cifras (ej:00012):");
     scanf("%s", &clvImp);
     
     if(strlen(clvImp) != 5)
     {
       printf("el identificador debe ser de 5 posiciones");
     }
     else
     {     
       //000024CAB0401S000500012N000000
       strncpy(comando, "000024CAB0401S0005", 18);
       strncpy(comando +18, clvImp, 5);
       strncpy(comando +23, "N000000", 7);
       comando[30]='\0';
       
       strncpy(Entrada_dll,comando,strlen(comando));       
       memset(Salida_dll,' ', sizeof(Salida_dll));

       lon_mensaje = strlen(Entrada_dll);
       retorno = (manejhsm) (clave1, "26", 0, Entrada_dll, lon_mensaje, socket_con, Salida_dll);     
     }
     break;
   }

   case 26:
   {
     printf("escriba el comando a enviar:");
     scanf("%s", &comando);
     
     //importar la clave de transporte
     //strcpy(comando,"000102CAB02020700D003209FCEA5275260665FEBE73EB1001A707D0000D0032EB4C4A9E8806D72DEB4C4A9E8806D72DL0206757434S");
     
     //crear arqc:
     //strcpy(comando,"000129CAB050330S000500014N0006877BA107271000077019010800000000025000000000000000724000000C000072401070200CEB4FBA67C00005F03A0A002000000");
     strncpy(Entrada_dll,comando,strlen(comando));       
     memset(Salida_dll,' ', sizeof(Salida_dll));

     lon_mensaje = strlen(Entrada_dll);
     retorno = (manejhsm) (clave1, "26", 0, Entrada_dll, lon_mensaje, socket_con, Salida_dll);     

     break;
   }
   case 30:
   {
      printf("datos a hashear: ");
      scanf("%s", &datoEn);

  //    strncpy(Entrada_dll,"14", 2);
      //strncpy(Entrada_dll+2, datoEn, strlen(datoEn));
      strncpy(Entrada_dll, datoEn, strlen(datoEn));
      memset(Salida_dll,' ', sizeof(Salida_dll));
      lon_mensaje = strlen(Entrada_dll);
      retorno = (manejhsm) (clave1, "30", 0, Entrada_dll, lon_mensaje,socket_con, Salida_dll);
      break;
   }
   
   case 31:
   {
      printf("datos para RSA: ");
      scanf("%s", &datoEn);

      sprintf(subsis, "%.4s", "0000");
      //retorno = (claveshsm)("RSA_CLAVE                                         ", subsis, "000000" ,clave1, clave2);
        retorno = (claveshsm)("FIRMA_IFRAME_IDEMIA                               ", subsis, "000000" ,clave1, clave2);

      printf ("clave hsm comando 31" "%s\n",clave1);
      if (retorno<0)
      {
          printf("Error al obtener la clave\n");
      }
      else
      {
        clave1[strlen(clave1)]='\0';
        strncpy(Entrada_dll, datoEn, strlen(datoEn));
        memset(Salida_dll,' ', sizeof(Salida_dll));
        lon_mensaje = strlen(Entrada_dll);
        printf ("clave1," "%s\n",clave1);

        retorno = (manejhsm) (clave1, "31", 0, Entrada_dll, lon_mensaje,socket_con, Salida_dll);
      }
      break;
   }
   case 17:
  //cifrado cadena hexadecimal
   {
      printf("cadena a cifrar: ");   
      scanf("%s", &datoEn);
      
      sprintf(subsis, "%.4s", "0000");
      retorno = (claveshsm)("PIN_IDEMIA                                        ", subsis, "000000" ,clave1, clave2);
   //   retorno = (claveshsm)("KMCA.SMARTPIN.TEW.00000001                        ", subsis, "000000" ,clave1, clave2);
      if (retorno<0)
      {
          printf("Error al obtener la clave\n");
      }
      else
      {
        clave1[strlen(clave1)]='\0';      
        strncpy(Entrada_dll, datoEn, strlen(datoEn));
        memset(Salida_dll,' ', sizeof(Salida_dll));
        lon_mensaje = strlen(Entrada_dll);
        printf ("clave1," "%s\n",clave1);
        //manejhsm(char * clavehsm, char* comandohsm, char * datoshsm, long NumByte, int sfd, char * prespuestahsm) 
        retorno = (manejhsm) (clave1, "17", 0, Entrada_dll, lon_mensaje,socket_con, Salida_dll);
      }
      break;
   }
   case 18:
  //descifrado cadena hexadecimal
   {
      printf("cadena a descifrar: ");
      scanf("%s", &datoEn);
      
      sprintf(subsis, "%.4s", "0000");
      retorno = (claveshsm)("PIN_IDEMIA                                        ", subsis,"000000" ,clave1, clave2);
      if (retorno<0)
      {
          printf("Error al obtener la clave\n");
      }
      else
      {
         clave1[strlen(clave1)]='\0';      
         strncpy(Entrada_dll, datoEn, strlen(datoEn));
         memset(Salida_dll,' ', sizeof(Salida_dll));
         lon_mensaje = strlen(Entrada_dll);
         retorno = (manejhsm) (clave1, "18", 0, Entrada_dll, lon_mensaje ,socket_con, Salida_dll);
      }
      break;
   }  
  default:  
   {
    printf("error en la opcion, que no existe\n");
    break;
   }
  }

  //printf("Recibido:  >%d<\n", retorno);
  printf(">>");
  printf(Salida_dll);
  printf("<<\n");         
  //desconexhsm(socket_con);
}//fin main


int funHexa_ascii (char * cadena, char * resultado)
{

 int lonCadena = 0;
 int i = 0; // Contador para recorrer cadena hexadecimal
 int j = 0; // Contador para recorrer cadena ascii
 char cIz = 0;
 char cDe = 0;
 char auxC =0;
     
   lonCadena = strlen(cadena);
   
   if(lonCadena==0)
   {
      return(0);
   }

   for(i = 0; i < lonCadena; i++)
   {
       if(i % 2 != 0) /// en posicion impar tengo la pareja pq empieza en 0
       { 
          cDe=cadena[i];
          auxC = funHex_to_ascii(cIz, cDe);                    
          resultado[j]=auxC;
          
          j++;
       }else // si es par guardo el caracter de la izquierda
       {
          cIz = cadena[i];
       }
   }
   resultado[lonCadena/2]='\0';
   return(lonCadena/2);
}

int funHex_to_int(char c){

     int auxNum=0;
     char auxChar[2];
     
     //printf("funHex_to_int >%c<", c);
     
     if(c=='A')
        auxNum = 10;
     else
     if(c=='B')
        auxNum = 11;
     else        
     if(c=='C')
        auxNum = 12;
     else
     if(c=='D')
        auxNum = 13;
     else
     if(c=='E')
        auxNum = 14;
     else
     if(c=='F')
        auxNum = 15;
     else
     {        
        sprintf(auxChar,"%c",c);
        auxChar[1]='\0';
        auxNum = atoi(auxChar);
    }
    return(auxNum);
}

int funHex_to_ascii(char i, char d){
        int high = funHex_to_int(i) * 16;
        int low = funHex_to_int(d);
        return high+low;
}


