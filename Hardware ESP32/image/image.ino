#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Lecture Hall 1";
const char* password = "LectureHall1-SOE";

const char* serverName = "http://10.1.7.193:5000/capture";

#define BUTTON_PIN 18

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button pressed, sending capture request...");

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverName);

      int httpResponseCode = http.POST("");
      if (httpResponseCode == 200) {
        Serial.println("Image capture request sent successfully");
      } else {
        Serial.print("Error: ");
        Serial.println(httpResponseCode);
      }
      
      http.end();
    } else {
      Serial.println("Error in WiFi connection");
    }
    
    delay(1000);
  }
}
