#include "WiFi-MQTT-Connection.h"
#include "MyDisplay.h"

void setup() {
  // Start the serial monitor to show output
  Serial.begin(115200);
  // 设置引脚模式
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  // 确保 TRIG_PIN 初始为低电平
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
  measure_distance();

  sendmqtt();

  Serial.println("Loop++ Loop++ Loop++ ");
  delay(500);
}

