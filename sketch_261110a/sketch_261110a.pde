import processing.serial.*;

Serial myPort;
String rawData = "";
float[] serialVal;
float startTime;
float time;
double mappedTime;
long finalDuration;
float speed;

void setup() {
  size(900, 600);
  serialVal = new float[9];
  
  startTime = millis();
  
  printArray(Serial.list());
  
  String portName = Serial.list()[2];


  myPort = new Serial(this, portName, 115200);

  myPort.bufferUntil('\n');
  
  //textMode(CENTER);
  //textAlign(CENTER, CENTER);
}

//buffer
//| act clr | | kln tbl | | cycle lgth | | max | | var |
//[0] [1] [2] [3] [4] [5]      [6]         [7]     [8]   [9] [10]

void draw() {
  background(0);
  
  time = millis() - startTime;
  finalDuration = 60000;           //cycle duration///
  //finalDuration = (long)((36*pow(10, 5)) * 1);
  mappedTime = map(time, 0, finalDuration, 0, 1);
  
  float dist = width - 20;
  float range = (height - 130)/2;
  
  
  System.out.println(serialVal[0]);
  
  textAlign(CENTER, CENTER);
  
  
  fill(serialVal[0], serialVal[1], serialVal[2]);
  square(10, 10, 100);
  fill(serialVal[3], serialVal[4], serialVal[5]);
  square(120, 10, 100);
  noFill();
  strokeWeight(1);
  stroke(255);
  rect(10, 120, width - 20, height - 130);
  
  strokeWeight(1);
  
  
  pushMatrix();
  
  translate(10, (height - 10) - (height - 130)/4);
  scale(1, -1);
  
  
  
  
  stroke(128);
  line(0, 0, dist, 0);
  line(0, -range/2, 0, range);
  line((float)(mappedTime * dist), 0, (float)(mappedTime * dist), func((float)mappedTime) * range);
  
  for(float i = 0; i <= 10; i++)
  {
   line((i/10)*dist, -5, (i/10)*dist, 5); 
   fill(128);
   pushMatrix();
   scale(1, -1);
   text(String.format("%.1f", i/10), (i/10)*dist, 10);
   popMatrix();
   noFill();
  }
  
  
  stroke(255);
  
  
  beginShape();
  
  for(int i = 0; i < dist; i++)
  {
   float x = i/dist;
   float y = func(x);
   vertex(i, y * range);
  }
  
  endShape();
  
  popMatrix();
  
  noStroke();
  
  fill(255);
  textSize(18);
  
  textAlign(LEFT, CENTER);
  text("Cycle Duration: " + serialVal[6] + " hours", 230, 20);
  text("Maxium Relative Intensity: " + serialVal[6], 230, 40);
  text("Variation For Curve: " + serialVal[7], 230, 60);
  
  
  //fill(255);
  //textSize(12);
  //text("Sensor Value: " + serialVal[0] + " , " + + serialVal[1] + " , " + serialVal[2] + " , ", 40, height / 2);
}

float func(float x)
{
  float H = 0.25 + (mouseX/width)*0.75;
  float L = H *  (mouseY/height);
 return ((H - L)*exp(-5.2 * pow( x - 0.5, 2 ))) + L;
  
}

void serialEvent(Serial myPort) {

  rawData = myPort.readStringUntil('\n');

  if (rawData != null) {

    rawData = trim(rawData);

    String[] parts = rawData.split(",\\s*");



    for (int i = 0; i < parts.length; i++) {
      serialVal[i] = Float.parseFloat(parts[i]);
      //System.out.println(Float.parseFloat(parts[i]));
    }
  }
}
