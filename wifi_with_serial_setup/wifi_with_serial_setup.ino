
#include <ESP8266WiFi.h>
#include <stdio.h>
#include <ESP8266HTTPClient.h>
#include "config.h"



#define STATE_ATTEMPT_CONNECT 0
#define STATE_DISPLAY_OPTIONS 1
#define STATE_DISPLAY_CONFIG 2
#define STATE_SAVE_CONFIG 3
#define STATE_CUSTOM_JOB 4
#define STATE_DISPLAY_DEVICE_INFO 5

volatile int State=0;

Config config;


void setup()
{
  
  
  Serial.begin(115200);
  delay(200);
  Serial.println("Initializing....");
  delay(200);
  Serial.println("Press any key to enter setup..");
  delay(5000);

  if (Serial.available() && Serial.read())
  {
    State=STATE_DISPLAY_OPTIONS;
    return;
  }

  State=STATE_ATTEMPT_CONNECT;
  
}

void loop() {

   switch(State)
   {
      case STATE_ATTEMPT_CONNECT:
         AttemptConnect(&config);
         break;
      case STATE_DISPLAY_OPTIONS:
         PrintOptions();
         break;
      case STATE_DISPLAY_CONFIG:
        DisplayConfig();
        State=STATE_DISPLAY_OPTIONS;
        break;
      case STATE_SAVE_CONFIG:
        ModifyConfig();
        State=STATE_DISPLAY_DEVICE_INFO;
        break;
      case STATE_DISPLAY_DEVICE_INFO:
        DisplayDeviceInfo();
        State=STATE_DISPLAY_OPTIONS;
        break;
      case STATE_CUSTOM_JOB:
        CustomJob();
        break;
   }
}

void AttemptConnect(Config* config)
{

  
  int timeOutCounter=0;
  
  
  Serial.println("Loading config..");
  Config tmp=GetConfig();
  strcpy(config->WifiSSid,tmp.WifiSSid);
  strcpy(config->WifiPass,tmp.WifiPass);
  
  Serial.print("Connecting to ssid ");
  Serial.println(config->WifiSSid);
  
  WiFi.begin(config->WifiSSid,config->WifiPass);
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
   Serial.print(".");
    delay(1000);
    timeOutCounter++;
    if (timeOutCounter>20)
    {
      break;  
    }
  }

  if (timeOutCounter>20){
    Serial.println("Could not connect to ssid.");
    State=STATE_DISPLAY_OPTIONS;
  }
  else
  {
    Serial.println("starting custom job...");
    State=STATE_CUSTOM_JOB;
  }
}


void CustomJob()
{
    Serial.println("starting custom job.");
    delay(5000);
    
    //put custom code here.
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
      State=STATE_DISPLAY_CONFIG;
      break;
     case  '3' :
      State=STATE_SAVE_CONFIG;
      break;
     case  '4' :
      State=STATE_DISPLAY_DEVICE_INFO;
      break;
     case  '5' :
      State=STATE_CUSTOM_JOB;
      break;
     default:
      Serial.println("Invalid selection");  
      State=STATE_DISPLAY_OPTIONS;
  }
}




void DisplayConfig()
{
    Config tmp=GetConfig();
    Serial.println("Wifi config");
    Serial.println("========================");
    Serial.print("SSID : "); Serial.print(tmp.WifiSSid);
    Serial.println("");
    Serial.print("Password : "); Serial.print(tmp.WifiPass);
    Serial.println("\r\n");
}




void DisplayDeviceInfo()
{
  Serial.printf("index heap size: %u bytes\n", ESP.getFreeHeap());
}


void ModifyConfig()
{
  
  Config modVal;
  
  Serial.println("Enter the SSID:");
  ReadSerial(modVal.WifiSSid);
  
  Serial.println("Enter the Password:");
  ReadSerial(modVal.WifiPass);

  SaveConfig(modVal);

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
