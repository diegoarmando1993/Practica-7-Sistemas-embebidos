#include <18f4520.h>
#fuses XT, NOWDT, PUT, NODEBUG, NOBROWNOUT, NOPROTECT, NOLVP, NOWRT
#use delay(clock=4000000)
#include<ds1307.c>
#use rs232(baud=9600, bits = 8 , parity = N ,xmit=PIN_C6,rcv=PIN_C7)


int timer=0;
int segundos=0; 
int minutos=0; 
int horas=0; 
int dias=0; 
int mes=0; 
int year=0; 
int dow=0;

char dataRecived[50];
int index = 0;




void analizarTrama();
void analyzeTokens();
void limpiarTrama();
boolean isValidNumber(char);
int charToInt(char);

#int_rda
void isr_rda()
{
   char buffer;
   //Hay datos pendientes ??
   if(kbhit())
   {
      //Si es el backspace borramos la trama 
      int asciiKEY;
      buffer= getc();
      asciiKEY = buffer;
      if(asciiKEY == 8)
      {
         if(index>0)
         {
            dataRecived[index] = ' ';
            index--;
         }
      }
      else
      {
         if(index>50)index=0;
         
         dataRecived[index] = buffer;
         index++;
      }
 
   }
}

#int_timer0
void timer0()
{
   timer=true;
}

int test(int bcd) 
{ 
   int decena;
   int unidad;
   int binary;
   
   decena = bcd;
   decena >>=4;
   decena = decena * 10;
   
   unidad = bcd & 0x0f;
   
   binary = unidad + decena;
   return binary;
   
} 

void main(void)
{
   enable_interrupts(global);
   enable_interrupts(int_rb);
   enable_interrupts(int_rda);
   enable_interrupts(INT_TIMER0);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_16);
   set_timer0(3036);

   while(true)
   {
      analizarTrama();
      if(timer)
      {
         getDate(dias,mes,year,dow);
         getTime(horas,minutos,segundos);
         printf("%i / %i / %i \r", dias , mes , year);
         printf("%i : %i : %i \r\r" , horas , minutos ,segundos);
         timer=false;
      }
   }
}

void analizarTrama()
{
     /* Analizamos si la trama esta completa */
   int i;
   //Analizar si esta completa
   boolean inicioTrama = false;
   boolean finalTrama = false;
   for( i=0; i<=index;i++)
   {
      if(dataRecived[i] == '<')
      {
         inicioTrama = true;
      }
      else if(dataRecived[i] == '>')
      {
         finalTrama = true;
      }
   }
   
   /*LA TRAMA ESTA COMPLETA*/
   if(inicioTrama == true && finalTrama == true)
   {
      analyzeTokens();
      limpiarTrama();
   }
}

void analyzeTokens()
{
 
   /*
   *  s = segundos
   *  m = minutos
   *  h = horas
   *  D = dias
   *  M = mes
   *  A = año
   */
   switch(dataRecived[1])
   {
      case 's':
      
         if(isValidNumber(dataRecived[2]) && isValidNumber(dataRecived[3]))
         {
            int unidad;
            int decena;
            
            decena = CharToInt(dataRecived[2]) * 10;
            unidad = charToInt(dataRecived[3]);
            segundos = decena + unidad;
            if(segundos<=59)
            {
                setTime(dias,mes,year,dow,horas,minutos,segundos);
               printf("Segundos establecidos con %i  \r" , segundos);
            }
            else
            {
                printf(" Segundos Invalidos   , recuerda que es de 00 a 59 \r");
            }
           
            
         }
         else
         {
            printf("Numero no valido \r");
         }
         
         break;
         
      case 'm':
      
      
      
      if(isValidNumber(dataRecived[2]) && isValidNumber(dataRecived[3]))
         {
            int unidad;
            int decena;
            
            decena = CharToInt(dataRecived[2]) * 10;
            unidad = charToInt(dataRecived[3]);
            minutos = decena + unidad;
            if(minutos<=59)
            {
               setTime(dias,mes,year,dow,horas,minutos,segundos);
               printf("Minutos establecidos con %i  \r" , minutos);
            }
            else
            {
                printf(" Minutos Invalidos   , recuerda que es de 00 a 59 \r");
            }
           
            
         }
         else
         {
            printf("Numero no valido \r");
         }
         
         
         break;
         
         
      case 'h':
      
      
      
      
      if(isValidNumber(dataRecived[2]) && isValidNumber(dataRecived[3]))
         {
            int unidad;
            int decena;
            
            decena = CharToInt(dataRecived[2]) * 10;
            unidad = charToInt(dataRecived[3]);
            horas = decena + unidad;
            if(horas<=23)
            {
               setTime(dias,mes,year,dow,horas,minutos,segundos);
               printf("Horas establecidos con %i  \r" , minutos);
            }
            else
            {
                printf(" Horas Invalidos   , recuerda que es de 00 a 23 \r");
            }
           
            
         }
         else
         {
            printf("Numero no valido \r");
         }
     
         break;
         
         
      case 'D':
        if(isValidNumber(dataRecived[2]) && isValidNumber(dataRecived[3]))
         {
            int unidad;
            int decena;
            
            decena = CharToInt(dataRecived[2]) * 10;
            unidad = charToInt(dataRecived[3]);
            dias = decena + unidad;
            if(dias<=31)
            {
               setTime(dias,mes,year,dow,horas,minutos,segundos);
               printf("Dias establecidos con %i  \r" , dias);
            }
            else
            {
                printf(" Dias Invalidos   , recuerda que es de 00 a 31 \r");
            }
           
            
         }
         else
         {
            printf("Numero no valido \r");
         }

         break;
         
      case 'M':
         if(isValidNumber(dataRecived[2]) && isValidNumber(dataRecived[3]))
         {
            int unidad;
            int decena;
            
            decena = CharToInt(dataRecived[2]) * 10;
            unidad = charToInt(dataRecived[3]);
            mes = decena + unidad;
            if(mes>0 && mes<=12)
            {
               setTime(dias,mes,year,dow,horas,minutos,segundos);
               printf("Mes establecidos con %i  \r" , mes);
            }
            else
            {
                printf(" Mes Invalidos   , recuerda que es de 01 a 12 \r");
            }
           
            
         }
         else
         {
            printf("Numero no valido \r");
         }
         break;
         
       
      case 'A':
         if(isValidNumber(dataRecived[2]) && isValidNumber(dataRecived[3]) && isValidNumber(dataRecived[4]) && isValidNumber(dataRecived[5]))
         {
            int unidad;
            int decena;
         
          
            decena = CharToInt(dataRecived[4]) * 10;
            unidad = charToInt(dataRecived[5]);
            
            year = decena + unidad;
           
            if(year>=0)
            {
               setTime(dias,mes,year,dow,horas,minutos,segundos);
               printf("Years establecidos  \r" );
            }
            else
            {
                printf("No se soportan años antes de cristo \r");
            }
           
            
         }
         else
         {
            printf("Numero no valido \r");
         }
         break;
         
         default:
          printf("Valor Invalido \r");
         break;
   }
   
   
   
   
  
   
}

void limpiarTrama()
{
   int i;
   for(i=0; i<=index; i++)
   {
      dataRecived[i] = ' ';
   }
}

boolean isValidNumber(char ascii)
{
   boolean valid;
   int number;
   valid=false;
   number= ascii;
   if(number>=48 && number<=57)
   {
      valid = true;
   }
   
      
   return valid;
}  


int charToInt(char charNumber)
{
   int numberInt;
   numberInt = charNumber;
   numberInt -=48;
   
   return numberInt;
   
}




