#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long StartTime;
unsigned long StopTime;
String T;
String Time;
String S;

#define LOOP 36
#define MOTOR 14
#define TOUCH_PIN 25
#define BUZZER 13

bool GameOver = true;
int count = 5;
//network credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

//Domain name and URL path or IP address with path

bool Touch() {
  bool touch = false;
  if (digitalRead(TOUCH_PIN) == HIGH) {
    touch = true;
    return touch;
  }
}

void buzzer() {
  GameOver = true;
  digitalWrite(BUZZER, HIGH);
  while (count != 2) {
    blink();
  }
  count = 5;
  digitalWrite(BUZZER, LOW);

  // Serial.println("Enter 1 if completed else 0: ");
  // while (!Serial.available());
  // S = Serial.readString();
  // if (S == "1\n") {
    
  //   int t = T.toInt();
  //   httpUpload(Time, t);
  // } else {
    // Serial.print("Game Over");
  // }


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     Time: ");
  lcd.setCursor(0, 1);
  lcd.print("    " + Time + " s");
  // Serial.println("Time: " + Time);
  delay(2000);
}

void blink() {
  lcd.clear();
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("   GAME OVER");
  delay(500);
  count--;
}
void httpUpload(String a, int b) {  //time, token

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    String serverName = "http://example.com/ESP32/index.php?time=" + a + "&token=" + b;
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    // Specify content-type header
    //http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare your HTTP POST request data
    String httpRequestData = "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");

    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  Serial.println("Data Upload Successful");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Data Upload");
  lcd.setCursor(0, 1);
  lcd.print("  Successful");
  delay(1000);
}

// bool TouchDetect(){
//     if(analogRead(36) > 2000){
//     if(val==true){
//     Serial.println("HIGH");
//     val = false;
//     }
//   }
//   delay(100);
//   if(analogRead(36) <2000){
//     val = true;
//   }
// }

void setup() {

  pinMode(BUZZER, OUTPUT);
  pinMode(MOTOR, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(LOOP, INPUT);

  digitalWrite(MOTOR, HIGH);

  // Serial.begin(115200);

  // WiFi.begin(ssid, password);
  // Serial.println("Connecting");
  // while (WiFi.status() != WL_CONNECTED) {
  //   lcd.clear();
  //   lcd.setCursor(0, 0);
  //   lcd.print("Connecting to");
  //   lcd.setCursor(0, 1);
  //   lcd.print("      WIFI");
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.print("Connected to WiFi network with IP Address: ");
  // Serial.println(WiFi.localIP());

  lcd.init();
  lcd.backlight();
  // (you can also pass in a Wire library object like &Wire2)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     WELCOME");
}

void loop() {
  if (!GameOver) {
    //Serial.println(analogRead(LOOP));
    if (analogRead(LOOP) > 4000) {
      StopTime = millis();
      float interval = (StopTime - StartTime) / 1000.0;
      Time = String(interval);
      buzzer();
    }
  } else {
    // Serial.println("Enter Token no: ");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("    WELCOME");
    delay(2000);
    // while (!Serial.available());
    // T = Serial.readString();
    // T.trim();
    // Serial.println("Token: " + T);

    while (digitalRead(TOUCH_PIN) != HIGH) {  //OFF
      // lcd.clear();
      // lcd.setCursor(0, 0);
      // lcd.print("Token No: " + T);
      // delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Touch the Button");
      lcd.setCursor(0, 1);
      lcd.print("to Start");
    }
    GameOver = false;
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("    GAME ON");
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    StartTime = millis();
  }
}
