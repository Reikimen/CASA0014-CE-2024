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

If the user is seated, the Chrono Lumina Light illuminates and calculates the Temperature-Humidity Index (THI) based on temperature and humidity, which is classified into ten levels. The THI value is displayed in a gradient of colors from low to high, ranging from violet and cyan to orange-red and red. The direction of rotation of the light ring reflects changes in the environment: as the HI decreases, the ring rotates clockwise, indicating improved environmental conditions; as the HI increases, the ring rotates counterclockwise, indicating deteriorated environmental conditions.

> What's Temperature-Humidity Index? — Temperature-Humidity Index is used for indicate the level of stress and discomfort experienced by individuals.
>
> Here are some refer websites and files.
>
> [How Do I Determine?: How do I calculate Temperature-Humidity Index (THI)? - | Ag Proud](https://www.agproud.com/articles/27297-how-do-i-determine-how-do-i-calculate-temperature-humidity-index-thi)
>
> [Temperature Humidity Index (THI) Calculator Online](https://www.erp-information.com/calculators/temperature-humidity-index-thi-calculator?utm_content=cmp-true)

### Materials for "Heat Alert"

ESP8266 Dev Board x 1

Chrono Lumina Light x 1

HC-SR04 x 1 (for detecting distance)

SHT30 x 1 (for temp and humidity)

Dupont x several

### The Design of "Heat Alert"

#### The Repo Structure



#### The workflow of "Heat Alert"



#### Pin Circuit and Modeling



### Challenges and Limitations















