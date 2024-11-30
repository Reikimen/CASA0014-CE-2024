#define SECRET_SSID "CE-Hub-Student"
#define SECRET_PASS "casa-ce-gagarin-public-service"
#define SECRET_MQTTUSER "student"
#define SECRET_MQTTPASS "ce2021-mqtt-forget-whale";

const char* ssid          = SECRET_SSID;
const char* password      = SECRET_PASS;
const char* mqtt_username = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;
const char* mqtt_server   = "mqtt.cetools.org";
const int mqtt_port       = 1884;

WiFiClient espClient;               // Client for Wi-Fi connection
PubSubClient client(espClient);     // MQTT client object

char clientId[] = "";               // Used to set a unique client ID

// edit this for the light you are connecting to
char mqtt_topic_pixel[] = "student/CASA0014/light/3/pixel/";              // used to control individual pixels on the ring
char mqtt_topic_all[] = "student/CASA0014/light/3/all/";                  // used to control all LED's in one go
char mqtt_topic_brightness[] = "student/CASA0014/light/3/brightness/";    // used to set the brightness of all the LED's 
char mqtt_topic_demo[] = "student/CASA0014/light/3/demo/";                // used to control individual pixels on the ring
char mqtt_topic_fire[] = "student/CASA0014/light/3/fire/";                // used to control individual pixels on the ring

// Connection function Wi-Fi
void startWifi(){
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // Wait for the Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(600);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// MQTT connection function
void reconnectMQTT() {
  if (WiFi.status() != WL_CONNECTED) {
    startWifi();  // Reconnect if the Wi-Fi is disconnected
  }

  // Loop until the MQTT server is connected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Create a random client ID
    String clientId = "LuminaSelector";
    clientId += String(random(0xffff), HEX);
    
    // Try to connect to the MQTT server
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // You can subscribe after the connection is successful
      // client.subscribe("topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // If the connection fails, wait 5 seconds and try again
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

