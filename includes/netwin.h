//////////////////////////////////////////////////////////////////////////////
//   netwin.h   :       Campos generales para netwin,
//                  y para el manejo de los servicios, threads, etc.
//////////////////////////////////////////////////////////////////////////////
#ifndef _NETWIN_H
#define _NETWIN_H

#include <stdio.h>
#include "cnetplus.h"

#ifdef __cplusplus
extern "C" {
#endif

//  directorio general de la aplicacion


//#define SCO_NETPLUS
//      Estos defines se incluyen para facilitar el establecimiento de
//      los "WaitableTimer".
#define _SECOND ((__int64) 10000000)
#define _MINUTE (60 * _SECOND)
#define _HOUR   (60 * _MINUTE)
#define _DAY    (24 * _HOUR)

int globalErrorLevel=-1;


//////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif
