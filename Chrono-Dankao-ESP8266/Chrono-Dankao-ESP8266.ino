#include <Wire.h>
#include <ESP8266WiFi.h>          // Wi-Fi library for ESP8266
#include <PubSubClient.h>         // Lib for MQTT
#include "WiFi-MQTT-Connection.h"
#include "MyDisplay.h"

void setup() {
  // Start the serial monitor to show output
  Serial.begin(115200);
  // Initializes I2C communication
  Wire.begin();
  // Set the pin mode
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  // Make sure TRIG_PIN is initially low
  digitalWrite(TRIG_PIN, LOW);
  delay(500);

  WiFi.setHostname("Lumina 114514cdk");
  startWifi();
  client.setServer(mqtt_server, mqtt_port);
  Serial.println("setup complete");
}

void loop() {
  // we need to make sure the arduino is still connected to the MQTT broker
  // otherwise we will not receive any messages
  if (!client.connected()) {
    reconnectMQTT();
  }
  // we also need to make sure we are connected to the wifi
  // otherwise it will be impossible to connect to MQTT
  if (WiFi.status() != WL_CONNECTED){
    startWifi();
  }
  // check for messages from the broker and ensuring that any outgoing messages are sent.
  client.loop();

  measure_distance(); // Check whether there is user affront
  measure_sht30();    // Check the Temp and Humidty, calculate the Heat-Humity Index, check whether the index is increasing or decreasing
  sendmqtt();         // Set the light brightness, light color, and the position of the lit Pixel, and send the data through MQTT

  Serial.println("Loop++ Loop++ Loop++ ");
  delay(300);
}

