# Smart Manhole Monitoring System

## Overview
The Smart Manhole Monitoring System leverages IoT technology to detect and monitor water levels and unauthorized access to manhole covers. The system uses a PIC microcontroller to read sensor data, which is then sent to Firebase for real-time monitoring. A web dashboard provides visual insights and alerts for effective management.

## Technologies
- **Embedded Systems**: PIC Microcontroller, C programming
- **IoT**: ESP8266 Wi-Fi Module, Firebase Realtime Database
- **Web Development**: HTML, TailwindCSS, JavaScript, Chart.js
- **Sensors**: Ultrasonic (HC-SR04), Tilt Sensor
- **Communication**: UART, Firebase Integration

## Features
- **Water Level Monitoring**: Detects the water level using an ultrasonic sensor and triggers alerts if the water level exceeds a threshold.
- **Tilt Sensor Integration**: Detects unauthorized access or opening of the manhole cover using a tilt sensor.
- **Real-time Data Upload**: Sensor data is uploaded to Firebase in real-time, enabling remote monitoring.
- **Web Dashboard**: A responsive dashboard displays live data and trends, providing real-time insights into the system’s status.
- **Alerts**: Alerts and notifications for abnormal conditions like high water levels or open manhole covers.

## Project Structure

### Microcontroller Code
- **C Code** for the PIC Microcontroller that reads data from the ultrasonic and tilt sensors and sends it via UART.
- **UART Communication** between the microcontroller and the ESP8266 for data transmission.

### Firebase Integration
- **ESP8266 Firmware** to receive sensor data and upload it to Firebase for cloud-based storage and real-time monitoring.

### Web Dashboard
- **Responsive UI** built with HTML and TailwindCSS for displaying the system status, water level, and manhole status.
- **Chart.js** integration for real-time visualization of the water distance trend.

## Installation

### Prerequisites
- PIC Microcontroller
- Ultrasonic Sensor (HC-SR04)
- Tilt Sensor
- ESP8266 Wi-Fi Module
- Firebase account for database and authentication

### Setup
1. **Microcontroller Setup**: Upload the provided C code to the PIC microcontroller.
2. **Firebase Setup**: Configure your Firebase project and use the provided authentication details in the ESP8266 code.
3. **Dashboard**: Host the HTML dashboard on your web server to visualize the data in real-time.

