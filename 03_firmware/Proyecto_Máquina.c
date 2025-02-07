#include <16f877a.h> 
#use delay (clock=4000000) 
#byte puerto_B = 0x06 // Identificador para el puerto B. 
#fuses xt, nowdt, put  //SE UTILIZARA FRECUENCIA EXTERNA Y NO SE UTILIZARA EL WATCHDOG
#define LCD_DB4   PIN_C4
#define LCD_DB5   PIN_C5
#define LCD_DB6   PIN_C6
#define LCD_DB7   PIN_C7
#define LCD_RS    PIN_C3
#define LCD_RW    PIN_C1
#define LCD_E     PIN_C2

#include <flex_lcd.c>


int c=0;
int g=0;
int p;
int r=1;
#int_RB //Interrupciones por algunos de los puertos RBa a Rb7
void RB_isr(void) //función de interrupción 
{ 

   switch(puerto_B) 
   { 
      case 239://Cuando pulse el sensor 2 para el motor principal
     
      output_low(PIN_D5);//Paro motor principal
      delay_ms(500);
      break; 
      case 223://Paro de emergencia 
      printf(lcd_putc,"\f");
      do{
      output_low(PIN_D0);//Detiene el motor secundario
      output_low(PIN_D1);//Detiene el motor secundario
      output_low(PIN_D7);//Detiene selenoide
      output_low(PIN_D5);//Frena motor
      lcd_gotoxy(7,1);                       //escribe el mensaje en la posicion x=3,y=1
      printf(lcd_putc,"PARO");           //escribe el mensaje en la pantalla
      lcd_gotoxy(4,2);                       //escribe el mensaje en la posicion x=3,y=1
      printf(lcd_putc,"Emergencia");
      }while(input(PIN_D3)==0);
      printf(lcd_putc,"\f");
      lcd_gotoxy(1,1);                       //escribe el mensaje en la posicion x=3,y=1
      printf(lcd_putc,"Proceso Iniciado");
      break;  
   } 
}
void MotorSecundario()
{
 output_high(PIN_D0);
 return;
}
void FuncionStart()
{
 output_high(PIN_D7);//inicia selenoide
 delay_ms(1000);
 return;
}
void GiroMotorPrincipal()
{
 output_high(PIN_D5);//Arranca motor principal
 delay_ms(2000);//Espera un tiempo 
 output_low(PIN_D5);//Frena motor
}
void GiroIzqMotorSecundario()
{
  output_low(PIN_D0);//Detiene el motor secundario
  delay_ms(500);
  output_low(PIN_D7);//Detiene selenoide
  output_high(PIN_D5);//Arranca motor principal
  delay_ms(2000);//Espera un tiempo 
  
  output_low(PIN_D5);//Frena motor
  output_high(PIN_D7);//inicia selenoide
  output_high(PIN_D1);//Invierte el giro
  
  return;
}
void GiroDerMotorSecundario()
{
 output_low(PIN_D1);//Detiene el motor secundario
 delay_ms(500);
 output_low(PIN_D7);//Detiene selenoide
 output_high(PIN_D5);//Arranca motor principal
 delay_ms(2000);//Espera un tiempo 
 output_low(PIN_D5);//Frena motor
 output_high(PIN_D7);//inicia selenoide
 output_high(PIN_D0);//Invierte el giro
 
 return;
}
void PosicionInicial3()//Posicion inicial para el sensor 3
{
 output_low(PIN_D0);//Detiene el motor secundario
 delay_ms(500);
 output_low(PIN_D7);//Detiene selenoide
 delay_ms(500);
 do
 {
  output_high(PIN_D6);//Invierte el giro del motor principal
  output_high(PIN_D5);//
 }while(input(PIN_D4)==0);//Hasta que vea el sensor 1
  output_low(PIN_D5);//
 output_low(PIN_D6);//Detiene el motor principal

 do
 {
  output_high(PIN_D1);//Regresa el motor secundario al estado inicial
 }while(input(PIN_A1)==0);//Hasta que lo vea el sensor 4
 output_low(PIN_D1);//Detiene el motor secundario
 lcd_gotoxy(1,1);                       //escribe el mensaje en la posicion x=3,y=1
 printf(lcd_putc,"Inicio         ");                    

 return;
}
void PosicionInicial4()//Posicion inicial para el sensor 4
{
 output_low(PIN_D1);//Detiene el motor secundario
 delay_ms(500);
 output_low(PIN_D7);//Detiene selenoide
 delay_ms(500);
 do
 {
  output_high(PIN_D6);//Invierte el giro del motor principal
  output_high(PIN_D5);//
 }while(input(PIN_D4)==0);//Hasta que vea el sensor 1
 output_low(PIN_D5);//
 output_low(PIN_D6);//Detiene el motor principal
 lcd_gotoxy(1,1);                       //escribe el mensaje en la posicion x=3,y=1
 printf(lcd_putc,"Inicio         ");                      
 return;
}
void main()
{
 lcd_init();          //inicia comunicacion con lcd y lo configura
 delay_ms(500);
 lcd_gotoxy(5,1);                       //escribe el mensaje en la posicion x=3,y=1
 printf(lcd_putc,"MAQUINA");           //escribe el mensaje en la pantalla
 lcd_gotoxy(4,2);                       //escribe el mensaje en la posicion x=3,y=1
 printf(lcd_putc,"AUTOMATICA");
 DELAY_MS(500);
 printf(lcd_putc,"\f");
 SET_TRIS_D(0x18);//Configura 00011000 para el puerto D
 SET_TRIS_B(0xFF);//Configura puerto B como entradas
 SET_TRIS_A(0xff);
 enable_interrupts(INT_RB);//habilita interrupción particular 
 enable_interrupts(GLOBAL);//habilita interrupción global 
 output_D(0x18);
 
 output_low(PIN_D4);//
 output_high(PIN_D2);
 
 while(true)
 {
  if(input(PIN_D3)&& C==0 && input(PIN_A1))//Verifica si se pulsa el start asigna valor a variable
  {
   lcd_gotoxy(1,1);                       //escribe el mensaje en la posicion x=3,y=1
   printf(lcd_putc,"Proceso Iniciado");
   FuncionStart();
   FuncionStart();
   MotorSecundario();
   c=1;//Solo se pulsara una vez.
   p=1;
  }
  while(p==1)//Proceso
  {
     if(input(PIN_A0)==1&&g==0&&r==1)//Sensor 3
     {
      lcd_gotoxy(1,2);                       //escribe el mensaje en la posicion x=3,y=1
      printf(lcd_putc,"Sensor 3");
      GiroIzqMotorSecundario();
      if(input(PIN_B4)==0)
      {
       r=0;
      }
      g=1;
     }
     if(input(PIN_A1)==1&&g==1&&r==1)//Sensor 4
     {
      lcd_gotoxy(1,2);                       //escribe el mensaje en la posicion x=3,y=1
      printf(lcd_putc,"Sensor 4");
      GiroDerMotorSecundario();
      if(input(PIN_B4)==0)
      {
       r=0;
      }
      g=0;
     }
     if(input(PIN_A0)==1&&g==0&&r==0)//Actua cuando el motor Principal llega a su fin
     {
      lcd_gotoxy(1,2);                       //escribe el mensaje en la posicion x=3,y=1
      printf(lcd_putc,"Sensor 3");
      lcd_gotoxy(1,1);                       //escribe el mensaje en la posicion x=3,y=1
      printf(lcd_putc,"Finalizado        ");
      PosicionInicial3();
      g=1;
      p=0;
      c=0;
      r=1;
     }
     if(input(PIN_A1)==1&&g==1&&r==0)//Actua cuando el motor Principal llega a su fin
     {
      lcd_gotoxy(1,2);                       //escribe el mensaje en la posicion x=3,y=1
      printf(lcd_putc,"Sensor 4");
      lcd_gotoxy(1,1);                       //escribe el mensaje en la posicion x=3,y=1
      printf(lcd_putc,"Finalizado        ");
      PosicionInicial4();
      g=0;
      p=0;
      c=0;
      r=1;
     }
  }
 }
}

