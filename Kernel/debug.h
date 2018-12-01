#ifndef __MONITOR_H
#define __MONITOR_H

#include "kernel.h"
#include "../Placa/uart.h"

/* Configuracoes especificas */
#define SYSCLK 19200000

void TTYout(void); //executado a cada vez que ocorre uma interupção do relógio
void TTYstart(void);
void TTYshared(int id);
void TTYplot(void);

#endif
