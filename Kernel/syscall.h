#ifndef __sysCALL_H
#define __sysCALL_H

#include "scheduler.h"
#include "kernel.h"

enum sys_temCall{
  TASKCREATE,
  SEM_WAIT,
  SEM_POST,
  SEM_INIT,
  WRITELCDN,
  WRITELCDS,
  EXITTASK,
  SLEEP,
  MSLEEP,
  USLEEP,
  LIGALED,
  DESLIGALED,
  START, 
  TASKJOIN,
  SETMYNAME,
  NKPRINT,
  GETMYNUMBER,
  NKREAD,
};

enum TaskEstados{
  INITIAL,
  READY,
  RUNNING,
  DEAD,
  BLOCKED
};

void DoSystemCall(unsigned int *stack,Parameters *arg);
void sys_taskcreate(int *ID,void (*task)());
void sys_semwait(sem_t *semaphore);
void sys_sempost(sem_t *semaphore);
void sys_seminit(sem_t *semaphore, int value);
void sys_taskexit(void);
void sys_sleep(unsigned int segundo);
void sys_msleep(unsigned int mili);
void sys_usleep(unsigned int micro);
void sys_ligaled();
void sys_desligaled();
void sys_start(int scheduler);
void sys_taskjoin(int id);
void sys_setmyname(const char *name);
void sys_getmynumber(int *number);
void sys_nkread(char *tipo,void *value);

#endif
