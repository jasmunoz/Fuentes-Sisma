#include <stdlib.h>
#include "svm.h"
#include "nptpars.h"
int   dame_seg(char *buf);
void carga_subs(SVM *Psvm,NPSPARS *reg)
{
#define SVM_PARS (Psvm->str_svm.svmpars)
 char aux[13];

   memset(aux,0,sizeof(aux));
   strncpy(Psvm->pasubsis,reg->pasubsis,sizeof(reg->pasubsis));
   strncpy(Psvm->pasubsis+2,reg->pasubapl,sizeof(reg->pasubapl));
   Psvm->svmtip=TIP_SUBSIS;
   Psvm->estado='0';
   memset(aux,0,sizeof(aux));
   strncpy(aux,reg->pasemaf1,sizeof(reg->pasemaf1));
   SVM_PARS.pasemsub=strtol(aux,(char **) 0,10);
   strncpy(SVM_PARS.paestado,reg->paestado,sizeof(reg->paestado));
   memset(aux,0,sizeof(aux));
   strncpy(aux,reg->patrepet,sizeof(reg->patrepet));
   Psvm->str_svm.svmpars.patrepet=dame_seg(aux);
   memset(aux,0,sizeof(aux));
   strncpy(aux,reg->patrerep,sizeof(reg->patrerep));
   SVM_PARS.patrerep=dame_seg(aux);
   memset(aux,0,sizeof(aux));
   strncpy(aux,reg->patresnf,sizeof(reg->patresnf));  
   SVM_PARS.patresnf=dame_seg(aux);
   strncpy(SVM_PARS.pacidest,reg->pacidesti,sizeof(reg->pacidesti));
   strncpy(SVM_PARS.paciadqui, reg->paciadqui, sizeof(reg->paciadqui));
   strncpy(SVM_PARS.paidred, reg->paidred, sizeof(reg->paidred));
   strncpy(SVM_PARS.pafecses, reg->rdfses.pafecses, sizeof(reg->rdfses.pafecses));
   strncpy(SVM_PARS.panip,    reg->panip,  sizeof(reg->panip));
   strncpy(SVM_PARS.paportnume, reg->paportnume, sizeof(reg->paportnume));
   strncpy(SVM_PARS.panumses, reg->rdfses.panumses, sizeof(reg->rdfses.panumses));
   printf("*** Carga_subs.c - valor de SVM_PARS.panumses >%.3s< \n", SVM_PARS.panumses);
}
