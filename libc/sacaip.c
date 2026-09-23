#include <winsock2.h>

void fprintLog(char ficheroPlantilla[50], char* format, ...);
extern char nomProg[50];

int saca_ip_local(char *ip)
{
   WSADATA wsaData;
   char *auxiliar=NULL;
   struct hostent *host=NULL;
   struct in_addr direccion;
   int ok=0;
   char ipaux[16];
   int i;

   auxiliar=malloc(25);

   if (!WSAStartup(MAKEWORD(2, 2), &wsaData))
   {
        if (gethostname(auxiliar, 25) != SOCKET_ERROR)
        {
            host = gethostbyname(auxiliar);
            if (host != NULL)
            {
                memcpy(&direccion, host->h_addr_list[0], sizeof(struct in_addr));
                strcpy(ipaux, inet_ntoa(direccion));
                memset(ip,' ',16);
                strncpy(ip,ipaux,strlen(ipaux)); 
                ip[15]='\0';
                fprintLog(nomProg, "IP >%s<\n",ip); 
                ok=1;
            }
        }
        WSACleanup(); 
    }
    free(auxiliar);
    return ok;
}