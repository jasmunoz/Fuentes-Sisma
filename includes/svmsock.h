#ifndef SVMSOCK_H
#define SVMSOCK_H

#define VACIO -1

#define MAXSOCK 40 

/****************************************/
/*  tabla para los canales del pmanej   */
/****************************************/
typedef struct
{
   int      num_ip;
   char     nompadre[11];
   int      pidpadre;
   int      pidhijo;
   int      pidnieto;
   int      signieto;
} tbsock;

tbsock    tabla_sock [MAXSOCK];

#define MAX_REG_TBSOCK (MAXSOCK)
#define TAM_TBSOCK       ((MAX_REG_TBSOCK+1)*sizeof(tbsock))


#define SVMnieto  (psvmsock+posic)->pidnieto
#define SVMhijo   (psvmsock+posic)->pidhijo
#define SVMpadre  (psvmsock+posic)->pidpadre
#define SVMnombre (psvmsock+posic)->nompadre
#define SVMip     (psvmsock+posic)->num_ip
#define SVMsig    (psvmsock+posic)->signieto

#endif
