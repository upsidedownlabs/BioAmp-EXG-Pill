 /*OLED_BPM - BioAmp EXG Pill
 https://github.com/upsidedownlabs/BioAmp-EXG-Pill
 https://github.com/upsidedownlabs/Heart-BioAmp-Arduino-Firmware

 Upside Down Labs invests time and resources providing this open source code,
 please support Upside Down Labs and open-source hardware by purchasing
 products from Upside Down Labs!

 Copyright (c) 2021 - 2024 Upside Down Labs - contact@upsidedownlabs.tech
 Copyright (c) 2021 - 2024 Aryan Prakhar - aryanprakhar1010@gmail.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.*/


// Include necessary libraries
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <math.h>
#include <CircularBuffer.hpp>

// Define constants for OLED display
#define OLED_Address 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// Define constants for ECG sampling and processing
#define SAMPLE_RATE 125
#define BAUD_RATE 115200
#define INPUT_PIN A2
#define OUTPUT_PIN 13
#define DATA_LENGTH 16

// Initialize OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire,-1);

// Global variables for BPM calculation
uint32_t avg = 0;
int data_index = 0;
bool peak = false;
int reading = 0;
uint8_t BPM = 0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
unsigned long PulseInterval = 0;

// Circular buffer to store pulse intervals
CircularBuffer<int,30> buffer;

void setup() {
  // Initialize serial communication
  Serial.begin(BAUD_RATE);
  
  // Set pin modes
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_Address)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Set initial display settings
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  // Calculate elapsed time since last loop
  static unsigned long past = 0;
  unsigned long present = micros();
  unsigned long interval = present - past;
  past = present;

  // Timer for maintaining consistent sampling rate
  static long timer = 0;
  timer -= interval;

  // Sample and process ECG signal at specified rate
  if(timer < 0){
    timer += 1000000 / SAMPLE_RATE;
    
    // Read sensor value and normalize
    float sensor_value = analogRead(INPUT_PIN);
    float signal = ECGFilter(sensor_value)/512;
    
    // Detect peaks in the signal
    peak = Getpeak(signal);
    
    // Blink LED on detected peak
    digitalWrite(OUTPUT_PIN, peak);

    // Process detected peaks for BPM calculation
    if(peak && IgnoreReading == false){
        if(FirstPulseDetected == false){
          FirstPulseTime = millis();
          FirstPulseDetected = true;
        }
        else{
          SecondPulseTime = millis();
          PulseInterval = SecondPulseTime - FirstPulseTime;
          buffer.unshift(PulseInterval);
          FirstPulseTime = SecondPulseTime;
        }
        IgnoreReading = true;
      }
      if(!peak){
        IgnoreReading = false;
      }  
      
      // Calculate and display BPM when buffer is full
      if (buffer.isFull()){
        // Calculate average pulse interval
        for(int i = 0 ;i < buffer.size(); i++){
          avg+=buffer[i];
        }
        avg=avg/ buffer.size();
        
        // Calculate BPM
        BPM = (1.0/avg) * 60.0 * 1000;
        avg = 0;
        buffer.pop();
        
        // Display BPM if it's within a reasonable range
        if (BPM < 240){
          Serial.print("BPM ");
          Serial.println(BPM);
          Serial.flush();
          
          // Update OLED display with new BPM
          display.clearDisplay();
          display.setTextSize(1);
          display.setCursor(24, 3);
          display.print("HEART_RATE(BPM): ");
          display.setCursor(50,26);
          display.setTextSize(3);
          display.println(BPM);
          display.display();
        }
      }  
  }
}

// Function to detect peaks in the ECG signal
bool Getpeak(float new_sample) {
  // Buffers for data, mean, and standard deviation
  static float data_buffer[DATA_LENGTH];
  static float mean_buffer[DATA_LENGTH];
  static float standard_deviation_buffer[DATA_LENGTH];
  
  // Check for peak
  if (new_sample - mean_buffer[data_index] > (DATA_LENGTH/2) * standard_deviation_buffer[data_index]) {
    data_buffer[data_index] = new_sample + data_buffer[data_index];
    peak = true;
  } else {
    data_buffer[data_index] = new_sample;
    peak = false;
  }

  // Calculate mean
  float sum = 0.0, mean, standard_deviation = 0.0;
  for (int i = 0; i < DATA_LENGTH; ++i){
    sum += data_buffer[(data_index + i) % DATA_LENGTH];
  }
  mean = sum/DATA_LENGTH;

  // Calculate standard deviation
  for (int i = 0; i < DATA_LENGTH; ++i){
    standard_deviation += pow(data_buffer[(i) % DATA_LENGTH] - mean, 2);
  }

  // Update mean buffer
  mean_buffer[data_index] = mean;

  // Update standard deviation buffer
  standard_deviation_buffer[data_index] =  sqrt(standard_deviation/DATA_LENGTH);

  // Update data_index
  data_index = (data_index+1)%DATA_LENGTH;

  // Return peak
  return peak;
}

// Band-Pass Butterworth IIR digital filter for ECG signal
// Sampling rate: 125.0 Hz, frequency range: [0.5, 44.5] Hz
// Filter is order 4, implemented as second-order sections (biquads)
float ECGFilter(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - 0.70682283*z1 - 0.15621030*z2;
    output = 0.28064917*x + 0.56129834*z1 + 0.28064917*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - 0.95028224*z1 - 0.54073140*z2;
    output = 1.00000000*x + 2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.95360385*z1 - 0.95423412*z2;
    output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.98048558*z1 - 0.98111344*z2;
    output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}
