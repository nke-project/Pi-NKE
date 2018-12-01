/*
 * Objetivo:   Testar a chamada ligaled e desligaled
 * Descrição:  Pisca um led se a chamada ligaled e desligaled estiverem funcionando
 * Estado atual: ---
 */

#include "../Kernel/kernel.h"

sem_t s0;

int t1, t2, t3, t4;
/* vazio */
void task1()
{
  int i, k,c=0 ;
 while(1){
//getmynumber(&k);    
//ligaled(16); 
	
 //}

//for(i=0;i<500000;i++);
    //desligaled(16);   
    getmynumber(&k);
    setmyname("_Task1_");
    //sempost(&s0);
    //for(i=0;i<500000;i++);
 }     
  taskexit();
}
/* vazio */
void task2()
{
  int i, k;
   while(1) {
    //desligaled(16); 
	
 //}
   getmynumber(&k);
    setmyname("_Task2_");
    //sempost(&s0);
   //desligaled(16);
   // for(i=0;i<500000;i++);
  }  
  taskexit();
}
/* Pisca led */
void task3()
{
  int i,k; 
 while(1){
    ligaled(16); 
//getmynumber(&k);
    for(i=0;i<500000;i++);
 }

    //getmynumber(&k);
    //setmyname("_Task3_");
  //  ligaled(16);
    //RPI_GetGpio()->GPSET0 = (1 << 16);
    //for(i=0;i<500000;i++);
    //desligaled(16);
    //RPI_GetGpio()->GPCLR0 = (1 << 16);
    //for(i=0;i<500000;i++);
  
  taskexit();
}
/* Testa funcoes */
void task4()
{
  int i,k;
  while(1) {
    
//getmynumber(&k);
    //setmyname("_Task4_");
    desligaled(16);
    for(i=0;i<500000;i++);  
}  
  taskexit();
}

int main(int argc, char *argv[])
{
  seminit(&s0, 1);
  taskcreate(&t1,task1);
  taskcreate(&t2,task2);
  taskcreate(&t3,task3);
  taskcreate(&t4,task4);
  
  start(RR);
  return 0;
}
