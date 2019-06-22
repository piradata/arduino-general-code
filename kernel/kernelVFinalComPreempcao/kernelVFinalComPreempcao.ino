#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8

#define BUTTON1 15 //A1
#define BUTTON2 16 //A2
#define BUTTON3 17 //A3
#define PRESS 0
#define UNPRESS 1

#define POT_DIO 0
#define BUZZER_DIO 3

#define OFF 255
#define ON 254

#define charC 0XC6
#define charO 0XC0
#define charL 0XC7
#define chard 0XA1

#define charH 0X89
#define charI 0XCF

#define DBRN 8 //DeBoucing Resolution Number

//#define DEBUG

#include "data.h"
#include "botoes.h"
#include "utilidades.h"
#include "funcoes.h"
#include "confint.h"
#include "sched.h"

void setup()
  {
    Sched_Init();
    
    FuncG_init();  //display task
    FGP = Sched_AddT(FuncG, 1, 5);
    
    FuncX_init(); //led task
    FXP = Sched_AddT(FuncX, 1, 2000);
    
    FuncY_init();  //buzzer task
    FYP = Sched_AddT(FuncY, 1, 1000);

    conf_interrupts();

    while (true);
  }

  ISR(TIMER1_COMPA_vect)
  { //timer1 interrupt
    noInterrupts();
    Sched_Schedule();
    Sched_Dispatch();
    interrupts();
  }
