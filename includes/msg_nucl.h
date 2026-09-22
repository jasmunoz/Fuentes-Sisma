#ifndef MSG_NUCL_H
#define MSG_NUCL_H

#include "cnetplus.h"
#include "cforitno.h"

/*--- mensaje hacia o desde el nucleo netplus  ---*/
typedef struct mensaje_nuc
 {
    long           tipo;
    formato_interno  fiforitn;
 } mensaje_nuc;

#endif


