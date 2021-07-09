#include <SPI.h>
#include <RH_RF95.h>

#include <Wire.h>
#include "Adafruit_SI1145.h"

Adafruit_SI1145 uv = Adafruit_SI1145();

// Singleton instance of the radio driver
RH_RF95 rf95;
float frequency = 915.0;  //frequency settings
const float max_volts = 5.0;
const float max_analog_steps = 1023.0;

float temperature,humidity,tem,hum;
char tem_1[8]={"\0"},hum_1[8]={"\0"};
char *node_id = "<22345>";  //From LG01 via web Local Channel settings on MQTT.Please refer <> dataformat in here.
uint8_t datasend[64];
unsigned int count = 1;
char                 databuffer[35];

void setup()
{
      Serial.begin(9600);
      Serial.println(F("Start MQTT Example"));
      if (!rf95.init())
      Serial.println(F("init failed"));
      rf95.setFrequency(frequency);
      rf95.setTxPower(20);
      rf95.setSpreadingFactor(7);
      rf95.setSignalBandwidth(125000);
      rf95.setCodingRate4(5);
      rf95.setSyncWord(0x34);

      if (! uv.begin()) {
        Serial.println("Didn't find Si1145");
        while (1);
      }
}


void getBuffer()                                                                    //Get weather status data
{

  Serial.println("===================");
  Serial.print("Vis: "); Serial.println(uv.readVisible());
  Serial.print("IR: "); Serial.println(uv.readIR());
  float UVindex = uv.readUV();
  // the index is multiplied by 100 so to get the
  // integer index, divide by 100!
  UVindex /= 100.0;  
  Serial.print("UV: ");  Serial.println(UVindex);

  delay(3000);
}
void dhtWrite()
{
    char data[50] = "\0";
    for(int i = 0; i < 50; i++)
    {
       data[i] = node_id[i];
    }

    dtostrf(tem,0,1,tem_1);
    dtostrf(hum,0,1,hum_1);

 //    Serial.println(tem_1);
 //    strcat(data,"t=");
     strcat(data,databuffer);
//     strcat(data,"&h=");
//     strcat(data,hum_1);
     strcpy((char *)datasend,data);
     
   //Serial.println((char *)datasend);
    //Serial.println(sizeof datasend);
      
}

void SendData()
{
      Serial.println(F("Sending data to LG01"));
           
   
      rf95.send((char *)datasend,sizeof(datasend));  
      rf95.waitPacketSent();  // Now wait for a reply
    
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);

     if (rf95.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
     
      Serial.print("got reply from LG01: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is LoRa server running?");
  }
  delay(3000);
}
    
    


void loop()
{
    Serial.print("###########    ");
    Serial.print("COUNT=");
    Serial.print(count);
    Serial.println("    ###########");
     count++;
 //    dhtTem();
     getBuffer();
 //    dhtWrite();
//     SendData();
}
