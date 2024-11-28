#include <Wire.h>
#include <ESP8266WiFi.h>     // 用于ESP8266的Wi-Fi库
#include <PubSubClient.h>     // 用于MQTT的库

#define SECRET_SSID "CE-Hub-Student"
#define SECRET_PASS "casa-ce-gagarin-public-service"
#define SECRET_MQTTUSER "student"
#define SECRET_MQTTPASS "ce2021-mqtt-forget-whale";

const char* ssid          = SECRET_SSID;
const char* password      = SECRET_PASS;
const char* mqtt_username = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;
const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1884;


WiFiClient espClient;        // 用于Wi-Fi连接的客户端
PubSubClient client(espClient);  // MQTT客户端对象

char clientId[] = "";  // 用于设置唯一的客户端ID

// Wi-Fi连接函数
void startWifi(){
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // 等待Wi-Fi连接
  while (WiFi.status() != WL_CONNECTED) {
    delay(600);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// MQTT连接函数
void reconnectMQTT() {
  if (WiFi.status() != WL_CONNECTED) {
    startWifi();  // 如果Wi-Fi断开则重新连接
  }

  // 循环直到连接上MQTT服务器
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // 创建一个随机的客户端ID
    String clientId = "LuminaSelector";
    clientId += String(random(0xffff), HEX);
    
    // 尝试连接到MQTT服务器
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // 连接成功后可以进行订阅
      // client.subscribe("topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // 连接失败则等待5秒钟后再试
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, int length) {
  // Handle incoming messages
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}