/********/

#include "cnetplus.h"
#include "cryptdef.h"

/* prototipos */
int fdesepin(int longi, char *sempaq, char *sdesem);
int fempapin(int longi, char *sempaq, char *sdesem);
int lookup (char *label, immense * key, long *pos);
void B_CIFRA(immense clave_s, int length, char *text_plain,
             char *text_cipher, int modo, int *resultado);

int fcalcmac(int tipomsg, int red, int suba_iso,char *info_seg,
               char *texto, int longitud,char *mac_calculado)
{

  char tipo_msg[5];
  char *texto_encriptado;
  char p_desemp_16[16];
  char p_desemp_06[6];
  int nn, ll;

#if 0
#ifdef MAC_PRUEBAS
  char cma011[]="CA01MAC0100PRU";
  char cma012[]="CA01MAC0200PRU";
  char cma021[]="CA02MAC0100PRU";
  char cma022[]="CA02MAC0200PRU";
  char cma031[]="CA03MAC0100PRU";
  char cma032[]="CA03MAC0200PRU";
#else
  char cma011[]="CA01MAC0100000";
  char cma012[]="CA01MAC0200000";
  char cma021[]="CA02MAC0100000";
  char cma022[]="CA02MAC0200000";
  char cma031[]="CA03MAC0100000";
  char cma032[]="CA03MAC0200000";
#endif
#endif

  char cm200101[]="CMPRICEADQ0100";
  char cm200102[]="CMPRICEADQ0200";
  char cm200201[]="CMPRICEEMI0100";
  char cm200202[]="CMPRICEEMI0200";
  char cm200301[]="CMPRICEENV0100";
  char cm200302[]="CMPRICEENV0200";
  char cm200401[]="CMPRICEREC0100";
  char cm200402[]="CMPRICEREC0200";

  char CM300001[]="CMSIO00ADQ0100";
  char CM300002[]="CMSIO00ADQ0200";

  int modo = CBC;
  int longcifr;
  int resultado;
  long resultadol;
  char *clave;
  immense clave_s;
  char * p_infoseg;
  
  //RAQUEL ahora mismo no se calcula el mac solo se comprueba

 fprintLog("====================fcalcmac \n");
 switch(red)
 {
  case 20:
   p_infoseg = info_seg + 9;
   break;
  case 30:
  case 31:
   if(!strncmp(info_seg, "00", 2))
    return(77);
   p_infoseg = info_seg;
   break;
  default:
   fprintLog("SIN MAC red >%02d<\n", red);
   return(55);
 }

 memset(mac_calculado, '\0', 8);
 sprintf(tipo_msg, "%d", tipomsg);

 switch(suba_iso)
 {
  case 0:
  case 2:
   if (!strncmp(p_infoseg, "02" , 2))
    clave=CM300002;
   else
    clave=CM300001;
   break;
  case 1:
   if (!strncmp(p_infoseg, "02" , 2))
    clave=cm200102;
   else
    clave=cm200101;
   break;
  case 3:
   if (!strncmp(p_infoseg, "02" , 2))
    clave=cm200302;
   else
    clave=cm200301;
   break;
  case 4:
   if (!strncmp(p_infoseg, "02" , 2))
    clave=cm200402;
   else
    clave=cm200401;
   break;
  default:
   fprintLog("SIN MAC suba_iso >%02d<\n", suba_iso);
   return(55);
 }

 fprintLog("label >>%s<<\n", clave);
 resultado = lookup(clave , &clave_s , &resultadol);
 if (resultado != ERROK)
 {
  fprintLog("error >%d< en fichero DES %ld\n", resultado, resultadol);
  return(35);
 }

 longcifr = longitud%8 ? (longitud/8 +1)*8 : longitud;

 texto_encriptado = malloc(longcifr);
 B_CIFRA(clave_s,longcifr,texto,texto_encriptado,modo,&resultado);
 if (resultado != ERROK)
 {
  fprintLog("error el B_CIFRA %d\n",resultado);
  free(texto_encriptado);
  return(35);
 }

 memcpy(mac_calculado , texto_encriptado + longcifr - 8, 8);

fprintLog("Ultimos 8 bytes:\n");
fhexdumpLog(mac_calculado,8);
 if((red == 30) || (red == 31))/*** MAC para SIO **/
 {
  retorno = fdesepin(8, mac_calculado, p_desemp_16);
  for(nn = 0, ll = 0; nn < 16 && ll < 6; nn++)
  {
   if(isdigit(p_desemp_16[nn]))
   {
    p_desemp_06[ll] = p_desemp_16[nn];
    ll++;
   }
  }
  for(nn = 0; nn < 16 && ll < 6; nn++)
  {
   if(!isdigit(p_desemp_16[nn]))
   {
    p_desemp_06[ll] =  p_desemp_16[nn] - 'A' + '0'; 
    ll++;
   }
  }
  memset(mac_calculado, '\0', 8);
  retorno = fempapin(6, mac_calculado, p_desemp_06);
 }

#ifdef TRAZA 
 fprintLog("texto:\n");  
 fhexdumpLog(texto,longitud);
 fprintLog("texto_encriptado:\n");
 fhexdumpLog(texto_encriptado,longcifr); 
 fprintLog("mac_calculado:\n");
 fhexdumpLog(mac_calculado,8);
#endif
/*
*/

 free(texto_encriptado); 
 return(0);
}
