// Drowsiness Detection - EXG Pill

// Copyright (c) 2021 Upside Down Labs

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <math.h>

#define SampleRate 75
#define BaudRate 115200
#define inputPin A0
#define window 10
#define Drowsiness_threshold 5000
#define blink_indicator LED_BUILTIN

int index = 0;
int peak = 0;
float EOGSignal;

void setup() {
  // Serial connection begin
  Serial.begin(BaudRate);
  
  // Setup Input & Output pin
  pinMode(inputPin, INPUT);
  pinMode(blink_indicator, OUTPUT);
}

void loop() {
  // Blink timestamps
  static unsigned long past_blink;
  static unsigned long present_blink; 
  long blink_interval;
  
  // Calculate elapsed time
  static unsigned long past = 0;
  unsigned long present = micros();
  unsigned long interval = present - past;
  past = present;

  // Run timer
  static long timer = 0;
  timer -= interval;

  // Sample
  if(timer < 0){
    timer += 1000000 / SampleRate;
    
    // Sample and Nomalize input data (-1 to 1)
    float sensorValue = analogRead(inputPin);
    EOGSignal = EOGFilter(sensorValue)/512;
    
    // Get peak
    int peak = getPeak(EOGSignal);
    
    // Print sensorValue and peak
    Serial.print(EOGSignal);
    Serial.print(",");
    Serial.println(peak);
    
    // Indicate on peak
    digitalWrite(blink_indicator, peak);       

    // Update blink timestamps
    present_blink= millis();
    blink_interval= present_blink - past_blink;
    
    if(peak==1)
      past_blink= present_blink;

    // Print blink interval
    Serial.print("blink_interval: ");
    Serial.println(blink_interval);

    // Drowsiness detection
    if(blink_interval>= Drowsiness_threshold)
      digitalWrite(blink_indicator,  HIGH);
  }
}

int getPeak(float newSample){
  // Buffers for data, mean, and standard deviation
  static float dataBuffer[window];
  static float meanBuffer[window];
  static float standardDeviationBuffer[window];
  
  // Check for peak
  if (newSample - meanBuffer[index] > (window*1.2) * standardDeviationBuffer[index]) {
    dataBuffer[index] = newSample + dataBuffer[index];
    peak = 1;
  } else {
    dataBuffer[index] = newSample;
    peak = 0;
  }

  // Calculate mean
  float sum = 0.0, mean, standardDeviation = 0.0;
  for (int i = 0; i < window; ++i){
    sum += dataBuffer[(index + i) % window];
  }
  mean = sum/window;

  // Calculate standard deviation
  for (int i = 0; i < window; ++i){
    standardDeviation += pow(dataBuffer[(i) % window] - mean, 2);
  }

  // Update mean buffer
  meanBuffer[index] = mean;

  // Update standard deviation buffer
  standardDeviationBuffer[index] =  sqrt(standardDeviation/window);

  // Update index
  index = (index+1)%window;

  // Return peak
  return peak;
}

// Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 75.0 Hz, frequency: [0.5, 19.5] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
// Reference: 
// https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
// https://courses.ideate.cmu.edu/16-223/f2020/Arduino/FilterDemos/filter_gen.py
float EOGFilter(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - 0.02977423*z1 - 0.04296318*z2;
    output = 0.09797471*x + 0.19594942*z1 + 0.09797471*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - 0.08383952*z1 - 0.46067709*z2;
    output = 1.00000000*x + 2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.92167271*z1 - 0.92347975*z2;
    output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.96758891*z1 - 0.96933514*z2;
    output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}
