#include "scheduler.h"
#include <stdint.h>
#include <stdbool.h>

#include "../Placa/rpi-armtimer.h"
#include "../Placa/rpi-base.h"
#include "../Placa/rpi-gpio.h"
#include "../Placa/rpi-interrupts.h"

void InsertReadyList(int id)
{
  switch(SchedulerAlgorithm)
  {
    case RR:
      if(ready_queue.queue[ready_queue.head]!=0) //coloca na fila apenas se o começo é diferente do fim
      {
    	if(ready_queue.tail<MaxNumberTask-1)
    		ready_queue.tail++;
    	else
    		ready_queue.tail=0;
      }
      ready_queue.queue[ready_queue.tail]=id;
      break;
    default:
      break;
  }
}

void WakeUP()
{
  int i=2;
  for(i=2;i<=NumberTaskAdd; i++)
  { 
    //sleep
    if(Descriptors[i].Time>0)
    {
      Descriptors[i].Time--;
      if(Descriptors[i].Time <= 0 && Descriptors[i].State == BLOCKED)
      {
        Descriptors[i].State = READY;
        InsertReadyList(i);
      }
    }
  }
}

void Dispatcher(void) //responsavel por realizar o disparo/restauração da tarefa
{
  int i; // declarado para teste
  switch(Descriptors[TaskRunning].State)
  {
    case DEAD:
    case BLOCKED://RPI_GetGpio()->GPSET0 = (1 << 16); // acendeu
    case READY: //RPI_GetGpio()->GPSET0 = (1 << 16); // acendeu
      TaskRunning=ready_queue.queue[ready_queue.head];
      ready_queue.queue[ready_queue.head]=0;
      if(ready_queue.head!=ready_queue.tail)
      {
	    //RPI_GetGpio()->GPSET0 = (1 << 16);
    	    //for(i=0;i<500000;i++); //chegou aqui
	    if(ready_queue.head<MaxNumberTask-1)
	    	ready_queue.head++;
	    else
	    	ready_queue.head=0;
      }
      if(Descriptors[TaskRunning].State!=READY) {
	    //RPI_GetGpio()->GPCLR0 = (1 << 16);
    	    //for(i=0;i<500000;i++); //chegou aqui
	      Dispatcher();
      } else {

#ifdef DEBUG_SYSTEM
	    TTYout();
#endif
#ifdef DEBUG_PLOT
	    TTYplot();
#endif
#ifdef SHARED_NUMBER
	    TTYshared(0);
#endif           
	      RPI_GetArmTimer()->IRQClear = 1;
		    Descriptors[TaskRunning].State=RUNNING;
	      RestoreContext(Descriptors[TaskRunning].SP);
      }
      break;
    case INITIAL:
#ifdef DEBUG_SYSTEM
      TTYout(); 
#endif
#ifdef DEBUG_PLOT
	    TTYplot();
#endif
#ifdef SHARED_NUMBER
	    TTYshared(0);
#endif
MoveToSP(&KernelStack[299]);
	    RPI_GetArmTimer()->IRQClear = 1;
      Descriptors[TaskRunning].State=RUNNING;
      GoTask(Descriptors[TaskRunning].SP,Descriptors[TaskRunning].EP);
      break;
    default:
#ifdef DEBUG_SYSTEM
      TTYout();
#endif
#ifdef DEBUG_PLOT
	    TTYplot();
#endif
#ifdef SHARED_NUMBER
	    TTYshared(0);
#endif

//      RPI_GetGpio()->GPCLR0 = (1 << 16);
      RPI_GetArmTimer()->IRQClear = 1; 
      Descriptors[TaskRunning].State=RUNNING;   
      RestoreContext(Descriptors[TaskRunning].SP);
      break;
  }

}

void Select(unsigned int *pilha)
{
  Descriptors[TaskRunning].SP=pilha; //salva a pilha do contexto da task
  MoveToSP(&KernelStack[289]);
  //WakeUP();
  //printk("\npasso do wakeuUP");
  U0getchar();
  switch (SchedulerAlgorithm)
  {
    case RR://  RPI_GetGpio()->GPSET0 = (1 << 16);  //acendeu
	    if(TaskRunning!=1)
	      Descriptors[TaskRunning].State=READY;
      if(TaskRunning>1)
    	  InsertReadyList(TaskRunning);
      break;
    default:  
      break;
  }
  Dispatcher();  
}
