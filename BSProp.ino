#include <Time.h>
#include <Wire.h> 
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
//#include <LiquidCrystal.h>
#include <SD.h>
#include <Adafruit_NeoPixel.h>

/*
 Blacksheep MilSim Prop

 Based largly off of
 Arduino Bomb Pro - http://yin.mainstreamds.com/
 original creator yinbot@gmail.com
 
 Original coding created 4,Sep, 2010
 Modified 24 May 2014
 by Ignacio Lillo
 
 Edited by:
 Zach Hanson
 13 Nov 2014
 
 */

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
//LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {
    '1','2','3','a'                          }
  ,
  {
    '4','5','6','b'                          }
  ,
  {
    '7','8','9','c'                          }
  ,
  {
    '*','0','#','d'                          }
};

byte rowPins[ROWS] = {
  A4, A5, 13, 12}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  A0, A1, A2, A3
}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


char enteredText[8];
byte time[4];
byte refresh=0;//1 if is refreshed once time...
char password[8];
int key=-1;
char lastKey;
char var;
boolean passwordEnable=false;

//Buttons for lcd shield
char BT_RIGHT = '4';
char BT_UP = 'a';
char BT_DOWN = 'b';
char BT_LEFT = '6';
char BT_SEL = 'd';   // Ok key  
char BT_CANCEL = 'c';
char BT_DEFUSER = 'x';   // not implemented

//leds

Adafruit_NeoPixel strip = Adafruit_NeoPixel(133, 6, NEO_GRB + NEO_KHZ800);
const int YELLOWLED = 11;
const int GREENLED = 10;
//const int BLUELED = 12;
//mosfet
boolean mosfetEnable = false;
const int mosfet = 9;
//IS VERY IMPORTANT THAT YOU TEST THIS TIME. BY DEFAULT IS IN 1 SEC. THAT IS NOT TOO MUCH. SO TEST IT!
const int MOSFET_TIME = 5000;

//TIME INTS
int GAMEHOURS = 0;
int GAMEMINUTES = 45;
int BOMBMINUTES = 4;
int ACTIVATESECONDS = 5;

boolean endGame = false;

boolean doStatus = false; //for Demolition
boolean sdStatus = false; //search and destroy game enable used in config
boolean saStatus = false; //same but Sabotage
boolean start = true;
boolean defuseando;
boolean cancelando;
// SOUND TONES
boolean soundEnable = true;
int tonepin = 8; // Pin 13 for the sound
int tonoPitido = 3000;
int tonoAlarma1 = 700;
int tonoAlarma2 = 2600;
int tonoActivada = 1330;
int errorTone = 100;

unsigned long iTime;
unsigned long timeCalcVar;
unsigned long alphaTime;
unsigned long bravoTime;
unsigned long iZoneTime;//initial time for zone
unsigned long timea;
unsigned long timeb;
byte team=0; // 0 = neutral, 1 = alpha team, 2 = bravo team

void setup(){
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines
  Serial.begin(9600);
  //  lcd.init();                      // initialize the lcd 
  //  lcd.backlight();
  tone(tonepin,2400,30);
  lcd.setCursor(1,0);  
  lcd.print("BLACKSHEEP MILSIM");// you can add your team name or someting cool
  lcd.setCursor(0,1);
  lcd.print("DIGITAL PROP SYSTEM");// you can add your team name or someting cool
  lcd.setCursor(0,2);
  lcd.print("V0.2 By Zach Hanson");// you can add your team name or someting cool
  lcd.setCursor(2,3);
  lcd.print("Push ANY Button");
  keypad.waitForKey();
  keypad.setHoldTime(50);
  keypad.setDebounceTime(50);
  keypad.addEventListener(keypadEvent);
  delay(400);
  pinMode(GREENLED, OUTPUT);     
//  pinMode(8, OUTPUT);  
//  digitalWrite(8,HIGH);
  pinMode(YELLOWLED, OUTPUT); 
  pinMode(mosfet, OUTPUT);  
  // CONFIGURE THE BARS OF PROGRESS BAR
  byte bar1[8] = {
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
  };
  byte bar2[8] = {
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
  };
  byte bar3[8] = {
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
  };
  byte bar4[8] = {
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
  };
  byte bar5[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  byte up[8] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B11111,
    B00000,
    B00000,
  };

  byte down[8] = {
    B00000,
    B00000,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000,
  };
  lcd.createChar(0,bar1);
  lcd.createChar(1,bar2);
  lcd.createChar(2,bar3);
  lcd.createChar(3,bar4);
  lcd.createChar(4,bar5);
  lcd.createChar(5,up);
  lcd.createChar(6,down);
}

void loop(){
  menuPrincipal();
}
void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
    case PRESSED:
      switch (key){

      }
    break;
    case RELEASED:
      switch (key){
         case 'd': defuseando= false;
         //Serial.println("d Releases");
         break;
         case 'c': cancelando=false;
         //Serial.println("c Releases");
         break;
      }
    break;
    case HOLD:
      switch (key){
        case 'd': defuseando= true;
        //Serial.println("d hold");
        break;
        case 'c': cancelando=true;
        //Serial.println("c hold");
        break;
      }
    break;
  }
}

void disarmedSplash(){
  endGame = false;
  digitalWrite(YELLOWLED, LOW); 
  digitalWrite(GREENLED, LOW);
  if(sdStatus || saStatus){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("BOMB DISARMED");
    lcd.setCursor(3,1);
    lcd.print("GOODS WIN");
    digitalWrite(GREENLED, HIGH);  
    delay(5000);
    digitalWrite(GREENLED, LOW); 
  }
  //end code
  lcd.clear();
  lcd.print("Play Again?");
  lcd.setCursor(0,1);
  lcd.print("A : Yes B : No");
  digitalWrite(YELLOWLED, LOW);  
  digitalWrite(GREENLED, LOW); 
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      //We have two options, search & destroy and sabotage play again options so!
      if(sdStatus){
        startGameCount();
        search();
      }
      if(saStatus){
        saStatus=true;
        startGameCount();
        start=true; //to set iTime to actual millis() :D
        sabotage();
      }
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      menuPrincipal();
      break;
    }  
  } 
}

void explodeSplash(){
  digitalWrite(YELLOWLED, LOW);  
  digitalWrite(GREENLED, LOW); 
  cls();
  delay(100);
  endGame = false;
  lcd.setCursor(1,0);
  lcd.print("TERRORISTS WIN");
  lcd.setCursor(4,1);
  lcd.print("GAME OVER");
  for(int i = 200; i>0; i--)// this is the ultra hi definition explosion sound xD
  {
    tone(tonepin,i);
    delay(20);
  }
  noTone(tonepin);
  if(mosfetEnable){
    activateMosfet(); 
  }
  delay(5000);
  cls();

  //end code
  lcd.print("Play Again?");
  lcd.setCursor(0,1);
  lcd.print("A : Yes B : No");
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      //We have two options, search & destroy and sabotage play again options so!
      if(sdStatus){
        startGameCount();
        search();
      }
      if(saStatus){
        saStatus=true;
        startGameCount();
        start=true; //to set iTime to actual millis() :D
        sabotage();
      }
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      menuPrincipal();

      break;
    }  
  } 
}

