int micIN= A0;
int audioVal = 0;

#include <LiquidCrystal.h>
LiquidCrystal lcd(0, 1, 8, 9, 10, 11); //Initialize LCD




//A function that is called in the main loop and is the code that makes the screen continue running
void screen() {

  lcd.setCursor(0, 0); // set the cursor to column 0, line 2
  delay(200);//delay of 0.3sec

  lcd.scrollDisplayRight();//shifting data on LCD

  lcd.setCursor(0, 0);// set the cursor to column 0, line1
}
void setup() {

  lcd.begin(16, 2);
  Serial.begin(9600);
  delay(1000); 

}

void loop() {
 //audioVal = analogRead(micIN);
 //Serial.println(audioVal);
 //delay(600);
  //screen();
  
  writeToScreen("Hearing a sound:","     300 Hz     ");
}

void writeToScreen(String line1, String line2) {
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
    
}

