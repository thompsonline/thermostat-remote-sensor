#include <SparkFunBME280.h>

#include <WiFi101.h>

#define WLAN_SSID "<SSID>"
#define WLAN_PASS "<PASS>"


BME280 mySensor;

int sensorPin = 0;
int wifiResetButton = 5;
int wifiStatus = WL_IDLE_STATUS;

volatile int resetButtonState = 0;

const int moduleID = 2;
const int statusLEDPin = 13;
const int updateFrequency = 10000; // Seconds * 1000

const bool debug = true;

WiFiClient client;
byte packetNum;

uint8_t lastLEDStatus = HIGH;

void setup() {
  packetNum = 0;
  Serial.begin(19200);
  Serial.println("Starting up");

  pinMode(wifiResetButton, INPUT);
  //attachInterrupt(0, pin_ISR, CHANGE);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  pinMode(statusLEDPin, OUTPUT);

  //WiFi.begin(WLAN_SSID,WLAN_PASS);
  WiFi.beginProvision();
  WiFi.lowPowerMode();

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(statusLEDPin, HIGH);
    delay(500);
    digitalWrite(statusLEDPin, LOW);
    delay(500);
  }

  digitalWrite(statusLEDPin, HIGH);

  if (debug) printWiFiStatus();

  mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x77;

  mySensor.settings.runMode = 3; //Forced mode
  mySensor.settings.tStandby = 0;
  mySensor.settings.filter = 0;
  mySensor.settings.tempOverSample = 1;
  mySensor.settings.pressOverSample = 1;
  mySensor.settings.humidOverSample = 1;
  delay(100);

  Serial.print("Sensor Start: "); Serial.println(mySensor.begin(), HEX);

  float val = mySensor.readTempF();
  Serial.print("initial temp "); Serial.println(val);
  val = mySensor.readFloatHumidity();
  Serial.print("initial humid "); Serial.println(val);
}

void loop() {
  float humid = mySensor.readFloatHumidity();
  float tempF = mySensor.readTempF();

  if (debug) Serial.print(" analog; "); Serial.print(tempF); Serial.print(" F. Humidity "); Serial.print(humid); Serial.println("%");

  lastLEDStatus = ++lastLEDStatus % 2;
  digitalWrite(statusLEDPin, lastLEDStatus);
  packetNum += 1;

  if (client.connect({192, 168, 1, 10}, 80)) {
    client.print("GET /remotesensor/savesensor.php?loc=basement&temp=");
    client.print(tempF);
    client.print("&humid=");
    client.print(humid);
    client.print("&mod=");
    client.print(moduleID);
    client.println(" HTTP/1.0");
    client.println();
    client.flush();
    client.stop();
  } else {
    if (debug) Serial.println("Unable to connect to server");
  }
  delay(updateFrequency);
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
