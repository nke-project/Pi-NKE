#include <string.h>
#include "../Kernel/kernel.h"
int t[MaxNumberTask-1];

void teste1()
{
  int id,i;
  getmynumber(&id);
  while(1){
     getmynumber(&id);
  
    //RPI_GetGpio()->GPCLR0 = (1 << 16);
    for(i=0;i<500000;i++);
    } 
   taskexit();
}

void teste2()
{
  int id,i;
//getmynumber(&id);
//int e;
  while(1){
    getmynumber(&id);
  
    //RPI_GetGpio()->GPSET0 = (1 << 16);
    for(i=0;i<500000;i++);
    }
  taskexit();
}

void teste3()
{ 
  char g;
  int id,i;
  //getmynumber(&id);
  while(1){    
  //RPI_GetGpio()->GPCLR0 = (1 << 16); 
    nkread("%c", &g);
    if (g == 'l') {
	    RPI_GetGpio()->GPSET0 = (1 << 16);
    } else if(g == 'd') {
    	RPI_GetGpio()->GPCLR0 = (1 << 16);
    } 
   nkprint("A task recebeu:'%c'",&g);
    }
  taskexit();
}

int main (int argc, char *argv[])
{
  	int i;
	i=1;
	taskcreate(&t[i],teste1); i++;
	taskcreate(&t[i],teste2); i++;
	taskcreate(&t[i],teste3); i++;
  printk("testeeeeeeeee\n", 0);	
	// nkprint("Task %d criada [caracter]... \n",&i);	
	//taskcreate(&t[i],teste4); i++;
	// nkprint("Task %d criada [string]... \n",&i);
	start(RR);
	return 0;
}
