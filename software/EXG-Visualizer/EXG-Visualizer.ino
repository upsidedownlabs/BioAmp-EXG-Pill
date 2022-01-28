/*

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

   This file is part of Esp32_oscilloscope project: https://github.com/BojanJurca/Esp32_oscilloscope
   Esp32 oscilloscope is also fully included in Esp32_web_ftp_telnet_server_template project: https://github.com/BojanJurca/Esp32_web_ftp_telnet_server_template
  
   Esp32 oscilloscope is built upon Esp32_web_ftp_telnet_server_template. As a stand-alone project it uses only those 
   parts of Esp32_web_ftp_telnet_server_template that are necessary to run Esp32 oscilloscope.
  
   Copy all files in the package into Esp32_oscilloscope directory, compile them with Arduino (with FAT partition scheme) and run on ESP32.
   
   October, 13, 2021, Bojan Jurca

*/

#include <WiFi.h>

// we'll need FAT file system for storing uploaded oscilloscope.html  
#include "file_system.h"

// network settings
#define HOSTNAME                  "Oscilloscope"      // define the name of your ESP32 here
#define MACHINETYPE               "ESP32 NodeMCU"     // describe your hardware here
#define DEFAULT_STA_SSID          "pablo"                  // define default WiFi settings
#define DEFAULT_STA_PASSWORD      "Mot9323772083"
#define DEFAULT_AP_SSID           "ESP32_AP"          // HOSTNAME 
#define DEFAULT_AP_PASSWORD       "nopassword"        // "YOUR_AP_PASSWORD" // must be at leas 8 characters long
#include "network.h"

// simplify the use of FTP server in this project by avoiding user management - anyone is alowed to use it
#define USER_MANAGEMENT NO_USER_MANAGEMENT
#include "user_management.h"

// include FTP server - we'll need it for uploading oscilloscope.html to ESP32
#include "ftpServer.hpp"
ftpServer *ftpSrv;

// include Web server - we'll need it to handle oscilloscope WS requests
#include "webServer.hpp"
httpServer *webSrv;

// oscilloscope
#include "oscilloscope.h"

// take a look at https://github.com/BojanJurca/Esp32_web_ftp_telnet_server_template for details on how this works


String httpRequestHandler (String& httpRequest, httpServer::wwwSessionParameters *wsp);

void wsRequestHandler (String& wsRequest, WebSocket *webSocket);

void setup () {  
  Serial.begin (115200);

  // mount file system, WEB server will search for files in /var/www/html directory
  // FFat.format (); // uncomment this if something goes wrong to delete all configuration files already written
  mountFileSystem (true);
  if (!isDirectory ("/var/www/html")) { FFat.mkdir ("/var"); FFat.mkdir ("/var/www"); FFat.mkdir ("/var/www/html"); } // so that you won't have to make them manualy yourself

  // connect ESP STAtion to WiFi
  startWiFi ();
 
  // start FTP server so we can upload .html and .png files into /var/www/html directory
  ftpSrv = new ftpServer ((char *) "0.0.0.0", 21, NULL);
  if (ftpSrv) // did ESP create FTP server instance?
    if (ftpSrv->started ()) Serial.printf ("[%10lu] FTP server has started.\n", millis ());
    else                    Serial.printf ("[%10lu] FTP server did not start. Compile it in Verbose Debug level to find the error.\n", millis ());
  else                      Serial.printf ("[%10lu] not enough free memory to start FTP server\n", millis ()); // shouldn't really happen

  // start WEB server 
  webSrv = new httpServer (httpRequestHandler, wsRequestHandler, 8192, (char *) "0.0.0.0", 80, NULL);
  if (webSrv) // did ESP create HttpServer instance?
    if (webSrv->started ()) Serial.printf ("[%10lu] WEB server has started.\n", millis ());
    else                    Serial.printf ("[%10lu] WEB server did not start. Compile it in Verbose Debug level to find the error.\n", millis ());
  else                      Serial.printf ("[%10lu] not enough free memory to start WEB server\n", millis ()); // shouldn't really happen

  // initialize GPIOs you are going to use here:
  // ...

              // ----- examples - delete this code when if it is not needed -----
              // example: PWM on built-in led (GPIO 2)
              ledcSetup (0, 83, 10);                    // channel 0, 83 Hz, 10 bit resolution (1024 values)
              ledcAttachPin (2, 0);                     // attach pin 2 (built-in led) to channel 0
              ledcWrite (0, 307);                       // PWM of built-in LED (channel 0 -> pin 2) with 1/3 of period (307 out of 1023)
              // example: ADC on GPIO 32
              analogReadResolution (12);                // set 12 bit analog read resolution (defaule)
              analogSetPinAttenuation (32, ADC_11db);   // set attenuation for pin 32 to 2600 mV range (default)

}

void loop () {

}

String httpRequestHandler (String& httpRequest, httpServer::wwwSessionParameters *wsp) {  
  // handle HTTP requests
  // note that webServer handles HTTP requests to .html files by itself (in our case oscilloscope.html if uploaded into /var/www/html/ with FTP previously)
  // so you don't have to handle this kind of requests yourself

  if (httpRequest.substring (0, 6) == "GET / " || httpRequest.substring (0, 16) == "GET /index.html ") {
    httpRequest = "GET /oscilloscope.html HTTP/1.1";  // change httpRequest
    return "";                                        // now tell web server to handle it itself
  }
  // if the request is GET /oscilloscope.html we don't have to interfere - web server will read the file from file system
 
  return ""; // HTTP request has not been handled by httpRequestHandler - let the webServer handle it itself
}

void wsRequestHandler (String& wsRequest, WebSocket *webSocket) {  
  // handle WS requests. GET /runOscilloscope is the only WS request we'll get from oscilloscope.html, 
  // runOscilloscope function will handle it on the server side - it will communicate with Javascript client in oscilloscope.html

  if (wsRequest.substring (0, 21) == "GET /runOscilloscope " ) { runOscilloscope (webSocket); } // called from oscilloscope.html
}
