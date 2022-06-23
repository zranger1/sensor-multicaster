## sensor-multicaster

Allows you to use a single Pixelblaze Sensor board to send data to multiple
Pixelblazes via UDP multicast.  

#### Requirements
- A Pixelblaze
- A Sensor Board
- An ESP8266 for the sending unit
- Additional ESP8266s, one per receiver

You will also need the Arduino IDE, with the ESP8266 community library installed. Some
experience with Arduino/ESP8266 wiring and C programming is very helpful. 
 
#### Quick Guide

(Additional instructions in code comments)

##### On the sender:
 Connect the ESP82866 and the sensor board to power (3v3) and ground, and connect the sensor board's
 TX pin to the ESP8266's GPIO13 (D7) pin. Set the _SSID and _PASS variables for your wifi network, then
 compile and upload.
 
##### On each receiver 
Connect the ESP82866 to the Pixelblaze's power (3v3) and ground pins, and connect GPIO15 (D8) on
on the ESP8266 to the RX pin on the Pixelblaze's expansion header. Set the _SSID and _PASS variables for your wifi network, then compile and upload.

Once your whole system is powered up, sensor data from the sender will be continuously transmitted to all the receivers. Enjoy your new superpower!
