#include <time.h>
SYSTEMTIME t; 

//void fprintLog(char* format, ...);
int fjuliana_of(fecha *fechag,juliana *fechaj); 
int fgregoriana_of(juliana fechaj,fecha *fechag);
void fponelunes_of(int clave, fecha *fechaj,  fecha *lunes_ant);
void fresta_dias_of(juliana * fechaj, int dias);
void fponemeses_of(int meses, fecha *fechag, int dia_inicio);
int finicializar_periodo_of(int periodo, fecha *pfecha_operacion, 
         fecha *pfecha_inicio);
int fcompara_fechas_of (fecha *fecha1, fecha *fecha2);
int fsemanal_of(int periodo, fecha *pfecha_operacion, fecha *pfecha_inicio);
int funoyquince_of(fecha *pfecha_operacion, fecha *pfecha_inicio);
int fmeses_of(int periodo, fecha *pfecha_operacion, fecha *pfecha_inicio);
int fdiaria_of(int periodo, fecha *pfecha_operacion, fecha *pfecha_inicio);
int fdiferencia_en_dias_of(fecha *pfecha_operacion, fecha *pfecha_inicio);


int fcaldisp_of(int periodo,fecha *pfecha_operacion,fecha *pfecha_inicio)
{

  int retorno=0;
  fecha fecha_auxiliar;

/*control de fechas erroneas*/
 if (pfecha_inicio->ss == 0)
  pfecha_inicio->ss = 19;
 if (pfecha_inicio->mm == 0)
  pfecha_inicio->mm = 01;
 if (pfecha_inicio->dd == 0)
  pfecha_inicio->dd = 01;
/*fin control de fechas erroneas*/


 if (fcompara_fechas_of(pfecha_operacion, pfecha_inicio) == 2)
 {
    fprintLog("FECHA DE INICIO MAYOR QUE OPERACION\n");
  return (2);  /*** error ***/
 }
 else
 {
  if (periodo) /** periodo distinto de cero **/
  {
   fprintLog("DENTRO DEL PERIODO \n");
   fecha_auxiliar.ss = pfecha_inicio->ss;
   fecha_auxiliar.aa = pfecha_inicio->aa;
   fecha_auxiliar.mm = pfecha_inicio->mm;
   fecha_auxiliar.dd = pfecha_inicio->dd;
   retorno=finicializar_periodo_of(periodo,pfecha_operacion,pfecha_inicio);
   if (!retorno)
   {
    pfecha_inicio->ss = fecha_auxiliar.ss;
    pfecha_inicio->aa = fecha_auxiliar.aa;
    pfecha_inicio->mm = fecha_auxiliar.mm;
    pfecha_inicio->dd = fecha_auxiliar.dd;
    return (0); /*** no hay periodo y no inicializa ***/
   }
   else
   {
#ifdef TRAZA
 fprintLog("FECHA  INI>%02d%02d%02d%02d< FIN>%02d%02d%02d%02d<\n",
              fecha_auxiliar.ss, fecha_auxiliar.aa,
              fecha_auxiliar.mm, fecha_auxiliar.dd,
              pfecha_inicio->ss, pfecha_inicio->aa,
              pfecha_inicio->mm, pfecha_inicio->dd);
#endif
    return (retorno);
   }//else de retorno;
  }//fin periodo;
 }//fin comparar fechas;
 return(0);
}

/*****************************************************************/
/*** FUNCION: fcompara_fechas_of                                  ***/
/*** DESCRIPCION: Esta funcion recibe dos fechas del tipo      ***/
/*** struc fecha y las compara                                 ***/
/***  devolviendo 0 si son iguales, 1 si la fecha del primer   ***/
/***  parametro es mayor y 2 si es menor                       ***/
/*****************************************************************/


  int fcompara_fechas_of (fecha *fecha1, fecha *fecha2)

  {
  if (fecha1->ss < fecha2->ss)
     return(2); /* fecha1 menor */
  else
     if (fecha1->ss > fecha2->ss)
        return(1); /* fecha1 mayor */
     else
        if (fecha1->aa < fecha2->aa)
           return(2);  /* fecha1 menor */
        else
          if (fecha1->aa > fecha2->aa)
             return (1); /* fecha1 mayor * */
          else
             if (fecha1->mm < fecha2->mm)
                return (2);
             else
                if (fecha1->mm > fecha2->mm)
                   return(1);
                else
                   if (fecha1->dd < fecha2->dd)
                      return(2);
                   else
                      if (fecha1->dd > fecha2->dd)
                         return(1);
                      else     /***  son iguales  ***/
                         return(0);
  }



/***                               ***/
/***  funcion inicializar periodo  ***/
/***                               ***/


int finicializar_periodo_of(int periodo, fecha *pfecha_operacion,
         fecha *pfecha_inicio)
{
  int retorno = 0;
fecha *pfecha_sistema;

  /* prueba */
   GetLocalTime(&t);

//fprintLog("======GETLOCALTIME %02d%02d/%02d\n",t.wYear,t.wMonth,t.wDay);
     pfecha_sistema->ss = t.wYear/100;
     pfecha_sistema->aa = t.wYear - pfecha_sistema->ss*100;
     pfecha_sistema->mm = t.wMonth;
     pfecha_sistema->dd = t.wDay;

    fprintLog("========SISTEMA=%02d%02d%02d/%02d\n",pfecha_sistema->ss,pfecha_sistema->aa,pfecha_sistema->mm,pfecha_sistema->dd);
   /***********fin prueba **/

 switch (periodo)
 {
  case 80:
  case 81:
      fprintLog("cambio semana \n");

     fprintLog("f_operacion semanal>%.2d%.2d%.2d%.2d<\n", pfecha_operacion->ss, pfecha_operacion->aa, pfecha_operacion->mm, pfecha_operacion->dd);


         if (fcompara_fechas_of(pfecha_operacion, pfecha_sistema) == 1)
         {
            fprintLog("FECHA DE LA OPERACICON ES MAYOR QUE LA DEL SISTEMA \n");
             pfecha_operacion->ss = pfecha_sistema->ss;
             pfecha_operacion->aa = pfecha_sistema->aa;
             pfecha_operacion->mm = pfecha_sistema->mm;
             pfecha_operacion->dd = pfecha_sistema->dd;
          fprintLog("operacion cambiada por la del sistema semanal>%.2d%.2d%.2d%.2d<\n",
              pfecha_operacion->ss, pfecha_operacion->aa, pfecha_operacion->mm, pfecha_operacion->dd);

         }
   retorno = fsemanal_of(periodo, pfecha_operacion,pfecha_inicio);
   break;
  case 82:
   retorno = funoyquince_of(pfecha_operacion,pfecha_inicio);
   break;
  case 83:
  case 84:
  case 85:
  case 86:
  case 87:     /**** Mensual con dia de inicio 6 ***/
      fprintLog("cambio mes \n");
   retorno = fmeses_of(periodo, pfecha_operacion, pfecha_inicio);
   break;
  default:
                          /* periodo diario  */
      fprintLog("cambio de fecha diario \n");

     fprintLog("f_operacion diario>%.2d%.2d%.2d%.2d<\n", pfecha_operacion->ss, pfecha_operacion->aa, pfecha_operacion->mm, pfecha_operacion->dd);


         if (fcompara_fechas_of(pfecha_operacion, pfecha_sistema) == 1)
         {
            fprintLog("FECHA DE LA OPERACICON ES MAYOR QUE LA DEL SISTEMA diario\n");
            return(2);
         }
   retorno = fdiaria_of(periodo,pfecha_operacion,pfecha_inicio);
 }
 return(retorno);
}

/************************************/
/***      periodo diario          ***/
/************************************/


  int fdiaria_of(int periodo, fecha *pfecha_operacion, fecha *pfecha_inicio)
  {

  if (periodo <= (fdiferencia_en_dias_of(pfecha_operacion,pfecha_inicio)))
     {
     pfecha_inicio->ss = pfecha_operacion->ss;
     pfecha_inicio->aa = pfecha_operacion->aa;
     pfecha_inicio->mm = pfecha_operacion->mm;
     pfecha_inicio->dd = pfecha_operacion->dd;
     return (1);  /*** hay que inicializar periodo **/
     }
  else
     return (0); /** no hay que inicializar periodo **/

  }


/***********************************/
/***      periodo semanal        ***/
/***********************************/

int fsemanal_of(int periodo, fecha *pfecha_operacion, fecha *pfecha_inicio)
{
  fecha inicio_periodo;
  fecha lunes_anterior;
  int retorno;

 fponelunes_of(periodo, &inicio_periodo, &lunes_anterior);
 retorno = fcompara_fechas_of(pfecha_inicio, &inicio_periodo);

 if(retorno < 2)   /* pfecha_inicio >= inicio_periodo */
  return(0);
 else
 {
  if(periodo == 80)
  {
   pfecha_inicio->ss = inicio_periodo.ss;
   pfecha_inicio->aa = inicio_periodo.aa;
   pfecha_inicio->mm = inicio_periodo.mm;
   pfecha_inicio->dd = inicio_periodo.dd;
  }
  else
  {
   pfecha_inicio->ss = lunes_anterior.ss;
   pfecha_inicio->aa = lunes_anterior.aa;
   pfecha_inicio->mm = lunes_anterior.mm;
   pfecha_inicio->dd = lunes_anterior.dd;
  }
 }
 return(1);
}

/*************************************/
/***  uno y quince                 ***/
/*************************************/

  int funoyquince_of(fecha *pfecha_operacion, fecha *pfecha_inicio)

  {

  fecha fecha_auxiliar;
  fecha_auxiliar.ss = pfecha_inicio->ss;
  fecha_auxiliar.aa = pfecha_inicio->aa;
  fecha_auxiliar.mm = pfecha_inicio->mm;
  fecha_auxiliar.dd = pfecha_inicio->dd;

  if (pfecha_inicio->dd < 15)
     pfecha_inicio->dd = 1;
  else
     pfecha_inicio->dd = 15;

  if (pfecha_operacion->ss != pfecha_inicio->ss)
     pfecha_inicio->ss = pfecha_operacion->ss;

  if (pfecha_operacion->aa != pfecha_inicio->aa)
     pfecha_inicio->aa = pfecha_operacion->aa;

  if (pfecha_operacion->mm != pfecha_inicio->mm)
     pfecha_inicio->mm = pfecha_operacion->mm;

  if ((pfecha_operacion->dd > 15) && (pfecha_inicio->dd == 1))
     pfecha_inicio->dd = 15;
  else
     if ((pfecha_operacion->dd < 15) && (pfecha_inicio->dd == 15))
        pfecha_inicio->dd = 1;

  if (fcompara_fechas_of(&fecha_auxiliar,pfecha_inicio) == 0)
     {
     pfecha_inicio->ss = fecha_auxiliar.ss;
     pfecha_inicio->aa = fecha_auxiliar.aa;
     pfecha_inicio->mm = fecha_auxiliar.mm;
     pfecha_inicio->dd = fecha_auxiliar.dd;
     return (0);
     }
  else
     return (1);

  }



/************************************/
/***      mensual                 ***/
/************************************/

int fmeses_of(int periodo, fecha *pfecha_operacion, fecha *pfecha_inicio)
{
   unsigned int numero_meses;
   fecha fecha_auxiliar;
   int dia_inicio = 0;

    /*** al ser mensual el dia siempre es el mismo del mes. Depende además del código de periodo ***/
 dia_inicio = 1;
 switch (periodo)
 {
  case 87:
   dia_inicio = 6;
  case 83:
   numero_meses = 1;
   break;
  case 84:
   numero_meses = 3;
   break;
  case 85:
   numero_meses = 6;
   break;
  case 86:
   numero_meses = 12;
   break;
 }

 fponemeses_of(numero_meses, pfecha_inicio, dia_inicio);

 pfecha_inicio->mm += numero_meses;
 if (pfecha_inicio->mm > 12)
 {
  pfecha_inicio->aa++;
  pfecha_inicio->mm -= 12;
  if (pfecha_inicio->aa == 100)
  {
   pfecha_inicio->aa = 0;
   pfecha_inicio->ss++;
  }
 }

 if (fcompara_fechas_of(pfecha_operacion,pfecha_inicio) == 2)  /** (pfecha_operacion < pfecha_inicio) **/
  return (0);
 else
 {
  do
  {
   fecha_auxiliar.ss = pfecha_inicio->ss;
   fecha_auxiliar.aa = pfecha_inicio->aa;
   fecha_auxiliar.mm = pfecha_inicio->mm;
   fecha_auxiliar.dd = pfecha_inicio->dd;
   pfecha_inicio->mm += numero_meses;
   if (pfecha_inicio->mm > 12)
   {
    pfecha_inicio->aa++;
    pfecha_inicio->mm -= 12;
    if (pfecha_inicio->aa == 100)
    {
     pfecha_inicio->aa = 0;
     pfecha_inicio->ss++;
    }
   }
  } while(fcompara_fechas_of(pfecha_operacion,pfecha_inicio) == 1);

  pfecha_inicio->ss = fecha_auxiliar.ss;
  pfecha_inicio->aa = fecha_auxiliar.aa;
  pfecha_inicio->mm = fecha_auxiliar.mm;
  pfecha_inicio->dd = fecha_auxiliar.dd;
  return (1);
 }
}


/***********************************/
/***      diferencia en dias     ***/
/***********************************/


  int fdiferencia_en_dias_of(fecha *pfecha_operacion, fecha *pfecha_inicio)

  {
   unsigned int diasanio=0, diferencia;
  int bisiesto_ini = 0;
  juliana juliana_operacion,juliana_inicio;

  fjuliana(pfecha_operacion,&juliana_operacion);
  bisiesto_ini = fjuliana_of(pfecha_inicio, &juliana_inicio);

  if (juliana_operacion.anio == juliana_inicio.anio)
     diferencia = juliana_operacion.dia - juliana_inicio.dia;
  else
     {
      while (juliana_operacion.anio != juliana_inicio.anio)
          {
          if (bisiesto_ini == 2)
             diasanio = + 366;
          else
             diasanio = + 365;
          juliana_inicio.anio++;
          }
      diferencia = (diasanio - juliana_inicio.dia) + juliana_operacion.dia;
     }
  return(diferencia);
  }



/*****************************************************************/
/*** FUNCION: fgregoriana                                      ***/
/*** DESCRIPCION: Esta funcion recibe una fecha del tipo       ***/
/*** struc juliana y  devuelve un puntero a una structura      ***/
/***  gregoriana y un entero indicando si hay error            ***/
/***   0 - operacion correcta                                  ***/
/***   1 - fecha invalida                                      ***/
/***   2 - bisiesto                                            ***/
/*****************************************************************/

int fgregoriana_of(juliana fechaj,fecha *fechag)

  {

  int resto,sumagj,x;

  int dias[]={31,28,31,30,31,30,31,31,30,31,30,31};

  if ((fechaj.anio % 4 == 0) && (fechaj.anio % 100 != 0) ||
      (fechaj.anio % 400 == 0))
      {
      resto = 0;
      dias[1] = 29;
      }
  else
      resto = 1;

  if ((fechaj.dia > 366) ||
     (resto && fechaj.dia > 365))
     return(1);

  if ((fechaj.anio >= 1900) && (fechaj.anio <= 1999))
     {
     fechag->ss = 19;
     fechag->aa = fechaj.anio - 1900;
     }
  else
     {
     fechag->ss = 20;
     fechag->aa = fechaj.anio - 2000;
     }

  sumagj= 0;

  for (x=0;sumagj < fechaj.dia;x++)
      {
      sumagj += dias[x];
      }

  fechag->mm = x;
  sumagj     -= dias[x-1];
  fechaj.dia -= sumagj;
  fechag->dd = fechaj.dia;
  if(!resto)
   return(2);
  return(0);
  }



/*****************************************************************/
/*** FUNCION: fjuliana                                         ***/
/*** DESCRIPCION: Esta funcion recibe una fecha del tipo       ***/
/*** struc fecha y  devuelve un puntero a una structura        ***/
/***  juliana y un entero indicando si hay error o es          ***/
/***  bisiesto.                                                ***/
/***       Los posibles retornos son :                         ***/
/***            0 - operacion  correcta                        ***/
/***            1 - mes invalido                               ***/
/***            2 - operacion correcta y a$o bisiesto          ***/
/***            3 - dia del mes invalido                       ***/
/*****************************************************************/

  int fjuliana_of(fecha *fechag,juliana *fechaj)

  {

  int resto,sumagj;
  int total;
  int dias[]={0,31,59,90,120,151,181,212,243,273,304,334};

  total=fechag->aa+100*fechag->ss;
  if ((total % 4 == 0) && (total % 100 != 0) || (total % 400 == 0))
     resto = 0;
  else
     resto = 1;

  if ((fechag->mm > 12) || (fechag->mm < 1))
     return(1);

  switch (fechag->mm)
    {
    case 4:
    case 6:
    case 9:
    case 11:
            if (fechag->dd > 30)
               return(3);
            break;
    case 2:
               if (fechag->dd > 29)
                  return(3);
               if (resto && fechag->dd > 28)
                  return(3);
    default:
            if (fechag->dd > 31)
               return(3);
    }

  fechaj->anio = total;
  fechaj->dia  = fechag->dd;
  sumagj       = dias [fechag->mm - 1];

  if ((resto == 0) && (fechag->mm > 2))
     fechaj->dia  += (1 + sumagj);
  else
     fechaj->dia += sumagj;

  if (resto == 0)
     return(2);
  else
     return(0);

  }



/********************************************************************/
/***  FUNCION : fponelunes                                        ***/
/***  DESCRIPCION : Esta funcion pone el lunes anterior a la      ***/
/***  fecha de la operacion para periodos semanales               ***/
/***  recibe un puntero a una structura gregoriana y un entero    ***/
/***  que indica si es una semana o dos el periodo (80 u 81)      ***/
/***  y devuelve un puntero a una structura gregoriana (aaaammdd) ***/
/***  los dias de la semana que devuelve el sistema son:          ***/
/***  0 - domingo, 1 - lunes, 2 - martes, 3 - miercoles,          ***/
/***  4 - jueves, 5 - viernes, 6 - sabado                         ***/
/********************************************************************/

void  fponelunes_of(int clave, fecha * pfecha_inicio, fecha * plunes_ant)
{
  time_t          clock;
  struct tm      *tm;
  juliana juliana_inicio;

 (void) time (&clock);
 tm = localtime (&clock);

 juliana_inicio.anio = tm->tm_year + 1900;
 juliana_inicio.dia = tm->tm_yday+1;

 if(!tm->tm_wday)
  tm->tm_wday = 6;
 else
  tm->tm_wday--;   /* dias que nos separan del lunes anterior */

 fresta_dias_of(&juliana_inicio, tm->tm_wday);
 if(clave == 81)
 {
  fgregoriana_of(juliana_inicio, plunes_ant);
  fresta_dias_of(&juliana_inicio, 7);
 }

 fgregoriana_of(juliana_inicio, pfecha_inicio);
}


/*****************************************************************/
/***  FUNCION : fponemeses_of                                  ***/
/***  DESCRIPCION : Esta funcion inicializa fechas al          ***/
/***  principio de los periodos mensuales.                     ***/
/***  recibe un entero con los meses de la clave del periodo y ***/
/***  un puntero a una structura gregoriana y devuelve un      ***/
/***  puntero a una fecha gregoria con la fecha de inicio del  ***/
/***  periodo                                                  ***/
/*****************************************************************/

void fponemeses_of(int meses,fecha *fechag, int dia_inicio)
{

 fechag->dd = dia_inicio;

 switch(meses)
 {
  case 1:
   break;
  case 3:
   switch (fechag->mm)
   {
    case 1:
    case 2:
    case 3:
     fechag->mm = 1;
     break;
    case 4:
    case 5:
    case 6:
     fechag->mm = 4;
     break;
    case 7:
    case 8:
    case 9:
     fechag->mm = 7;
     break;
    default:
     fechag->mm = 10;
   }
   break;
  case 6:
   switch (fechag->mm)
   {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
     fechag->mm = 1;
     break;
    default:
     fechag->mm = 7;
   }
   break;
  case 12:
   fechag->mm = 1;  
 }
}

void fresta_dias_of(juliana * fechaj, int dias)
{
  int pendientes;
  int dias_ano;

 for(pendientes = dias; pendientes;)
 {
  if(fechaj->dia > pendientes)
  {
   fechaj->dia -= pendientes; 
   pendientes = 0;
  }
  else
  {
   fechaj->anio--;
   if(!(fechaj->anio%4) && (fechaj->anio%100 || !(fechaj->anio%400)))
    dias_ano = 366;
   else
    dias_ano = 365;
   pendientes -= fechaj->dia;
   fechaj->dia = dias_ano;
  }
 }
}
