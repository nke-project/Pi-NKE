#include "initkernel.h"
#include <string.h>
#include <stdlib.h>

#include "../Placa/rpi-gpio.h"
#include "../Placa/rpi-armtimer.h"
#include "../Placa/rpi-systimer.h"
#include "../Placa/rpi-interrupts.h"

void StartKernel(void)
{
  U0init();
  ini_serial_fila();
  MoveToSP(&Descriptors[1].Stack[SizeTaskStack-10]);
  StartReadyList();
  CreateIdleAndMain();
  /* Write 1 to the GPIO16 init nibble in the Function Select 1 GPIO
     peripheral register to enable GPIO16 as an output */
  RPI_GetGpio()->GPFSEL1 |= (1 << 18);
  
  /* Enable the timer interrupt IRQ */
  RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;

  /* Setup the system timer interrupt */
  /* Timer frequency = Clk/256 * 0x400 */
  RPI_GetArmTimer()->Load = 0x400;

  /* Setup the ARM Timer */
  RPI_GetArmTimer()->Control =
          RPI_ARMTIMER_CTRL_23BIT |
          RPI_ARMTIMER_CTRL_ENABLE |
          RPI_ARMTIMER_CTRL_INT_ENABLE |
          RPI_ARMTIMER_CTRL_PRESCALE_256;
  _enable_interrupts();
//  main(0,0);

  while(1);
}

void CreateIdleAndMain(void)
{
  static int TidMain=1, TidIdle=0;
  /*Criaçao da Idle Task */
  Descriptors[0].Tid=TidIdle;
  Descriptors[0].EP=idletask;
  Descriptors[0].State=INITIAL;
  Descriptors[0].Join=0;
  Descriptors[0].SP=&Descriptors[0].Stack[SizeTaskStack-10];
  Descriptors[0].Prio=0;
  /*Criaçao da Main */
  Descriptors[1].Tid=TidMain;
  Descriptors[1].EP=(void(*)())main;
  Descriptors[1].State=INITIAL;
  Descriptors[1].Join=0;
  Descriptors[1].SP=&Descriptors[1].Stack[SizeTaskStack-10];
  Descriptors[1].Prio=0;
  TaskRunning=1;
  NumberTaskAdd=1;
}

void StartReadyList(void)
{
  int i; 
  for(i=0;i<MaxNumberTask-1;i++)
	  ready_queue.queue[i]=0;
  ready_queue.head=0;
  ready_queue.tail=0;
  return;
}

void ini_serial_fila(void)
{ 
   int i;
	for(i=0; i<MaxNumberTask; i++)
	     serial_fila[i] = 0;
}
