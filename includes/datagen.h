#ifndef DATAGEN_H
#define DATAGEN_H
#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
#include "cnetplus.h"
#include "cforitno.h"
#include "nptinci.h"

typedef struct tm_dec {         /* see ctime(S) */
 int tm_sec;                    /* time of day, seconds */ 
 int tm_min;                    /* time of day, minutes */
 int tm_hour;                   /* time of day, hours (24 hour clock) */
 int tm_mday;                   /* day of month (1-31) */
 int tm_mon;                    /* month of year (0-11) */
 int tm_year;                   /* year - 1900 */
 int tm_siglo;                   /* siglo - 1 */
 int tm_mls;                    /* milesimas de segundo */
} tm_dec;

extern void fechorsys(struct tm_dec *tmPtr);
extern void lpad(char *buf,char c, int lon);
extern char *itoa_ux(int valor);
extern int  comp_fec(char *f1,char *f2);
extern void pinta_decimal(float buf, int ndec, char *resultado);
extern void pone_coma_decimal(int ent, int dec, char *buf, char *resultado);
extern void quita_coma_decimal(char *buf, char *resultado);
extern char *dtoa(double);
extern int fgraba_incidencia_cp(char *coderror, char *numope, char *nompro, char *nomfic);
//
int conecSVM(void **);
int crearSVM(void **,int *,int,char *);
int fgraba_log(formato_interno * pforitn);
int  LeerSock(int sfd,int timeout,char *msg,int len);
int  EnviaSock(int sfd,int timeout,char *msg,int len);
void getLenBin(char *pBinary, int len);
int getLenInt(char *pBinary, int len);
int LeerCola(int cola,int timeout,char *Datosmsg,int Lonmsg,int Tipomsg);
int  EnviaCola(int cola,int timeout,char *Datosmsg,int Lenmsg,int Tipomsg);
void fhexdump(FILE * pf, void * vaddr, int len);
void hexdump(char * addr, int len);
int fempapin(int longi, char *sempaq, char *sdesem);
int fdesepin(int longi, char *sempaq, char *sdesem);
int potencia(int base, int exponente);
int asc2bin(char * pcadena, int * numerico, int longi);
int bin2asc(char * pcadena, int numerico, int * longi);

void monta_clprevia(char * clave, formato_interno * pforitn);
#endif

