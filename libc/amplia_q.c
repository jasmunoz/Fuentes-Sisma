 #include "svm.h"
#include <errno.h> 

void amplia_Q(int cola,int tamano)
{
   struct msqid_ds qstat; 

    if (msgctl(cola,IPC_STAT,&qstat) < 0) /* get msg specs */
       {
#if defined (TRAZA) && (TRAZA > 1)
       fprintf(stderr,"error estado cola en amplia_q %d\n",errno); 
#endif
       }
    else
       {
         qstat.msg_qbytes = tamano; 

        if (msgctl(cola,IPC_SET,&qstat) < 0)
          {
#if defined (TRAZA) && (TRAZA > 1)
          fprintf(stderr,"error(%d) amplia cola(%d) a (%ld) bytes\n",
                           errno,cola,qstat.msg_qbytes);
#endif
          }
    }
}
