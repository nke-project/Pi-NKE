/*
 * Objetivo:	 Testar as chamadas nao bloqueantes do sistema
 * Descrição:	 Pisca um led se todas chamadas nao bloqueantes estiverem funcionando
 * Estado atual: Funcionando corretamente
 */

#include "../Kernel/kernel.h"

sem_t s0;

int t1, t2, t3, t4;
/* vazio */
void task1() 
{
  int k;
  while(1) {
    getmynumber(&k);
  }  
  taskexit();
}
/* vazio */
void task2()
{
  int k;
  while(1) {
    getmynumber(&k);
  }  
  taskexit();
}
/* Liga led */
void task3()
{
  int i, k; 
  while(1) {
    getmynumber(&k);
    setmyname("_Task3_");
    seminit(&s0, 1);
    sempost(&s0);
    ligaled(16);
    //RPI_GetGpio()->GPSET0 = (1 << 16);
    for(i=0;i<500000;i++);
  }  
  taskexit();
}
/* desliga led */
void task4()
{
  int k, i;
  while(1) {
    getmynumber(&k);
    setmyname("_Task4_");
    seminit(&s0, 1);
    sempost(&s0);
    desligaled(16);
    //RPI_GetGpio()->GPCLR0 = (1 << 16);
    for(i=0;i<500000;i++);
  }  
  taskexit();
}

int main(int argc, char *argv[])
{
  taskcreate(&t1,task1); // Usada para nao bugar o sistema
  taskcreate(&t2,task2); // Usada para nao bugar o sistema
  taskcreate(&t3,task3);
  taskcreate(&t4,task4);
  start(RR);
  return 0;
}
