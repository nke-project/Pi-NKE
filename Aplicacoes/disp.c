/*
 * Objetivo:   Auxilia a debugar o U0putchar
 * Descrição:  Pisca um led e escreve "T" na porta serial
 * Estado atual: pisca o led e escreve lixo nfa serial
 */

#include "../Kernel/kernel.h"
//#include "../Placa/uart.h"

int t1, t2, t3, t4;
volatile int y, x = 0;
/* vazio */
void task1()
{
  int i, k;
  while(1) {
    //getmynumber(&k);
    //setmyname("_Task1_");
    nkprint("\nTask 1? %d", &x);
    }  
  taskexit();
}

/* vazio */
void task2()
{
  int i, k;
  float f = 81.18;
  while(1) {
    getmynumber(&k);
    setmyname("_Task2_");
    x++;
    nkprint("\nTask 2? %d", &x);
  }  
  taskexit();
}

/* Testa fucoes */
void task3()
{
  int k = 312, x = 0;
  while(1) {
    for (y = 0; y < 500000; ++y);
    x++;
    nkprint("\nTo funcionando tambem? %d", &x);
    //printk("Tem alguem ai?\n"); 		// escreve 'T'
    //desligaled(16);		// desliga o led
    //for (i = 0; i < 500000; ++i);
    //x++;
    //printk("...%f...\n", x); // trava no %d
    //printfloat(128, 0);
    
}
  taskexit();
}
/* Testa funcoes */
void task4()
{
  int k;
  float f = 9.253;
  while(1) {
    //getmynumber(&k);
    //setmyname("_Task4_");
    x++;
    nkprint("\nTask4 (int)? %d", &x);
    //nkprint("\nTask4 (float)? %f", &f);
  }  
  taskexit();
}

int main(int argc, char *argv[])
{
  taskcreate(&t1,task1);
  //taskcreate(&t2,task2);
  //taskcreate(&t3,task3);
  taskcreate(&t4,task4);
  start(RR);
  return 0;
}
