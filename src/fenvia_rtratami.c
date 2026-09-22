//PRJ0257005 - Revisado tokenizacion
#include "nptinci.h"

/***************************************************************************
   FUNCION   FENVIA_RTRATAMI
   DESCRIPCION: Envia un mensaje al subsistema autorizador o nucleo
***************************************************************************/
int fenvia_rtratami (formato_interno *pforitn)
{
  int retorno;
  mensaje_nuc msgnuc; 
  int long_msgnuc = sizeof(msgnuc.fiforitn);
  void sigue();

#if defined (TRAZA) && (TRAZA > 10)
 fprintLog(nomProg, "-------FUNCION   FENVIA_RTRATAMI ----------\n"); 
 fflush(stdout);

#endif
 msgnuc.tipo = 1111;
 memcpy(&msgnuc.fiforitn, pforitn, long_msgnuc);  

   /*   envia a rtratami  */
 signal(SIGALRM,sigue);
 alarm(ESSNDCOL);
 retorno=msgsnd(qrtratami, &msgnuc, long_msgnuc,0);
 fprintLog(nomProg, "Longitud envio long_msgnuc >%d< msngnuc >%s< cola de envio >%i<\n", long_msgnuc, (char*) &msgnuc.fiforitn, qrtratami); //PRJ0257005 - Pongo el número de cola en el que escribe
 perror("msgsnd");
 alarm(0);
 signal(SIGALRM,SIG_IGN);
 fprintLog(nomProg, "------- Enviado fuera - cola de envio >%i< --------\n", qrtratami);  //PRJ0257005 - Pongo el número de cola en el que escribe
 fprintLog(nomProg, "----------------------------------------------------------\n\n");
 fflush(stdout);
 if (retorno == -1)
 {
  strcpy(coderror, "249"); 
  strcpy(reginci.innomfic, "QTRATA00");  
  //Añadido para Monitorizacion SISMA 
  fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo escritura en QTRATA00 codigo de error >%.3s<\n", coderror);

  return(1); 
 }
 return (0);
}
