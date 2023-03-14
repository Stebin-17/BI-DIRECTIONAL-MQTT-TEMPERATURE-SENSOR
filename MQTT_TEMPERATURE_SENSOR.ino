/*
 Basic MQTT example

 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 
*/

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0x77, 0xED };
IPAddress ip(192, 168, 0, 112);
IPAddress server(54, 87, 92, 106);
IPAddress myDns(192, 168, 0, 1);

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Pin definitions
#define LM35_PIN A1 // LM35 temperature sensor analog pin
#define RED_LED_PIN 8 // red LED pin
#define GREEN_LED_PIN 9 // green LED pin
float temp_val;

// MQTT topic
const char* MQTT_TOPIC = "WIZ-Temperature";

// MQTT client
EthernetClient ethClient;
PubSubClient client(ethClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Convert the payload to a float
  char str_payload[length + 1];
  memcpy(str_payload, payload, length);
  str_payload[length] = '\0';
  float temperature = atof(str_payload);

  // Blink the LED based on the temperature
  if (temperature >= 50.0) {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
  } else {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, resubscribe
      client.subscribe(MQTT_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Ethernet.init(17);  // WIZnet W5100S-EVB-Pico W5500-EVB-Pico W6100-EVB-Pico
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(LM35_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }

  client.setServer(server, 1883);
  client.setCallback(callback);

  // give the Ethernet shield a second to initialize:
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read the temperature from the LM35 sensor
  int temp_adc_val = analogRead(LM35_PIN);
  temp_val = (temp_adc_val * 4.88); /* Convert adc value to equivalent voltage */
  temp_val = (temp_val/10); /* LM35 gives output of 10mv/°C */
  
  // Publish the temperature to the MQTT broker
  char str_temperature[10];
  dtostrf(temp_val, 4, 2, str_temperature);
  client.publish(MQTT_TOPIC, str_temperature);

}
