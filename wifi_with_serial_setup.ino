
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <stdio.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define SSID_WIFI_ADDR 0
#define SSID_PWD_ADDR 20

#define STATE_IDLE 0
#define STATE_DISPLAY_OPTIONS 1
#define STATE_DISPLAY_WIFI_CONFIG 2
#define STATE_SAVE_WIFI_CONFIG 3
#define STATE_CUSTOM_JOB 4
#define STATE_DISPLAY_DEVICE_INFO 5 

volatile int STATE=0;

char WifiSSid[20];
char WifiPass[20];


void setup()
{
  int timeOutCounter=0;
  
  Serial.begin(115200);
  delay(200);

  Serial.println("Press any key to enter setup..");
  delay(5000);

  if (Serial.available() && Serial.read())
  {
    STATE=STATE_DISPLAY_OPTIONS;
    return;
  }
  
  
  Serial.println("Getting wifi config..");
  ReadWifiConfig();

  Serial.print("Connecting to ssid ");
  Serial.println(WifiSSid);
  
  WiFi.begin(WifiSSid, WifiPass);
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
    timeOutCounter++;
    if (timeOutCounter>20)
    {
      break;  
    }
  }

  if (timeOutCounter>20){
    Serial.println("Could not connect to ssid.");
    STATE=STATE_DISPLAY_OPTIONS;
  }else{
    Serial.println("Connected.");
    STATE=STATE_DISPLAY_OPTIONS;
  }
  
}

void loop() {

   switch(STATE)
   {
      case STATE_IDLE:
         Idle();
         break;
      case STATE_DISPLAY_OPTIONS:
         PrintOptions();
         break;
      case STATE_DISPLAY_WIFI_CONFIG:
        DisplayWifiConfig();
        STATE=STATE_DISPLAY_OPTIONS;
        break;
      case STATE_SAVE_WIFI_CONFIG:
        SaveWifiConfig();
        ReadWifiConfig();
        STATE=STATE_DISPLAY_DEVICE_INFO;
        break;
      case STATE_DISPLAY_DEVICE_INFO:
        DisplayDeviceInfo();
        STATE=STATE_DISPLAY_OPTIONS;
        break;
      case STATE_CUSTOM_JOB:
        CustomJob();
        STATE=STATE_DISPLAY_OPTIONS;
        break;
   }

  
}

void CustomJob()
{
  HTTPClient http;
  String buffer;
  String postStr="{username:\"xxx\",password:\"yyy\",devices:false,products:false}";
  http.begin("https://beekeeper.hivehome.com/1.0/global/login","504639b7f1ee5bc1e0f6604912795653ebbfee96");
  http.header("POST / HTTP/1.1");
  http.header("Content-Type: application/json");

  int httpCode=http.POST(postStr);
  buffer=http.getString();
  Serial.println(httpCode);
  Serial.println(buffer);
  http.end();   
}

  
void Idle()
{
  delay(500);
  Serial.println("Idling..");
}

void PrintOptions()
{
  char input[20];
  Serial.println("Press 1 to reset.");
  Serial.println("Press 2 to read wifi credentials.");
  Serial.println("Press 3 to save wifi credentials.");
  Serial.println("Press 4 to get device info.");
  Serial.println("Press 5 to run custom job.");


  ReadSerial(input);
  Serial.println(input);
  switch(input[0])
  {
    case  '1' :
      ESP.restart();
      break;
     case  '2' :
      STATE=STATE_DISPLAY_WIFI_CONFIG;
      break;
     case  '3' :
      STATE=STATE_SAVE_WIFI_CONFIG;
      break;
     case  '4' :
      STATE=STATE_DISPLAY_DEVICE_INFO;
      break;
     case  '5' :
      STATE=STATE_CUSTOM_JOB;
      break;
     default:
      Serial.println("Invalid selection");  
      STATE=STATE_DISPLAY_OPTIONS;
  }
}

void ReadWifiConfig()
{
  EEPROM.begin(40);
  delay(200);
  EEPROM.get(SSID_WIFI_ADDR,WifiSSid);
  EEPROM.get(SSID_PWD_ADDR,WifiPass);
  EEPROM.end();
}


void DisplayWifiConfig()
{
    Serial.println("Wifi config");
    Serial.println("========================");
    Serial.print("SSID : "); Serial.print(WifiSSid);
    Serial.println("");
    Serial.print("Password : "); Serial.print(WifiPass);
    Serial.println("\r\n");
}


void DisplayDeviceInfo()
{
  Serial.printf("index heap size: %u bytes\n", ESP.getFreeHeap());
}

void SaveWifiConfig()
{
  char ssid[20];
  char pass[20];
  
  Serial.println("Enter the SSID:");
  ReadSerial(ssid);
  Serial.println("Enter the Password:");
  ReadSerial(pass);
   
  EEPROM.begin(40);
  delay(200);

  EEPROM.put(SSID_WIFI_ADDR,ssid);
  EEPROM.put(SSID_PWD_ADDR,pass);
  EEPROM.commit();
  EEPROM.end();

  Serial.println("Config saved.");
}




void ReadSerial(char *buffer)
{
  int counter=0;
  char chr[2];
  chr[1]='\0';
  strcpy(buffer,"");
  while(true)
  {
      if (Serial.available())
      {
        chr[0]=Serial.read();
        if (chr[0]=='\r')
        {
          break;
        }
        if (chr[0]!='\n')
        {
          strcat(buffer,chr);
          counter++;
        }
        if (counter>20)
        {
          break;  
        }
      }
  }
}


