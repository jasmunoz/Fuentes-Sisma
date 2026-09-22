//PRJ0257005 - Revisado tokenizacion
/****************************************************************    
 INICFORI.C                                                         
     funcion que inicializa todos los campos del formato interno   
     a los valores correctos, teniendo en cuenta el tipo de mensaje
     que vaya a construirse.                                      
                                                                 
 AREA DE PASO :                                                 
 - entero con el tipo de mensaje a construir                   
                                                              
 - puntero a un campo tipo formato interno, que a la vuelta de la   
   funcion quedara inicializado                                        
********************************************************************/ 
#include "cforitno.h"
#include <stdio.h>
#include <string.h>                                                
int inicfori_comun(formato_interno *pfi)
{                                                                  
 memset(pfi, ' ', sizeof(formato_interno));
/***----------------   datos procesales   --------------------***/
 strcpy(pfi->fidatpro.finumope , "      ");
                                                                
 strcpy(pfi->fidatpro.fifecha0,     "        ");
 strcpy(pfi->fidatpro.fihora00, "        ");
 
 strcpy(pfi->fidatpro.fiorigen , "  ");
 strcpy(pfi->fidatpro.firesfin , "  ");

 strcpy(pfi->fidatpro.firespri , "  ");

 strcpy(pfi->fidatpro.finetpri , "    ");

 strcpy(pfi->fidatpro.ficodfin , "   ");
 strcpy(pfi->fidatpro.ficlatab , "            ");
 strcpy(pfi->fidatpro.fitipred , " ");

 sprintf(pfi->fidatpro.fitienor, "%08d", 0);
 sprintf(pfi->fidatpro.fitisare, "%08d", 0);
 sprintf(pfi->fidatpro.fitienre, "%08d", 0);

 strcpy(pfi->fidatpro.fitipcue , "  ");

 strcpy(pfi->fidatpro.fisubres , "  ");
  strcpy(pfi->fidatpro.fisubori , "  ");
 strncpy(pfi->fidatpro.fiusofut , "                               ", 31);
 strcpy(pfi->fidatpro.fiparaut , "  ");

  strncpy(pfi->fidatpro.ficomisi, "00000000", 8);
 strncpy(pfi->fidatpro.fitipcom , "  ",2);
 strncpy(pfi->fidatpro.ficomadi , "     ",5);

 strcpy(pfi->fidatpro.fipidori,"00000");
 strcpy(pfi->fidatpro.finumprv,"      ");
 pfi->fidatpro.finototal[0] = ' '; 
 strcpy(pfi->fidatpro.fioriini,"  ");
 strcpy(pfi->fidatpro.firesprs,"  ");
 // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la lontitud total del PAN cifrado ni para inicializarlo
 strcpy(pfi->fidatpro.fipancif,"                   ");
 

/***----------------   datos iso comunes   --------------------***/
 strcpy(pfi->fiisocom.fiiso000 , "0000");
 strcpy(pfi->fiisocom.fiidtran , "      ");
                                                                    
 strcpy(pfi->fiisocom.fifechor.aa , "00");
 strcpy(pfi->fiisocom.fifechor.mm , "00");
 strcpy(pfi->fiisocom.fifechor.dd , "00");
 strcpy(pfi->fiisocom.fifechor.hh , "00");
 strcpy(pfi->fiisocom.fifechor.mi , "00");
 strcpy(pfi->fiisocom.fifechor.ss , "00");

 strcpy(pfi->fiisocom.fi032094.longitud , "00");
 strcpy(pfi->fiisocom.fi032094.datos , "           ");

 strcpy(pfi->fiisocom.fiiso024 , "   ");
 strcpy(pfi->fiisocom.respusal , "   ");

}


////////////////////////////////////////////////////////////////////////////////////////////////
int inicfori (int tipomsg, formato_interno *pfi)      
////////////////////////////////////////////////////////////////////////////////////////////////
{                                                                  
  int l_campo;
  int retorno;

 retorno = inicfori_comun(pfi);

/***--------------   datos iso particulares   ---------------***/    

//DMND0036176 AÑADIDOS LOS MENSAJES PREAUTORIZACION
switch(tipomsg)                                                     
{
  case 100:
  case 120:
  case 130:
  case 200:
  case 210:
  case 220:
  case 230:
  case 260:
  case 270:
  case 420:
  case 430:
  case 440:
  case 450:
  case 600:
  case 610:
  case 620:
  case 630:
  case 700:
  case 710:
  case 720:
  case 730:
  case 1100:
  case 1110:
  case 1200:
  case 1210:
  case 1120:
  case 1121:
  case 1130:
  case 1220:
  case 1221:
  case 1230:
  case 1240:
  case 1241:
  case 1250:
  case 1420:
  case 1421:
  case 1430:
  case 1440:
  case 1441:
  case 1450:
  case 1442:
  case 5000:
  case 5010:
  case 5020:
  case 5030:
  case 5300:
  case 5310:
  case 5200:
  case 5210:
/***--------------       autorizaciones       ---------------***/  
 // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la lontitud total del PAN ni para inicializarlo 
   strcpy(pfi->fidatiso.fiisoaut.finumPAN.longitud , "00");           
   strcpy(pfi->fidatiso.fiisoaut.finumPAN.datos ,                    
          "                   ");                                   
   strcpy(pfi->fidatiso.fiisoaut.fiaut003.dig12 , "  ");           
   strcpy(pfi->fidatiso.fiisoaut.fiaut003.dig34 , "  ");           
   strcpy(pfi->fidatiso.fiisoaut.fiaut003.dig56 , "  ");
   strcpy(pfi->fidatiso.fiisoaut.fiaut004, "000000000000");
   strcpy(pfi->fidatiso.fiisoaut.fiaut005, "000000000000");
   strcpy(pfi->fidatiso.fiisoaut.fiaut006, "000000000000");
   strcpy(pfi->fidatiso.fiisoaut.fiaut009 , "        ");           
   strcpy(pfi->fidatiso.fiisoaut.fiaut010 , "        ");           
   strcpy(pfi->fidatiso.fiisoaut.fifeccad.aa , "  ");           
   strcpy(pfi->fidatiso.fiisoaut.fifeccad.mm , "  ");            
   strcpy(pfi->fidatiso.fiisoaut.fiaut018 , "0000");           
   strcpy(pfi->fidatiso.fiisoaut.finumita , "00");              
   strcpy(pfi->fidatiso.fiisoaut.fiaut025 , "0000");        
   strcpy(pfi->fidatiso.fiisoaut.fiaut028.aa, "00");            
   strcpy(pfi->fidatiso.fiisoaut.fiaut028.mm, "00");           
   strcpy(pfi->fidatiso.fiisoaut.fiaut028.dd, "00");          
   strcpy(pfi->fidatiso.fiisoaut.fiaut029 , "   ");           
   strcpy(pfi->fidatiso.fiisoaut.fiaut035.longitud , "00");           
   memset(pfi->fidatiso.fiisoaut.fiaut035.datos , ' ', 37);
   pfi->fidatiso.fiisoaut.fiaut035.datos [37] = '\0';
   strcpy(pfi->fidatiso.fiisoaut.fiaut037 , "000000000000");           
   strcpy(pfi->fidatiso.fiisoaut.fiaut038 , "      ");         
   strcpy(pfi->fidatiso.fiisoaut.fiaut041 , "            ");           
   strcpy(pfi->fidatiso.fiisoaut.fiaut042 , "                ");           
   memset(pfi->fidatiso.fiisoaut.fiaut043, ' ', 40);
   pfi->fidatiso.fiisoaut.fiaut043 [40] = '\0';
   l_campo = sizeof(pfi->fidatiso.fiisoaut.fiaut048);
   memset(&pfi->fidatiso.fiisoaut.fiaut048, ' ', l_campo);
   *((char *)&pfi->fidatiso.fiisoaut.fiaut048 + l_campo - 1) = '\0';
   strcpy(pfi->fidatiso.fiisoaut.fiaut049 , "0000");           
   strcpy(pfi->fidatiso.fiisoaut.fiaut051 , "   ");          
   strcpy(pfi->fidatiso.fiisoaut.fiaut053 , "                "); //PRJ0258542 - pasa de 16 a 24, pero solo para mensajes 18xx, en estos se mantiene
   strcpy(pfi->fidatiso.fiisoaut.fiaut056.longitud , "00");           
   memset(pfi->fidatiso.fiisoaut.fiaut056.datos, ' ', 35);
   pfi->fidatiso.fiisoaut.fiaut056.datos [35] = '\0';
   strcpy(pfi->fidatiso.fiisoaut.fiaut064 , "        ");          
   strcpy(pfi->fidatiso.fiisoaut.fiaut062.bit62_16, "                              "); //PRJ0257005 - Inicializo nuevo campo
   strncpy(pfi->fidatiso.fiisoaut.fiaut102.longitud,"00",2); ////// TRANSF         
   memset(pfi->fidatiso.fiisoaut.fiaut102.datos ,' ',28); ////// TRANSF         
   strncpy(pfi->fidatiso.fiisoaut.fiaut103.longitud,"00",2); ////// TRANSF      
   memset(pfi->fidatiso.fiisoaut.fiaut103.datos ,' ',28); ////// TRANSF            
   strcpy(pfi->fidatiso.fiisoaut.fiaut128 , "        "); ////// TRANSF         
   break;                                                            
  case 304:                                                          
  case 314:
  case 1304:                                                          
  case 1314:                                                         
  case 1324:                                                        
  case 1325:                                                       
  case 1334:                                                                                                               
/***--------------          ficheros          ---------------***/      
   strcpy(pfi->fidatiso.fiisofic.fific002.longitud , "00");           
   strcpy(pfi->fidatiso.fiisofic.fific002.datos ,                    
          "                   ");                                   
   strcpy(pfi->fidatiso.fiisofic.fific053 , "                ");   
   strcpy(pfi->fidatiso.fiisofic.fific072.longitud , "000");      
   memset(pfi->fidatiso.fiisofic.fific072.datos, ' ', 256);
   pfi->fidatiso.fiisofic.fific072.datos [256] = '\0';
   strcpy(pfi->fidatiso.fiisofic.fific093.longitud , "00");     
   strncpy(pfi->fidatiso.fiisofic.fific093.datos , "           " , 11); 
   strcpy(pfi->fidatiso.fiisofic.fific101.longitud , "00");            
   strcpy(pfi->fidatiso.fiisofic.fific101.datos , "                 ");                
   strcpy(pfi->fidatiso.fiisofic.fific053 , "                ");     
   break;                                                      
  case 1644:
  case 5100:
/***--------------       administrativos      ---------------***/   
   strcpy(pfi->fidatiso.fiisoaut.fiaut003.dig12 , "  ");           
   strcpy(pfi->fidatiso.fiisoaut.fiaut003.dig34 , "  ");           
   strcpy(pfi->fidatiso.fiisoaut.fiaut003.dig56 , "  ");  
   strcpy(pfi->fidatiso.fiisoadm.fiadm011, "      ");           
   strcpy(pfi->fidatiso.fiisoadm.fiadm012, "            ");           
   strcpy(pfi->fidatiso.fiisoadm.fiadm024, "   ");           
   strcpy(pfi->fidatiso.fiisoadm.fiadm039, "   ");           
    
   strcpy(pfi->fidatiso.fiisoadm.fiadm072.longitud , "000");           
   memset(pfi->fidatiso.fiisoadm.fiadm072.datos, ' ', 256);
   pfi->fidatiso.fiisoadm.fiadm072.datos [256] = '\0';
   strcpy(pfi->fidatiso.fiisoadm.fiadm093.longitud , "00");          
   strncpy(pfi->fidatiso.fiisoadm.fiadm093.datos , "           " , 11); 
   strcpy(pfi->fidatiso.fiisoadm.fiadm094.longitud , "00");          
   strncpy(pfi->fidatiso.fiisoadm.fiadm094.datos , "           " , 11); 
                                                                       
   break;                                                             
  case 1804:
  case 1805: //PRJ0258542
  case 1814:
  case 1824:
  case 1825: //PRJ0258542
  case 1834:
//PRJ0258542 - INI 
/***--------------           control          ---------------***/      
   	strncpy(pfi->fidatiso.fiisoctr.codmcd , "  " , 2);
   	strncpy(pfi->fidatiso.fiisoctr.codopcion , " " ,1);
  	strncpy(pfi->fidatiso.fiisoctr.fictr011, " ", 6);
  	strcpy(pfi->fidatiso.fiisoctr.fictr012.aa, "00");
  	strcpy(pfi->fidatiso.fiisoctr.fictr012.mm, "00");
  	strcpy(pfi->fidatiso.fiisoctr.fictr012.dd, "00");
  	strcpy(pfi->fidatiso.fiisoctr.fictr012.hh, "00");
  	strcpy(pfi->fidatiso.fiisoctr.fictr012.mi, "00");
  	strcpy(pfi->fidatiso.fiisoctr.fictr012.ss, "00");
  	strcpy(pfi->fidatiso.fiisoctr.fictr024, "000");
  	memset(pfi->fidatiso.fiisoctr.fictr048.contr.cardia, ' ', 8);
   	pfi->fidatiso.fiisoctr.fictr048.contr.cardia [8] = '\0';
   	strcpy(pfi->fidatiso.fiisoctr.fictr093.longitud , "00");          
   	strncpy(pfi->fidatiso.fiisoctr.fictr093.datos , "           " , 11);
   	strcpy(pfi->fidatiso.fiisoctr.fictr094.longitud , "00");          
   	strncpy(pfi->fidatiso.fiisoctr.fictr094.datos , "           " , 11);
/***--------------       cambio de claves     ---------------***/      
  	strncpy(pfi->fidatiso.fiisocla.ficla011, " ", 6);
  	strncpy(pfi->fidatiso.fiisocla.ficla012, " ", 12);
  	strcpy(pfi->fidatiso.fiisocla.ficla024, "000");
  	strcpy(pfi->fidatiso.fiisocla.ficla025, "0000");
    strcpy(pfi->fidatiso.fiisocla.ficla033.longitud, "00");
  	strncpy(pfi->fidatiso.fiisocla.ficla033.datos, " ", 11);
  	strcpy(pfi->fidatiso.fiisocla.ficla039, "000");
  	strncpy(pfi->fidatiso.fiisocla.ficla053.control, " ", 2);
    strncpy(pfi->fidatiso.fiisocla.ficla053.identificador, " ", 8);
    strncpy(pfi->fidatiso.fiisocla.ficla053.algoritmo, " ", 2);
    strncpy(pfi->fidatiso.fiisocla.ficla053.longitud, " ", 4);
    strncpy(pfi->fidatiso.fiisocla.ficla053.proteccion, " ", 2);
    strncpy(pfi->fidatiso.fiisocla.ficla053.resto, " ", 6);
    strcpy(pfi->fidatiso.fiisocla.ficla093.longitud, "00");
  	strncpy(pfi->fidatiso.fiisocla.ficla093.datos, " ", 11);
    strcpy(pfi->fidatiso.fiisocla.ficla094.longitud, "00");
  	strncpy(pfi->fidatiso.fiisocla.ficla094.datos, " ", 11);
    strcpy(pfi->fidatiso.fiisocla.ficla096.longitud, "000");
  	strncpy(pfi->fidatiso.fiisocla.ficla096.datos, " ", 360);
  	strcpy(pfi->fidatiso.fiisocla.ficla028, "      ");
  	strcpy(pfi->fidatiso.fiisocla.ficla029, "   ");
//PRJ0258542 - FIN
                     
   	break;                                                            
  case 500:
  case 510:
  case 520:
  case 1500:
  case 1510:
  case 1520:
  case 1521:
  case 1530:
  case 1524:                                                          
  case 1525:                                                         
  case 1534:                                                        
  case 1604:                                                        
  case 1614:                                                        
/***-------------- conciliacion/totales ---------------***/
   strcpy(pfi->fidatiso.fiisotot.monedafac, "    ");            
   strcpy(pfi->fidatiso.fiisotot.monedacon, "    ");            
   strcpy(pfi->fidatiso.fiisotot.fitot028.aa, "00");            
   strcpy(pfi->fidatiso.fiisotot.fitot028.mm, "00");           
   strcpy(pfi->fidatiso.fiisotot.fitot028.dd, "00");          
   strcpy(pfi->fidatiso.fiisotot.fitot029, "   ");          
   strcpy(pfi->fidatiso.fiisotot.fitot074 , "0000000000");          
   strcpy(pfi->fidatiso.fiisotot.fitot075 , "0000000000");          
   strcpy(pfi->fidatiso.fiisotot.fitot076 , "0000000000");          
   strcpy(pfi->fidatiso.fiisotot.fitot077 , "0000000000");          
   strcpy(pfi->fidatiso.fiisotot.fitot086 , "0000000000000000");          
   strcpy(pfi->fidatiso.fiisotot.fitot087 , "0000000000000000");          
   strcpy(pfi->fidatiso.fiisotot.fitot088 , "0000000000000000");          
   strcpy(pfi->fidatiso.fiisotot.fitot089 , "0000000000000000");          
   strcpy(pfi->fidatiso.fiisotot.fitot097 , "C0000000000000000");  
   memset(pfi->fidatiso.fiisotot.fitot4815, ' ',180);
   
   break;                                                               
  default:                                                             
   return (1);                                                      
}             /***      fin del switch de tipomsg       ***/       
                                                                
return (0);                                                    
                                                              
}             /***      fin de la funcion       ***/         
                                                            
