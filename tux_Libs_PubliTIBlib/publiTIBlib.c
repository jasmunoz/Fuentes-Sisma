/************************************************************************
*
*  (c) Copyright 2001, Hewlett-Packard Company,  Unpublished
*  All rights reserved under the Copyright laws of Spain.
*
*  Hewlett-Packard assumes no responsibility for the use or reliability
*  of its software on equipment that is not furnished by Hewlett-Packard.
*
*  Hewlett Packard Company.
*  Professional Services Organization
*  Km 16,400 Las Rozas
*  Madrid, 28230
*  (91) 631-1600
*
*************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <atmi.h>
#include <userlog.h>
#include <sys/types.h>
#include <unistd.h>
#include "publiTIBlib.h" 


#define PUBLISERVICE "PUBLISERVICE"

/***************************************************************************/
int PubliIni(char *status)
/***************************************************************************/
{

   TPINIT *tpinfo;
   
   /* Inicializamos el estado con ERROR. */
   memcpy(status, "KO", 2);
  

   if ((tpinfo = (TPINIT *)tpalloc("TPINIT", NULL,
                                   sizeof(struct tpinfo_t))) == NULL) {
      fprintf(stderr, "PubliIni: Error en tpalloc tpinfo: %s\n",
              tpstrerror(tperrno));
      return 0;
   }

   sprintf(tpinfo->usrname, "PUBLI-%d", getpid());
   strcpy(tpinfo->cltname, "");
   strcpy(tpinfo->passwd, "");
   strcpy(tpinfo->grpname, "");
   tpinfo->flags = 0;
   tpinfo->datalen = 0;
   tpinfo->data = 0;

   if (tpinit(tpinfo) == -1) {
      tpfree((char *)tpinfo);
      fprintf(stderr, "PubliIni: Error en tpinit: %s\n", tpstrerror(tperrno));
      return 0;
   }

   tpfree((char *)tpinfo);

   /* Todo ha ido bien, ponemos OK. */
   memcpy(status, "OK", 2);

   /* JGL migIA */
 
   return 1;
}

#define MAX_RECVLEN 32

/***************************************************************************/
int PubliEnviaMensaje(char *msg, int *msglen, char *status)
/***************************************************************************/
{
   long   sendlen, recvlen;
   char   *sendbuf, *recvbuf;
   int    ret;

   /* Inicializamos el estado con ERROR. */
   memcpy(status, "KO", 2);
   sendlen = *msglen;

   if ((sendbuf = (char *)tpalloc("STRING", NULL, sendlen+1)) == NULL) {
      fprintf(stderr, "PubliTib -> Error en tpalloc sendbuf: %s\n",
              tpstrerror(tperrno));
      return 0;
   }

   if ((recvbuf = (char *)tpalloc("STRING", NULL, MAX_RECVLEN)) == NULL) {
      fprintf(stderr, "PubliTib -> Error en tpalloc recvbuf: %s\n",
              tpstrerror(tperrno));
      return 0;
   }

   memcpy(sendbuf, msg, sendlen);
   sendbuf[sendlen] = 0;

   if ((ret = tpcall(PUBLISERVICE, (char *)sendbuf, sendlen, &recvbuf,
                     &recvlen, 0)) == -1) {
      fprintf(stderr, "PubliTib -> Error en tpcall: %s\n",
              tpstrerror(tperrno));
      tpfree(sendbuf);
      tpfree(recvbuf);
      return 0;
   }

   if (recvlen > MAX_RECVLEN && recvbuf) {
      if((recvbuf = (char *) tprealloc(recvbuf, recvlen+1)) == NULL) {
         fprintf(stderr, "PubliTib -> Error en tprealloc: %s\n",
                 tpstrerror(tperrno));
         userlog("Error en tprealloc: %s\n",tpstrerror(tperrno));
         tpfree(sendbuf);
         return 0;
      }
   }

   /* El estado de como ha ido lo envia el servicio PUBLISERVICE. */
   memcpy(status, recvbuf, 2);
   fprintf(stdout, "PubliTib -> Estado Envio Mensaje XML: %2.2s.\n", status);
   tpfree(sendbuf);
   tpfree(recvbuf);

   return 0;
}

/***************************************************************************/
int PubliFin()
/***************************************************************************/
{
   tpterm();
   return 0;
}

