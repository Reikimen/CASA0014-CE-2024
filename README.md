---
typora-root-url: ./
---

# Heat Alert — Project for CASA0014

![HeatAlert-cut](/image/HeatAlert-cut.jpg)

("Heat Alert" is the little frog-shaped ornament pictured above.)

## Feeling drowsy while studying or working? It might not the reason you're uninterested — it could be pyrexia or heat stroke!

When I was young, as my mother was a primary school teacher, I was often caught studying by her every summer holiday. Although I didn't particularly love studying, I didn't hate it either. However, one day, the state of learning was terrible - a huge stack of wrong answers, but also sometimes nap on the table. At first, my mother was very angry with my performance, blaming me for my poor attitude to study. But gradually, she sensed that I seemed to be really unwell. Strangely enough, I neither had a cold or a fever, nor did I have a stomachache, so what was going on?

It was only after we arrived at the hospital that it dawned on me and my mother that I had been struck by pyrexia that afternoon. It is commonly believed that this acute illness occurs during the hot summer months (June to August) and is often caused by strong direct sunlight, especially in the mid-latitude regions of China. However, I wasn't outside in the sun that afternoon, so we didn't associate it with pyrexia at first. According to the doctor's explanation, there are actually many causes of pyrexia: for example, staying in a hot, unventilated environment for a long period of time, where the accumulation of heat in the body exceeds the ability to dissipate heat, or the body's poor adaptability to hot environments. In addition, when the temperature and humidity rise at the same time, the risk of heat stroke is greatly increased.

This experience made me realize that pyrexia can not only occur outdoors, it can also creep up indoors, especially in a hot and humid environment. At that time, if we could have realized the discomfort of the environment earlier, we might have been able to avoid physical discomfort or even danger. 

## So, what can we do about it?

After reflecting on this experience, an idea came to my mind: Is it possible to use a device that monitors the comfort level of the environment in real time and alerts people in time to prevent heat stroke? So the project **"Heat Alert"** was designed - it monitors temperature, humidity and other data, and provides smart alerts to help people better avoid the risk of heat stroke.

### User Experience of "Heat Alert"

The frog-shaped ornament can be placed in front of a desk to automatically detect seat occupancy and indicate the comfort level of the current study or office environment through the Chrono Lumina Light. 

If the user is seated, the Chrono Lumina Light illuminates and calculates the **Temperature-Humidity Index (THI)** based on temperature and humidity, which is classified into ten levels. The THI value is displayed in a gradient of colors from low to high, ranging from violet and cyan to orange-red and red. 

**The direction of rotation of the light ring reflects changes in the environment:** as the THI decreases, the ring rotates clockwise, indicating improved environmental conditions; as the THI increases, the ring rotates counterclockwise, indicating deteriorated environmental conditions. 

This is an example demonstration of a THI rising and then falling correspondence: [Showcase.qt](image/Showcase.qt) 



> **What's Temperature-Humidity Index?** — Temperature-Humidity Index is used for indicate the level of stress and discomfort experienced by individuals:
>
> ​	***THI = Tdb – [0.55 – (0.55 x RH/100)] x (Tdb – 58)***
>
> Here are some refer websites and files.
>
> [How Do I Determine?: How do I calculate Temperature-Humidity Index (THI)? - | Ag Proud](https://www.agproud.com/articles/27297-how-do-i-determine-how-do-i-calculate-temperature-humidity-index-thi)
>
> [Temperature Humidity Index (THI) Calculator Online](https://www.erp-information.com/calculators/temperature-humidity-index-thi-calculator?utm_content=cmp-true)

<img src="/image/Weather_Hazard_Guide.png" alt="Weather_Hazard_Guide" style="zoom: 40%;" />



### Materials for "Heat Alert"

```json
ESP8266 Dev Board x 1

Chrono Lumina Light x 1

HC-SR04 x 1 (for detecting distance)

SHT30 x 1 (for temp and humidity)

Dupont x several
```



### The Design of "Heat Alert"

#### The Repo Structure

```plaintext
- Sensor-Sys/
            - Chrono-Dankao-ESP8266/
                        - Chrono-Dankao-ESP8266.ino
                        - MyDisplay.h
                        - WiFi-MQTT-Connection.h
            - CASA0014-v1.0.f3d
            - Components/
                        - HC-SR04/..
                        - SHT30-org-from-2023/..
                        - ...
            - images/
                        - HeatAlert-cut.jpg
                        - ESP8266-Pins.png
                        - ...
            - README.md
```

**The repo contains the following components:**

> - Chrono-Dankao-ESP8266/
>
> 	- The main program folder, opened with an Arduino and burned into the ESP8266 
>
> - CASA0014-v1.0.f3d
>
> 	- Autodesk Fusion 360 File for "Heat Alert"
>
> - Components/
>
> 	- Components folder, which contains how to use each component, some of the improved non-blocking runtime programs, and programs for checking circuit connections (e.g., items for checking the connection of I2C devices: I2C_address_search)
>
> - images/
>
> 	- For storing images in the README
>
> - README.md

#### Main program folder - Chrono-Dankao-ESP8266/

Here is an overview of what each file in the main program does:

> ##### Chrono-Dankao-ESP8266.ino
>
> - Initialization (including sensor, network and display).
>
> - Timed reading of sensor data.
>
> - Calling functions of other modules (e.g. Wi-Fi status check, connect, reconnect, distance measurement, temperature and humidity detection) to realize the main logic of the project.
>
> 	
>
> ##### WiFi-MQTT-Connection.h
>
> - Wi-Fi  and MQTT Configuration
>
> - **Main Functions:**
>- `startWifi()`: connects to Wi-Fi.
> 	
>- `reconnectMQTT()`: Ensure the MQTT connection is stable and reconnect automatically if necessary.
> 	
>- `callback()`: process incoming MQTT messages.
> 
>
> 		
>##### MyDisplay.h (Display & Perception Logic)
> 
>- Sensor data reading and processing:
> 
>	- Temperature and humidity are measured using the SHT30 sensor and the Temp-Humdity Index (THI) is calculated using a formula.
> 	- The display color (`DisplayColor`) and rotation direction (`DisplayDirection`) are determined based on the Heat Index.
> 
>- MQTT message sending:
> 
>	- Send light status (color, brightness, pixel status) to the server via MQTT.
> 
>- Distance Measurement:
> 
>	- Measure the distance between the object and the sensor using the ultrasonic sensor (trigger pin and echo pin) and control the display according to the threshold.
> 
>- **Main functions:**
> 	- `measure_sht30()`: handles temperature and humidity measurement and color selection logic.
> 	- `measure_distance()`: measure object distance.
> 	- `sendmqtt()`: send light control data.
> 	- `setbrightness()` and `setcolor()`: set brightness and color for pixels.
> 



#### The workflow of "Heat Alert"

<img src="/image/HeatAlert-LoopWorkflow.png" alt="HeatAlert-LoopWorkflow" style="zoom:60%;" />

First, the project is powered up (Initialisation), the ESP8266 is powered on, connected to Wi-Fi and MQTT server (and their connectivity is checked periodically), and the ultrasonic sensor (HC-SR04) and the temperature and humidity sensor (SHT30) are initialised. 

After initialisation, if the user is detected in front of the table, it proceeds into environmental monitoring; if it is not detected, it remains in standby. SHT30 collects temperature and humidity data and is used to calculate the THI. The colour reflects the comfort level of the environment and the direction of rotation reflects the trend of the comfort level.



#### Pin Allocation, Circuit and Modeling

##### Pin Allocation & Circuit Design:

<img src="/image/ESP8266-Pins.png" alt="ESP8266-Pins" style="zoom:50%;" />

The above figure shows the pinout of the ESP8266, and the following figure shows the corresponding circuit connections:

![Connection](/image/Connection.jpg)



##### Modeling:

<img src="/image/modeling.png" alt="modeling" style="zoom: 50%;" />

In fact, the above design ignores the problem of placing the ultrasonic sensor into the mould. Therefore, in the actual prototype, it is required to carefully saw off the raised part at the back of the mould, then paste the sawed off part back after placing the ultrasonic transducer in the mould. This is a small design error that can be easily (and should be) fixed in future developments.



## What other contributions can we make to society through the (IoT) technologies employed in this project?



### IoT in Built Environments

**Environmental comfort monitoring:** Some smart air conditioners (e.g. Midea's i+ air conditioning system) are able to adjust the temperature and humidity automatically based on indoor data, providing a comfortable indoor environment while reducing energy consumption.

**Smart Cities:** Smart transport, waste management and public lighting optimisation, for example. London's smart traffic light system adjusts signal durations based on real- time traffic flow, reducing congestion and carbon emissions.

**Healthcare applications:** IoT devices such as the Apple Watch and the Xiaomi sports bracelet, which have very useful functions such as monitoring heart rate and steps, and wearable devices to monitor user health data.



### IoT in Natural Environments

**Wildlife conservation**: For example, the ‘BAT BOX’ built by UCL and Intel at Elizabeth Park monitors wildlife activity and provides data to support conservation efforts.

**Agriculture**: Helping farmers to optimise irrigation and fertiliser application, reduce resource wastage and increase yields. Israel has developed an IoT-based precision drip irrigation system.



### Socio-Technical Scenarios

**Smart Home and Accessible Design**: Smart home devices help people with disabilities more easily do their daily activities, such as controlling lights or opening doors with voice commands (e.g. Xiaomi's Smart Home).

**Energy Efficiency**: Using IoT technology to optimise energy distribution in the smart grid in real time - China Southern Power Grid has deployed IoT smart meters in a number of cities to visualise and dynamically regulate electricity consumption data.

**Community Resilience**: IoT devices such as air quality sensors help communities identify sources of pollution and develop countermeasures. For example, Beijing's network of PM2.5 sensors helps citizens understand air pollution and adjust their outdoor activities. 



## Challenges and Limitations (Reflection)

Although the project went quite smoothly and successfully implemented most of the designed features, this project still has its limitations:

1. **Energy Consumption**
   Continuously running sensors and lighting systems may result in high energy consumption, especially when used for long periods of time.

2. **User experience**

   The Thermal Comfort Index (THI) needs to be visually presented to the user through colour and direction of rotation. However, the colour is perceived and understood differently by different groups of people, which may affect the effectiveness of the information conveyed.

3. **User Privacy and Security**

   The MQTT protocol itself does not mandate encryption or authentication, which allows attackers to easily intercept unencrypted communication content, leading to sensitive data leakage.
