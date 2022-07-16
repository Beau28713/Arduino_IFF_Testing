
//  AutoBanks Avionics Test System
//  Performs self test of the AutoBanks System
//   and sets the parameters for testing 
//   the AN/APG-66 RADAR COMPUTER
 
// Includes
#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)write(args);
#else
#define printByte(args)print(args,BYTE);
# endif
 
// Constants  for Discretes
const int oPinP1Miss = 3;
const int oPinP2Miss = 4; 
const int oPin5vOK = 5;
const int oPin12vOK = 6;
const int oPin15vOK = 7;
const int oPinRadarReady = 8;
const int oPinACOG = 9;
const int oPinBus1 = 10; 
const int oPinBus2 = 11;
const int oPinBus3 = 12;
const int oPinBus4 = 13;
int pinNumber = 3;
 
// Constants for DIGIBUS output
const int oPinRady = A0;
const int oPinSB19 = A1;
const int oClk = A2;
const int oPinLPRFOK = A3;
const int oPinAConGND = A4;
const int oPinSquatTest = A5;
const int oPinAntCaged = A6;
const int oPinWrapOK = A7;
const int oPinBusAError = A8;
const int oPinBusBError = A9;
const int oPinPSHot = A10;
const int oPinMissingPhase = A11;
int pinDBNum = A0;

const int iMiss0 = 35;
const int iMiss1 = 36;
const int iGot5V = 32;
const int iGot12V = 33;
const int iGot15V = 34;
const int iRady = 37;
const int iOnGround = 31;
const int idigiBus = 40;
const int iSerialBus = 41;
const int iMuxA = 42;
const int iMuxB = 43;

                                                              
// LCD parameters and constants
uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
uint8_t note[8]  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t duck[8]  = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
uint8_t retarrow[8] = {  0x1,0x1,0x5,0x9,0x1f,0x8,0x4};
   
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x3F and 20 chars and 4 lines
 
// Setup all the test parameters for this series
void setup() 
{ 
    Serial.begin(9600);
    while(!Serial)
    {
      // wait for serial to connect.
    }
    
    // discretes
    for( pinNumber = 3; pinNumber <= 13; pinNumber++ )
    { 
      pinMode (pinNumber, OUTPUT);
       digitalWrite(pinNumber, LOW);
    } 
     
    //  DigiBus output pins  
    for( pinDBNum = A0; pinDBNum <= A11; pinDBNum++ )
    { 
       pinMode (pinDBNum, OUTPUT);
       analogWrite(pinDBNum, 0);
     }

   for ( pinNumber = 31; pinNumber < 36; pinNumber++ )
   {
      pinMode ( pinNumber, INPUT );
   }

   //this one is special. Lol.
   pinMode ( iMiss1, OUTPUT );

   for ( pinNumber = 37; pinNumber < 44; pinNumber++ )
   {
      pinMode ( pinNumber, INPUT );
   }
   
  //  LCD initialization code
  lcd.init();                
  lcd.backlight();
   
  lcd.createChar(0, bell);
  lcd.createChar(1, note);
  lcd.createChar(2, clock);
  lcd.createChar(3, heart);
  lcd.createChar(4, duck);
  lcd.createChar(5, check);
  lcd.createChar(6, cross);
  lcd.createChar(7, retarrow);
  lcd.home();

  // display Copywrite notice and version
  displayCopyrightInfo();
  
   // run panel test
   testPanel();
 
   // show the main menu
   displayMenu();
 
}  // end setup
 
//  Display copyright info
void displayCopyrightInfo(void)
{ 
  lcd.setCursor(6, 1);
  lcd.print("AutoBanks");
  lcd.setCursor(5, 2);
  lcd.print("Test System");
  lcd.setCursor(1, 3);
  lcd.print("  copyright 2019 ");
  lcd.setCursor(0, 4);
  lcd.print("Banks Aerospace, Inc");
  delay(4000);
}
  
// Display the menu
void  displayMenu(void)
{
  lcd.clear();
  lcd.print("     MAIN MENU");
  
  lcd.setCursor(0,1);
  lcd.print("1. Discrete I/O Tests");
 
  lcd.setCursor(0,2);
  lcd.print("2. DigiBus Tests");
  
  lcd.setCursor(0,3);
  lcd.print("3. 1553 Tests");
  delay (2000);

}

// display all keycodes
void displayKeyCodes(void) 
{
  uint8_t i = 0;
  while (1) 
  {
    lcd.clear();
    lcd.print("Codes 0x"); lcd.print(i, HEX);
    lcd.print("-0x"); lcd.print(i+16, HEX);
    lcd.setCursor(0, 1);
    for (int j=0; j<16; j++) 
    {
      lcd.printByte(i+j);
    }
    i+=16;
    
    delay(4000);
  }
}
void doDiscretes()
{
  int val = 0; //for carrying the read value to the write value  
  
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("Testing Discretes"); 
  lcd.setCursor(2,2);
  lcd.print("Testing: P/S");
  for(pinNumber = 3; pinNumber <= 13; pinNumber++)
  {
         digitalWrite(pinNumber, HIGH);
         delay (500);
         digitalWrite(pinNumber, LOW);
         if(pinNumber == 8)
         {
            lcd.setCursor(2,2);
            lcd.print("Testing: RADAR");
         }
         if( pinNumber == 10)
         {
            lcd.setCursor(2,2);
            lcd.print("Testing: Bus Comms");
         }
  }

  //measure the pins available and update the LEDs
  val = digitalRead(iMiss0);
  if (val==HIGH)
  {
    digitalWrite(oPinP1Miss, LOW);  // this code clears the missing phase LEDs on the front panel.
    digitalWrite(oPinP2Miss, LOW);  // and mark them both good
  }else
  {
    digitalWrite(oPinP1Miss, HIGH); // Otherwise indicate missing phase on front panel LEDs.
    digitalWrite(oPinP2Miss, HIGH); // Mark them both bad.
  }
  digitalWrite(iMiss1, HIGH);       //assert +5 VDC to simulate successful 3 phase power on other LRUs
  
  val = digitalRead(iGot5V);        // DB25(2), Wire Num 12
  digitalWrite(oPin5vOK, val);

  val = digitalRead(iGot12V);       // DB25(3), Wire num 11
  digitalWrite(oPin12vOK, HIGH);

  val = digitalRead(iGot15V);       // DB25(4), Wire num 10
  digitalWrite(oPin15vOK, val);

  val = digitalRead(iRady);         // DB25(7), Wire num 7
  digitalWrite(oPinRadarReady, val);

  val = digitalRead(iOnGround);     // DB25(1), Wire num 13
  digitalWrite(oPinACOG, val);

  val = digitalRead(idigiBus);
  digitalWrite(oPinBus1, val);

  val = digitalRead(iSerialBus);
  digitalWrite(oPinBus2, val);

  val = digitalRead(iMuxA);
  digitalWrite(oPinBus3, val);

  val = digitalRead(iMuxB);
  digitalWrite(oPinBus4, val);

//iMiss0 iMiss1 iGot5V iGot12v iGot15V iRady 
//iOnGround idigiBus iSerialBus iMuxA iMuxB
//oPinP1Miss oPinP2Miss oPin5vOK oPin12vOK oPin15vOK
//oPinRadarReady oPinACOG oPinBus1 oPinBus2 oPinBus3 oPinBus4
  
  delay (2000);
  displayMenu();
}

void doDigibus(void)
{
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("Testing Digibus"); 
  lcd.setCursor(2,2);
  lcd.print("Not Implemented");
  delay(4000);
  displayMenu();
}

void do1553(void)
{
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("Testing 1553"); 
  lcd.setCursor(2,2);
  lcd.print("Watch the PC");
  delay(2000);
  displayMenu();
}

void doError(void)
{
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("Command not"); 
  lcd.setCursor(2,2);
  lcd.print("understood");
  delay(2000);
  displayMenu();
}

void testPanel(void)
{
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("Testing front panel"); 
  lcd.setCursor(2,2);
  lcd.print("Testing: P/S");
  
  for(pinNumber = 3; pinNumber <= 13; pinNumber++)
  {
         digitalWrite(pinNumber, HIGH);
         delay (500);
         digitalWrite(pinNumber, LOW);
         if(pinNumber == 8)
         {
            lcd.setCursor(2,2);
            lcd.print("Testing: RADAR");
         }
         if( pinNumber == 10)
         {
            lcd.setCursor(2,2);
            lcd.print("Testing: Bus Comms");
         }
  }
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("Testing front panel"); 
  lcd.setCursor(2,2);
  lcd.print("Testing:");
  lcd.setCursor(3,3);
  lcd.print("CPU Serial Bus");

  for(pinDBNum = A0; pinDBNum <= A11; pinDBNum++)
  {
       analogWrite(pinDBNum, 2400);
       delay (500);
       analogWrite(pinDBNum, 0);
  }
  delay (1000);
  displayMenu();
}

/*********************************
// MAIN LOOP //
*********************************/
void loop() 
{
  // local loop variable to store typed in characters.
  char newChar;
    
  // when characters arrive over the serial port...
  if (Serial.available()) 
  {
    // wait a bit for the entire message to arrive
    delay(100);
    
    // clear the screen
    lcd.clear();
    
    // read all the available characters
    while (Serial.available() > 0) 
    {
      newChar = Serial.read();
      // display the character to the LCD
      lcd.write(newChar);
      Serial.write(newChar);
    }
  
      // Respond to the character typed in.
      switch (newChar)
      {
        case '1':
          doDiscretes();
          break;
        case '2':
          doDigibus();
          break;
        case '3':
          do1553();
          break;
        default:
          doError();
      }
      
    }
    delay(500);
    
}
