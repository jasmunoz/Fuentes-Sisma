#ifndef _COLA_COMUN_H
#define _COLA_COMUN_H
 

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/ipc.h>

/* Longitud maxima de mensajes */
//#define MSG_LEN 10240  
#define MAX_BUFFER 100

#define E_QVACIA           1
#define E_TIMEOUT          2
#define E_LENGERR          3
#define E_QBUSY            5
#define E_NOSID            7 /* Error al selecciona la id */   



/* Prototipos de funciones de lectura y escritura en cola */
int EscribirCola_c(char *Nombcola, char* Msj, int LonMsj, int* idHj, int tiempo);
int leerCola_c(char *Nombcola, char* Msj, int *LonMsj, int idHj, int tiempo);
int leer_cola_cob(char *cob_nombcola, char *cob_tiempo, char *cob_Msj,char *cob_lonj);
int leer_cola_cob_id(char *cob_nombcola, char *cob_idhij, char *cob_tiempo, char *cob_msj,char *cob_lonj);
int escribir_cola_id(char *cob_nombcola,char *cob_idhij, char *cob_tiempo, char *cob_Msj,char *cob_lonj);

                      
#endif

