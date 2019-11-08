#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

TinyGPSPlus gps;  // The TinyGPS++ object

SoftwareSerial ss(4, 5); // The serial connection to the GPS device

const char* ssid = "moto g(6) 9541";
const char* password = "58929541";
/*
float latitude , longitude;
String lat_str , lng_str;

*/

char clat[11];
char clng[11];

//thingspeak cloud details
WiFiClient espClient;
unsigned long channelno=748624;
const char* writeapi="VYEWLEE8C578TK93";

WiFiServer server(80);
void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //server.begin();
  //Serial.println("Server started");
  ThingSpeak.begin(espClient);
  // Print the IP address
  Serial.println(WiFi.localIP());

}

void loop()
{
  bool isGpsLocationValid = false;
        do
        {  
            while (ss.available()>0)
            {
              //Serial.write(ss.read());
             char c = byte(ss.read());
                if (gps.encode(c)) 
                {    
                    if (gps.location.isValid() && gps.location.isUpdated())
                    {
                        dtostrf(gps.location.lat(), 11, 6, clat);
                        if(clat)
                           ThingSpeak.writeField(channelno,1,clat,writeapi);
                        Serial.print("Published lat");
                        delay(500);
                        Serial.print(clat);
                        dtostrf(gps.location.lng(), 11, 6, clng); 
                        Serial.print(clng);
                        if(clng)
                           ThingSpeak.writeField(channelno,2,clng,writeapi);
                        Serial.print("Published long");                      
                        isGpsLocationValid = true;
                        Serial.println(" ");
                    }
                }
           }
            
        } while (isGpsLocationValid == false);
       delay(4000);
}
