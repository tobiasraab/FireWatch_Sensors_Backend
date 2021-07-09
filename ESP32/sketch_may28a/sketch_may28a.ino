 /*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
*********/

//mqtt
#include <WiFi.h>
#include <PubSubClient.h>
// const unsigned int SENSOR_POWER = 12;
const int bits = 10;
const int interval = 6000;
//connection:
const char *SSID = "FRITZ!Box 7590 ZU 2,4GHz";
const char *PSK = "35189882138836698231";
const char *MQTT_BROKER = "mqtt.hfg.design";
const char *MQTT_CLIENT_ID = "waldbrandSensorstation1";
const char *TOPIC = "/sweavs/waldbrand/sensorstation_1";
float tempC;
WiFiClient espClient;
PubSubClient client(espClient);
char msg[150];
String clientId;
// const String sensorStationId = "sensorStation1"


//Temp Sensor
//device 0 ID: 28C8272842200104
//device 1 ID: 28FFC261B5160506
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged TO GPIO 4
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Number of temperature devices found
int numberOfDevices;

// We'll use this variable to store a found device address
DeviceAddress tempDeviceAddress;

void setup() {
  // start serial port
  Serial.begin(115200);

  // Start up the library
  sensors.begin();

  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Loop through each device, print out address
  for (int i = 0; i < numberOfDevices; i++) {
    // Search the wire for address
    if (sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }






  //mqtt
  //  pinMode(SENSOR_POWER, OUTPUT);
  //  digitalWrite(SENSOR_POWER, LOW);
  //  delay(10);
  //  Serial.begin(115200);
  connect_wifi();
  client.setServer(MQTT_BROKER, 1883);
  //own mac:
  clientId = composeClientID();
  Serial.print("clientID: ");
  Serial.println(clientId);
}
void connect_wifi()
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PSK);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Reconnecting...");
    if (!client.connect(MQTT_CLIENT_ID))
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  float humusMoisturePercentage = map(analogRead(A0), 0, 4095, 0, 100);
  humusMoisturePercentage = 100 - humusMoisturePercentage;
  Serial.print("humusMoisturePercentage: ");
  // Serial.println(analogRead(A0));
  Serial.println(humusMoisturePercentage);

  float streuMoisturePercentage = map(analogRead(32), 0, 4095, 0, 100);
  streuMoisturePercentage = 100 - streuMoisturePercentage;
  Serial.print("streuMoisturePercentage: ");
  // Serial.println(analogRead(A0));
  Serial.println(streuMoisturePercentage);
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  int i;
  float tempDevice0;
  float tempDevice1;
  // Loop through each device, print out temperature data
  for (i = 0; i < numberOfDevices; i++) {
    // Search the wire for address
    if (sensors.getAddress(tempDeviceAddress, i)) {
      // Output the device ID
      Serial.print("Temperature for device: ");
      Serial.println(i, DEC);
      if (i == 0) {
        tempDevice0 = tempC;
      } else if (i == 1) {
        tempDevice1 = tempC;
      }
      // Print the data
      tempC = sensors.getTempC(tempDeviceAddress);
      Serial.print("Temp C: ");
      Serial.print(tempC);
      Serial.print(" Temp F: ");
      Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
    }
  }
  //analogReadResolution(bits);
  //digitalWrite(SENSOR_POWER, HIGH);
  //delay(10);
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  String output = "{\"mac\":\"" + clientId + "\",\"sensorStationId\":" +  "\"sensorStation1\"" + ",\"tempHumus\":" + tempDevice0 + ",\"tempStreu\":" + tempDevice1 + ",\"moistureHumus\":" + humusMoisturePercentage + ",\"moistureStreu\":" + streuMoisturePercentage + "}";
  String(output).toCharArray(msg, 150);
  Serial.print(output);
  Serial.println();
  client.publish(TOPIC, msg);



  delay(20*60*1000);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}






//mqtt
String composeClientID()
{
  uint8_t mac[6];
  WiFi.macAddress(mac);
  clientId += macToStr(mac);
  return clientId;
}
String macToStr(const uint8_t *mac)
{
  String result;
  for (int i = 0; i < 6; ++i)
  {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}
