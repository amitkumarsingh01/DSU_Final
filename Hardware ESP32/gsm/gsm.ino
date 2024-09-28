#include <SoftwareSerial.h>

SoftwareSerial SIM7670Serial(2, 3);
int sencev;
int sv=8;

void sendATCommand(const char* cmd, const char* expectedResponse, unsigned long timeout) {
  SIM7670Serial.println(cmd);
  String response = "";
  unsigned long startTime = millis();
  bool responseOK = false;

  while (millis() - startTime < timeout) {
    while (SIM7670Serial.available() > 0) {
      char c = SIM7670Serial.read();
      response += c;
    }
    if (response.indexOf(expectedResponse) != -1) {
      responseOK = true;
      break;
    }
  }
  Serial.println(response);

  if (responseOK)
    Serial.println("Response OK");
  else
    Serial.println("Timeout without expected Response");

}

void setup() {
  Serial.begin(115200);
  SIM7670Serial.begin(115200);
  sendATCommand("AT+CGMM", "OK", 1000); 
  sendATCommand("AT+CMGF=1", "OK", 1000); 
  pinMode(sv,INPUT_PULLUP);
}

void sendSMS(String number, String message) {
  String cmd = "AT+CMGS=\"" + number + "\"\r\n";
  SIM7670Serial.print(cmd);
  delay(100);
  SIM7670Serial.println(message);
  delay(100);
  SIM7670Serial.write(0x1A);
  delay(100);
}
void loop() {
  sencev=digitalRead(sv);

  if(sencev==0){
    sendSMS("7827191427", "I am in an emergency,please help.This is my location https://maps.app.goo.gl/rBjBB6QkqfaMTvY78");
    SIM7670Serial.println("ATD +917827191427;");
    delay(10000);
  }


 if(SIM7670Serial.available()) {
    String response = SIM7670Serial.readString();
    Serial.println(response);
    if(response.indexOf("CONNECT") != -1) {
      Serial.println("Call Connected");
    }
  }  
}