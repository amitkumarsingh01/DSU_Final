#include <TinyGPS.h>
TinyGPS gps;
#include <SoftwareSerial.h>
SoftwareSerial ss(16,17);
#include <WiFi.h>
const char* ssid     = "Project";
const char* password = "12345678";
WiFiServer server(80);
String yazi;
void setup()
{
  pinMode(15,OUTPUT);
  digitalWrite(15,LOW);
    Serial.begin(9600);
ss.begin(9600);
    Serial.print("Connecting to ");
    Serial.println(ssid);

WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");   
    Serial.println(WiFi.localIP());
    server.begin();
} 

void loop(){
  smartdelay(1000);
 WiFiClient client = server.available();   
    float flat, flon;
  unsigned long age;
   gps.f_get_position(&flat, &flon, &age);
  
  if (client) {                             
    Serial.println("new client");          
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {            
        char c = client.read();          
          if (c == '\n') {                      
          if (currentLine.length() == 0) {  
      
yazi="<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><meta http-equiv='X-UA-Compatible' content='ie=edge'><title>My Google Map</title><style>#map{height:400px;width:100%;}</style></head> <body><h1>My Google Map</h1><div id='map'></div><script>function initMap(){var options = {zoom:8,center:{lat:";
    yazi+=flat;
    yazi+=",lng:";
    yazi+=flon;
    yazi+="}};var map = new google.maps.Map(document.getElementById('map'), options);google.maps.event.addListener(map, 'click', function(event){addMarker({coords:event.latLng});});var markers = [{coords:{lat:";
yazi+=flat;
yazi+=",lng:";
yazi+=flon;
yazi+="}}];for(var i = 0;i < markers.length;i++){addMarker(markers[i]);}function addMarker(props){var marker = new google.maps.Marker({position:props.coords,map:map,});if(props.iconImage){marker.setIcon(props.iconImage);}if(props.content){var infoWindow = new google.maps.InfoWindow({content:props.content});marker.addListener('click', function(){infoWindow.open(map, marker);});}}}</script><script async defer src='https://maps.googleapis.com/maps/api/js?key=AIzaSyDHNUG9E870MPZ38LzijxoPyPgtiUFYjTM&callback=initMap'></script></body></html>";
 
 
 client.print(yazi);

            client.println();
            break;
          } else {   currentLine = ""; }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("client disconnected");
    }
}
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}