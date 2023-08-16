# Factory Monitor System

## Overview
The Factory Monitor System is a comprehensive solution designed to monitor and manage various environmental parameters in a factory setting. The system is built around the ATmega32 microcontroller and utilizes a variety of sensors and components to ensure the safety and efficiency of your factory operations.

## Components
ATmega32 Microcontroller: The brain of the system, responsible for coordinating all other components.
4x20 LCD: Used to display real-time data from the sensors.
LM35 Temperature Sensor: Monitors the temperature within the factory.
Gas Sensor: Detects harmful gases that may be present in the air.
Humidity Sensor: Measures the moisture content in the air.
EEPROM Internal Memory: Stores logs of any detected hazards for future reference.
Ultrasonic Sensor: Detects if someone comes near the device.
Buzzer: Sounds an alarm when a hazard is detected.
LED: Provides visual indication of hazard detection.
Operation
The system continuously monitors the factory environment using its array of sensors. If a hazard is detected (such as high temperature, presence of harmful gases, or high humidity), the system will start an alarm, turning on the buzzer and LED. The details of the hazard are logged in the EEPROM internal memory for later analysis.

If someone approaches the device while the alarm is active (as detected by the ultrasonic sensor), the buzzer and LED will turn off until they move away. This allows personnel to approach the device safely to investigate and address the hazard.

Once the hazard has been addressed and conditions return to normal, the system will resume its normal operation. If conditions do not improve, the alarm will start again, ensuring continuous monitoring and safety.

## Note
Please ensure that all components are properly connected and powered for optimal operation. Always follow safety guidelines when operating in a factory environment.
