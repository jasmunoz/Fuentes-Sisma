/*
 Recibe un puntero a un registro de log y genera una correctora a la 
 operacion 
*/
int flog_a_anu00(NPSLOGO * reg_log, formato_interno * pforitn)
{
  int ll =0;

 memcpy(pforitn, &reg_log->loforitn, sizeof(reg_log->loforitn));
 strcpy(pforitn->fiisocom.fiiso024,"400");
 strcpy(pforitn->fidatiso.fiisoaut.fiaut025,"4007");  // motivo de la correctora: temporizador

 strcpy(pforitn->fidatpro.fiorigen,"00");
 strcpy(pforitn->fidatpro.ficodfin,"   ");
 strcpy(pforitn->fiisocom.respusal,"912");

 memset(pforitn->fidatiso.fiisoaut.fiaut056.datos, ' ',
       sizeof(pforitn->fidatiso.fiisoaut.fiaut056.datos));

 ll = atoi(pforitn->fiisocom.fiiso000);
 switch (ll)                                                            
 {                                                                           
  case 1100:                                                               
  case 1110:                                                               
   strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos,"1100",4);
   break;
  case 1120:
  case 1121:
  case 1130:
   strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos, "1120", 4);
   break;
  case 1240:
  case 1241:
  case 1250:
   strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos, "1240", 4);
   break;
  case 1200:
  case 1210:
   if(!strncmp(reg_log->loforitn.fidatpro.firesfin,"00",2))
    strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos,"1220",4);
   else
    strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos,"1200",4);
   break;
  case 1220:
  case 1221:
  case 1230:
   strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos, "1220", 4);
   break;
  case 1420:
  case 1421:
  case 1430:
   strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos, "1420", 4);
   break;
  case 1440:
  case 1441:
  case 1450:
   strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos, "1440", 4);
   break;
  default:
   strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos,
            pforitn->fiisocom.fiiso000, 4);
 }

 strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+4,
          pforitn->fiisocom.fiidtran,6);

 strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+10,
          pforitn->fiisocom.fifechor.aa, 2);
 strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+12,
          pforitn->fiisocom.fifechor.mm, 2);
 strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+14,
          pforitn->fiisocom.fifechor.dd, 2);
 strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+16,
          pforitn->fiisocom.fifechor.hh, 2);
 strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+18,
          pforitn->fiisocom.fifechor.mi ,2);
 strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+20,
          pforitn->fiisocom.fifechor.ss ,2);
 strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+22,
          pforitn->fiisocom.fi032094.longitud,2);

 strncpy(pforitn->fidatiso.fiisoaut.fiaut056.datos+24,
    pforitn->fiisocom.fi032094.datos,11);
 pforitn->fidatiso.fiisoaut.fiaut056.datos[35]='\0'; 

 for (ll=0;ll<11;ll++)
  if (pforitn->fidatiso.fiisoaut.fiaut056.datos[ 24 + ll] == ' ') 
   break; 
 sprintf(pforitn->fidatiso.fiisoaut.fiaut056.longitud,"%02d",24 + ll);  

 strcpy(pforitn->fiisocom.fiiso000, "1420");  

 return(0);
}
