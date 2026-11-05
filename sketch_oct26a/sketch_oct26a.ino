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
const int resetTogglePin = 2;
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




void setup() {
  Serial.begin(9600);
  
}

void loop() {
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

  analogRead(cycleDurationPin);
  analogRead(intensityMaxSliderPin);
  analogRead(intensityVarSliderPin);
  analogRead(tempDialPin);

  //////////

  //Map raw here:
  cycleDuration = map(cycleDurationRaw, 0, 1023, 0, 1);  //convert to hours: (cycleDuration * 10)   /////camp to whole hours / half hours ?????
  intensityMax = map(intensityMaxDataRaw, 0, 1023, 0, 1);
  intensityVar = map(intensityVarDataRaw, 0, 1023, 0, 1);
  tempData = map(tempDataRaw, 0, 1023, -1, 1);  // negative means more cold, positive means more hot

  ////////What would happen if the user changed the duration after like say 2 hours?? like if it was set to 5 hours originally, then waited 2 hours, then changed it to 8 hours?


  //Map output here if needed for PWM:
  coldTempValue = map(coldTempValueRaw, 0, 1, 0, 255);
  hotTempValue = map(hotTempValueRaw, 0, 1, 0, 233.75); /// max 5500K

  analogWrite(coldTempPin, coldTempValue);
  analogWrite(hotTempPin, hotTempValue);


  Serial.print("Cycle Duration Mapped: ");
  Serial.println(cycleDuration);
  Serial.print("Intensity Max Mapped: ");
  Serial.println(intensityMax);
  Serial.print("Intensity Var Mapped: ");
  Serial.println(intensityVar);
  Serial.print("temperature Input Mapped: ");
  Serial.println(tempData);
  Serial.print("temperature Cold Mapped: ");
  Serial.println(coldTempValue);
  Serial.print("temperature Hot Mapped: ");
  Serial.println(hotTempValue);
  Serial.print("Total Output ( color ): ");
  Serial.println(( coldTempValue + hotTempValue ) / 255.0);
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
