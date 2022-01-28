/*
   HeartRateDetection
   https://github.com/upsidedownlabs/BioAmp-EXG-Pill

   Upside Down Labs invests time and resources providing this open source code,
   please support Upside Down Labs and open-source hardware by purchasing
   products from Upside Down Labs!

   Copyright (c) 2021 Moteen Shah moteenshah.02@gmail.com
   Copyright (c) 2021 Upside Down Labs - contact@upsidedownlabs.tech

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
   SOFTWARE.
*/

/*
    Based on Neil Kolban example for IDF:
   https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
   Ported to Arduino ESP32 by Evandro Copercini updated by chegewara
   Create a BLE server that, once we receive a connection, will send periodic notifications.
   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/

// Modificado por Juan A. Villalpando
// http://kio4.com/arduino/160i_Wemos_ESP32_BLE.htm

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <math.h>
#include <CircularBuffer.h>

#define SAMPLE_RATE 125
#define BAUD_RATE 115200
#define INPUT_PIN 36
#define OUTPUT_PIN 13
#define DATA_LENGTH 16

int avg = 0;
int data_index = 0;
bool peak = false;
int reading = 0;
float BPM = 0.0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
unsigned long PulseInterval = 0;
CircularBuffer<int,30> buffer;

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "180D"
#define CHARACTERISTIC_UUID "2A37"


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setup() {
  Serial.begin(115200);
  
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  
  // Create the BLE Device
  BLEDevice::init("MyESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?
  // attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // notify changed value
    if (deviceConnected) {
       
//        delay(500); // bluetooth stack will go into congestion, if too many packets are sent.
    static unsigned long past = 0;
    unsigned long present = micros();
    unsigned long interval = present - past;
    past = present;

  // Run timer
  static long timer = 0;
  timer -= interval;

  // Sample
  if(timer < 0){
    timer += 1000000 / SAMPLE_RATE;
    // Sample and Nomalize input data (-1 to 1)
    float sensor_value = analogRead(INPUT_PIN);
    float signal = ECGFilter(sensor_value)/512;
    // Get peak
    peak = Getpeak(signal);
    // Print sensor_value and peak
    // Blink LED on peak
    digitalWrite(OUTPUT_PIN, peak);

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
      if (buffer.isFull()){
        for(int i = 0 ;i < buffer.size(); i++){
          avg+=buffer[i];
        }
        BPM = (1.0/avg) * 60.0 * 1000 * buffer.size();
        avg = 0;
        buffer.pop();
        if (BPM < 240){
          Serial.print("BPM ");
          Serial.println(BPM);
          // pCharacteristic->setValue((uint8_t*)&value, 4);
          String alea = (String) BPM; // Lo convierte en String.
          pCharacteristic->setValue(alea.c_str()); // Pone el numero aleatorio
          pCharacteristic->notify();
          Serial.flush();
        }
      }  
  }
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
   
}
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

// Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 125.0 Hz, frequency: [0.5, 44.5] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
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
