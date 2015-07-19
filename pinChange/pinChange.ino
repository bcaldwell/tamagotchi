
volatile int value = 0;

void setup()
{
  cli(); // disables interrupts
  PCICR |= 0b00000101; // Enables Ports B and D Pin Change Interrupts
  PCMSK0 |= 0b00000001; // PCINT0
  PCMSK2 |= 0b00010000;
  sei(); // enables interrupts

  Serial.begin(9600);
}

void loop()
{
  
}


ISR(PCINT0_vect)
{
  value++;
  Serial.print("0: ");
  Serial.println(value);
}

ISR(PCINT2_vect)
{
  value++;
  Serial.print("2: ");
  Serial.println(value);
}






