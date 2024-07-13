                                 Telegram Controlled Home Automation with ESP32
This project demonstrates how to control home appliances using a Telegram bot and an ESP32 microcontroller. It allows you to remotely turn on/off lights and fans, monitor their states, and respond to motion detection using Telegram commands.


Prerequisites:
Arduino IDE installed
ESP32 board support installed in Arduino IDE

Necessary libraries:
UniversalTelegramBot
ArduinoJson

Hardware Requirements:
ESP32 microcontroller
Motion sensor
Relay
WiFi connection

Installation:
Clone the repository
Copy code: git clone https://github.com/your-username/your-repo.git
Open the Arduino IDE.

Install the required libraries:

UniversalTelegramBot
ArduinoJson


Replace the following placeholders in the code:
ssid: Replace with your WiFi network name.
password: Replace with your WiFi network password.
BOTtoken: Replace with your Telegram Bot token obtained from BotFather.
CHAT_ID: Replace with your Telegram chat ID.
Upload the code to your ESP32 board.

Usage
Power up your ESP32 board.
Connect it to your WiFi network.
Start a conversation with your Telegram bot.

Use the following commands to control your devices(which can be changed as per usage):
start: Displays available commands.
lights_on, lights_off: Control lights.
fan1_on, fan1_off, fan2_on, fan2_off: Control fans.
all_off: Turn off all devices.
state: Get current states of devices.


Circuit Diagram:
Included in the Repository.

Contributing:
Feel free to contribute to this project by forking it and submitting pull requests.

License:
This project is licensed under the MIT License - see the LICENSE file for details.


