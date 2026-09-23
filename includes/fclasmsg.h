#ifndef FCLASMSG_H
#define FCLASMSG_H

/**/
/*   definicion de valores para los tipos de mensajes   */  
/**/
#ifndef DESCONOCIDO
#define  DESCONOCIDO -1
#endif 

#define  PETICION_NO_CONTROL 1
#define  PETICION_CONTROL    2
#define  RESPUESTAS_NUCLEO   3
#define  RESPUESTAS_OTROS    4
#define  PETICION_CONTROL_SIO    5
#define  RESPUESTAS_CONTROL_SIO    6
#define  PETICION_CIERRE_SIO    7
#define  RESPUESTAS_CIERRE_SIO    8
#define  PETICION_CIERRE 9
#define  RESPUESTA_CIERRE 10
#define  ADMINISTRATIVO 11

#endif
