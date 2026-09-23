#ifndef FTIMER_H
#define FTIMER_H

#include "cnetplus.h"
#include "cforitno.h"

#define TMP_REL 0       /** tiempo relativo **/
#define TMP_ABS 1       /** tiempo absoluto **/

int ftimer(formato_interno * pforitn, int pid_ptimer,
                       int segundos, int tipo_tiempo);
#endif
