// --- WiFi Settings ---
// Inga unga laptop hotspot details correct-a irukanum
const char* ssid = "ATCHUUHH 4258";      
const char* password = "ATCHAYA0000";  

// --- Server Settings ---
// Laptop Hotspot use pannum pothu IP eppovumae 192.168.137.1 thaan
const char* serverUrl = "http://192.168.137.1:5000/status";

// --- I2C LCD Settings (Added) ---
// ESP32 Default I2C Pins: SDA=21, SCL=22
#define I2C_SDA 21
#define I2C_SCL 22
#define LCD_ADDR 0x27  // Unga LCD-oda address (vethiyasamna 0x3F check pannunga)
#define LCD_COLS 16
#define LCD_ROWS 2

// --- Sensor Pins ---
#define TAMPER_SENSOR_PIN 34  // Voltage Divider (10k/22k) Input Pin
#define DOOR_SENSOR_PIN 4     // IR Sensor Input Pin

// --- Actuator Pins ---
#define SERVO_PIN 13          // Main Door Servo
#define AUX_SERVO_PIN 15      // Secondary Lock Servo
#define RELAY_PIN 12          // DC Motor (Engine) Relay
#define BUZZER_PIN 14         // Security Alarm Buzzer

// --- GPS Settings ---
#define GPS_RX 16             // ESP32 RX2 Pin (Connect to GPS TX)
#define GPS_TX 17             // ESP32 TX2 Pin (Connect to GPS RX)

// --- Servo Positions ---
#define DOOR_LOCKED_POS 0     
#define DOOR_UNLOCKED_POS 90

// --- RFID SECTION REMOVED ---