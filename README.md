## sensor-multicaster

Allows you to use a single Pixelblaze Sensor board and a handful of inexpensive ESP8266s to send
sensor data to multiple Pixelblazes via UDP multicast.  

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

#### Notes
In almost all cases, you will not need to change the multicast address or port.  The multicast address 224.0.0.12:6000 has
worked on all the networks I've tested. 

If it doesn't work on yours, first be sure your firewall isn't filtering multicast packets, then
google "multicast address range",  and pick another address 'till you find one that works.

I don't guarantee that this will work correctly in complex multi-Pixelblaze setups with the Pixelblaze providing the wifi access point.  The ESP32 can only
handle a limited number of devices, and I no idea if it handles multicast correctly.  I'd suggest using an inexpensive 
router as an AP in these cases -- it's faster, more reliable, more expandable and more secure. 

#### Donation
If this project saves you time and effort, please consider a small donation to help support further development.
Every donut, and cup of coffee helps me make more cool software!  :-)

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate/?hosted_button_id=YM9DKUT5V34G8)

