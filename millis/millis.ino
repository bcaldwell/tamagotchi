void setup()
{
  cli();
  TCCR1A = 0;             // normal counting mode
  TIMSK1 |= (1 << TOIE1);
  TCCR1B = _BV(CS10);      // set prescale to 1
  TCNT1 = 0;              // clear the timer count
  Serial.begin(9600);
  sei();
}
volatile double counter = 0;

void loop()
{
  // int sreg = SREG;                  // store IRQ flags
  // cli ();                  // all IRQs off

  // SREG = sreg;                  // restore IRQ flags
  Serial.print(millis());
  Serial.print(",");

  Serial.println(timer());
}

unsigned long timer (){
  return (TCNT1 + counter * 65536) / ( F_CPU/1000L);
}

unsigned long timerSeconds (){
  return (TCNT1 + counter * 65536) / ( F_CPU);
}

ISR(TIMER1_OVF_vect)
{
  counter++;
}
