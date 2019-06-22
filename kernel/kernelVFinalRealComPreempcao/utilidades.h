void WriteCharToSegment(byte Segment, byte Value)
{
  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, Value);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment]);
  digitalWrite(LATCH_DIO, HIGH);
}

void botao1Read(void)
{
  if (digitalRead(BUTTON1) == PRESS) {
    if (bot1t == DBRN && bot1 == UNPRESS) {
      bot1 = PRESS;
      Tasks[FXP].exec++;
    } else if (bot1t == DBRN && bot1 == PRESS) {
      //GURP(HCF);
    } else if (bot1t < DBRN && bot1 == UNPRESS) {
      bot1t++;
    } else if (bot1t < DBRN && bot1 == PRESS) {
      bot1t = DBRN;
    }
  } else {
    if (bot1t == 0 && bot1 == PRESS) {
      bot1 = UNPRESS;
    } else if (bot1t == 0 && bot1 == UNPRESS) {
      //GURP(HCF);
    } else if (bot1t > 0 && bot1 == PRESS) {
      bot1t--;
    } else if (bot1t > 0 && bot1 == UNPRESS) {
      bot1t = 0;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void botao2Read(void)
{
  if (digitalRead(BUTTON2) == PRESS) {
    if (bot2t == DBRN && bot2 == UNPRESS) {
      bot2 = PRESS;
      if (Tasks[FYP].period / 2 < 20) Tasks[FYP].period = 200;
      else Tasks[FYP].period = Tasks[FYP].period / 2;
    } else if (bot2t == DBRN && bot2 == PRESS) {
      //GURP(HCF);
    } else if (bot2t < DBRN && bot2 == UNPRESS) {
      bot2t++;
    } else if (bot2t < DBRN && bot2 == PRESS) {
      bot2t = DBRN;
    }
  } else {
    if (bot2t == 0 && bot2 == PRESS) {
      bot2 = UNPRESS;
    } else if (bot2t == 0 && bot2 == UNPRESS) {
      //GURP(HCF);
    } else if (bot2t > 0 && bot2 == PRESS) {
      bot2t--;
    } else if (bot2t > 0 && bot2 == UNPRESS) {
      bot2t = 0;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void botao3Read(void)
{
  if (digitalRead(BUTTON3) == PRESS) {
    if (bot3t == DBRN && bot3 == UNPRESS) {
      bot3 = PRESS;
      if (estado == 0) {
        estado = 1;
      } else {
        estado = 0;
      }
    } else if (bot3t == DBRN && bot3 == PRESS) {
      //GURP(HCF);
    } else if (bot3t < DBRN && bot3 == UNPRESS) {
      bot3t++;
    } else if (bot3t < DBRN && bot3 == PRESS) {
      bot3t = DBRN;
    }
  } else {
    if (bot3t == 0 && bot3 == PRESS) {
      bot3 = UNPRESS;
    } else if (bot3t == 0 && bot3 == UNPRESS) {
      //GURP(HCF);
    } else if (bot3t > 0 && bot3 == PRESS) {
      bot3t--;
    } else if (bot3t > 0 && bot3 == UNPRESS) {
      bot3t = 0;
    }
  }
}
