# ESP8266-WIFI-with-Serial-setup
A ESP8266 sketch which gives a simple serial based terminal to save and configure WIFI configuration in flash.

This sketch can form the basis of a framework to build devices that need the ability to configure the wifi settings dynamically by the user.

 the code first tries to retrieve any ssid/password details from flash and tries to connect to the access point in question.
 
 If the connection efforts fail, the code will give a simple menu option to either read or append the existing wifi settings.
 
 
 ## Update 28th August 2021
 
 Did further improvements - moved the config into it's own file. Now the config data is stored in a struct which makes it easier to store additional custom values without impacting the
 EEPROM saving logic.
