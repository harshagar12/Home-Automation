#ifdef ESP32
#include <WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>//By: Brian Lough
#include <ArduinoJson.h>//By: Benoit Blanchon

// Replace with your network credentials
const char* ssid = "your-ss-id";
const char* password = "your-wifi-password";


// Initialize Telegram BOT
#define BOTtoken "your-Bot-Token-(Get-from-Botfather)" 

// Use @myidbot to find out the chat ID of an individual or a group
#define CHAT_ID "your-chat-id"

#ifdef ESP8266
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Pin definitions(Change as per your wish)
const int lights = 32;
const int fan1 = 33;
const int fan2 = 16;
const int relayControlPin = 17;
const int motionSensorPin = 21;

// Timer: Auxiliary variables
volatile bool motionDetected = false;
unsigned long lastMotionTime = 0;
const unsigned long motionTimeout = 10000; // 10 seconds timeout for motion detection(Can be changed)

// Initialize Default Relay state
bool ledState1 = HIGH;
bool ledState2 = HIGH;
bool ledState3 = HIGH;
bool ledState4 = HIGH;

void IRAM_ATTR onMotionDetected() {
  motionDetected = true;
  lastMotionTime = millis();
}

//Checks for new messages and reponds respectively
void handleNewMessages() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  //Recieves New Messages
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    String text = bot.messages[i].text;
    Serial.println(text);

    //Displays all the message inputs(Can change as per your requirement)
    if (text == "start") {
      String welcome = "Welcome!\n";
      welcome += "Use the following commands to control your devices:\n\n";
      welcome += "lights_on - Turn lights ON\n";
      welcome += "lights_off - Turn lights OFF\n";
      welcome += "fan1_on - Turn fan 1 ON\n";
      welcome += "fan1_off - Turn fan 1 OFF\n";
      welcome += "fan2_on - Turn fan 2 ON\n";
      welcome += "fan2_off - Turn fan 2 OFF\n";
      welcome += "all_off - Turn everything OFF\n";
      welcome += "state - Get current state of all appliances\n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "lights_on") {
      bot.sendMessage(chat_id, "Lights set to ON", "");
      ledState1 = LOW;
      digitalWrite(lights, ledState1);
      Serial.println("Lights Set to ON.");
    }

    if (text == "lights_off") {
      bot.sendMessage(chat_id, "Lights set to OFF", "");
      ledState1 = HIGH;
      digitalWrite(lights, ledState1);
      Serial.println("Lights Set to OFF.");
    }

    if (text == "fan1_on") {
      bot.sendMessage(chat_id, "Fan 1 set to ON", "");
      ledState3 = LOW;
      digitalWrite(fan1, ledState3);
      Serial.println("Fan 1 Set to ON.");
    }

    if (text == "fan1_off") {
      bot.sendMessage(chat_id, "Fan 1 set to OFF", "");
      ledState3 = HIGH;
      digitalWrite(fan1, ledState3);
      Serial.println("Fan 1 Set to OFF.");
    }

    if (text == "fan2_on") {
      bot.sendMessage(chat_id, "Fan 2 set to ON", "");
      ledState2 = LOW;
      digitalWrite(fan2, ledState2);
      Serial.println("Fan 2 Set to ON.");
    }

    if (text == "fan2_off") {
      bot.sendMessage(chat_id, "Fan 2 set to OFF", "");
      ledState2 = HIGH;
      digitalWrite(fan2, ledState2);
      Serial.println("Fan 2 Set to OFF.");
    }

    if (text == "all_off") {
      bot.sendMessage(chat_id, "Everything set to OFF", "");
      digitalWrite(fan1, ledState4);
      digitalWrite(fan2, ledState4);
      digitalWrite(lights, ledState4);
      Serial.println("Everything Set to OFF.");
    }

    if (text == "state") {
      String stateMessage = "Current state:\n";
      stateMessage += "Lights: " + String(ledState1 == LOW ? "ON" : "OFF") + "\n";
      stateMessage += "Fan 1: " + String(ledState3 == LOW ? "ON" : "OFF") + "\n";
      stateMessage += "Fan 2: " + String(ledState2 == LOW ? "ON" : "OFF") + "\n";
      stateMessage += "Motion Light: " + String(digitalRead(relayControlPin) == LOW ? "ON" : "OFF");
      bot.sendMessage(chat_id, stateMessage, "");
    }
  }
}

//Checks for Motion and Responds Respectively
void handleMotionSensor() {
  if (motionDetected) {
    motionDetected = false;
    Serial.println("Motion Detected!");
    ledState4 = LOW;
    digitalWrite(relayControlPin, ledState4);
    Serial.println("Motion Light Set to ON.");
  }

  //Turns of Light after 10 seconds of Inactivity
  if (millis() - lastMotionTime > motionTimeout) {
    Serial.println("Motion stopped...");
    digitalWrite(relayControlPin, HIGH);
    ledState4 = HIGH;
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");
    client.setTrustAnchors(&cert);
  #endif

  pinMode(lights, OUTPUT);
  digitalWrite(lights, ledState1);

  pinMode(fan2, OUTPUT);
  digitalWrite(fan2, ledState2);

  pinMode(fan1, OUTPUT);
  digitalWrite(fan1, ledState3);
  
  pinMode(motionSensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(motionSensorPin), onMotionDetected, RISING);

  pinMode(relayControlPin, OUTPUT);
  digitalWrite(relayControlPin, ledState4);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  #endif

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  bot.sendMessage(CHAT_ID, "Connected to WiFi", "");
  bot.sendMessage(CHAT_ID, WiFi.localIP().toString(), "");
}

void loop() {
  handleMotionSensor();

  if (WiFi.status() == WL_CONNECTED) {
    handleNewMessages();
  }
}