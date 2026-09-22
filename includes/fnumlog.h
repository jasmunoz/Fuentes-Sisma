#ifndef FNUMLOG_H
#define FNUMLOG_H

#include "cnetplus.h"
#include "deffich.h"
#include "nptpara.h"

typedef struct
{
  char ch_log_actual;
  char ch_log_anterior;
  char ch_log_siguiente;
  NOMFICH log_anterior;
  NOMFICH log_actual;
  NOMFICH log_siguiente;
} stres_numlog;

int   fnumlog(char chLog, stres_numlog * presp);
#endif

