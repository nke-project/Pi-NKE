#include"uart.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
 
static volatile uint32_t *uart_base;
 
   char dado[25];   //vetor de 25 caracteres para armazenar os dados chegados da serial
   int pos=0;        //posi��o a ser inserido o caracter no vetor dado acima
   int dot;          //registra a posi��o no vetor acima que est� o separador decimal   
   float V = .0;    //valor real convertido
   int N_Inteiro = 0;// valor inteiro convertido 
   int cv = 0;          // contador de v�rgulas
   unsigned short Nfloat=0;
   
/* Configuracao da Porta Serial 0 */
void U0init(void)
{ //usei uart_clock sendo 19.2MHz para c�lculos do gerador de baud-rate...
  uint left;//para armazenar a parte fracion�ria do divisor de baud-rate.
  uart_base = UART0;
  uart_base[UART_CR] = 0x00; // desabilita uart
  uart_base[UART_LCRH] = 0x60;//flush fifo de transmiss�o
  uart_base[UART_ICR] = 0x7FF; /* Clear all interrupt status */
  uart_base[UART_IBRD] = UART_CLK/(16*UART_BAUD);//parte inteira do gerador de baud-rate   uart_clock / (16xbaud-rate)  
  left = (UART_CLK%(16*UART_BAUD));//parte fracion�ria do divisor de baud-rate.
  uart_base[UART_FBRD] = (left * 4 + UART_BAUD / 2) / UART_BAUD; //peguei em um exemplo, n�o est� no datasheet.
  uart_base[UART_IMSC] = UART_RXI;
  uart_base[UART_IMSC] = UART_TXI;
  uart_base[UART_LCRH] = 0x70; // 8, N, 1 - fifo desabilitado = 0x60, para 8,N,1 - fifo habilitado = 0x70
  uart_base[UART_CR] = 0x301; // habilita uart
 
}
/***********************************************************************************************************************/
float StrToFloat(char valor[25])/* Converte string em dado[25] para float */
{ 
 int i;
 float F1 = 1.0;
 float F2 = 10.0;
     if(valor[0] != '-'){ //se for positivo
    for(i=dot-1; i>=0; i--)
    {
        V+=((float)valor[i]-0x030) * F1;
         F1*=10.0;
    } 
    for(i=dot+1; i<pos; i++)
    {
        V+=((float)valor[i]-0x030) / F2;
         F2*=10.0;
    }
    // printk("....... %.2f ........\n",V);// ok, mas as vezes falha nas �ltimas casas decimais
 return(V);
    }
    else{ // se for negativo
    for(i=dot-1; i>=1; i--)
    {
        V+=((float)valor[i]-0x030) * F1;
         F1*=10.0;
    } 
    for(i=dot+1; i<pos; i++)
    {
        V+=((float)valor[i]-0x030) / F2;
         F2*=10.0;
    }
    V *= -1.0;
    // printk("....... %f ........\n",V);// ok, mas as vezes falha nas �ltimas casas decimais
 return(V);
    }
}
/***********************************************************************************************************************/
int StrToInt(char valor[25])/* Converte string em dado[25] para inteiro 07/03/2015 */
{   
 
 int i,fator;
  fator=1; N_Inteiro=0;
     if(valor[0] != '-'){ //se for positivo
    for(i=pos-1; i>=0; i--)
    { 
        N_Inteiro+=(valor[i]-0x030) * fator;
         fator*=10;
    } 
   //  printk("....... %d ........\n",N_Inteiro);// 
 return(N_Inteiro);
    }
    else{ // se for negativo
    for(i=pos-1; i>=1; i--)
    { 
        N_Inteiro+=(valor[i]-0x030) * fator;
         fator*=10;
    } 
     N_Inteiro *= -1;
    // printk(".......%d ........\n",N_Inteiro);// 
 return(N_Inteiro);
    }
}
 
 /* fun��o para chamar o conversor do dado armazenado no buffer de 25 bytes no seu 
   valor float correspondente e passar para a serial_queue da task e liberar a task. */
void v_real(void){
int i;
if(serial_fila[posicao_a_att]>=0)
        {   
        if(serial_queue[posicao_a_att].real)
         {
         *serial_queue[posicao_a_att].real = StrToFloat(dado);
          Descriptors[serial_queue[posicao_a_att].tid].State=READY;
           InsertReadyList(serial_queue[posicao_a_att].tid);
            serial_fila[posicao_a_att] = 0;
             posicao_a_att++;
                if(posicao_a_att == MaxNumberTask) posicao_a_att = 0;   
           pos=0; //zera posicionador na string dado[25]
          dot=0;//reseta posi��o do separador de casas decimais
           V=0.0;
         }else{// OPA A TASK CORRENTE N�O EST� ESPERANDO UM FLOAT...
            printk("INPUT float ERROR!!  SYSTEM ABORTED!!\n");  
            posicao_a_att = -1;
               for (i=0; i<MaxNumberTask-1; i++)//termina todas as tasks
            {
              Descriptors[i].State=DEAD;
            }           
            }  
         }
             
}
 /* fun��o para copiar a string armazenada no buffer de 25 bytes
    e passar para a serial_queue da task e liberar a task. */
void v_string(void){
int i;
        if(serial_fila[posicao_a_att]>=0)
        { //printk("STRING!\n"); printk("%d\n", Nfloat); 
        if(serial_queue[posicao_a_att].string)
         {   
        strncpy(serial_queue[posicao_a_att].string, dado, pos+1);
         Descriptors[serial_queue[posicao_a_att].tid].State=READY;
              InsertReadyList(serial_queue[posicao_a_att].tid);
               serial_fila[posicao_a_att] = 0;
                posicao_a_att++;
               if(posicao_a_att == MaxNumberTask) posicao_a_att = 0;  
          pos=0; //zera posicionador na string dado[25]
         Nfloat=0;
         }else{// OPA A TASK CORRENTE N�O EST� ESPERANDO UMA STRING...
            printk("INPUT string ERROR!!  SYSTEM ABORTED!!\n"); 
            posicao_a_att = -1;
               for (i=0; i<MaxNumberTask-1; i++)//termina todas as tasks
            {
              Descriptors[i].State=DEAD;
            }   
            }
        }
}
 /* fun��o para chamar o conversor do dado armazenado no buffer de 25 bytes no seu 
   valor inteiro correspondente e passar para a serial_queue da task e liberar a task. */
void v_inteiro(void){
int i;
        if(serial_fila[posicao_a_att]>=0)
        { 
          if(serial_queue[posicao_a_att].inteiro)
           {  
             *serial_queue[posicao_a_att].inteiro = StrToInt(dado);
             Descriptors[serial_queue[posicao_a_att].tid].State=READY;
                 InsertReadyList(serial_queue[posicao_a_att].tid);
                 serial_fila[posicao_a_att] = 0;
                 posicao_a_att++;
                 if(posicao_a_att == MaxNumberTask) 
                posicao_a_att = 0;  
                pos=0; //zera posicionador na string dado[25]
              }else{// OPA A TASK CORRENTE N�O EST� ESPERANDO UM INTEIRO... 
             printk("INPUT int ERROR!!  SYSTEM ABORTED!!\n");   
             posicao_a_att = -1;
                     for (i=0; i<MaxNumberTask-1; i++)//termina todas as tasks
             {
            Descriptors[i].State=DEAD;
             }  
          }
        }
}
 /* fun��o para copiar a caracter armazenada no buffer de 25 bytes
    e passar para a serial_queue da task e liberar a task. */
void v_caracter(void){
    int i;
    if(serial_fila[posicao_a_att]>=0)
    { 
      if(serial_queue[posicao_a_att].caracter)
      {   
        *serial_queue[posicao_a_att].caracter = dado[0];
        Descriptors[serial_queue[posicao_a_att].tid].State=READY;
        InsertReadyList(serial_queue[posicao_a_att].tid);
        serial_fila[posicao_a_att] = 0;
        posicao_a_att++;
        if(posicao_a_att == MaxNumberTask) posicao_a_att = 0;  
        pos=0; //zera posicionador na string dado[25]
        }else{// OPA A TASK CORRENTE N�O EST� ESPERANDO UM INTEIRO... 
        printk("INPUT char ERROR!!  SYSTEM ABORTED!!\n");   
        posicao_a_att = -1;
                for (i=0; i<MaxNumberTask-1; i++)//termina todas as tasks
            {
            Descriptors[i].State=DEAD;
        }   
        }
    }
}
 
/* Fun��o que recebe o dado, byte por byte  e armazena no buffer de 25 bytes,  se for enviado mais de 24
   caracteres sem ser enviado um enter para dizer que o dado est� pronto para ser convertido ou copiado,
   uma menssagem � emitida avisando para pressionar <ENTER>.*/
int U0getchar(void)
{ 
 volatile int i,j,c;
 
  while (uart_base[UART_FR] & UART_RXI) {        
        return -1;  //enquanto n�o chega dado novo, n�o fazer nada.  
    }
        c = uart_base[UART_DR]; //recebe o dado
 
  if(c==',') cv++;
  if(c =='\n'){//se apertou enter converte dado armazenado na vari�vel dado[25]
  dado[pos]='\0';//insere terminador da string
  for(i=0; i<pos; i++)                                                                   //se n�o for n�mero ou uma v�rgula 
    if(!(dado[i]-0x030>=0 && dado[i]-0x030<=9 ) && dado[i] != ',') Nfloat++;//n�o ser� um float, sinaliza-se...
      if(dado[0] == '-')Nfloat--;// se dado[0] � um sinal de menos pode ser que seja um n�mero negativo ent�o decrementa sinalizador
         if(!Nfloat && dot && cv == 1){//         ****SE ENTROU AQUI � PQ � UM VALOR REAL****   
            v_real(); cv = 0;}
         else if(!Nfloat && dot && cv != 1){//       ****SE ENTROU AQUI � PQ era pra ser real mas foi digitada mais que uma v�rugula ****
             v_string(); cv = 0;}
        else if(Nfloat>=1 && pos>1){//       ****SE ENTROU AQUI � PQ � UMA STRING****
             v_string(); cv = 0;}
        else if(!Nfloat && !dot){//      ****SE ENTROU AQUI � PQ � UM VALOR INTEIRO****
            v_inteiro(); cv = 0;}
        else if(pos==1){//         ****SE ENTROU AQUI � PQ � UM CHAR****
            v_caracter(); cv = 0;}
    }else{  //     ****SE N�O CHEGOU UM '\n', VAI ARMAZENANDO NO VETOR DADO****     
     if (c == ',')
         dot = pos; // se chegou uma v�rgula salva posi��o do separador decimal se � a �nica n�o no in�cio da strig
       if(pos <24){
      dado[pos] = c; 
       pos++; //incrementa posi��o de inser��o na string dado[25], serve tb como tamanho do dado "ok"
        }else{
            U0puts(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n>>>>>>>>>>> Voc� digitou 24 caracteres, pressione ENTER para enviar. <<<<<<<<<<<\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"); 
            }
    }
}
/*********************************        FIM DE U0getchar()      *****************************************************/
 
/* Envia um caractere */
void U0putchar(int c)
{ int a,b;  
    while (uart_base[UART_FR] & UART_TXI) {
    /*DO NOTHING*/
    }
    uart_base[UART_DR] = c;/* Transmite */
}
 
/* Envia uma mensagem pela UART0 */
void U0puts(char *txt)
{
  while(*txt) U0putchar(*txt++);
}
 
static inline void putk(int c)
{
  U0putchar(c);
 
}
 
static inline void printfloat(double val,int size)
{
  int chr[64];
  int i = 0;
 if(((int)val == 0) && (size == 0)) putk('0');
 if(val<0) {U0putchar('-'); val*=-1; }
  for(i=0;i<size;i++)
  {
    if((int)val==0)
    {
      if(i==1)
        putk('.');
      putk('0');
    }
    val*=10;
  }
  for(i=0;!(val<1);i++)
  {
    chr[i] = 0x30 + ((int)val % 10);
    val = val/10;
  }
  i--;
  while (i >= 0)
  {
    if(i==size-1 && size!=0)
      putk('.');
    putk(chr[i]);
    i--;
  }
}
 
static inline void printhexU(int val)
{
  static const char *hex="0123456789ABCDEF";
  int i=32;
  int k=7;
  while (i >= 4) 
  {
    putk(hex[(val >> (k * 4)) & 0xF]);
    k--;
    i-=4;
  }
}
 
static inline void printhexL(int val)
{
  static const char *hex="0123456789abcdef";
  int i=32;
  int k=7;
  while (i >= 4) 
  {
    putk(hex[(val >> (k * 4)) & 0xF]);
    k--;
    i -= 4;
  }
}
 
static inline void printbinary(int val, int size)
{
  if (!size)
    return;
  size -= 1;
  if (size > 32)
    size = 32;
  int i = size;
  while (i >= 0)
  {
    putk(0x30 + ((val >> i) & 1));
    i--;
  }
}
 
void sys_nkprint(char *fmt,void *number)
{
//  va_list va;
  int *aux;
  float *aux2;
  char *aux3;
  int size=0;
  int accuracy=1;
  while (*fmt)
  {//RPI_GetGpio()->GPSET0 = (1 << 16); // acendeu
    if (*fmt != '%')
    { //RPI_GetGpio()->GPSET0 = (1 << 16); // acendeu
      putk(*fmt);
//      if (*fmt == '\n')
//        putk('\r');
      fmt++;
    }
    else
    {
      fmt++; //RPI_GetGpio()->GPSET0 = (1 << 16); // acendeu
      switch(*fmt)
      {
    case '%': 
      putk(*fmt);
      break;
    case 'd':
    aux=number;
      printfloat(*aux,0);
      break;
    case 'f':
    aux2=number;// RPI_GetGpio()->GPSET0 = (1 << 16); // acendeu
      printfloat(*aux2,6);
      break;
    case '.':
      fmt++;
      while(*fmt != 'f')
      {
        size*=accuracy;
        size+= (*fmt - '0');
        accuracy*=10;
        fmt++;
      }
    aux2=number;
      printfloat(*aux2,size);
      break;
    case 'x':
    aux=number;
      printhexL(*aux);
      break;
    case 'X':
    aux=number;
      printhexU(*aux);
      break;
  case 'c':
    aux3=number;
    putk(*aux3);
    break;
  case 's':
      sys_nkprint((char *)number,0); //RPI_GetGpio()->GPSET0 = (1 << 16); // acendeu
      break;
    case 'b':
      fmt++;
      switch(*fmt)
      {
        case 'b':
          size=8;
          break;
        case 'w':
          size=16;
          break;
        case 'd':
          size=32;
          break;
        default:
          fmt -= 2;
          size = 32;
          break;
      }
    aux=number;
      printbinary(*aux, size);
      break;
    default:
      break;
      }
    fmt++;
    }
  }
//  va_end(va);
}
 
#ifdef PRINTK
void printk(char *fmt, ...)
{
  va_list va;
  int size=0;
  int accuracy=1;
 
  va_start(va, fmt);
 
  while (*fmt)
  {
    if (*fmt != '%')
    {
      putk(*fmt);
//      if (*fmt == '\n')
//        putk('\r');
      fmt++;
    }
    else
    {
      fmt++;
      switch(*fmt)
      {
    case '%':
      putk(*fmt);
      break;
    case 'd':
      printfloat(va_arg(va, int),0);
      break;
    case 'f':
      printfloat(va_arg(va, double),6);
      break;
    case '.':
      fmt++;
      while(*fmt != 'f')
      {
        size*=accuracy;
        size+= (*fmt - '0');
        accuracy*=10;
        fmt++;
      }
      printfloat(va_arg(va, double),size);
      break;
    case 'x':
      printhexL(va_arg(va, int));
      break;
    case 'X':
      printhexU(va_arg(va, int));
      break;
    case 's':
      printk(va_arg(va, char *));
      break;
    case 'b':
      fmt++;
      switch(*fmt)
      {
        case 'b':
          size=8;
          break;
        case 'w':
          size=16;
          break;
        case 'd':
          size=32;
          break;
        default:
          fmt -= 2;
          size = 32;
          break;
      }
      printbinary(va_arg(va, int), size);
      break;
    default:
      break;
      }
    fmt++;
    }
  }
  va_end(va);
}
#endif

