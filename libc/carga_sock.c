#include "cnetplus.h"
#include "svmsock.h"

void limpia_tbsock(tbsock *pp);
int dame_posisvmsock(tbsock *psvmsock);

int carga_socks(tbsock *psvmsock)
{
  int ll,salir=0;

for (ll=0 ; ll<MAX_REG_TBSOCK ; ll++)
   {
   limpia_tbsock(psvmsock+ll);
   }  

return(salir);
}
void limpia_tbsock(tbsock *pp)
{
   memset(pp->nompadre , ' ' , sizeof(pp->nompadre));
   pp->num_ip=VACIO;
   pp->pidpadre=VACIO;
   pp->pidhijo=VACIO;
   pp->pidnieto=VACIO;
   pp->signieto=VACIO;
}

int dame_posisvmsock(tbsock *psvmsock)
{
  int posic;

for (posic=0 ; posic<MAX_REG_TBSOCK ; posic++)
   {
   if (SVMip==VACIO) return(posic); 
   }
return(-1);
}
