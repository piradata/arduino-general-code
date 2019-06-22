//const byte SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
const byte SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};
char SetDisplay = 0, estado = 0;

const byte LED[] = {13, 12, 11, 10};
char led = 0;

char bot1 = UNPRESS, bot2 = UNPRESS, bot3 = UNPRESS;
char bot1t = 0, bot2t = 0, bot3t = 0;

char buzzer = 1;

char FGP, FXP, FYP;

char cur_task = 20;

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
