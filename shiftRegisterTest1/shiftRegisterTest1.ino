//**************************************************************//
//  Name    : shiftOutCode, Hello World                                
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis 
//  Date    : 25 Oct, 2006    
//  Modified: 23 Mar 2010                                 
//  Version : 2.0                                             
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                           
//****************************************************************

//Pin connected to ST_CP of 74HC595
int latchPin = 5;
//Pin connected to SH_CP of 74HC595
int clockPin = 7;
////Pin connected to DS of 74HC595
int dataPin = 6;



void setup() {
  Serial.begin(9600);
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}
 int hungryAmount = 15;
 int happyAmount = 15;
 int shiftAmount = 0;
 int timeElapsed = 0;
 int lastFeedTime = 0;
 int lastHappyTime = 0;
 int timeCounter = 0;
 int counter = 0;
 
 void Hungry(){
  if (millis()- lastFeedTime > 3000) {
    if (hungryAmount > 0 && hungryAmount < 16){
      hungryAmount = hungryAmount >> 1;
      Serial.print(hungryAmount);
      lastFeedTime = millis();
      
      
      }
    }
 }

  void Happiness(){
  if (millis()- lastHappyTime > 3000) {
    if (happyAmount > 0 && happyAmount < 16){
      happyAmount = happyAmount >> 1;
      Serial.print(happyAmount);
      lastHappyTime = millis();
      
      }
    }
 }


void MASHUP(){

  shiftAmount = (happyAmount << 4 ) + hungryAmount;
   
  }

void loop() {
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    Happiness();
    Hungry();  
    MASHUP();
    shiftOut(dataPin, clockPin, MSBFIRST, shiftAmount);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(500);
  }
}
