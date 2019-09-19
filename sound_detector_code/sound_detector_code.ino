/******************************************************************************
 * sound_detector_demo.ino
 * Sound detector sample sketch
 * Byron Jacquot @ SparkFun Electronics
 * February 19, 2014
 * https://github.com/sparkfun/Sound_Detector
 * 
 * This sketch demonstrates the use of the Sparkfun Sound Detector board.
 *
 * The Sound Detector is a small board that combines a microphone and some 
 * processing circuitry.  It provides not only an audio output, but also a 
 * binary indication of the presence of sound and an analog representation 
 * of it's amplitude.  
 *
 * This sketch demonstrates two different modes of usage for the Sound
 * Detector.  The gate output (a binary indication that is high when sound
 * is present, and low when conditions are quiet) is used to fire a pin-change 
 * ISR, which lights an LED when the sound is present.  The envelope output 
 * (an analog voltage to rises to indicate the amplitude of the sound) is 
 * sampled in the loop(), and it prints an indication of the level to the 
 * serial terminal. 
 *
 * For more details about the Sound Detector, please check the hookup guide.
 *
 * Connections:
 * The Sound Detector is connected to the Adrduino as follows:
 * (Sound Detector -> Arduino pin)
 * GND → GND
 * VCC → 5V
 * Gate → Pin 4
 * Envelope → A0
 * 
 * Resources:
 * Additional library requirements: none
 * 
 * Development environment specifics:
 * Using Arduino IDe 1.0.5
 * Tested on Redboard, 3.3v/8MHz and 5v/16MHz ProMini hardware.
 * 
 * This code is beerware; if you see me (or any other SparkFun employee) at the
 * local, and you've found our code helpful, please buy us a round!
 * 
 * Distributed as-is; no warranty is given.
 ******************************************************************************/

 // Define hardware connections
#define PIN_GATE_IN 4
#define PINE_GATE_IN2 5
#define PIN_ANALOG_IN A1
#define PIN_ANALOG_IN2 A2
#define PIN_GATE_IN3 6
#define PINE_GATE_IN4 7
#define PIN_ANALOG_IN3 A3
#define PIN_ANALOG_IN4 A4

void setup()
{
  Serial.begin(9600);
  // Display status
  Serial.println("Initialized");
}

void loop()
{
  check1();
  check2();
  check3();
  check4();
}

void check1()
{
  int value;

  // Check the envelope input
  value = analogRead(PIN_ANALOG_IN);

  // Convert envelope value into a message
  Serial.print("Status: ");
  if(value <= 10)
  {
    Serial.println("Quiet on 1.");
  }
  else if( (value > 10) && ( value <= 30) )
  {
    Serial.println("Moderate on 1.");
  }
  else if(value > 30)
  {
    Serial.println("Loud on 1.");
  }

  // pause for 1 second
  delay(1000);
}
void check2() 
{
  int value;

  // Check the envelope input
  value = analogRead(PIN_ANALOG_IN2);

  // Convert envelope value into a message
  Serial.print("Status: ");
  if(value <= 10)
  {
    Serial.println("Quiet on 2.");
  }
  else if( (value > 10) && ( value <= 30) )
  {
    Serial.println("Moderate on 2.");
  }
  else if(value > 30)
  {
    Serial.println("Loud on 2.");
  }

  // pause for 1 second
  delay(100);
}
void check3() 
{
  int value;

  // Check the envelope input
  value = analogRead(PIN_ANALOG_IN3);

  // Convert envelope value into a message
  Serial.print("Status: ");
  if(value <= 10)
  {
    Serial.println("Quiet on 3.");
  }
  else if( (value > 10) && ( value <= 30) )
  {
    Serial.println("Moderate on 3.");
  }
  else if(value > 30)
  {
    Serial.println("Loud on 3.");
  }

  // pause for 1 second
  delay(100);
}
void check4() 
{
  int value;

  // Check the envelope input
  value = analogRead(PIN_ANALOG_IN4);

  // Convert envelope value into a message
  Serial.print("Status: ");
  if(value <= 10)
  {
    Serial.println("Quiet on 4.");
  }
  else if( (value > 10) && ( value <= 30) )
  {
    Serial.println("Moderate on 4.");
  }
  else if(value > 30)
  {
    Serial.println("Loud on 4.");
  }

  // pause for 1 second
  delay(100);
}
