Calculator on ATmega32 using FreeRTOS



This project is a simple calculator that runs on an ATmega32 microcontroller and uses FreeRTOS as the operating system. The calculator can perform (only one digit calculation) basic arithmetic operations such as addition, subtraction, multiplication, and division. The calculator has a 4x4 keypad for input and a 16x2 LCD for output.

Hardware Requirements



To run this project, you will need the following hardware components:

An ATmega32 microcontroller
A 4x4 keypad
A 16x2 or 20x4 LCD (Code support both)
A breadboard and some wires
A USBasp programmer

Software Requirements
To compile and upload this project, you will need the following software tools:\

AVR Studio 7 or later
WinAVR
FreeRTOS 10.4.3 or later
avrdude

Project Structure


This project consists of four main tasks that run concurrently on FreeRTOS:\

LCD_Task: This task is responsible for initializing and updating the LCD display. It reads the input from the keypad task and the result from the calculation task and displays them on the LCD.
Keypad_Task: This task is responsible for scanning and debouncing the keypad input. It sends the pressed key to the LCD task and the calc task via queues.
Timing_Task: This task is responsible for update time on LCD every second by sending current time to LCD through queue.
Calculation_Task: This task is responsible for evaluating the postfix expression and performing the arithmetic operations. It sends the result to the LCD task via a queue.

How to Run?


To run this project, follow these steps:

Connect the hardware components.
Download or clone this repository to your local machine.
Open the project folder in AVR Studio and select your device (ATmega32) and programmer (USBasp) in the project settings.
Build the project and upload it to your microcontroller using avrdude.
Enjoy your calculator!
