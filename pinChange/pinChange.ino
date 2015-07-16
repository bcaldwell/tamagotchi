volatile int value = 0;

void setup()
{
  cli();
  PCICR |= 0b00000001; // Enables Ports B Pin Change Interrupts
  PCMSK0 |= 0b00000001; // PCINT0
  PCMSK1 |= 0b00001000; // PCINT11
  sei();

  Serial.begin(9600);
}

void loop()
{
  Serial.println(value);
}


ISR(PCINT0_vect)
{
  value++;
}



