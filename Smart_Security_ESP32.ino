#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>
#include <TinyGPS++.h>
#include <LiquidCrystal_I2C.h> 
#include "Config.h"

// Components
Servo doorServo, auxServo;
TinyGPSPlus gps;
HardwareSerial SerialGPS(2); 
LiquidCrystal_I2C lcd(0x27, 16, 2); 

bool isEngineOn = false, isDoorLocked = true, isAuxLocked = true, theftDetected = false;
String gpsData = "Initializing...";

void setup() {
  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17); 
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Loading..");

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TAMPER_SENSOR_PIN, INPUT); 
  pinMode(DOOR_SENSOR_PIN, INPUT);   

  doorServo.attach(SERVO_PIN);
  auxServo.attach(15); 
  
  updateHardware();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  lcd.clear();
  lcd.print("WiFi Connected!");
}

void loop() {
  // 1. GPS Section
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }

  if (gps.location.isUpdated() && gps.location.isValid()) {
      gpsData = String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  } else {
      gpsData = (gps.charsProcessed() < 10) ? "GPS Error" : "Searching...";
  }

  // 2. Sensor Reading
  int tamperTrigger = digitalRead(TAMPER_SENSOR_PIN); 
  int doorTrigger = digitalRead(DOOR_SENSOR_PIN);     

  // 3. Flask Communication
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl); 
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<400> doc;
    // Engine OFF-a irukkumpothu trigger aana thaan Theft Mode
    doc["tampering"] = (tamperTrigger == HIGH && !isEngineOn);
    doc["door_break"] = (doorTrigger == LOW); 
    doc["gps"] = gpsData;

    String body;
    serializeJson(doc, body);
    int code = http.POST(body);

    if (code == 200) {
        String response = http.getString();
        StaticJsonDocument<512> res;
        DeserializationError error = deserializeJson(res, response);
        
        if (!error) {
            isEngineOn = res["isEngineOn"];
            isDoorLocked = res["isDoorLocked"];
            isAuxLocked = res["isAuxLocked"]; 
            theftDetected = res["theftDetected"];
            
            // --- THEFT ALERT LOGIC START ---
            if (theftDetected == true) {
                digitalWrite(BUZZER_PIN, HIGH);
                // LCD-la immediate display update
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("! THEFT MODE !");
                lcd.setCursor(0, 1);
                lcd.print("SYSTEM LOCKED");
                Serial.println("🚨 ALERT: THEFT DETECTED!");
            } else {
                digitalWrite(BUZZER_PIN, LOW);
            }
            // --- THEFT ALERT LOGIC END ---

            Serial.print("Update Received -> Engine: "); Serial.print(isEngineOn);
            Serial.print(" | Door: "); Serial.println(isDoorLocked);
        }
    } else {
        Serial.print("HTTP Error Code: "); Serial.println(code);
    }
    http.end();
  }

  // 4. Hardware and LCD Refresh
  updateHardware(); 
  if (!theftDetected) updateLCD(); // Theft illana mattum normal LCD refresh
  
  delay(200); 
}

void updateLCD() {
    lcd.setCursor(0, 0);
    lcd.print("D:"); lcd.print(isDoorLocked ? "LOCK" : "OPEN");
    lcd.print("  A:"); lcd.print(isAuxLocked ? "LOCK" : "OPEN");
    
    lcd.setCursor(0, 1);
    lcd.print("Engine: ");
    lcd.print(isEngineOn ? "ON " : "OFF");
}

void updateHardware() {
  if (theftDetected) {
    digitalWrite(RELAY_PIN, LOW); // Engine cut-off
    doorServo.write(0);           // Lock Door
    auxServo.write(0);            // Lock Aux
  } else {
    digitalWrite(RELAY_PIN, isEngineOn ? HIGH : LOW);
    doorServo.write(isDoorLocked ? 0 : 90);
    auxServo.write(isAuxLocked ? 0 : 90);
  }
}