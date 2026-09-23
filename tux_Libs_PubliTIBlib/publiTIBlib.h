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

#ifndef _PUBLITIB_H_
#define _PUBLITIB_H_

int PubliIni(char *status);
int PubliEnviaMensaje(char *msg, int *msglen, char *status);
int PubliFin();

#endif

