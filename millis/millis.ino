void setup()
{
  cli();
  TCCR1A = 0;             // normal counting mode
  TIMSK1 |= (1 << TOIE1);
  TCCR1B = _BV(CS10);      // set prescale to 64
  TCNT1 = 0;              // clear the timer count
  Serial.begin(9600);
  sei();
}
double timer1 = 0;
volatile double counter = 0;


void loop()
{
  // int sreg = SREG;                  // store IRQ flags
  // cli ();                  // all IRQs off

  // SREG = sreg;                  // restore IRQ flags
  Serial.print(millis());
  Serial.print(",");
  
  Serial.println((TCNT1 + counter*64000)/( F_CPU ));
//  timer1 = TCNT1;            // read it back
}

ISR(TIMER1_OVF_vect)
{
//  digitalWrite(13, !digitalRead(13));
counter++;
}

//// avr-libc library includes
////#include <avr/io.h> //  can be omitted
////#include <avr/interrupt.h> // can be omitted
//#define LEDPIN 13
///* or use
//DDRB = DDRB | B00100000;  // this sets pin 5  as output                       // without changing the value of the other
//                         // pins
//*/
//volatile double counter = 0;
//void setup()
//{
//pinMode(LEDPIN, OUTPUT);
//
//// initialize Timer1
//cli();         // disable global interrupts
//TCCR1A = 0;    // set entire TCCR1A register to 0
//TCCR1B = 0;    // set entire TCCR1A register to 0
//
//// enable Timer1 overflow interrupt:
//TIMSK1 |= (1 << TOIE1);
//// Preload with value 3036
////use 64886 for 100Hz
////use 64286 for 50 Hz
////use 34286 for 2 Hz
//TCNT1=0x0BDC;
//// Set CS10 bit so timer runs at clock speed: (no prescaling)
//TCCR0B = (_BV(CS02) | _BV(CS00));
////TCCR1B |= (1 << CS12); // Sets bit CS12 in TCCR1B
//// This is achieved by shifting binary 1 (0b00000001)
//// to the left by CS12 bits. This is then bitwise
//// OR-ed into the current value of TCCR1B, which effectively set
//// this one bit high. Similar: TCCR1B |= _BV(CS12);
////  or: TCCR1B= 0x04;
//
//// enable global interrupts:
//sei();
//Serial.begin(9600);
//}
//
//ISR(TIMER1_OVF_vect)
//{
//counter++;
//}
//
//void loop() {
//  Serial.println(counter);
//}
//
////
////void setup() {
////  // put your setup code here, to run once:
////  Serial.begin(9600);
//////  TCCR0B = (_BV(CS02) | _BV(CS00));
//////  sbi(TIMSK0, TOIE0);
////Timer1.attachInterrupt(callback);
////}
////
////void loop() {
////  // put your main code here, to run repeatedly:
//////  Serial.print (millis());
//////  Serial.print("       ");
//////  Serial.println(timer());
////  read();
////}
////
////void callback(){
//////  Serial.println('hi');
////}
////
////unsigned long timer(){
////  return TCNT0;
////}
//////
//////SIGNAL(TIMER0_OVF_vect){
//////  Serial.println("__________________");
//////}
//////{
//////    // copy these to local variables so they can be stored in registers
//////    // (volatile variables must be read from memory on every access)
//////    unsigned long m = timer0_millis;
//////    unsigned char f = timer0_fract;
//////
//////    m += MILLIS_INC;
//////    f += FRACT_INC;
//////    if (f >= FRACT_MAX) {
//////        f -= FRACT_MAX;
//////        m += 1;
//////    }
//////
//////    timer0_fract = f;
//////    timer0_millis = m;
//////    timer0_overflow_count++;
//////}
//////
//////unsigned long timer()
//////{
//////    unsigned long m;
//////    uint8_t oldSREG = SREG;
//////
//////    // disable interrupts while we read timer0_millis or we might get an
//////    // inconsistent value (e.g. in the middle of a write to timer0_millis)
//////    cli();
//////    m = timer0_millis;
//////    SREG = oldSREG;
//////
//////    return m;
//////}
