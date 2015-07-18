
volatile int value = 0;

void setup()
{
  cli(); // disables interrupts
  PCICR |= 0b00000001; // Enables Ports B Pin Change Interrupts
  PCMSK0 |= 0b00000001; // PCINT0
  sei(); // enables interrupts

  Serial.begin(9600);
}

void loop()
{
  
}


ISR(PCINT0_vect)
{
  value++;
  Serial.println(value);
}



