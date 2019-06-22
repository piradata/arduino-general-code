int Sched_Init(void)
{
  int x;
  for (x = 0; x < 20; x++)
    Tasks[x].func = 0;
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
botao1Read();  
botao2Read();
botao3Read();
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
  }

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

