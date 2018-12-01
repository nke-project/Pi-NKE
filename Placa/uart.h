#ifndef __UART_H
#define __UART_H

#include "../Kernel/kernel.h"
#include "../Placa/rpi-gpio.h"
#include "../Placa/rpi-armtimer.h"
#include "../Placa/rpi-systimer.h"
#include "../Placa/rpi-interrupts.h"

#ifdef PRINTK
#include <stdarg.h>
#endif
/* Configuracoes especificas */
#define UART_BAUD 19200//para mudar a velocidade, altere este valor para o desejado. 
#define UART_CLK    3000000   //frequência de base
#define UART0       0x20201000// base addres UART0 
#define UART_DR	0x0	// data register ok
#define UART_RSR	0x4	// receive status register/error clear register
#define UART_FR	0x6	// flag register 0x18 not ok

#define UART_IBRD	0x9 // integer baud rate register
#define UART_FBRD	0xa	// Fractional baud rate register

#define UART_LCRH	0xb	// line control register, no datasheet seria 0x2c só que não .
#define UART_CR	0x30	// control register
#define UART_IFLS	0x34	// FIFO control register
#define UART_IMSC	0x38	// interrupt mask set/clear register
#define UART_MIS	0x40	// Status masked interrupt register
#define UART_ICR	0x11	// interrupts flags
// bits in registers
#define UART_RXI	(1 << 4)	// receive interrupt
#define UART_TXI	(1 << 5)	// transmit interrupt

void U0init(void); //inicia a aplicação com o terminal (USB)
int U0getchar(void); //envia um comando do PC para placa
void U0putchar(int c); //imprimir um caracter
void U0puts(char *txt); // imprime uma string
void sys_nkprint(char *fmt,void *number); //envia um conteúdo qualquer pela serial
void printk(char *fmt,...); //envia um conteúdo qualquer pela serial

#endif
