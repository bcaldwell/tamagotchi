#include "tamagotchi.h"

//Pin connected to ST_CP of 74HC595
int latchPin = 5;
//Pin connected to SH_CP of 74HC595
int clockPin = 7;
////Pin connected to DS of 74HC595
int dataPin = 6;

volatile double timerOverflow = 0;  //tracks timer1 overflow count

state hunger;
state happiness;
//button array:
//0:left
//1:middle
//2:right
button buttons[3];

void setup() {
  Serial.begin(9600);
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //add interupts on pins 2 and 3
  attachInterrupt(0, buttonHandler0, CHANGE); // interrupt 0 is mapped to pin 2 on the Uno
  attachInterrupt(1, buttonHandler1, CHANGE); // interrupt 0 is mapped to pin 2 on the Uno

  cli();  //enables interput blocking flag
  TCCR1A = 0;             // normal counting mode
  TIMSK1 |= (1 << TOIE1); //enable timer overflow interput
  TCCR1B = _BV(CS10);      // set prescale to 1
  TCNT1 = 0;              // clear the timer count
  Serial.begin(9600);
  sei();  //removes flag that blocks interputs
}

void loop() {
  hunger.update(timer());
  happiness.update(timer());
  ledWrite();
}

void ledWrite() {
  // take the latchPin low so
  // the LEDs don't change while you're sending in bits:
  digitalWrite(latchPin, LOW);
  // shift out the bits:
  int shiftAmount = (happiness.getWriteBits() << 4 ) + hunger.getWriteBits();
  shiftOut(dataPin, clockPin, MSBFIRST, shiftAmount);
  //take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
}

//handles those interrupts
void buttonHandler0(){
  buttons[0].toggle();
}
void buttonHandler1(){
  buttons[1].toggle();
}

//functions for millis()
unsigned long timer () {
  return (TCNT1 + timerOverflow * 65536) / ( F_CPU / 1000L);
}

unsigned long timerSeconds () {
  return (TCNT1 + timerOverflow * 65536) / ( F_CPU);
}

//timer1 overflow interput function
ISR(TIMER1_OVF_vect)
{
  timerOverflow++;
}
