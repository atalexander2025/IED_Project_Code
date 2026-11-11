import processing.serial.*;

Serial myPort;
String rawData = "";
float[] serialVal;
float startTime;
float time;
double mappedTime;
long finalDuration;
float speed, dist, range;
int toggle = 0;
int shape = 0;

char[] graphName = {'T', 'P', 'C', 'B'};

color[] kelvinColors = 
{
  color(250, 120, 80),
  color(255, 269, 136),
  color(255, 243, 239)
  //color(255, 219, 186),
  //color(255, 180, 107)
};

void setup() {
  size(900, 600);
  serialVal = new float[12];
  
  startTime = millis();
  
  printArray(Serial.list());
  
  String portName = Serial.list()[2];


  myPort = new Serial(this, portName, 115200);

  myPort.bufferUntil('\n');
  
  speed = 60.0;
  
  //textMode(CENTER);
  //textAlign(CENTER, CENTER);
}

//buffer
//| act clr | | kln tbl | | cycle lgth | | max | | var | | reset | | pr | | rltv tmp |
//[0] [1] [2] [3] [4] [5]      [6]         [7]     [8]      [9]     [10]      [11]

void draw() {
  background(0);
  
  time = millis() - startTime;
  //finalDuration = 60000;           //cycle duration///
  finalDuration = (long)((36*pow(10, 5)) * 1);
  mappedTime = min(1, map(time, 0, finalDuration/speed, 0, 1));
  
  dist = width - 20;
  range = (height - 130)/2;
  
  
  System.out.println(serialVal[0]);
  
  textAlign(CENTER, CENTER);
  
  
  fill(serialVal[0], serialVal[1], serialVal[2]);
  rect(10, 10, 40, 100);
  fill(serialVal[3], serialVal[4], serialVal[5]);
  rect(60, 10, 40, 100);
  noFill();
  strokeWeight(1);
  stroke(255);
  rect(10, 120, width - 20, height - 130);
  
  fill(0, 255, 0);
  if(mousePressed && abs(width - 30 - mouseX) <= 20 && abs(10 - mouseY) <= 5)
  {
   toggle = 0;
    
  }
  if(mousePressed && abs(width - 30 - mouseX) <= 20 && abs(25 - mouseY) <= 5)
  {
   toggle = 1;
    
  }
  
  if(mousePressed && abs(width - 30 - mouseX) <= 20 && abs(40 - mouseY) <= 5)
  {
   startTime = millis();
    
  }
  
  noStroke();
  rectMode(CENTER);
  fill(100); 
  rect(width - 30, 10, 40, 10);
  fill(140, 50, 10);
  rect(width - 30, 25, 40, 10);
  fill(10, 20, 30);
  rect(width - 30, 40, 40, 10);
  rectMode(CORNER);
  fill(140);
  text(">", width - 58, 10 + (toggle * 15));
  
  
  if(toggle == 0)
  {
    showIntensityGraph();
    
  }
  
  if(toggle == 1)
  {
    showTempGraph();
    
  }
  
  
  
  
  
  fill(255);
  textSize(18);
  
  textAlign(LEFT, CENTER);
  text("Cycle Duration: " + serialVal[6] + " hours", 110, 20);
  text("Maxium Relative Intensity: " + serialVal[6], 110, 40);
  text("Variation From Curve: " + serialVal[7], 110, 60);
  text("Relative Warmth: " + serialVal[11], 110, 80);
  
  textSize(15);
  text("Cold: Duty Cycle | Intensity | Adjusted: ", 370, 20);
  text("Warm: Duty Cycle | Intensity | Adjusted: ", 370, 40);
  text("Clock Toggle:  " + (int)serialVal[9], 370, 60);
  text("Power Toggle:  " + (int)serialVal[10], 370, 80);
  
  
  //fill(255);
  //textSize(12);
  //text("Sensor Value: " + serialVal[0] + " , " + + serialVal[1] + " , " + serialVal[2] + " , ", 40, height / 2);
}

void showIntensityGraph()
{
  strokeWeight(1);
  
  fill(255);
  text("Intensity over a Cycle", width/2, 140);
  
  pushMatrix();
  
  translate(10, (height - 10) - (height - 130)/4);
  
  
  scale(1, -1);
  
  stroke(85);
  line(0, range, dist, range); 
  
  
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
  
  
}





void showTempGraph()
{
  strokeWeight(1);
  
  fill(255);
  text("Color Temperature over a Cycle", width/2, 140);
  
  pushMatrix();
  
  translate(10, (height - 10) - (height - 130)/4);
  
  
  scale(1, -1);
  
  stroke(85);
  line(0, range, dist, range); 
  
  
  stroke(128);
  line(0, 0, dist, 0);
  line(0, -range/2, 0, range);
  
  
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
  
  
  
  
  //int shape = 3;
  
  for(int i = 0; i < dist; i++)
  {
   float x = i/dist;
   float y = graph(x, shape);
   //vertex(i, y * range);
   
   stroke( multiLerp(kelvinColors, min(1, max(0, y - (serialVal[12] * 0.25)))));
   line(i, 0, i, y * range);
  
  }
  

  
  stroke(color(0, 0, 255));
  strokeWeight(2);
  line((float)(mappedTime * dist), 0, (float)(mappedTime * dist), graph((float)mappedTime, shape) * range);
  
  
  
  popMatrix();
  
  noStroke();
  
  fill(multiLerp(kelvinColors, graph((float)mappedTime, shape)));
  rect(20, 130, 40, 40);
  
  rectMode(CENTER);
  
  
  
  for(int i = 0; i < 4; i++)
  {
    
    if( mousePressed && abs(width - 100 + ( i * 20) - mouseX) < 7.5 && abs(150 - mouseY) < 7.5)
    {
      shape = i;
    }
    fill(78);
    if(i == shape)
    {
     fill(128);
    }
    rect(width - 100 + ( i * 20), 150, 15, 15);
    fill(255);
    text(graphName[i], width - 100 + ( i * 20), 150);
    
  }
  
  rectMode(CORNER);
  
}







float graph(float x, int type)
{
  
  if( type == 0)
  {
    return -abs(2*x - 1) + 1;
    
  }
  if( type == 1)
  {
    return -pow(2*x - 1, 2) + 1;
    
  }
  if( type == 2)
  {
    return -pow(abs(2*x - 1), 0.5) + 1;
    
  }
  if( type == 3)
  {
    return exp( (-9* pow((2*x - 1), 2)));
    
  }
  
  return 0;
  //float H = 0.25 + (mouseX/(float)width)*0.75;
  //float L = H *  (mouseY/(float)height);
 
  
}




float func(float x)
{
  float H = 0.25 + (mouseX/(float)width)*0.75;
  float L = H *  (mouseY/(float)height);
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


color multiLerp(color[] colArray, float amt) {
  int n = colArray.length;
  if (amt <= 0) return colArray[0];
  if (amt >= 1) return colArray[n - 1];
  
  // Scale amt to the number of segments (n - 1)
  float scaled = amt * (n - 1);
  int i = floor(scaled);
  float localT = scaled - i; // remainder between 0 and 1 for the current segment
  
  return lerpColor(colArray[i], colArray[i + 1], localT);
}
