#include "tamagotchi.h"

//Pin connected to ST_CP of 74HC595
int latchPin = 5;
//Pin connected to SH_CP of 74HC595
int clockPin = 7;
////Pin connected to DS of 74HC595
int dataPin = 6;

boolean serialMode = true;

boolean alive = true;
volatile boolean inGame = false;

volatile double timerOverflow = 0;  //tracks timer1 overflow count

int val = 0;

state hunger;
state happiness;
life myLife;
//button array:
//0:left
//1:middle
//2:right
button buttons[4];

void setup() {
  Serial.begin(9600);

  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  //add interupts on pins 2 and 3
  attachInterrupt(0, buttonHunger, CHANGE); // interrupt 0 is mapped to pin 2 on the Uno
  attachInterrupt(1, buttonHappiness, CHANGE); // interrupt 1 is mapped to pin 3 on the Uno

  cli();  //enables interput blocking flag
  //Stuff to enable timer 1 overflow and the prescaler
  TCCR1A = 0;             // normal counting mode
  TIMSK1 |= (1 << TOIE1); //enable timer overflow interput
  TCCR1B = _BV(CS10);      // set prescale to 1
  TCNT1 = 0;              // clear the timer count

  // Pin change to enable interrupts
  PCICR |= 0b00000101; // Enables Ports B and D Pin Change Interrupts
  PCMSK0 |= 0b00000001; // PCINT0 -> Pin 8
  PCMSK2 |= 0b00010000;//PCINT20 -> Pin 4

  sei();  //removes flag that blocks interputs
  if (serialMode == true) {
    establishContact();
  }
}



void loop() {

  if (Serial.available() > 0) {
    val = Serial.read();
    Serial.println(val);
    if (val == 'f'){
      
      inGame = false;
      }
  }
  

  digitalWrite(9, myLife.flash(timer()));
  if( inGame == false){
  alive = hunger.update(timer()) && happiness.update(timer()) && myLife.isAlive();}

  ledWrite();
  for (int i = 0; i < 4; i++) {
    if (buttons[i].changed()) {
      Serial.print(i);
      Serial.print(",");
      Serial.println(buttons[i].getState());
    }
  }
  if (!alive) {
    //turn off leds

    //turn hunger and happiness lights on
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    digitalWrite(latchPin, HIGH);

    // alive meter
    digitalWrite(9, LOW);

    unsigned long timeAlive = myLife.timeAlive(timer());
    String aliveTime = "";
    aliveTime = "Time alive was:";
    aliveTime += timeAlive / 1000;
  
    Serial.println(aliveTime);

    String fedTime = "";
    // fedTime = "You fed him at: ";
    // fedTime += hunger.getTimes();
    // fedTime += " seconds.";
    // Serial.print(hunger.getTimes());

    while (true) {};

  }
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
void buttonHunger() {
  if (buttons[0].getState(true) == 1 && inGame == false ) {
    hunger.increaseState(timer());
  }
  buttons[0].toggle();
}

void buttonHappiness() {
  if (buttons[1].getState(true) == 1 && inGame == false ) {
    happiness.decreaseState(timer());
  }
  buttons[1].toggle();
}

//functions for millis()
unsigned long timer () {
  cli();  //enables interput blocking flag
  unsigned long time = TCNT1;
  sei();  //removes flag that blocks interputs
  return (TCNT1 + timerOverflow * 65536) / ( F_CPU / 1000L);
}

unsigned long timerSeconds () {
  cli();  //enables interput blocking flag
  unsigned long time = TCNT1;
  sei();  //removes flag that blocks interputs
  return (TCNT1 + timerOverflow * 65536) / ( F_CPU);
}

//timer1 overflow interput function
ISR(TIMER1_OVF_vect)
{
  timerOverflow++;
}
// PORT B -> masked to pin 8
ISR(PCINT0_vect)
{
  happiness.increaseState(timer(), true);
  buttons[2].toggle();
  inGame = true;
}

//PORT D -> masked to pin 4
ISR(PCINT2_vect)
{
  buttons[3].toggle();
  if ( inGame == false) {
    happiness.increaseState(timer());
  }

}


void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("A");   // send a capital A
    delay(300);


  }
}

