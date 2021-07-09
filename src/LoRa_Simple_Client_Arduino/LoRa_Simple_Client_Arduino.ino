#include <SPI.h>
#include <RH_RF95.h>
#include <SoftwareSerial.h>

// Singleton instance of the radio driver
RH_RF95 rf95;
float frequency = 915.0;  //frequency settings

SoftwareSerial mySerial(3, 4); // RX, TX

float temperature,humidity,tem,hum;
char tem_1[8]={"\0"},hum_1[8]={"\0"};
char *node_id = "<12345>";  //From LG01 via web Local Channel settings on MQTT.Please refer <> dataformat in here.
uint8_t datasend[64];
unsigned int count = 1;
char                 databuffer[35];
bool checkBuffer_ = false;

void setup()
{
      Serial.begin(9600);
      mySerial.begin(9600);
      Serial.println(F("Start MQTT Example"));
          if (!rf95.init())
      Serial.println(F("init failed"));
         rf95.setFrequency(frequency);
         rf95.setTxPower(20);
         rf95.setSpreadingFactor(7);
        rf95.setSignalBandwidth(125000);
        rf95.setCodingRate4(5);
        rf95.setSyncWord(0x34);

}

void checkBuffer(){
  if (databuffer[0]=='c'&&databuffer[4]=='s'&&databuffer[8]=='g'&&databuffer[12]=='t'&&databuffer[16]=='r'&&databuffer[20]=='p'&&databuffer[24]=='h'&&databuffer[27]=='b'){
    checkBuffer_ = true;
  }
  else checkBuffer_ = false;
}

void getBuffer()                                                                    //Get weather status data
{
  int index;
  for (index = 0;index < 35;index ++)
  {
 //   Serial.println(index);
    if(mySerial.available())
    {
      
      databuffer[index] = mySerial.read();
 //     Serial.println(databuffer[index]);
      if (databuffer[0] != 'c')
      {
        index = -1;
      }
    }
    else
    {
      index --;
    }
  }
//  Serial.println(databuffer);
  checkBuffer();
  delay(1000);
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

    strcat(data,databuffer);

    strcpy((char *)datasend,data);
     
    Serial.println((char *)datasend);
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
  delay(1000);
}
    
    


void loop()
{
 //   Serial.print("###########    ");
 //   Serial.print("COUNT=");
  //  Serial.print(count);
 //   Serial.println("    ###########");
 //   count++;
    getBuffer();
    if (checkBuffer_ == true){
      Serial.println("After checked");
      Serial.println(databuffer);
      dhtWrite();
      SendData();
    }
}
