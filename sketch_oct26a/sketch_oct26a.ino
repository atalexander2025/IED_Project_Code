//headers
#include <Adafruit_AS7341.h>
#include <Wire.h>

float kelvin_table[120][3] = {
    {255, 56, 0},
    {255, 71, 0},
    {255, 83, 0},
    {255, 93, 0},
    {255, 101, 0},
    {255, 109, 0},
    {255, 115, 0},
    {255, 121, 0},
    {255, 126, 0},
    {255, 131, 0},
    {255, 138, 18},
    {255, 142, 33},
    {255, 147, 44},
    {255, 152, 54},
    {255, 157, 63},
    {255, 161, 72},
    {255, 165, 79},
    {255, 169, 87},
    {255, 173, 94},
    {255, 177, 101},
    {255, 180, 107},
    {255, 184, 114},
    {255, 187, 120},
    {255, 190, 126},
    {255, 193, 132},
    {255, 196, 137},
    {255, 199, 143},
    {255, 201, 148},
    {255, 204, 153},
    {255, 206, 159},
    {255, 209, 163},
    {255, 211, 168},
    {255, 213, 173},
    {255, 215, 177},
    {255, 217, 182},
    {255, 219, 186},
    {255, 221, 190},
    {255, 223, 194},
    {255, 225, 198},
    {255, 227, 202},
    {255, 228, 206},
    {255, 230, 210},
    {255, 232, 213},
    {255, 233, 217},
    {255, 235, 220},
    {255, 236, 224},
    {255, 238, 227},
    {255, 239, 230},
    {255, 240, 233},
    {255, 242, 236},
    {255, 243, 239},
    {255, 244, 242},
    {255, 245, 245},
    {255, 246, 247},
    {255, 248, 251},
    {255, 249, 253},
    {254, 249, 255},
    {252, 247, 255},
    {249, 246, 255},
    {247, 245, 255},
    {245, 243, 255},
    {243, 242, 255},
    {240, 241, 255},
    {239, 240, 255},
    {237, 239, 255},
    {235, 238, 255},
    {233, 237, 255},
    {231, 236, 255},
    {230, 235, 255},
    {228, 234, 255},
    {227, 233, 255},
    {225, 232, 255},
    {224, 231, 255},
    {222, 230, 255},
    {221, 230, 255},
    {220, 229, 255},
    {218, 229, 255},
    {217, 227, 255},
    {216, 227, 255},
    {215, 226, 255},
    {214, 225, 255},
    {212, 225, 255},
    {211, 224, 255},
    {210, 223, 255},
    {209, 223, 255},
    {208, 222, 255},
    {207, 221, 255},
    {207, 221, 255},
    {206, 220, 255},
    {205, 220, 255},
    {207, 218, 255},
    {207, 218, 255},
    {206, 217, 255},
    {205, 217, 255},
    {204, 216, 255},
    {204, 216, 255},
    {203, 215, 255},
    {202, 215, 255},
    {202, 214, 255},
    {201, 214, 255},
    {200, 213, 255},
    {200, 213, 255},
    {199, 212, 255},
    {199, 212, 255},
    {198, 212, 255},
    {198, 212, 255},
    {198, 212, 255},
    {198, 212, 255},
    {197, 211, 255},
    {197, 211, 255},
    {197, 211, 255},
    {197, 211, 255},
    {197, 210, 255},
    {197, 210, 255},
    {196, 210, 255},
    {196, 210, 255},
    {195, 210, 255},
    {195, 210, 255},
    {195, 209, 255},
    {195, 209, 255}};

// #include "look_up.h";

Adafruit_AS7341 sensor;


//These range from 0 to 1023
const int cycleDurationPin = A0;
float cycleDurationRaw = 0;

const int intensityMaxSliderPin = A1;
float intensityMaxDataRaw = 0;

const int intensityVarSliderPin = A2;
float intensityVarDataRaw = 0;

const int tempDialPin = A3;
float tempDataRaw = 0;


//through Qwiic
float lightSenorDataRaw = 0;
//clock input
//LED driver


//digital Pins
const int powerPin = 7;
int powerToggle = 0;

const int resetTogglePin = 13;
int resetToggle = 0;


////////////output Readings/////////////

//PWM range 0 - 255
const int coldTempPin = 3;  //3000K
float coldTempValue = 0;
float coldTempValueRaw = 0;

const int hotTempPin = 5;  //6000K (5500K)
float hotTempValue = 0;
float hotTempValueRaw = 0;

float intensityMax = 0;
float intensityVar = 0;
float cycleDuration = 0;
float tempData = 0;

///////////////////////////////////////

//Time stuff here//////
float t = 0.2;  //temp



///Color Stuff Here///
int uInt16 = 65535;
float color[3];
float rawNM[8];
float finalKelvin;
double CIE_1931_lookup[8][3] = {
  { 0.07763, 0.00218, 0.3713 },
  { 0.3481, 0.0298, 1.783 },
  { 0.09564, 0.139, 0.813 },
  { 0.0291, 0.6082, 0.1117 },
  { 0.5121, 0.9, 0.00575 },
  { 1.026, 0.757, 0.0011 },
  { 0.6424, 0.265, 0.00005 },
  { 0.04677, 0.017, 0.0 }
};

//Select NM: //
/*
415,
445,
480,
515,
555,
590,
630,
680
*/

void setup() {
  Serial.begin(115200);

  pinMode(powerPin, OUTPUT);  // sets the digital pin 13 as output
  pinMode(resetTogglePin, OUTPUT);

  Wire1.begin();
 


  // Wait for communication with the host computer serial monitor
  while (!Serial) {
    delay(1);
  }
  
  if (!sensor.begin(AS7341_I2CADDR_DEFAULT, &Wire1)){
    Serial.println("Could not find AS7341");
    while (1) { delay(10); }
  }

  sensor.setATIME(100);
  sensor.setASTEP(999);
  sensor.setGain(AS7341_GAIN_256X);
}

void loop() {
  getColorChannels();
  getRGBfromNM();
  finalKelvin = getKelvinfromRGB(color[0], color[1], color[2]);
  controlMappings();
  float currBrightnessValue = calcBrightnessValue(intensityMax, intensityVar, t);  ///just adding t here as a variable for now...
  lerpColorTemp(currBrightnessValue, t);
  // Serial.println("Test");
  // /calcBrightness(color: 0 for cold, 1 for hot, b, v, t, probably returning the adjusted brightness value from 0 to 1)
}



void controlMappings() {
  //Duration Range: map from (0, 1023) --> (0, 1);
  //Intensity Max Brightness: map from (0, 1023) --> (0, 1)
  //Intensity Variation: map from (0, 1023) --> (0, 1)
  //Color Temp: map from (0, 1023) --> (-1, 1)

  //Normalized Duration Range: t should range from 0 to cycle length ( hours to millisecs ), then t_prime will map that from (0, length) --> (0, 1)

  //Read Pins here:

  cycleDurationRaw = analogRead(cycleDurationPin);
  intensityMaxDataRaw = analogRead(intensityMaxSliderPin);
  intensityVarDataRaw = analogRead(intensityVarSliderPin);
  tempDataRaw = analogRead(tempDialPin);

  powerToggle = digitalRead(powerPin);
  resetToggle = digitalRead(resetTogglePin);

  //////////

  //Map raw here:
  cycleDuration = map(cycleDurationRaw, 0.0, 1023.0, 0.6, 1.0);  //convert to hours: (cycleDuration * 10)   /////camp to whole hours / half hours ?????
  intensityMax = map(intensityMaxDataRaw, 0.0, 1023.0, 0.0, 1.0);
  intensityVar = map(intensityVarDataRaw, 0.0, 1023.0, 0.0, 1.0);
  tempData = map(tempDataRaw, 0.0, 1023.0, -1.0, 1.0);  // negative means more cold, positive means more hot

  ////////What would happen if the user changed the duration after like say 2 hours?? like if it was set to 5 hours originally, then waited 2 hours, then changed it to 8 hours?


  //Map output here if needed for PWM:
  coldTempValue = map(coldTempValueRaw, 0.0, 1.0, 0.0, 255.0);
  hotTempValue = map(hotTempValueRaw, 0.0, 1.0, 0.0, 233.75);  /// max 5500K

  analogWrite(coldTempPin, coldTempValue);
  analogWrite(hotTempPin, hotTempValue);


  // Serial.print("Cycle Duration Mapped: ");
  // Serial.println(cycleDuration);
  // Serial.print("Intensity Max Mapped: ");
  // Serial.println(intensityMax);
  // Serial.print("Intensity Var Mapped: ");
  // Serial.println(intensityVar);
  // Serial.print("temperature Input Mapped: ");
  // Serial.println(finalKelvin);
  Serial.print(color[0]);
  Serial.print(" , ");
  Serial.print(color[1]);
  Serial.print(" , ");
  Serial.print(color[2]);
  Serial.print(" , ");
  Serial.print(kelvin_table[(int)((finalKelvin - 1000)/100)][0]);
  Serial.print(" , ");
  Serial.print(kelvin_table[(int)((finalKelvin - 1000)/100)][1]);
  Serial.print(" , ");
  Serial.print(kelvin_table[(int)((finalKelvin - 1000)/100)][2]);
  Serial.print(" , ");
  Serial.print(cycleDuration);
  Serial.print(" , ");
  Serial.print(intensityMax);
  Serial.print(" , ");
  Serial.print(intensityVar);
  Serial.print(" , ");
  Serial.print(resetToggle);
  Serial.print(" , ");
  Serial.print(powerToggle);
  Serial.print(" , ");
  Serial.print(tempData);
  Serial.println();
  
  // Serial.print("temperature Cold Mapped: ");
  // Serial.println(coldTempValue);
  // Serial.print("temperature Hot Mapped: ");
  // Serial.println(hotTempValue);
  // Serial.print("Total Output ( color ): ");
  // Serial.println((coldTempValue + hotTempValue) / 255.0);
  // Serial.print("Percieved Temp: ");
  // Serial.println(finalKelvin);
  //Serial.println(Cycle Duration Mapped: + "cycleDuration");
}


float calcBrightnessValue(float b, float v, float t) {
  float baseFunc = pow(sin(PI * t), 2);
  return b * (baseFunc + ((1 - baseFunc) * v));
}

void lerpColorTemp(float b, float t) {
  float baseFunc = pow(cos(PI * t * 0.5), 2);
  coldTempValueRaw = b * baseFunc;
  hotTempValueRaw = b * (1 - baseFunc);
}


float map(float i, float x1, float y1, float x2, float y2) {
  return x2 + (i - x1) * ((y2 - x2) / (y1 - x1));
}

double map(double i, float x1, float y1, float x2, float y2) {
  return (float)(x2 + (i - x1) * ((y2 - x2) / (y1 - x1)));
}

void getRGBfromNM() {


  double x, y, z;

  for (int i = 0; i < 8; i++) {
    x += (double)(rawNM[i] * CIE_1931_lookup[i][0]);
    y += (double)(rawNM[i] * CIE_1931_lookup[i][1]);
    z += (double)(rawNM[i] * CIE_1931_lookup[i][2]);

    
  }

  

  double L2[3] = {
    max(0, (3.24045 * x) + (-1.53714 * y) + (-0.49853 * z)),
    max(0, (-0.96927 * x) + (1.87601 * y) + (0.04156 * z)),
    max(0, (0.05564 * x) + (-0.20403 * y) + (1.05723 * z))
  };

  //Serial.println( (float)L2[2] );
  L2[0] = map(L2[0], 0, 2.55, 0, 1);
  L2[1] = map(L2[1], 0, 2.55, 0, 1);
  L2[2] = map(L2[2], 0, 2.55, 0, 1);

  

  float R = L2[0] <= 0.0031308 ? 12.92 * L2[0] : (1.055 * pow(L2[0], 1.0 / 2.4)) - 0.055;
  float G = L2[1] <= 0.0031308 ? 12.92 * L2[1] : (1.055 * pow(L2[1], 1.0 / 2.4)) - 0.055;
  float B = L2[2] <= 0.0031308 ? 12.92 * L2[2] : (1.055 * pow(L2[2], 1.0 / 2.4)) - 0.055;

  color[0] = R * 255.0;
  color[1] = G * 255.0;
  color[2] = B * 255.0;
}


float getKelvinfromRGB(float R, float G, float B) {
  float bestK = pow((R - kelvin_table[0][0]), 2) + pow(0.6 * (G - kelvin_table[0][1]), 2) + pow((B - kelvin_table[0][2]), 2);
  float bestKInd = 0;

  for (int i = 0; i < 120; i++) {
    if (pow((R - kelvin_table[i][0]), 2) + pow(0.6 * (G - kelvin_table[i][1]), 2) + pow((B - kelvin_table[i][2]), 2) < bestK) {
      bestK = pow((R - kelvin_table[i][0]), 2) + pow(0.6 * (G - kelvin_table[i][1]), 2) + pow((B - kelvin_table[i][2]), 2);
      bestKInd = i;
    }
  }

  return 1000 + ((bestKInd ) * 100);
}

void getColorChannels() {
  sensor.readAllChannels();
  rawNM[0] = ((float)sensor.getChannel(AS7341_CHANNEL_415nm_F1)) / ((float)uInt16);
  rawNM[1] = ((float)sensor.getChannel(AS7341_CHANNEL_445nm_F2)) / ((float)uInt16);
  rawNM[2] = ((float)sensor.getChannel(AS7341_CHANNEL_480nm_F3)) / ((float)uInt16);
  rawNM[3] = ((float)sensor.getChannel(AS7341_CHANNEL_515nm_F4)) / ((float)uInt16);
  rawNM[4] = ((float)sensor.getChannel(AS7341_CHANNEL_555nm_F5)) / ((float)uInt16);
  rawNM[5] = ((float)sensor.getChannel(AS7341_CHANNEL_590nm_F6)) / ((float)uInt16);
  rawNM[6] = ((float)sensor.getChannel(AS7341_CHANNEL_630nm_F7)) / ((float)uInt16);
  rawNM[7] = ((float)sensor.getChannel(AS7341_CHANNEL_680nm_F8)) / ((float)uInt16);
}


