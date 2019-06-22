#define BUTTON1 A1
#define BUTTON2 A2
#define BUTTON3 A3
#define PRESS 0
#define UNPRESS 1

#define POT_DIO 0
#define BUZZER_DIO 3

#define OFF HIGH
#define ON LOW

char FXP, FYP, FZP;
const byte LED[] = {13, 12, 11, 10};
byte led = 0, bot1 = UNPRESS, bot2 = UNPRESS, bot3 = UNPRESS;

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

void setup() {
  Sched_Init();
  /* one-shot task */
  FuncX_init();
  FXP = Sched_AddT(FuncX, 1, 2000);
  /* periodic task */
  FuncY_init();
  FYP = Sched_AddT(FuncY, 1, 2000);

  conf_interrupts();

  while (true)
  {
    Sched_Dispatch();
  };
}


int Sched_Init(void) {
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

ISR(TIMER1_COMPA_vect)
{ //timer1 interrupt
  Sched_Schedule();
}

void Sched_Schedule(void) {
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
  if ((digitalRead(BUTTON1) == PRESS) && bot1 == UNPRESS)
  {
    if ((digitalRead(BUTTON1) == PRESS) && bot1 == UNPRESS)
    {
      if ((digitalRead(BUTTON1) == PRESS) && bot1 == UNPRESS)
      {
        if ((digitalRead(BUTTON1) == PRESS) && bot1 == UNPRESS)
        {
          bot1 = PRESS;
          Tasks[FXP].exec++;
        }
      }
    }
  }
  if ((digitalRead(BUTTON1) == UNPRESS) && bot1 == PRESS)
  {
    if ((digitalRead(BUTTON1) == UNPRESS) && bot1 == PRESS)
    {
      if ((digitalRead(BUTTON1) == UNPRESS) && bot1 == PRESS)
      {
        if ((digitalRead(BUTTON1) == UNPRESS) && bot1 == PRESS)
        {
          bot1 = UNPRESS;
        }
      }
    }
  }
};

void Sched_Dispatch(void)
{
  int x;
  for (x = 0; x < 20; x++)
  {
    if ((Tasks[x].func) and (Tasks[x].exec > 0))
    {
      Tasks[x].exec--;
      Tasks[x].func();
      /* Delete task
         if one-shot */
      //if (!Tasks[x].period) Tasks[x].func = 0;
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


void FuncX_init() {
  pinMode(LED[0], OUTPUT);
  pinMode(LED[1], OUTPUT);
  pinMode(LED[2], OUTPUT);
  pinMode(LED[3], OUTPUT);
  digitalWrite(LED[0], LOW);
  digitalWrite(LED[1], HIGH);
  digitalWrite(LED[2], HIGH);
  digitalWrite(LED[3], HIGH);
}

void FuncY_init() {
  pinMode(BUZZER_DIO, OUTPUT);
  digitalWrite(BUZZER_DIO, OFF);
}

void FuncX() {
  led += 1;
  if (led == 4) led = 0;

  digitalWrite(LED[0], HIGH);
  digitalWrite(LED[1], HIGH);
  digitalWrite(LED[2], HIGH);
  digitalWrite(LED[3], HIGH);

  digitalWrite(LED[led], LOW);
}

void FuncY() {
  //int tempo = (analogRead(POT_DIO) - 500) * 10;

  analogWrite(BUZZER_DIO, 250);
  //delay(tempo);
  delay(1000);
  analogWrite(BUZZER_DIO, 255);

  //Serial.println(tempo);
}

