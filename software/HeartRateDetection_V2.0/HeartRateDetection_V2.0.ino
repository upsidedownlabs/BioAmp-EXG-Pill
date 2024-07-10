// HeartRateDetection_V2.0 - BioAmp EXG Pill
// https://github.com/upsidedownlabs/BioAmp-EXG-Pill
// https://github.com/upsidedownlabs/Heart-BioAmp-Arduino-Firmware

// Upside Down Labs invests time and resources providing this open source code,
// please support Upside Down Labs and open-source hardware by purchasing
// products from Upside Down Labs!

// Copyright (c) 2021 - 2024 Upside Down Labs - contact@upsidedownlabs.tech
// Copyright (c) 2021 - 2024 Aryan Prakhar - aryanprakhar1010@gmail.com

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


// Define constants
#define SAMPLE_RATE 125       // Sampling rate in Hz
#define BAUD_RATE 115200      // Serial communication baud rate
#define INPUT_PIN A2          // Analog  pin for ECG signal
#define OUTPUT_PIN 13         // Digital output pin for LED
#define DATA_LENGTH 26        // Length of data buffer for peak detection
#define buffer_size 5         // Size of circular arrays for BPM calculation

// Global variables for BPM calculation
int data_index = 0;
uint32_t tdiff = 0;           // Time difference between consecutive peaks
uint8_t BPM = 0;              // Calculated Beats Per Minute

// Circular arrays for BPM calculation
int timeBuffer_index = -1;
int sumBuffer_index = -1;
uint32_t timeBuffer[buffer_size] = {0};
uint32_t sumBuffer[buffer_size] = {0};

uint32_t previous_time = 0;
uint32_t sum = 0;
uint32_t average = 0;

// Variables for peak detection and debouncing
unsigned long lastPeakStartTime = 0;
unsigned long peakCooldownPeriod = 2; // Cooldown period in milliseconds
bool isPeakActive = false;
bool peakEvent = false;

void setup() {
  // Initialize serial communication
  Serial.begin(BAUD_RATE);
  // Set pin modes
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
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
    peakEvent = Getpeak(signal);

    // Blink LED on detected peak
    digitalWrite(OUTPUT_PIN, peakEvent);

    // Calculate BPM when a peak is detected
    if(peakEvent){
      // Update time buffer for peak timing
      if(timeBuffer_index == -1){
        timeBuffer_index = (timeBuffer_index + 1) % buffer_size;
        timeBuffer[timeBuffer_index] = micros();
      }
      else{
        previous_time = timeBuffer[timeBuffer_index];
        timeBuffer_index = (timeBuffer_index + 1) % buffer_size;
        timeBuffer[timeBuffer_index] = micros();
        tdiff = timeBuffer[timeBuffer_index] - previous_time;
      }
    
      // Calculate BPM using circular arrays
      if(sumBuffer_index == -1){
        sumBuffer_index = (sumBuffer_index + 1) % buffer_size;
      }
      else{
        if(sumBuffer[sumBuffer_index] == 0){
          sumBuffer[sumBuffer_index] = tdiff;
          sum = sum + sumBuffer[sumBuffer_index];
          sumBuffer_index = (sumBuffer_index + 1) % buffer_size;
        }
        else{
          sum = sum - sumBuffer[sumBuffer_index];
          sumBuffer[sumBuffer_index] = tdiff;
          sum = sum + sumBuffer[sumBuffer_index];
          sumBuffer_index = (sumBuffer_index + 1) % buffer_size;
          average = sum / buffer_size;
          BPM = (60 * 1000000) / average;
          Serial.print("BPM : ");
          Serial.println(BPM);
        }
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
  bool rawPeak = false;
  if (new_sample - mean_buffer[data_index] > (DATA_LENGTH/2) * standard_deviation_buffer[data_index]) {
    data_buffer[data_index] = new_sample + data_buffer[data_index];
    rawPeak = true;
  } else {
    data_buffer[data_index] = new_sample;
    rawPeak = false;
  }

  // Event-based debounce logic
  bool peakEvent = false;
  unsigned long currentTime = micros();
  
  if (rawPeak && !isPeakActive && (currentTime - lastPeakStartTime > peakCooldownPeriod)) {
    // Start of a new peak
    isPeakActive = true;
    lastPeakStartTime = currentTime;
    peakEvent = true;
  } else if (!rawPeak && isPeakActive) {
    // End of the current peak
    isPeakActive = false;
  }

  // Calculate mean
  float sum = 0.0, mean, standard_deviation = 0.0;
  for (int i = 0; i < DATA_LENGTH; ++i){
    sum += data_buffer[(data_index + i) % DATA_LENGTH];
  }
  mean = sum / DATA_LENGTH;

  // Calculate standard deviation
  for (int i = 0; i < DATA_LENGTH; ++i){
    standard_deviation += pow(data_buffer[(i) % DATA_LENGTH] - mean, 2);
  }

  // Update mean buffer
  mean_buffer[data_index] = mean;

  // Update standard deviation buffer
  standard_deviation_buffer[data_index] = sqrt(standard_deviation / DATA_LENGTH);

  // Update data_index
  data_index = (data_index + 1) % DATA_LENGTH;

  // Return peak event
  return peakEvent;
}

// Band-Pass Butterworth IIR digital filter for ECG signal
// Sampling rate: 125.0 Hz, frequency range: [0.5, 44.5] Hz
// Filter is order 4, implemented as second-order sections (biquads)
// Reference:
// https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
// https://courses.ideate.cmu.edu/16-223/f2020/Arduino/FilterDemos/filter_gen.py
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
