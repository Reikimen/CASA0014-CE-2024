// I2C address of the SHT30 sensor
#define Addr_SHT30 0x44
// Variables that store SHT30 measurement data
uint8_t sht30_data[6];
float sht30_cTemp;     // Celsius temperature
float sht30_fTemp;     // Fahrenheit
float sht30_humidity;  // Relative humidity
float sht_HI = 0;
float sht_HI_old = 0;
// Divide according to comfort, and change the color of the display
float Level_HI[10] = {75.0000, 78.1111, 81.2222, 84.3333, 87.4444, 90.5556, 93.6667, 96.7778, 99.8889, 104.0000};
int DisplayColor = 0;
// Define RGB values for different levels and which pixels light up in the initial state
int Brightness[12] = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
const int R_0 = 127, G_0 = 0, B_0 = 255;    // Purple
const int R_1 = 0, G_1 = 72, B_1 = 255;
const int R_2 = 0, G_2 = 127, B_2 = 255;
const int R_3 = 0, G_3 = 255, B_3 = 127;
const int R_4 = 0, G_4 = 255, B_4 = 72;
const int R_5 = 72, G_5 = 255, B_5 = 0;
const int R_6 = 127, G_6 = 255, B_6 = 0;
const int R_7 = 255, G_7 = 255, B_7 = 0;
const int R_8 = 255, G_8 = 127, B_8 = 0;
const int R_9 = 255, G_9 = 0, B_9 = 0;      // Red
int R_CDK = 0, G_CDK = 0, B_CDK = 0;
// When a decrease in comfort is detected, rotate counterclockwise; As the concentration rises, it rotates clockwise
int DisplayDirection = 0; // 0 for Counter Clock Wise; 1 for Clock Wise ; 11451 for not move

#define TRIG_PIN 12   // Trigger pin D6
#define ECHO_PIN 13   // Echo pin D7
long distance = 114;  // Define a default distance value
int Dis_CDK = 0;      // 1 for distance smaller than 60, 0 for distance larger... 

// Function: Shift the array right
void rotateRight(int arr[], int size) {
    int last = arr[size - 1];  // Save the rightmost element
    for (int i = size - 1; i > 0; i--) {
        arr[i] = arr[i - 1];  // Move each element one digit to the right
    }
    arr[0] = last;  // The rightmost element jumps to the leftmost
}

// Function: Shift the array left
void rotateLeft(int arr[], int size) {
    int first = arr[0];  // Save the leftmost element
    for (int i = 0; i < size - 1; i++) {
        arr[i] = arr[i + 1];  // Move each element one bit to the left
    }
    arr[size - 1] = first;  // The leftmost element jumps to the rightmost
}

// Setup the brightness for highest by default, also change the id of pixels (which are on) according to the DisplayDirection which is decided by descided by SHT30(HI)
void setbrightness(){
  // send a message to update the light`
  char mqtt_message[100];
  sprintf(mqtt_message, "{\"brightness\": 60}");
  Serial.println(mqtt_topic_brightness);
  Serial.println(mqtt_message);
  if (client.publish(mqtt_topic_brightness, mqtt_message)) {
    Serial.println("Message published");
  } else {
    Serial.println("Failed to publish message");
  }

  // Perform rotation operations according to the direction descided by SHT30(HI)
  if (DisplayDirection == 0) {
    rotateRight(Brightness, 12);  // rotateRight
  }
  else if (DisplayDirection == 1){
    rotateLeft(Brightness, 12);   // rotateLeft
  }
}

// Select the corresponding color according to the number in DisplayColor
void setcolor(){
  switch (DisplayColor) {
    case 0:
      R_CDK = R_0; G_CDK = G_0; B_CDK = B_0;
      break;
    case 1:
      R_CDK = R_1; G_CDK = G_1; B_CDK = B_1;
      break;
    case 2:
      R_CDK = R_2; G_CDK = G_2; B_CDK = B_2;
      break;
    case 3:
      R_CDK = R_3; G_CDK = G_3; B_CDK = B_3;
      break;
    case 4:
      R_CDK = R_4; G_CDK = G_4; B_CDK = B_4;
      break;
    case 5:
      R_CDK = R_5; G_CDK = G_5; B_CDK = B_5;
      break;
    case 6:
      R_CDK = R_6; G_CDK = G_6; B_CDK = B_6;
      break;
    case 7:
      R_CDK = R_7; G_CDK = G_7; B_CDK = B_7;
      break;
    case 8:
      R_CDK = R_8; G_CDK = G_8; B_CDK = B_8;
      break;
    case 9:
      R_CDK = R_9; G_CDK = G_9; B_CDK = B_9;
      break;
    default:
      R_CDK = R_0; G_CDK = G_0; B_CDK = B_0;
      break;
  }
}

void measure_distance(){
  // Send trigger signal
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);  // Wait 2 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); // Send pulses for 10 microseconds
  digitalWrite(TRIG_PIN, LOW);

  // Echo reading
  long duration = pulseIn(ECHO_PIN, HIGH); // Returns the duration of the echo signal, in microseconds

  // Calculated distance, speed assumed 343 m/s (20°C)
  // Formula: distance = (duration * speed_of_sound) / 2
  // duration indicates the round-trip time of the pulse, and speed_of_sound = 34300 cm/s
  distance = (duration * 0.0343) / 2;

  // Output distance to serial port
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= 60){
    Dis_CDK = 1;
  }
  else{
    Dis_CDK = 0;
  }
}

void measure_sht30() {
  // Send measurement command 0x2C06
  Wire.beginTransmission(Addr_SHT30);
  Wire.write(0x2C);
  Wire.write(0x06);
  Wire.endTransmission();
  delay(50);  // Wait for the SHT30 measurement to be completed

  // Request 6 bytes of data
  Wire.requestFrom(Addr_SHT30, 6);

  // Read 6 bytes of data
  if (Wire.available() == 6) {
    sht30_data[0] = Wire.read();
    sht30_data[1] = Wire.read();
    sht30_data[2] = Wire.read();  // CRC verification data, not used at present
    sht30_data[3] = Wire.read();
    sht30_data[4] = Wire.read();
    sht30_data[5] = Wire.read();  // CRC verification data, not used at present
  } else {
    Serial.println("Data read failed, please check sensor connection!");
    return;
  }

  // Calculate the temperature and humidity
  sht30_cTemp = ((((sht30_data[0] * 256.0) + sht30_data[1]) * 175) / 65535.0) - 45;
  sht30_fTemp = (sht30_cTemp * 1.8) + 32;
  sht30_humidity = ((((sht30_data[3] * 256.0) + sht30_data[4]) * 100) / 65535.0);

  // Output data to the serial port
  Serial.print("Relative humidity: ");
  Serial.print(sht30_humidity);
  Serial.println(" %RH");
  Serial.print("Celsius Temperature:");
  Serial.print(sht30_cTemp);
  Serial.println(" C");
  Serial.print("Fahrenheit Temperature:");
  Serial.print(sht30_fTemp);
  Serial.println(" F");

  sht_HI = sht30_fTemp + 0.55 * (1 - (sht30_humidity / 100)) * (sht30_fTemp - 58);
  Serial.print("Heat humidity index");
  Serial.println(sht_HI);

  for (int i = 0; i < 9; i++) {
    if (sht_HI >= Level_HI[9]){
      DisplayColor  = 9;
      break;
    }
    if (sht_HI <= Level_HI[1]){
      DisplayColor  = 0;
      break;
    }
    if (sht_HI >= Level_HI[i] && sht_HI < Level_HI[i + 1]) {
      DisplayColor  = i;  // Return the color number corresponding to the interval (0-9)
      break;
    } 
  }
  Serial.print("Current color:");
  Serial.println(DisplayColor);

  if (sht_HI_old < sht_HI - 0.1){
    DisplayDirection = 0;
  }
  else if (sht_HI_old > sht_HI + 0.1){
    DisplayDirection = 1;
  }
  else{
    DisplayDirection = 11451;
  }
  sht_HI_old = sht_HI;
}

// Set the light brightness, light color, and the position of the lit Pixel, and send the data through MQTT
void sendmqtt(){
  // Change color while rotating, for testing purposes only
  // DisplayColor++;
  // if (DisplayColor > 9){
  //   DisplayColor = 0;
  // }
  setbrightness();
  setcolor();

  // send a message to update the light
  char mqtt_message[100];

  for (int i = 0; i <= 11; i++){
    //  sprintf(mqtt_message, "{\"pixelid\": %d, \"R\": 0, \"G\": 255, \"B\": 0, \"W\": 200}", i);
    sprintf(mqtt_message, "{\"pixelid\": %d, \"R\": %d, \"G\": %d, \"B\": %d, \"W\": %d}", i, R_CDK*Brightness[i]*Dis_CDK, G_CDK*Brightness[i]*Dis_CDK, B_CDK*Brightness[i]*Dis_CDK, 10);
    Serial.println(mqtt_topic_pixel);
    Serial.println(mqtt_message);
    if (client.publish(mqtt_topic_pixel, mqtt_message)){
      Serial.println("Message published");
    } else {
      Serial.println("Failed to publish message");
    }
  }

}