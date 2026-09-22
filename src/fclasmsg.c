
/*************************************************************************
   FUNCION  FCLASE_TIPOMSG      llamada por precep20

   DESCRIPCION: Funcion que analiza el tipo de mensaje recibido de PRICE

   AREA DE PASO :
- entero con el valor del tipo de mensaje  

* devuelve el tipo de condicion, siendo los valores posibles :
             DESCONOCIDO
             PETICION_NO_CONTROL
             PETICION_CONTROL         (estos valores estan
             PETICION_CONTROL_SIO
             RESPUESTAS_CONTROL_SIO
             RESPUESTAS_NUCLEO        definidos en  fclasmsg.h    )
             RESPUESTAS_OTROS
*************************************************************************/

int fclase_tipomsg(int tipomsg)
{
  int retorno;

  fprintf(stdout,"========== fclase_tipomsg : %d\n",tipomsg);

 retorno = 0;
 
 //DMND0036176 añadidos los mensajes de preautorizacion
 switch(tipomsg)
 {
  case   1100:
  case   1200:
  case   1304:
  case   1305:
  case   1604:
   retorno = PETICION_NO_CONTROL;
   break;
  case   1120:
  case   1121:
  case   1124:
  case   1125:
  case   1140:
  case   1141:
  case   1144:
  case   1145:
  case   1220:
  case   1221:
  case   1224:
  case   1225:
  case   1240:
  case   1241:
  case   1244:
  case   1245:
  case   1324:
  case   1325:
  case   1420:
  case   1421:
  case   1424:
  case   1425:
  case   1440:
  case   1441:
  case   1442:
  case   1443:
  case   1444:
  case   1445:
  case   1520:
  case   1521:
   retorno = PETICION_NO_CONTROL;
   break;
  case   1804:
  case   1805: //PRJ0258542
  case   1824:
  case   1825: //PRJ0258542
   retorno = PETICION_CONTROL;
   break;
  case   1110:
  case   1210:
  case   1314:
  case   1614:
  case   1814:
  case   1834:
   retorno = RESPUESTAS_NUCLEO;
   break;
  case   1130:
  case   1134:
  case   1150:
  case   1154:
  case   1230:
  case   1250:
  case   1334:
  case   1430:
  case   1434:
  case   1450:
  case   1452:
  case   1454:
  case   1530:
   retorno = RESPUESTAS_NUCLEO;
   break;
  case   1524:
  case   1525:
  case   1544:
   retorno = PETICION_CIERRE;
   break;
  case   1534:
   retorno = RESPUESTA_CIERRE;
   break;
  case   1644:
   retorno = ADMINISTRATIVO;
   break;
   
/***
      retorno = RESPUESTAS_OTROS;
      break;
***/
  default:
   retorno = DESCONOCIDO;   
   break; 
 };          /*    fin del switch   */

 return(retorno); 
}

