// edit this for the light you are connecting to
// char mqtt_topic_demo[] = "student/CASA0014/light/18/brightness/";
char mqtt_topic_pixel[] = "student/CASA0014/light/3/pixel/";            // used to control individual pixels on the ring
char mqtt_topic_all[] = "student/CASA0014/light/3/all/";                // used to control all LED's in one go
char mqtt_topic_brightness[] = "student/CASA0014/light/3/brightness/";  // used to set the brightness of all the LED's 
char mqtt_topic_demo[] = "student/CASA0014/light/3/demo/";            // used to control individual pixels on the ring
char mqtt_topic_fire[] = "student/CASA0014/light/3/fire/";            // used to control individual pixels on the ring

int Brightness[12] = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};

const int R_0 = 127, G_0 = 0, B_0 = 255;// 紫色
const int R_1 = 0, G_1 = 72, B_1 = 255;
const int R_2 = 0, G_2 = 127, B_2 = 255;
const int R_3 = 0, G_3 = 255, B_3 = 127;
const int R_4 = 0, G_4 = 255, B_4 = 72;
const int R_5 = 72, G_5 = 255, B_5 = 0;
const int R_6 = 127, G_6 = 255, B_6 = 0;
const int R_7 = 255, G_7 = 255, B_7 = 0;
const int R_8 = 255, G_8 = 127, B_8 = 0;
const int R_9 = 255, G_9 = 0, B_9 = 0; // 红色
int R_CDK = 0, G_CDK = 0, B_CDK = 0;
int Dis_CDK = 0; // 1 for distance smaller than 60, 0 for distance larger... 
float Level_HI[10] = {75.0000, 78.1111, 81.2222, 84.3333, 87.4444, 90.5556, 93.6667, 96.7778, 99.8889, 104.0000};
int DisplayColor = 0;

// 当检测到CO2浓度下降时，逆时针旋转；浓度上升，顺时针旋转
int DisplayDirection = 0; // 0 for Counter Clock Wise; 1 for Clock Wise ; 11451 for not move

// 函数：右移数组
void rotateRight(int arr[], int size) {
    int last = arr[size - 1];  // 保存最右边的元素
    for (int i = size - 1; i > 0; i--) {
        arr[i] = arr[i - 1];  // 每个元素右移一位
    }
    arr[0] = last;  // 最右边的元素跳到最左边
}

// 函数：左移数组
void rotateLeft(int arr[], int size) {
    int first = arr[0];  // 保存最左边的元素
    for (int i = 0; i < size - 1; i++) {
        arr[i] = arr[i + 1];  // 每个元素左移一位
    }
    arr[size - 1] = first;  // 最左边的元素跳到最右边
}

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

  // 根据输入方向执行相应的旋转操作
  if (DisplayDirection == 0) {
      rotateRight(Brightness, 12);  // 右移
  } 
  else if (DisplayDirection == 1){
      rotateLeft(Brightness, 12);   // 左移
  }
}

// 根据DisplayColor的数字，选择对应的颜色
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

void sendmqtt(){
  // DisplayColor++;
  // if (DisplayColor > 9){
  //   DisplayColor = 0;
  // }
  setbrightness();
  setcolor();

  // send a message to update the light`
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

#define TRIG_PIN 12   // 触发引脚 D6
#define ECHO_PIN 13   // 回声引脚 D7
long distance = 114;

void measure_distance(){
  // 发送触发信号
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);  // 等待 2 微秒
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); // 发送 10 微秒的脉冲
  digitalWrite(TRIG_PIN, LOW);

  // 读取回声信号
  long duration = pulseIn(ECHO_PIN, HIGH); // 返回的是回声信号持续时间，单位：微秒

  // 计算距离，速度假定为 343 m/s (20°C)
  // 公式：distance = (duration * speed_of_sound) / 2
  // 其中，duration 是脉冲往返的时间，speed_of_sound = 34300 cm/s
  distance = (duration * 0.0343) / 2;

  // 输出距离到串口
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

// SHT30传感器的I2C地址
#define Addr_SHT30 0x44

// 存储SHT30测量数据的变量
uint8_t sht30_data[6];
float sht30_cTemp;     // 摄氏温度
float sht30_fTemp;     // 华氏温度
float sht30_humidity;  // 相对湿度
float sht_HI = 0;
float sht_HI_old = 0;

void measure_sht30() {
  // 发送测量命令 0x2C06
  Wire.beginTransmission(Addr_SHT30);
  Wire.write(0x2C);
  Wire.write(0x06);
  Wire.endTransmission();
  delay(50);  // 等待SHT30测量完成

  // 请求获取6字节的数据
  Wire.requestFrom(Addr_SHT30, 6);

  // 读取6字节的数据
  if (Wire.available() == 6) {
    sht30_data[0] = Wire.read();
    sht30_data[1] = Wire.read();
    sht30_data[2] = Wire.read();  // CRC校验数据，目前未用
    sht30_data[3] = Wire.read();
    sht30_data[4] = Wire.read();
    sht30_data[5] = Wire.read();  // CRC校验数据，目前未用
  } else {
    Serial.println("数据读取失败，请检查传感器连接！");
    return;
  }

  // 计算温度和湿度
  sht30_cTemp = ((((sht30_data[0] * 256.0) + sht30_data[1]) * 175) / 65535.0) - 45;
  sht30_fTemp = (sht30_cTemp * 1.8) + 32;
  sht30_humidity = ((((sht30_data[3] * 256.0) + sht30_data[4]) * 100) / 65535.0);

  // 输出数据到串口
  Serial.print("相对湿度：");
  Serial.print(sht30_humidity);
  Serial.println(" %RH");
  Serial.print("摄氏度温度：");
  Serial.print(sht30_cTemp);
  Serial.println(" C");
  Serial.print("华氏度温度：");
  Serial.print(sht30_fTemp);
  Serial.println(" F");

  sht_HI = sht30_fTemp + 0.55 * (1 - (sht30_humidity / 100)) * (sht30_fTemp - 58);
  Serial.print("热量湿度指数");
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
      DisplayColor  = i;  // 返回区间对应的颜色编号（0-9）
      break;
    } 
  }
  Serial.print("现在的颜色: ");
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
