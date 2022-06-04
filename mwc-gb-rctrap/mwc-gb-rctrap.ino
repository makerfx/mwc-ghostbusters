//todo - cycle the sfx off after 15 seconds because it stops automatically.

/*
* Magic Wheelchair RC Ghostbusters trap
* Based on work found in this video: https://www.youtube.com/watch?v=rYxwIrqE7q8
* and found in this GH repo: https://github.com/AssistiveTechStuff/AdaptiveRCController
*
* Important note for the SPEKTRUM receiver:
*  ppm[0] = channel 2
*  ppm[1] = channel 3
*  ppm[2] = channel 4
*  ppm[3] = channel 1
*  
*/

#define HEADLIGHT_ON_PPM 1900
#define HEADLIGHT_OFF_PPM 1000
#define SFX_ON_PPM 1900
#define SFX_OFF_PPM 1000



#include <EEPROM.h>
#include <Metro.h> //Include Metro library 

Metro blinkMetro = Metro(500);
Metro lcdKeyMetro = Metro(200);

bool headlightBlink = false;

bool headlightOn = false;
bool sfxOn = false;
bool steeringMode = false; //changed with the start button

/*
 * Download USB Host Shield Library First
 * https://github.com/felis/USB_Host_Shield_2.0/archive/master.zip
 */


// Includes for XBOX Controller
// https://github.com/felis/USB_Host_Shield_2.0/archive/master.zip
#include <XBOXONE.h>
USB Usb;
XBOXONE Xbox(&Usb);

// Includes and Defines for Adafruit RGB LCD Shield
// https://github.com/adafruit/Adafruit-RGB-LCD-Shield-Library/archive/refs/heads/master.zip
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
// Define RGB LCD Colours
#define RED 0x1
#define GREEN 0x2

// Setup some variables
int steeringTrim = 0;
int steeringTrimLCD = 0;
int speedSetting = 1;
int speedMax = 1590;
int speedMin = 1410;

////////////// PPM
/*
 * PPM generator originally written by David Hasko
 * on https://code.google.com/p/generate-ppm-signal/
 */

//////////////////////CONFIGURATION///////////////////////////////
#define CHANNEL_NUMBER 8  //set the number of chanels
#define CHANNEL_DEFAULT_VALUE 1500  //set the default servo value
#define FRAME_LENGTH 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PULSE_LENGTH 300  //set the pulse length
#define onState 1  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 3  //set PPM signal output pin on the arduino
//////////////////////////////////////////////////////////////////

#define CHANNEL_TO_MODIFY 11
#define SWITCH_STEP 100

/*this array holds the servo values for the ppm signal
 change these values in your code (usually servo values move between 1000 and 2000)*/
int ppm[CHANNEL_NUMBER];

int currentChannelStep;
uint8_t state = 0;


void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox USB Library Started"));

///////////////////////////////
//  previousSwitchValue = HIGH;

  //initiallize default ppm values
  for(int i=0; i<CHANNEL_NUMBER; i++){
    if (i == 2 || i == CHANNEL_TO_MODIFY) {
      ppm[i] = 1000;
    } else {
      ppm[i]= CHANNEL_DEFAULT_VALUE;
    }
  }


  steeringMode = EEPROM.read(0);
  
  
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)


  // Set up the LCD's number of columns and rows and display static text
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Who  You  Gonna");
  lcd.setCursor(0, 1);
  lcd.print("Call?    Carter!");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0, 0);
 lcd.print("Steer trim: 0     ");
  lcd.setCursor(0, 1);
  lcd.print("Speed: ");
  lcd.print(speedSetting);
  
  steeringModeRefreshLCD();
  functionsRefreshLCD();
  
  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;

  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();

  currentChannelStep = SWITCH_STEP;
}

void lcdButtonCheck() {
  uint8_t buttons = lcd.readButtons();

  if (buttons) {
    if (buttons & BUTTON_UP) {
      Serial.println("UP");
    }
    if (buttons & BUTTON_DOWN) {
      Serial.println("DOWN");
    }
    if (buttons & BUTTON_LEFT) {
      Serial.println("LEFT");
      steeringMode = 1;
      steeringModeRefreshLCD();
    }
    if (buttons & BUTTON_RIGHT) {
      Serial.println("RIGHT");
      steeringMode = 0;
      steeringModeRefreshLCD();
    }
    if (buttons & BUTTON_SELECT) {
      Serial.println("SELECT");
    }
  }
  
}

void headlight(bool state) {
  if (state) {
    headlightOn = 1;
    ppm[3]=HEADLIGHT_ON_PPM;
    Serial.println("Headlight ON");
  }
  else {
    headlightOn = 0;
    ppm[3]=HEADLIGHT_OFF_PPM;
    Serial.println("Headlight OFF");
  }
  functionsRefreshLCD();
}

void headlightToggle(){
  if (!headlightOn) return;
  if (!headlightBlink) return;
  
  if (ppm[3] != HEADLIGHT_ON_PPM) {
          ppm[3]=HEADLIGHT_ON_PPM;
        }
        else {
          ppm[3]=HEADLIGHT_OFF_PPM;
        }  
}

void loop() {
  Usb.Task();

 

  if (blinkMetro.check() == 1) headlightToggle();      
  if (lcdKeyMetro.check() == 1)  lcdButtonCheck();     
 
  
  if (Xbox.XboxOneConnected) {

    // Steering
    // Attached to Xbox Left Joystick X Axis (to invert swap the 1000 and 2000 in the map function)

    // First calculate trim
    // Steering trim adjusted by:
    //    Quadstick: L1 (i.e. left puff) or L3 (i.e. right puff) as these are available buttons with Quadstick plugged into XAC left USB, as per default XAC config
    //    XAC direct: Left and Right D-Pad
    //    XBOX Controller: L1 and R1 (bumpers)

    // Steering trim reset to zero by pressing either Back or Start. On Quadstick plugged into XAC left USB, this is either left sip or right sip

    if (Xbox.getButtonClick(L1) || Xbox.getButtonClick(LEFT)){ // Incrementally trim steering to turn left
      steeringTrim = steeringTrim - 5;
      steeringTrimRefreshLCD();
    }
    if (Xbox.getButtonClick(R1) || Xbox.getButtonClick(RIGHT) || Xbox.getButtonClick(L3)){ // Incrementally trim steering to turn right
      steeringTrim = steeringTrim + 5;
      steeringTrimRefreshLCD();
    }
    if (Xbox.getButtonClick(BACK)) { // Reset steering trim to 0
      steeringTrim = 0;
      steeringTrimRefreshLCD();
    }

    if (Xbox.getButtonClick(START)){ // Switch Steering Mode

    }

    if (steeringMode) {
      if (Xbox.getAnalogHat(LeftHatX)) {
        if (Xbox.getAnalogHat(LeftHatX) > 2500 || Xbox.getAnalogHat(LeftHatX) < -2500) {
          ppm[0] = map(Xbox.getAnalogHat(LeftHatX), 32768 , -32768, 1000, 2000) + steeringTrim;
        } else {
          ppm[0] = 1500 - steeringTrim;
        }
      }
    }
    else {
      if (Xbox.getAnalogHat(RightHatX)) {
        if (Xbox.getAnalogHat(RightHatX) > 2500 || Xbox.getAnalogHat(RightHatX) < -2500) {
          ppm[0] = map(Xbox.getAnalogHat(RightHatX), 32768 , -32768, 1000, 2000) + steeringTrim;
        } else {
          ppm[0] = 1500 - steeringTrim;
        }
      }
    }
    // Throttle
    // Attached to Xbox Left Joystick Y Axis
    // Allows adjustment of the speed range to allow a lower maximum speed to accommodate beginners
    // 'IF' statements provide a deadband so that any movement between 0 and 2500 (out of possible 32768) outputs zero throttle (i.e. PPM 1500)

    if (Xbox.getButtonClick(UP) && speedSetting < 4) {
      speedSetting = speedSetting + 1;
      Serial.println(speedSetting);
      speedSettingRefreshLCD();
    }

    if (Xbox.getButtonClick(DOWN) && speedSetting > 1) {
      speedSetting = speedSetting - 1;
      Serial.println(speedSetting);
      speedSettingRefreshLCD();
    }

    if (speedSetting == 1) {
      //speedMax = 1590;
      //speedMin = 1410;
      speedMax = 1650;
      speedMin = 1350;      
    }

    if (speedSetting == 2) {
      speedMax = 1700;
      speedMin = 1300;
    }

    if (speedSetting == 3) {
      speedMax = 1850;
      speedMin = 1150;
    }

    if (speedSetting == 4) {
      speedMax = 2000;
      speedMin = 1000;
    }

    if (Xbox.getAnalogHat(LeftHatY)) {
      if (Xbox.getAnalogHat(LeftHatY) > 2500 || Xbox.getAnalogHat(LeftHatY) < -2500) {
        ppm[1] = map(Xbox.getAnalogHat(LeftHatY), -32768 , 32768, speedMin, speedMax);
      } else {
        ppm[1] = 1500;
      }
    }

// All available XBOX Buttons - placed here for reference
    if (Xbox.getButtonClick(UP))
      Serial.println(F("Up"));
    if (Xbox.getButtonClick(DOWN))
      Serial.println(F("Down"));
    if (Xbox.getButtonClick(LEFT))
      Serial.println(F("Left"));
    if (Xbox.getButtonClick(RIGHT))
      Serial.println(F("Right"));

    if (Xbox.getButtonClick(START))
      Serial.println(F("Start"));
    if (Xbox.getButtonClick(BACK))
      Serial.println(F("Back"));
    if (Xbox.getButtonClick(XBOX))
      Serial.println(F("Xbox"));
 /* 
    if (Xbox.getButtonClick(SYNC))
      Serial.println(F("Sync"));


    if (Xbox.getButtonClick(L1))
      Serial.println(F("L1"));
    if (Xbox.getButtonClick(R1))
      Serial.println(F("R1"));
    if (Xbox.getButtonClick(L2))
      Serial.println(F("L2"));
    if (Xbox.getButtonClick(R2))
      Serial.println(F("R2"));
    if (Xbox.getButtonClick(L3))
      Serial.println(F("L3"));
    if (Xbox.getButtonClick(R3))
      Serial.println(F("R3"));
*/

    if (Xbox.getButtonClick(A)) {
        if (sfxOn == false) {
          sfxOn = true;
          ppm[2]=SFX_ON_PPM;
          Serial.println(F("SFX ON"));
        }
        else {
          sfxOn = false;
          ppm[2]=SFX_OFF_PPM;
          Serial.println(F("SFX OFF"));
        }
        functionsRefreshLCD();
      }

    if (Xbox.getButtonClick(B)) {
      Serial.println(F("B"));
      headlightBlink = !headlightBlink; 
      if (!headlightOn) headlight(true);  //turned on blink, but the headlight isn't on, so turn it off
      if (!headlightBlink && headlightOn) headlight(false);   //turned off blink, turn off the headlight too
      functionsRefreshLCD();
    }
      
      
    if (Xbox.getButtonClick(X))
      Serial.println(F("X"));

    //y is headlight
    if (Xbox.getButtonClick(Y)) {
       if (headlightOn) {
        headlight(false);
        headlightBlink = false;
       }
       else headlight(true);
    }

//    if (Xbox.getButtonPress(L2) > 0 || Xbox.getButtonPress(R2) > 0) {
//      if (Xbox.getButtonPress(L2) > 0) {
//        Serial.print(F("L2: "));
//        Serial.print(Xbox.getButtonPress(L2));
//        Serial.print("\t");
//      }
//      if (Xbox.getButtonPress(R2) > 0) {
//        Serial.print(F("R2: "));
//        Serial.print(Xbox.getButtonPress(R2));
//        Serial.print("\t");
//      }
//      Serial.println();
//    }


  }

  if (Xbox.XboxOneConnected == false){
    lcd.setBacklight(RED);
    ppm[1] = 1500; // if controller is disconnected for some reason, throttle gets zeroed
  } else {
    lcd.setBacklight(RED);
  }

//Uncomment these for debugging if you want to see the ppm value for each channel
//Note: uncommenting these can disrupt the ppm signal so should be re-commented before actual use


/*
Serial.print("CH1: ");Serial.print(ppm[0]);Serial.print("\t");
Serial.print("CH2: ");Serial.print(ppm[1]);Serial.print("\t");

Serial.print("CH3: ");Serial.print(ppm[2]);Serial.print("\t");
Serial.print("CH4: ");Serial.print(ppm[3]);Serial.print("\t");

Serial.print("\n");

Serial.print("CH5: ");Serial.print(ppm[4]);Serial.print("\t");
Serial.print("CH6: ");Serial.print(ppm[5]);Serial.print("\t");
Serial.print("CH7: ");Serial.print(ppm[6]);Serial.print("\t");
Serial.print("CH8: ");Serial.print(ppm[7]);Serial.print("\n");


Serial.println(speedSetting);
*/
}

ISR(TIMER1_COMPA_vect){  //leave this alone
  static boolean state = true;

  TCNT1 = 0;

  if (state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PULSE_LENGTH * 2;
    state = false;
  } else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;

    digitalWrite(sigPin, !onState);
    state = true;

    if(cur_chan_numb >= CHANNEL_NUMBER){
      cur_chan_numb = 0;
      calc_rest = calc_rest + PULSE_LENGTH;//
      OCR1A = (FRAME_LENGTH - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (ppm[cur_chan_numb] - PULSE_LENGTH) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }
  }
}

// Function to update steering trim setting information on LCD
// Converts steeringTrimLCD from negative and positive numbers to L and R
void steeringTrimRefreshLCD() {
  Serial.println(steeringTrim);
  steeringTrimLCD = map(steeringTrim, -100, 100, -20, 20); // Remap steering trim adjustment to single increments
  if (steeringTrimLCD < 0) {
    steeringTrimLCD = abs(steeringTrimLCD);
    lcd.setCursor(12, 0);
    lcd.print(String(steeringTrimLCD) + "L ");
  } else if (steeringTrimLCD > 0) {
    lcd.setCursor(12, 0);
    lcd.print(String(steeringTrimLCD) + "R ");
  } else {
    lcd.setCursor(12, 0);
    lcd.print(String(steeringTrimLCD) + "  ");
  }
}
void speedSettingRefreshLCD() {
  lcd.setCursor(7, 1);
  lcd.print(String(speedSetting));
}

void functionsRefreshLCD() {
  lcd.setCursor(13, 1);
  if (headlightOn) lcd.print("H");
  else lcd.print(" ");
  if (headlightBlink) lcd.print("B");
  else lcd.print(" ");
  if (sfxOn) lcd.print("S");
  else lcd.print(" ");
  
  
}
void steeringModeRefreshLCD() {
  EEPROM.write(0, steeringMode);

  lcd.setCursor(15,0);
  if (steeringMode) 
    lcd.print("L");
  else lcd.print("R");
}
  
