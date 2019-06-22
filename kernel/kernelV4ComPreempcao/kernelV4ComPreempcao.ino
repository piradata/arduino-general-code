#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8

#define BUTTON1 A1
#define BUTTON2 A2
#define BUTTON3 A3
#define PRESS 0
#define UNPRESS 1

#define POT_DIO 0
#define BUZZER_DIO 3

#define OFF 255
#define ON 245

#define charC 0XC6
#define charO 0XC0
#define charL 0XC7
#define chard 0XA1

#define charH 0X89
#define charI 0XCF

//DeBoucing Resolution Number
#define DBRN 8

//const byte SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
const byte SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};
const byte LED[] = {13, 12, 11, 10};
bool bot1 = UNPRESS, bot2 = UNPRESS, bot3 = UNPRESS;
byte SetBuzzer = 0, SetDisplay = 0, estado = 0, led = 0, bot1t = 0, bot2t = 0, bot3t = 0;
char FGP, FXP, FYP, cur_task = 20;

typedef struct {
  // period in ticks
  int period;
  // ticks until next activation
  int delay;
  // function pointer
  void (*func)(void);
  // activation counter
  int exec;
} Sched_Task_t;

Sched_Task_t Tasks[20];

int Sched_Init(void)
{
  int x;
  for (x = 0; x < 20; x++)
    Tasks[x].func = 0;
};

void conf_interrupts() {
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  //OCR1A = 31250; // compare match register 16MHz/256/2Hz
  //OCR1A = 31;    // compare match register 16MHz/256/2kHz
  OCR1A = 62;      // compare match register 16MHz/256/1kHz
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // enable all interrupts
};


void Sched_Schedule(void)
{
  int x;
  for (x = 0; x < 20; x++)
  {
    if ((Tasks[x].func) && (Tasks[x].delay))
    {
      Tasks[x].delay--;
      if (!Tasks[x].delay)
      {
        /* Schedule Task */
        Tasks[x].exec++;
        Tasks[x].delay = Tasks[x].period;
      }
    }
  }

  //botao 1
  if (digitalRead(BUTTON1) == PRESS)
  {
    if (bot1t == DBRN && bot1 == UNPRESS)
    {
      bot1 = PRESS;
      Tasks[FXP].exec++;
    }
    else if (bot1t == DBRN && bot1 == PRESS)
    {
      //GURP(HCF);
    }
    else if (bot1t < DBRN && bot1 == UNPRESS)
    {
      bot1t++;
    }
    else if (bot1t < DBRN && bot1 == PRESS)
    {
      bot1t = DBRN;
    }
  }
  else
  {
    if (bot1t == 0 && bot1 == PRESS)
    {
      bot1 = UNPRESS;
    }
    else if (bot1t == 0 && bot1 == UNPRESS)
    {
      //GURP(HCF);
    }
    else if (bot1t > 0 && bot1 == PRESS)
    {
      bot1t--;
    }
    else if (bot1t > 0 && bot1 == UNPRESS)
    {
      bot1t = 0;
    }
  }


  //botao 2

  if (digitalRead(BUTTON2) == PRESS)
  {
    if (bot2t == DBRN && bot2 == UNPRESS)
    {
      bot2 = PRESS;
      //code botao 2
    }
    else if (bot2t == DBRN && bot2 == PRESS)
    {
      //GURP(HCF);
    }
    else if (bot2t < DBRN && bot2 == UNPRESS)
    {
      bot2t++;
    }
    else if (bot2t < DBRN && bot2 == PRESS)
    {
      bot2t = DBRN;
    }
  }
  else
  {
    if (bot2t == 0 && bot2 == PRESS)
    {
      bot2 = UNPRESS;
    }
    else if (bot2t == 0 && bot2 == UNPRESS)
    {
      //GURP(HCF);
    }
    else if (bot2t > 0 && bot2 == PRESS)
    {
      bot2t--;
    }
    else if (bot2t > 0 && bot2 == UNPRESS)
    {
      bot2t = 0;
    }
  }

  //botao 3

  if (digitalRead(BUTTON3) == PRESS)
  {
    if (bot3t == DBRN && bot3 == UNPRESS)
    {
      bot3 = PRESS;
      if (estado == 0)
      {
        estado = 1;
      }
      else
      {
        estado = 0;
      }
    }
    else if (bot3t == DBRN && bot3 == PRESS)
    {
      //GURP(HCF);
    }
    else if (bot3t < DBRN && bot3 == UNPRESS)
    {
      bot3t++;
    }
    else if (bot3t < DBRN && bot3 == PRESS)
    {
      bot3t = DBRN;
    }
  }
  else
  {
    if (bot3t == 0 && bot3 == PRESS)
    {
      bot3 = UNPRESS;
    }
    else if (bot3t == 0 && bot3 == UNPRESS)
    {
      //GURP(HCF);
    }
    else if (bot3t > 0 && bot3 == PRESS)
    {
      bot3t--;
    }
    else if (bot3t > 0 && bot3 == UNPRESS)
    {
      bot3t = 0;
    }
  }
}

void Sched_Dispatch(void) {
  int prev_task = cur_task;
  int x;
  for (x = 0; x < cur_task; x++)
  {
    if ((Tasks[x].func) and (Tasks[x].exec > 0))
    {
      Tasks[x].exec--;

      cur_task = x;
      interrupts();

      Tasks[x].func();

      noInterrupts();
      cur_task = prev_task;
      /* Delete task
         if one-shot */
      //if (!Tasks[x].period) Tasks[x].func = 0;
      //if (!Tasks[x].period) Tasks[x].exec = 0;
      return; //prioridade rules
    }
  }
};

int Sched_AddT(void (*f)(void), int d, int p)
{
  int x;
  for (x = 0; x < 20; x++)
    if (!Tasks[x].func)
    {
      Tasks[x].period = p;
      Tasks[x].delay  = d;
      Tasks[x].exec   = 0;
      Tasks[x].func   = f;
      return x;
    }
  return -1;
};

void WriteCharToSegment(byte Segment, byte Value)
{
  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, Value);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment] );
  digitalWrite(LATCH_DIO, HIGH);
}

void FuncG_init() {
  pinMode(LATCH_DIO, OUTPUT);
  pinMode(CLK_DIO, OUTPUT);
  pinMode(DATA_DIO, OUTPUT);
}

void FuncX_init() {
  pinMode(LED[0], OUTPUT);
  pinMode(LED[1], OUTPUT);
  pinMode(LED[2], OUTPUT);
  pinMode(LED[3], OUTPUT);
  digitalWrite(LED[0], HIGH);
  digitalWrite(LED[1], HIGH);
  digitalWrite(LED[2], HIGH);
  digitalWrite(LED[3], HIGH);
}

void FuncY_init() {
  pinMode(BUZZER_DIO, OUTPUT);
  digitalWrite(BUZZER_DIO, OFF);
}

void FuncG() {
  if (estado == 0)
  {
    if (SetDisplay == 0) WriteCharToSegment(0, charC);
    if (SetDisplay == 1) WriteCharToSegment(1, charO);
    if (SetDisplay == 2) WriteCharToSegment(2, charL);
    if (SetDisplay == 3) WriteCharToSegment(3, chard);
  }
  else
  {
    if (SetDisplay == 0) WriteCharToSegment(0, charC);
    if (SetDisplay == 1) WriteCharToSegment(1, charH);
    if (SetDisplay == 2) WriteCharToSegment(2, charI);
    if (SetDisplay == 3) WriteCharToSegment(3, charL);
  }
  SetDisplay++;
  if (SetDisplay == 4) SetDisplay = 0;
}

void FuncX() {
  digitalWrite(LED[0], HIGH);
  digitalWrite(LED[1], HIGH);
  digitalWrite(LED[2], HIGH);
  digitalWrite(LED[3], HIGH);

  digitalWrite(LED[led], LOW);

  led += 1;
  if (led == 4) led = 0;
}

void FuncY() {
  //int forca = (analogRead(POT_DIO) * 10) % 255;
  if (SetBuzzer == 0)
  {
    //analogWrite(BUZZER_DIO, forca);
    analogWrite(BUZZER_DIO, ON);
    SetBuzzer = 1;
  }
  else
  {
    analogWrite(BUZZER_DIO, OFF);
    SetBuzzer = 0;
  }
}

void setup()
{
  Sched_Init();
  /*display task*/
  FuncG_init();
  FGP = Sched_AddT(FuncG, 1, 5);
  /*led task*/
  FuncX_init();
  FXP = Sched_AddT(FuncX, 1, 2000);
  /*buzzer task*/
  FuncY_init();
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
