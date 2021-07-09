#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

WiFiClient espClient;
PubSubClient client(espClient);  // Use esp wifi client send/agree message to broker.
SoftwareSerial mySerial(D7, D8); // RX, TX

// Wifi setting
#define ssid "dragino-1b1eb0"
#define passwordWifi  ""
// Broker setting
#define mqtt_server  "citlab.myftp.org"  // broker's server.
//#define user "qtvdlpzg"
//#define passwordBroker "QZeTctesHSwF"

char namespace_esp8266[] = "esp8266";
int port = 1883; 
char msg[60];
char time_msg[50];

void setup()
{
  //  Baudrate with speed 115200 for interface with computer.
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(50);

  ESP.eraseConfig();

  //--------------------- Wifi connection. ---------------------
  Serial.print(" Wifi connection. ");
  Serial.println(ssid);
  ESP.eraseConfig();
  WiFi.begin(ssid, passwordWifi); // start connect wifi.
 
  // Exit when connect.
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    //Serial.print('.');
  }
  //  Then connected.
  //Serial.println();
  //Serial.println(F(" WiFi connected."));
  Serial.println(F(" Address IP of ESP8266: "));
  Serial.println(WiFi.localIP());  // WiFi.localIP() - return address IP of ESP8266.
  //--------------------- End connect with Wifi ----------------
  // Initialize connect with broker.
  client.setServer(mqtt_server, port); // Send request connect with broker.
  client.setCallback(callback); // when broker send request --> function callback.
}
void loop()
{
  // Kiểm tra kết nối
  if (!client.connected()) {
    reconnect();
  }
  
  String ch;
  if (mySerial.available()) {
       ch = mySerial.readStringUntil('\n'); //đọc
        // Serial.println(ch);
        snprintf (msg, 75, ch.c_str());
        client.publish("sensor/mushroom/001", msg);
      Serial.print("Da nhan duoc: "); //trả về lại cho arduino
      Serial.println(ch);
      
//      DateTime now = rtc.now();
//      printDateTime(now);
      Serial.println();
      
   }
  client.loop();
}

  
void callback(char* topic, byte* payload, unsigned int length)
{
  String data;
  for (int i = 0; i < length; i++)
  {
    data = data + (String)((char)payload[i]);// convert payload from byte --> String and Save --> data
  }
  Serial.println(data);
  if (data.length() < 40 ){
    String command = data.substring(11, data.length() - 2);
    Serial.println(command);
    mySerial.println(command);
  }
  else {                                      // nếu chuỗi quá dài
    Serial.println(data);

  }
}

void reconnect() {
  // Chờ tới khi kết nối
  while (!client.connected()) {
    // Thực hiện kết nối với mqtt user và pass
    if (client.connect("ESP8266Client") ){
      Serial.println("connected");
      // Khi kết nối sẽ publish thông báo
     // client.publish("esp8266-demo", "ESP_reconnected");
      // ... và nhận lại thông tin này
      //client.subscribe(mqtt_topic_sub);
      client.subscribe("garden1/mode1");
      client.subscribe("garden1/light1");
      client.subscribe("garden1/pump1");
      client.subscribe("garden1/fan1");
      client.subscribe("garden1/temp1");
    } else {
      client.state();
      delay(5000);
    }
  }
}
