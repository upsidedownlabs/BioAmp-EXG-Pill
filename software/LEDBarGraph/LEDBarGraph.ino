// LED Bar Graph - EXG Pill

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

#define SampleRate 500
#define BaudRate 115200
#define inputPin A0
#define BufferSize 128

int circularBuffer[BufferSize];
int index, sum;
// LED pin numbers in-order
int LEDs[] = {4, 5, 6, 7, 8, 9, 10, 11, 12};
int TotalLEDs = sizeof(LEDs) / sizeof(LEDs[0]);

void setup() {
	// Serial connection begin
	Serial.begin(BaudRate);
	// Initialize all the LEDs
  	for (int i = 0; i < TotalLEDs; i++) {
    	pinMode(LEDs[i], OUTPUT);
  	}
}

void loop() {
	// Calculate elapsed time
	static unsigned long past = 0;
	unsigned long present = micros();
	unsigned long interval = present - past;
	past = present;

	// Run timer
	static long timer = 0;
	timer -= interval;

	// Sample and get envelop
	if(timer < 0) {
		timer += 1000000 / SampleRate;
		int sensorValue = analogRead(inputPin);
		int EMGSignal = EMGFilter(sensorValue);
		int envelop = getEnvelop(abs(EMGSignal));

		// Update LED bar graph
    	for(int i = 0; i<=TotalLEDs; i++){
    		if(i>(envelop-1)){
      			digitalWrite(LEDs[i], LOW);
    		} else {
      			digitalWrite(LEDs[i], HIGH);
    		}
  		}

		Serial.print(EMGSignal);
		Serial.print(",");
		Serial.println(envelop);
	}
}

// Envelop detection algorithm
int getEnvelop(int absEMG){
	sum -= circularBuffer[index];
	sum += absEMG;
	circularBuffer[index] = absEMG;
	index = (index + 1) % BufferSize;
	return (sum/BufferSize) * 2;
}

// Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 500.0 Hz, frequency: [74.5, 149.5] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
// Reference: 
// https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
// https://courses.ideate.cmu.edu/16-223/f2020/Arduino/FilterDemos/filter_gen.py
float EMGFilter(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - 0.05159732*z1 - 0.36347401*z2;
    output = 0.01856301*x + 0.03712602*z1 + 0.01856301*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -0.53945795*z1 - 0.39764934*z2;
    output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - 0.47319594*z1 - 0.70744137*z2;
    output = 1.00000000*x + 2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.00211112*z1 - 0.74520226*z2;
    output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}
