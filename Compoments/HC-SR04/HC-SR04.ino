#include <ESP8266WiFi.h>

// 定义 HC-SR04 的引脚
#define TRIG_PIN 12   // 触发引脚 D6
#define ECHO_PIN 13   // 回声引脚 D7

void setup() {
  // 初始化串口通信
  Serial.begin(115200);

  // 设置引脚模式
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // 确保 TRIG_PIN 初始为低电平
  digitalWrite(TRIG_PIN, LOW);

  // 等待稳定
  delay(1000);
}

void loop() {
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
  long distance = (duration * 0.0343) / 2;

  // 输出距离到串口
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 每秒读取一次
  delay(1000);
}
