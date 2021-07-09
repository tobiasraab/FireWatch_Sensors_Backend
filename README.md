# Fire Watch Sensor Backend
## About Fire Watch

Fire Watch is an open source product to help prevent forest fires in different climatic zones.
Our sensor stations will track the environmental conditions in your forest to give you all the information you need to prevent a fire.

## Hardware
* ESP32
* LoRaModule
* Humidity Sensor
* Temperature Sensor

![sensorImg1](./assets/images/sensorImg1.jpg)
![sensorImg2](./assets/images/sensorImg2.jpg)
![sensorImg3](./assets/images/sensorImg3.jpg)
![sensorImg4](./assets/images/sensorImg4.jpg)

## Communication Technologie
* WiFi (mqtt)

## Power
### Power  Supply
* rechargable battery

### Runing Period
* Deep Sleep: 0,005mA
* WiFi: 400mA
* ... -- 20min Deeplsleep -- 5min WiFi -- 20min Deeplsleep -- 5min WiFi -- ... : 80.004mA
* Battery: 30000mAh

--> Deep Sleep: 6000000h = 684 years<br>
--> WiFi:       75h<br>
--> Combined:   374h<br>


### Future Improvements
* testing power supply with wind energy
* implement solar panel



## Sensor Service
// pushDataInDatabase.js
* get data via mqtt connection
* insert data in databse collection 'sensorData'
* delete old database entrys

### Scalable
1) let multiple sensors send on LoRaWAN
2) insert received data in database collection 'sensorData'
3) delete old sensor sensor data in database



## Software Architecture

### Frontend Application
https://github.com/tobiasraab/FireWatch_App_Frontend

### Backend Application
#### User Service
https://github.com/tobiasraab/FireWatch_App_Backend_userService

#### Database Service
https://github.com/tobiasraab/FireWatch_APP_Backend_databaseService

#### Weather Service
https://github.com/tobiasraab/FireWatch_App_Backend_weatherService

### Sensor Backend
https://github.com/tobiasraab/FireWatch_Sensors_Backend

### MongoDB Database
https://www.mongodb.com/
#### Collection forests:<br>
* 0.2kB per forest
#### Collection sensorDatas:<br>
* 0.45kB per Sensor
#### Collection userData:<br>
* 0.5kB per user
#### Collection weather:<br>
* 0.8kB per forest




## Future Work
* Deep Sleep for ESP32 (save LoRaWan Token)
* implement LoRaWAN
