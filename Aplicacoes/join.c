/*
 * Objetivo:   Testar a chamada join
 * Descrição:  Pisca um led se a chamada join estiver funcionando
 * Estado atual: ---
 */

#include "../Kernel/kernel.h"

sem_t s0;

int t1, t2, t3, t4, id1;
/* Liga led */
void task1()
{
  int i;
  ligaled(16);
  taskjoin(t2);
  while(1) {
    /*_/TESTANDO CHAMADAS TASK(JOIN/EXIT)__*/
    desligaled(16);            //desiga led
    for(i=0;i<500000;i++);
    ligaled(16);              //liga led
    for(i=0;i<500000;i++);
  }
  taskexit();
}
/* Desliga led */
void task2()
{
  int i, k; 
  getmynumber(&k);
  setmyname("_Task2_");
  taskexit();
}
/* Nada de mais */
void task3()
{
  int i, k3; 
  while(1) {
  	getmynumber(&k3);
  	setmyname("_Task3_");
  }  
  taskexit();
}
/* Nada de mais */
void task4()
{
  int k4;
  while(1) {
    getmynumber(&k4);
    setmyname("_Task4_");
  }  
  taskexit();
}

int main(int argc, char *argv[])
{
  taskcreate(&t3,task3);
  taskcreate(&t4,task4);
  
  taskcreate(&t1,task1);
  taskcreate(&t2,task2);
  //taskcreate(&t3,task3);
  //taskcreate(&t4,task4);
  start(RR);
  return 0;
}
