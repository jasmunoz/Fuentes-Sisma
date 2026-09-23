#include "nptdhsm.h"
#include <openssl/ssl.h>
#include <openssl/err.h>


int manejhsm(char * clavehsm, char* comandohsm, int tipcla, char * datoshsm, long NumByte, int sfd, char * prespuestahsm); //PRJ0258542
int conexhsm(char* codent, NPSDHSM reg_hsm);
int claveshsm(char * comando, char * subsistema, char * bin, char * clave1, char * clave2);


// PanClaro debe tener \0 para que funcione
// retorna 0 si ha ido bien
//********************************************//
// PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño del pan, ni PanClaro ni PanCifrado
int fcifraPan(char *PanClaro, char *PanCifrado)
{
   int retorno = 0;
   char entrada_hsm[17]; // tamaño pan +1
   char salida_hsm[4096]; // tamaño maximo mensaje
   int long_hsm = 0;
   // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud total del pan
   char pan16[17];
   char clave1[151];
   char clave2[151];
   //char idclvCifrado[6];
   ////esto se quitará cuando siempre se cifre con hsm
   NPSDHSM reghsm;
   char conHSM[1];
   
   extern struct sfdssl * sockssl;  
   
   //RAQUEL AHORA MISMO NO SE CIFRA EL PAN POR AQUI
  
   if(strlen(PanClaro)== 0)
   {
      fprintf(stdout,"Pan no informado, no se cifra\n");
      return(0);
   }
      
  
   // cifrado interno subsistema 0000 y bin 000000 pq ciframos todos los bines igual
   retorno = claveshsm("CIFRAR_PAN_SEFIDE", "0000", "000000" ,clave1, clave2);
   if (retorno<0)
   {
	 printf("Error al obtener la clave\n");
	 return(-1); 
   }

   // ********************    ENTRADA HSM ******************//
   //entrada_hsm lleva 16 para el pan
  // strncpy(entrada_hsm, PanClaro,16);
  // entrada_hsm[16]='\0';
  // long_hsm=16;
    // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud de PanClaro
    
    strncpy(entrada_hsm,"0",1);
    strncpy(entrada_hsm + 1, PanClaro,16);
    entrada_hsm[17]='\0';
    long_hsm=17;
     
     
  //xavi 
//	 retorno = manejhsm(clave1, "14", entrada_hsm, long_hsm, sockssl, salida_hsm);
	 retorno = manejhsm(clave1, "14", 0, entrada_hsm, long_hsm, sfd_hsm, salida_hsm); //PRJ0258542

     if (retorno <= 0) //retorna el numero de bytes de la respuesta o un error en negativo
     {
        fprintf(stdout,"Error >%d< al encriptar pan\n", retorno);
        //////////// SI ES POR ERROR DE CONEXION VUELVO A CONECTAR Y LO INTENTO DE NUEVO
        if(retorno == -32 || retorno == - 145)
		{
		   fprintf(stdout,"Intenta reconectar\n");
           memset(&reghsm, ' ', sizeof(NPSDHSM));
           sfd_hsm=conexhsm("0001", reghsm);//por ahora la ponemos a pelo
           
//xavi
//           sockssl->sfd_ssl= sfd_hsm;
		   if (sfd_hsm > 0)
//xavi
//		      retorno = manejhsm(clave1, "14", entrada_hsm, long_hsm, sockssl, salida_hsm);
		      retorno = manejhsm(clave1, "14", 0, entrada_hsm, long_hsm, sfd_hsm, salida_hsm); //PRJ0258542
		   else
		      retorno = sfd_hsm;
		}
	 }
	 //// si no era conexión o también falla el segundo intento
	 if (retorno <= 0)
         return(retorno);
	 
	 
     //////// SI VA BIEN
     retorno=0;
     fprintf(stdout,"salida_hsm >%s<\n", salida_hsm);
     // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud de PanClaro ni PanCifrado
     if(strlen(PanClaro)>16)
     {
        strncpy(PanCifrado,salida_hsm,16);
	    strncpy(PanCifrado +16, PanClaro+16, (strlen(PanClaro)-16));
     }
     else
        strncpy(PanCifrado,salida_hsm,16);
   //}
   
   return(retorno);
}

int fdescifraPan(char *PanCifrado, char *PanClaro)
{
   int retorno = 0;
   char entrada_hsm[20]; // tamaño pan +1
   char salida_hsm[4096]; // tamaño maximo mensaje
   int long_hsm = 0;
   // PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total del pan16
   char pan16[17];
   char clave1[50];
   char clave2[50];
   //char idclvCifrado[6];
   ////esto se quitará cuando siempre se cifre con hsm
   NPSDHSM reghsm;
   char conHSM[1];
   
   // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud total de PanCifrado
   fprintf(stdout,"PanCifrado>%d<>%s<\n", strlen(PanCifrado), PanCifrado);
   
   if(strlen(PanCifrado)== 0)
   {
      fprintf(stdout,"Pan no informado, no se descifra\n");
      return(0);
   }
      
   
   
   // ********** IDENTIFICADOR DE LA CLAVE DE CIFRADO ************
   // cifrado interno subsistema 0000 y bin 000000 pq ciframos todos los bines igual
   retorno = claveshsm("CIFRAR_PAN_SEFIDE", "0000", "000000",clave1, clave2);
   if (retorno<0)
   {
	 printf("Error al obtener la clave\n");
	 return(-1); 
   }
 
   // ********************    ENTRADA HSM ******************
 
    strncpy(entrada_hsm,"0",1);
    strncpy(entrada_hsm + 1, PanClaro,16);
    entrada_hsm[17]='\0';
    long_hsm=17;
     
   
   //************ SOLO HAY CONEXION EN LA MAQUINA 15
   //             EN LA 16 CIFRAMOS CON GF AL FINAL
   retorno=tratar_fich("NPTDHSM",IGUAL,&reghsm, "0001");
   if (retorno)
   {
     fprintf(stdout,"Error >%d< en IGUAL Fichero NPTDHSM\n", retorno);
     return(-1); 
   }
   strncpy(conHSM,reghsm.dhprodt,1);
   
   ////////////////// CIFRADO PARA PRUEBAS //////////
   if (conHSM[0]=='0')
   {
       // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud total de PanClaro ni PanCifrado
      fprintf(stdout,"NO utilizamos hsm\n");
      // solo para tarjetas de 16 digitos
      strncpy(PanClaro, PanCifrado,16);
	  PanClaro[16]='\0';
   }
   else
   {
      fprintf(stdout,"SI utilizamos hsm\n");
     //sfd_hsm lo tiene que tener declarado el servicio desde
     //donde se ejecuta esta funcion, pej en el arranque del rtratami
     //que tiene la conexion previa al hsm.
   
     //Llamada a la libreria de hsm para descifrado de datos
     //retorno = manejhsm(entrada_hsm, salida_hsm, long_hsm,"descifra_pan", sfd_hsm); 
	 retorno = manejhsm(clave1, "15", 0, entrada_hsm, long_hsm, sfd_hsm, salida_hsm); //PRJ0258542
     fprintf(stdout,"despues de manejhsm retorno>%d<\n",retorno);

     if (retorno <= 0) // si no devuelve nada o error en negativo
     {
#if defined (TRAZA) 
        fprintf(stdout,"Error >%d< al desencriptar pan\n", retorno);
 #endif
        return(retorno);
     }
     
     retorno=0;
     fprintf(stdout,"salida_hsm >%s<\n", salida_hsm);
     if(strlen(PanCifrado)>16)
     {
        // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud de PanClaro ni PanCifrado
        strncpy(PanClaro,salida_hsm,16);
	    strncpy(PanClaro +16, PanCifrado+16, (strlen(PanCifrado)-16));
     }
     else
        strncpy(PanClaro,salida_hsm,16);
   }
   
   return(retorno);
}

