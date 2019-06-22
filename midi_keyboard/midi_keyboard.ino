/*
   midi_keyboard.c

   Copyright 2018 Unknown <rennelou@jarbas>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

    PC0 até o PC5 serão os pulsos

    PB0 até PB5 serão as entradas matriciais
    PD3 e PD4 serão entradas matriciais

    Entradas
    marron 8 pino
    marron-branco 7
    laranja 6
    laranja-branco 5
    verde 4
    verde-branco 3
    azul 2
    azul-branco 1
 
    Saidas
    marron 5
    marron-branco 4
    laranja 3
    laranja-branco 2
    verd 1
*/

#include <avr/io.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

//#define UART_BAUD 31250
#define UART_BAUD 115200
#define RX_BUFSIZE 80

#define NUM_ROWS 8
#define NUM_COLS 5


// Funções
static int uart_putchar(char, FILE*);
void uart_init();

static FILE uart;
//static FILE uart = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_RW);


int keyPressed[NUM_COLS][NUM_ROWS];
uint8_t keyToMidiMap[NUM_COLS][NUM_ROWS];


////Interrompe a cada 4ms
ISR(TIMER1_OVF_vect) {

  int colCtr;
  int rowCtr;

  int rowValue[NUM_ROWS];

  uint8_t NOTE_ON_CMD = (0x90 | (1 & 0x0F));
  uint8_t NOTE_OFF_CMD = (0x80 | (1 & 0x0F));
  uint8_t NOTE_VELOCITY = 127 & 0x7F;

  colCtr = 0;
  while(colCtr < NUM_COLS){
  //for (colCtr = 0; colCtr < NUM_COLS; colCtr++) {

    PORTC = ~_BV(colCtr);
    
    rowValue[0] = PINB & _BV(0);
    rowValue[1] = PINB & _BV(1);
    rowValue[2] = PINB & _BV(2);
    rowValue[3] = PINB & _BV(3);
    rowValue[4] = PINB & _BV(4);
    rowValue[5] = PINB & _BV(5);
    rowValue[6] = PIND & _BV(3);
    rowValue[7] = PIND & _BV(4);

    rowCtr = 0;
    while(rowCtr < NUM_ROWS){
    //for (rowCtr = 0; rowCtr < NUM_ROWS; rowCtr++) {
      uint8_t tmp;
      if (!rowValue[rowCtr] && !keyPressed[colCtr][rowCtr]) {
        keyPressed[colCtr][rowCtr] = 1;
      tmp = keyToMidiMap[colCtr][rowCtr] & 0x7F;
        //printf("note on %d 127\n", keyToMidiMap[colCtr][rowCtr]);
        fwrite(&NOTE_OFF_CMD,sizeof(uint8_t),1,stdout);
        fwrite(&tmp,sizeof(uint8_t),1,stdout);
        fwrite(&NOTE_VELOCITY,sizeof(uint8_t),1,stdout);      
      }
     
      
      if (rowValue[rowCtr] && keyPressed[colCtr][rowCtr]) {
        keyPressed[colCtr][rowCtr] = 0;
        tmp = keyToMidiMap[colCtr][rowCtr] & 0x7F;
        fwrite(&NOTE_OFF_CMD,sizeof(uint8_t),1,stdout);
        fwrite(&tmp,sizeof(uint8_t),1,stdout);
        fwrite(&NOTE_VELOCITY,sizeof(uint8_t),1,stdout);
        //printf("note off %d 127\n", keyToMidiMap[colCtr][rowCtr]);
      }
      rowCtr++;
      
    }
  colCtr++;
  
  }



}

int main() {

  cli();

  int note = 31;

  for (int colCtr = 0; colCtr < NUM_COLS; colCtr++) {
    for (int rowCtr = 0; rowCtr < NUM_ROWS; rowCtr++) {
      keyPressed[colCtr][rowCtr] = 0;
      keyToMidiMap[colCtr][rowCtr] = note;
      note++;
    }
  }

  //gambiarra por causa de bug inexplicavel
  keyToMidiMap[1][0] = 31;
  keyToMidiMap[2][0] = 39;
  keyToMidiMap[3][0] = 47;
  keyToMidiMap[4][0] = 55;
  keyToMidiMap[0][0] = 63;
  //fim da gambiarra
  
  DDRB &= 0b00111111;
  PORTB |= 0b00111111;

  DDRC |= 0b00111111;
  PORTC |= 0b00000001;

  DDRD &= 0b11100111;
  PORTD |= 0b00011000;

  // initialize timer1 overflow interrupt
  TIMSK1 = _BV(TOIE1); //Inicia interrupção
  TCCR1B |=  _BV(CS10); //Prescale
  TCCR1B &= ~(_BV(CS12) | _BV(CS11)); //prescale

  uart_init();
  fdev_setup_stream(&uart, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uart;
  
  sei();
  while (1) {}
  return 0;
}

void uart_init() {
#if F_CPU < 2000000UL && defined(U2X)
  UCSR0A = _BV(U2X0);
  UBRR0L = (F_CPU / (8UL * UART_BAUD)) - 1;
#else
  UBRR0L = (F_CPU / (16UL * UART_BAUD)) - 1;
#endif

  UCSR0B = _BV(TXEN0) | _BV(RXEN0);
}

static int uart_putchar(char c, FILE* file) {
  if (c == '\n') uart_putchar('\r', file);

  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
  return 0;
}
