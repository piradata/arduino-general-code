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
