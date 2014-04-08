#include <18f4520.h>
#fuses XT, NOWDT, PUT, NODEBUG, NOBROWNOUT, NOPROTECT, NOLVP, NOWRT
#use delay(clock=4000000)
#include<ds1307.c>
#use rs232(baud=9600, bits = 8 , parity = N ,xmit=PIN_C6,rcv=PIN_C7)
BYTE sec; 
BYTE min; 
BYTE hrs; 
BYTE day; 
BYTE month; 
BYTE yr; 
BYTE dow;
char dataRecived[50];
int index = 0;

#int_rda
void isr_rda()
{
   printf("olaaaa");
}


void main(void)
{
   enable_interrupts(global);
   enable_interrupts(int_rb);
   while(true)
   {
      printf("olis");
      delay_ms(1000);
   }
}
