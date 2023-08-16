# Adaptive Cruise Control Project
This project is an implementation of an adaptive cruise control system using ATmega32 microcontroller, Bluetooth module, LCD, LDR, 3 LEDs, DC motor, Ultrasonic and external EEPROM. The system can automatically adjust the speed of a vehicle to maintain a safe distance from the vehicle in front. The system can also communicate with a smartphone app via Bluetooth to turn on/off cruise system and other functionalities. 
The system also uses an Ultrasonic sensor to detect obstacles and avoid collisions.\

## Hardware Components
The hardware components used in this project are:

ATmega32 microcontroller: This is the main controller of the system. It reads the inputs from the LDR, the Bluetooth module and the Ultrasonic sensor, and controls the outputs to the LCD, the LEDs and the DC motor.\
Bluetooth module: This is used to establish a wireless connection between the system and a smartphone app. The module can receive commands from the app to turn on/off cruise system and other functionalities, like active waiting state and emergency brake system.\
LCD: This is used to display the current distance values on the system and any system error occured.\
LDR: This is used to measure the intensity of light reflected by the vehicle in front.\
3 LEDs: These are used to give indication about the system, Green LED means car is mving safely, Blue LED means emergency brakes are on and will stop the car if it's too close to another car and Yellow LED for indicating that car is in waiting state.\
DC motor: This is used to simulate the vehicle’s speed. The motor’s speed is controlled by a PWM signal from the ATmega32.\
Ultrasonic sensor: This is used to measure the distance from any obstacle in front of the vehicle. The sensor sends out ultrasonic waves and measures the time it takes for them to bounce back. The sensor can trigger an emergency brake if the distance is too short.\
External EEPROM: This is used to store the distance and car status (Cruis was on or not, Erros occured) settings in case of power loss or reset.

### Software Components
The software components used in this project are:

Microship IDE: This is used to program the ATmega32 microcontroller using C language.\
MIT App Inventor: This is used to create a smartphone app for Android devices using visual programming. The app can communicate with the Bluetooth module and send commands to change the speed and distance settings, and receive feedback to display the current speed and distance values.\

### How to Use
To use this project, follow these steps:

Connect all the hardware components.\
Upload for the ATmega32 microcontroller using Extrem burner.\
Install MIT App Inventor for Android devices.\
Turn on both the system and the smartphone app, and pair them via Bluetooth.\
Enjoy your adaptive cruise control system!\
Demo Video
Soon.
