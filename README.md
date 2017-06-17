Thermostat Remote Sensor

An Arduino project for a WiFi-attached remote temperature and humidity sensor based on an UNO,
an Adafruit WiFi101 sheild and a BCM280 sensor. The appliance peridically reports the current temperature
and humidity to the thermostat (thermostat-savesensor module) web interface.

The appliance is designed to report the readings to the thermostat via the thermostat-savesensor module.

Configuration:
When configuring for the first time the WiFi sheild enteres a provisioning mode where the SSID and password 
can be set via a web interface. Join the wifi101-XXXX network and browse to http://wifi101.

To change the network after initial configuration, uncomment the call to WiFi.begin() and update the SSID and
password parameters.

Wiring Pinout
Arduino
3.3v  - BCM280 3.3v/Vcc
Gnd   - BCM280 Gnd
SDA   - BCM280 SDA
SCL   - BCM280 SCL
