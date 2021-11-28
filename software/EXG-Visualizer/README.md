# ESP32 oscilloscope with Web user interface - see the signals the way ESP32 sees them

## Demo
![image info](.assests/../assets/Random%20analog.png)

## Setup instructions

1. Copy all files in this package into Esp32_oscilloscope directory.
2. Open Esp32_oscilloscope.ino with Arduino IDE.
3. Find and change YOUR-STA-SSID to your WiFi SSID and YOUR-STA-PASSWORD to your WiFi password.
4. In your Arduino IDE under Tools|Board make sure you choose "ESP32 Dev Module" or a board allowing partition schemes
5. Oscilloscope uses FAT file system so select one of FATFS partition schemas (Arduino IDE under Tools|Partition Scheme|...)
6. Compile sketch and run it on your ESP32 for the first time. Doing this, ESP32 flash memory will be formatted with FAT file system. WARNING: every information you have stored into ESP32�s flash memory will be lost.
7. FTP to your ESP32 (By using ftp command line utility or Windows explorer. User name and password are not required) and upload the following files into /var/www/html directory:

   - android-192-osc.png,
   - apple-180-osc.png,
   - oscilloscope.html.

```
C:\esp32_oscilloscope>ftp YOUR-ESP32-IP
Connected to 10.0.0.3.
220-ESP32 FTP server - everyone is allowed to login
User (10.0.0.3:(none)):
331 enter password
Password:
230 logged on, use "/" to refer to your home directory "/"
ftp> put android-192-osc.png /var/www/html/android-192.png
226 /var/www/html/android-192-osc.png transfer complete
ftp> put apple-180-osc.png /var/www/html/apple-180.png
226 /var/www/html/apple-180-osc.png transfer complete
ftp> put oscilloscope.html /var/www/html/oscilloscope.html
226 /var/www/html/visualizer.html transfer complete
ftp> ls
200 port ok
150 starting transfer
-r-xr-xrwx   1 owner    group            1818 Aug 24 10:47      /var/www/html/android-192-osc.png
-r-xr-xrwx   1 owner    group            1596 Aug 24 10:47      /var/www/html/apple-180-osc.png
-r-xr-xrwx   1 owner    group           39515 Aug 24 10:47      /var/www/html/visualizer.html
226 transfer complete
ftp>
```

8. Open http://YOUR-ESP32-IP/oscilloscope.html with your browser.
