#include <stdio.h>

void fhexdump(FILE * pf, void * vaddr, int len)
{
 int ii,nn;
 unsigned char cc;
 char linea[79];
 char * addr;
 int offset;

 addr = vaddr;
 offset = 0;
 nn=0;

 while(nn<len)
 {
  cc= *(addr+nn);
  sprintf(linea, "%7d  >%02X", nn, cc); 
  offset+=12;
  for (ii=1; ii < 16; ii++)
  {
   if (nn+ii < len)
   {
    cc = *(addr+nn+ii);
    sprintf(linea+offset, "_%02X", cc);
   }
   else
    sprintf(linea+offset, "   ");
   offset+=3;
  }
  sprintf(linea+offset, "<");
  offset++;
  sprintf(linea+offset, "  >");
  offset+=3;
  for (ii=0; ii < 16; ii++)
  {
   if (nn+ii < len)
   {
    cc = *(addr+nn+ii);
    if( (cc >=32) && (cc<=126) )
     sprintf(linea+offset, "%c", cc);
    else if(!cc)
     sprintf(linea+offset, "~");
    else
     sprintf(linea+offset, "."); 
   }
   else
    sprintf(linea+offset, " "); 
   offset++;
  }
  *(linea+offset) = '<';

  fprintf(pf, "%.*s\n", 78, linea);  
  
  offset = 0;
  nn += 16;
 }
}

void hexdump(char * addr, int len)
{
 fhexdump(stdout, addr, len);
}
