// testshapes demo for RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 32x64 RGB LED matrix.

// NOTE THIS CAN ONLY BE USED ON A MEGA! NOT ENOUGH RAM ON UNO!

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

// Rotary Encoder values

const int buttonPin = 2;     // the number of the pushbutton pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;
volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;

// LED MATRIX PINS
#define OE  11 //9
#define LAT 12 //10
#define CLK 13 //11
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

// Displays required
/*

MAIN DISPLAYS

  Bar graph all six sensors with temperature windows (i.e. Pizza)

  All six sensor readings

  Single (pair) of sensors readings (change colour to preset temperature window i.e. pizza)




SETUP DISPLAYS

  Sensor Selection

    Sensor
      1 
      2
      3
      4
      5
      6

*/

char sensor_sel[6] = { '1', '2', '3', '4', '5', '6' };

/*

  Display Setting

    Bargraph
    All sensors
    Single Sensor
    Pair Sensor
*/

char display_sel[4] = { 'BARGRAPH', 'ALL', 'SINGLE', 'PAIR' };

/*    

  Temperature presets (TEMP SETS)

    Pizza (PIZZA 350-400)
    Roast (ROAST 220-240)
    Slow cook (SLOW 180-140)
    Preset Off
*/

char temp_sel[4] = { 'PIZZA', 'ROAST', 'SLOW', 'OFF' };

/*
  Data Logging (DATA LOG)

    MYSQL LOG
      ON
      OFF


*/

char log_sel[2] = { 'ON', 'OFF' };

/*

  Wireless Data Control

      ON
      OFF

*/

void setup() {

    // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT); 
  
   Serial.begin (9600);

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE); 


  matrix.begin();
  
  // draw a pixel in solid white
  matrix.drawPixel(0, 0, matrix.Color333(7, 7, 7)); 
  delay(500);

  // fix the screen with green
  matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 7, 0));
  delay(500);

  // draw a box in yellow
  matrix.drawRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(7, 7, 0));
  delay(500);
  
  // draw an 'X' in red
  matrix.drawLine(0, 0, matrix.width()-1, matrix.height()-1, matrix.Color333(7, 0, 0));
  matrix.drawLine(matrix.width()-1, 0, 0, matrix.height()-1, matrix.Color333(7, 0, 0));
  delay(500);
  
  // draw a blue circle
  matrix.drawCircle(10, 10, 10, matrix.Color333(0, 0, 7));
  delay(500);
  
  // fill a violet circle
  matrix.fillCircle(40, 21, 10, matrix.Color333(7, 0, 7));
  delay(500);
  
  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  
  // draw some text!
  matrix.setTextSize(1);     // size 1 == 8 pixels high
  matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves

  matrix.setCursor(8, 0);    // start at top left, with 8 pixel of spacing
  uint8_t w = 0;
  char *str = "AdafruitIndustries";
  for (w=0; w<8; w++) {
    matrix.setTextColor(Wheel(w));
    matrix.print(str[w]);
  }
  matrix.setCursor(2, 8);    // next line
  for (w=8; w<18; w++) {
    matrix.setTextColor(Wheel(w));
    matrix.print(str[w]);
  }
  matrix.println();
  //matrix.setTextColor(matrix.Color333(4,4,4));
  //matrix.println("Industries");
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.println("LED MATRIX!");
  
  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print('3');
  matrix.setTextColor(matrix.Color333(7,4,0)); 
  matrix.print('2');
  matrix.setTextColor(matrix.Color333(7,7,0));
  matrix.print('x');
  matrix.setTextColor(matrix.Color333(4,7,0)); 
  matrix.print('6');
  matrix.setTextColor(matrix.Color333(0,7,0));  
  matrix.print('4');
  matrix.setCursor(34, 24);  
  matrix.setTextColor(matrix.Color333(0,7,7)); 
  matrix.print("*");
  matrix.setTextColor(matrix.Color333(0,4,7)); 
  matrix.print('R');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('G');
  matrix.setTextColor(matrix.Color333(4,0,7)); 
  matrix.print("B");
  matrix.setTextColor(matrix.Color333(7,0,4)); 
  matrix.println("*");


  // whew!
}

void loop() {
  // do nothing


      // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  
  Serial.println(encoderValue);

  
}


// Input a value 0 to 24 to get a color value.
// The colours are a transition r - g - b - back to r.
uint16_t Wheel(byte WheelPos) {
  if(WheelPos < 8) {
   return matrix.Color333(7 - WheelPos, WheelPos, 0);
  } else if(WheelPos < 16) {
   WheelPos -= 8;
   return matrix.Color333(0, 7-WheelPos, WheelPos);
  } else {
   WheelPos -= 16;
   return matrix.Color333(0, WheelPos, 7 - WheelPos);
  }
}



void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; //store this value for next time
}
