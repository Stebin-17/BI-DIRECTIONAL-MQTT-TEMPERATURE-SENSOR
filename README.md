<h1 align='center'> BI-DIRECTIONAL-MQTT-TEMPERATURE-SENSOR</h1>

## INTRODUCTION

The project is an Internet of Things (IoT) application that measures temperature using an LM35 sensor and publishes the data to a broker using MQTT protocol. The application is built on an Arduino board, which is connected to the internet using an Ethernet shield.The code initializes the Ethernet connection, sets up the MQTT client, and defines the pin connections for the LM35 sensor and LEDs. The code then enters a loop where it reads the temperature from the sensor, converts it to a string, and publishes it to the MQTT broker.

The code also listens for messages from the MQTT broker using a callback function. When a message is received, the code parses the payload and blinks an LED based on the temperature.Overall, this project demonstrates how to build a simple IoT application using an Arduino board, an Ethernet shield, and the MQTT protocol to publish and subscribe to data.


## CODE EXPLANANTION:
This is an Arduino sketch for an Ethernet-enabled device that reads temperature data from an LM35 temperature sensor, publishes the data to an MQTT broker, and subscribes to a topic to receive messages that control an LED.

Let's go through the code step by step:

```
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0x77, 0xED };
IPAddress ip(192, 168, 0, 112);
IPAddress server(54, 87, 92, 106);
IPAddress myDns(192, 168, 0, 1);
```

This section defines the MAC address and IP addresses used by the Ethernet shield. The MAC address is a unique identifier for the device, while the IP addresses specify the device's IP address, the IP address of the MQTT server, and the IP address of the DNS server.

```
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
```

These are the header files used by the sketch. SPI.h is used to communicate with the Ethernet shield, Ethernet.h provides the functionality to interact with the Ethernet hardware, and PubSubClient.h is used to connect to an MQTT broker.

```
#define LM35_PIN A1 // LM35 temperature sensor analog pin
#define RED_LED_PIN 8 // red LED pin
#define GREEN_LED_PIN 9 // green LED pin
float temp_val;
```
This section defines the pins used for the LM35 temperature sensor and the two LEDs, as well as a variable for storing the temperature value.

```
const char* MQTT_TOPIC = "WIZ-Temperature";
```

This defines the MQTT topic to which temperature data will be published and LED control messages will be subscribed.

```
EthernetClient ethClient;
PubSubClient client(ethClient);
```
This creates instances of the EthernetClient and PubSubClient classes.

```
void callback(char* topic, byte* payload, unsigned int length) {
  ...
}
```
This is the callback function that will be called whenever a message is received on the subscribed MQTT topic. The function extracts the payload and converts it to a float value, then controls the LEDs based on the temperature.

```
void reconnect() {
  ...
}
```
This function handles the logic for reconnecting to the MQTT broker if the connection is lost.

```
void setup()
{
  ...
}

```

This is the setup function that is called once when the device is powered on or reset. It initializes the serial connection, sets up the pins, initializes the Ethernet connection (either using DHCP or a fixed IP address), sets up the MQTT client, and waits for the Ethernet shield to initialize.

```
void loop()
{
  ...
}
```
This is the main loop function that is called repeatedly while the device is running. It checks if the MQTT client is connected and calls the reconnect function if necessary, then reads the temperature from the LM35 sensor, publishes it to the MQTT broker, and waits for a short delay before starting over.

## REFERENCES:

- WIZnet W5100S-EVB-Pico: https://www.wiznet.io/product-item/w5100s-evb-pico/
- MQTT Protocol: https://mqtt.org/
- PubSubClient Library: https://pubsubclient.knolleary.net/
- Arduino Ethernet Library: https://www.arduino.cc/en/Reference/Ethernet
- Arduino IDE: https://www.arduino.cc/en/software/
