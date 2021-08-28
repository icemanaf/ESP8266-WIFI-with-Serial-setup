
#include <EEPROM.h>



typedef struct 
{
  char WifiSSid[20];
  char WifiPass[20];
  //put other config values here
} Config;


void SaveConfig(Config config)
{
  int size=sizeof(Config);
  EEPROM.begin(size);
  EEPROM.put(0,config);
  
  EEPROM.commit();
  EEPROM.end();
  
}
  
Config GetConfig()
{
  Config config;
  
  int size=sizeof(Config);
  EEPROM.begin(size);
  delay(200);
  EEPROM.get(0,config);
  EEPROM.end();
  
  return config;
  
}
