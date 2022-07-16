#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd(0x3F, 20, 4);

void setup() 
{
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.print("Banks Aerospace, Inc."); 
  delay(2000);

  lcd.clear();
  lcd.print("  AutoBanks F-14");
  lcd.setCursor(0,1);
  lcd.print("     c. 2019");
  delay(2000);

  
}

void loop()
{
  // put your main code here, to run repeatedly:
  
  // main menu goes here.
  lcd.clear();
  lcd.print("     MAIN MENU");
  
  lcd.setCursor(0,1);
  lcd.print("1. Mode 1 Test");
  //delay (2000);
  
  //lcd.clear();
  lcd.setCursor(0,2);
  lcd.print("2. Mode 2 Test");
  
  lcd.setCursor(0,3);
  lcd.print("3. Mode 3/A Test");
  delay (2000);
  
}
