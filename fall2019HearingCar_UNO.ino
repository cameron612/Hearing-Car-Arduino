/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
*/
#include <LiquidCrystal.h>
//clipping indicator variables
boolean clipping = 0;

//data storage variables
byte newData = 0;
byte prevData = 0;
unsigned int time = 0;//keeps time and sends vales to store in timer[] occasionally
int timer[10];//sstorage for timing of events
int slope[10];//storage for slope of events
unsigned int totalTimer;//used to calculate period
unsigned int period;//storage for period of wave
byte index = 0;//current storage index
float frequency;//storage for frequency calculations
int maxSlope = 0;//used to calculate max slope as trigger point
int newSlope;//storage for incoming slope data

//variables for decided whether you have a match
byte noMatch = 0;//counts how many non-matches you've received to reset variables if it's been too long
byte slopeTol = 3;//slope tolerance- adjust this if you need
int timerTol = 10;//timer tolerance- adjust this if you need

//variables for amp detection
unsigned int ampTimer = 0;
byte maxAmp = 0;
byte checkMaxAmp;
byte ampThreshold = 10;//raise if you have a very noisy signal

//variables for loc detection
int rightPin = 4;
int leftPin = 5;
int frontPin = 6;
int backPin = 7;

int leftSound = 0;
int rightSound = 0;
int frontSound = 0;
int backSound = 0;
void calcDirection();

int lr = 0; //0 meaning both the signals are off, 1 is left on and 2 is right on
int fb = 0;// 0 meaning bith the signals are off, 1 is the forward signal on and the 2 meaning the back signal is on

/** Sound circuit 
 *  
 *  
 *  
 */
LiquidCrystal lcd(2, 3, 8, 9, 10, 11);
void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEST");
  pinMode(13,OUTPUT);//led indicator pin
  pinMode(12,OUTPUT);//output pin
  Serial.println("Starting..");

  pinMode(rightPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(frontPin, INPUT);
  pinMode(backPin, INPUT);
  
  cli();//diable interrupts
  
  //set up continuous sampling of analog pin 0 at 38.5kHz
 
  //clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;
  
  ADMUX |= (1 << REFS0); //set reference voltage
  ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only
  
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16mHz/32=500kHz
  ADCSRA |= (1 << ADATE); //enabble auto trigger
  ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements
  
  sei();//enable interrupts
}

ISR(ADC_vect) {//when new ADC value ready
  
  PORTB &= B11101111;//set pin 12 low
  prevData = newData;//store previous value
  newData = ADCH;//get value from A0

  if (prevData < 127 && newData >=127){//if increasing and crossing midpoint
    newSlope = newData - prevData;//calculate slope
    if (abs(newSlope-maxSlope)<slopeTol){//if slopes are ==
      //record new data and reset time
      slope[index] = newSlope;
      timer[index] = time;
      time = 0;
      if (index == 0){//new max slope just reset
        PORTB |= B00010000;//set pin 12 high
        noMatch = 0;
        index++;//increment index
      }
      else if (abs(timer[0]-timer[index])<timerTol && abs(slope[0]-newSlope)<slopeTol){//if timer duration and slopes match
        //sum timer values
        totalTimer = 0;
        for (byte i=0;i<index;i++){
          totalTimer+=timer[i];
        }
        period = totalTimer;//set period
        //reset new zero index values to compare with
        timer[0] = timer[index];
        slope[0] = slope[index];
        index = 1;//set index to 1
        PORTB |= B00010000;//set pin 12 high
        noMatch = 0;
      }
      else{//crossing midpoint but not match
        index++;//increment index
        if (index > 9){
          reset();
        }
      }
    }
    else if (newSlope>maxSlope){//if new slope is much larger than max slope
      maxSlope = newSlope;
      time = 0;//reset clock
      noMatch = 0;
      index = 0;//reset index
    }
    else{//slope not steep enough
      noMatch++;//increment no match counter
      if (noMatch>9){
        reset();
      }
    }
  }
    
  if (newData == 0 || newData == 1023){//if clipping
    PORTB |= B00100000;//set pin 13 high- turn on clipping indicator led
    clipping = 1;//currently clipping
  }
  
  time++;//increment timer at rate of 38.5kHz
  
  ampTimer++;//increment amplitude timer
  if (abs(127-ADCH)>maxAmp){
    maxAmp = abs(127-ADCH);
  }
  if (ampTimer==1000){
    ampTimer = 0;
    checkMaxAmp = maxAmp;
    maxAmp = 0;
  }
  
}

void reset(){//clea out some variables
  index = 0;//reset index
  noMatch = 0;//reset match couner
  maxSlope = 0;//reset slope
}


void checkClipping(){//manage clipping indicator LED
  if (clipping){//if currently clipping
    PORTB &= B11011111;//turn off clipping indicator led
    clipping = 0;
  }
}


void loop(){
  checkClipping();
  if (checkMaxAmp>ampThreshold){
    
    lcd.clear();
    lcd.setCursor(0,0);
    
    frequency = 38500/float(period);//calculate frequency timer rate/period
    //Serial.print(frequency);
    int freq = frequency;
    determineSound(frequency);
  }
}

void determineSound(int freq){
  if((freq >= 350 && freq < 600) || (freq >= 120 && freq < 180)){
      calcDirection(0);
  }else if( freq >= 850 && freq < 1100){
    calcDirection(1);
  }else{
     Serial.println("Neither");
  }
}

void calcDirection(int hs){
  leftSound = digitalRead(leftPin);
  rightSound = digitalRead(rightPin);
  frontSound = digitalRead(frontPin);
  backSound = digitalRead(backPin);
  lr = 0; //0 meaning both the signals are off, 1 is left on and 2 is right on
  fb = 0;// 0 meaning bith the signals are off, 1 is the forward signal on and the 2 meaning the back signal is on
  if(leftSound == HIGH && rightSound == LOW){
    lr = 1;
    condition1();
  }else if (leftSound == LOW && rightSound == HIGH){
    lr = 2;
    condition1();
  }else if(frontSound == HIGH && backSound == LOW) {
    fb = 1;
    condition2();
  } else if(frontSound == LOW && backSound == HIGH) {
    fb = 2;
    condition2();
  }
  printDirection(fb, lr, hs);
}

void condition1()
{
  if(frontSound == HIGH && backSound == LOW)
    {
      fb = 1;
    }
    else if (frontSound == LOW && backSound == HIGH)
    {
      fb = 2;
    }
}
void condition2()
{
   if (leftSound == HIGH && rightSound == LOW)
    {
      lr = 1;
    }
    else if(leftSound == LOW && rightSound == HIGH)
    {
      lr = 2;
    }
}

void printDirection(int fb, int lr, int hs) {
  if (hs == 0) {
    lcd.print("Horn");
  } else if (hs == 1) {
    lcd.print("Siren");
  }
  if (!(fb == 0 && lr == 0)) {
    lcd.print(" to the ");
  }
  if (fb == 0) {
    if (lr == 1) {
      lcd.print("Left");
    } else if (lr == 2) {
      lcd.print("Right");
    }
  } else if (lr == 0) {
    if (fb == 1) {
      lcd.print("Front");
    } else if (fb == 2) {
      lcd.print("Back");
    }
  } else if (lr == 1) {
    if (fb == 2) {
      lcd.print("Back Left");
    } else if (fb == 1) {
      lcd.print("Front Left");
    }
    
  } else if (lr == 2) {
    if (fb == 2) {
      lcd.print("back right");
    } else if (fb == 1) {
      lcd.print("front right");
    }
    
  }
  delay(1000);
  lcd.clear();
}
