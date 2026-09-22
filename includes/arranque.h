#ifndef ARRANQUE_H
#define ARRANQUE_H

#include "cnetplus.h"
#include "colas.h" 



typedef struct NPSCONF{ 

  char confindi[3];  /* codigo 00 siempre para cargar toda la tabla con procedimiento CARGA*/
  char confcdsu[5];   /* subistema-subaplicaicon */
  char confdsej[9];  /* nombre con que se levanta el proceso */
  char confpara[21];   /* El maximo permitido es 20 */
  char confcola[5];   /* nombre de la cola o SVM */
  char conftipp[2]; /* Como parar el proceso: Q=cola, S=svm, N=no parar */   
  int  confnmpr; /* Numero de posiciones reservadas en svm para proceso, cuantos tenemos que inicializar*/   
  int  confcapr; /* Numero de copias a arrancar automaticamente */   
  char confcsal[5];  /* Cola de salida para cuando el pamnej no es parte de SISMA */  
} NPSCONF;

NPSCONF tabla_conf [50];


#endif
