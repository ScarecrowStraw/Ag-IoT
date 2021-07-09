 // DS18B20 library
#include <OneWire.h>
#include <DallasTemperature.h>

#include <SoftwareSerial.h>

// DHT library
#include <DHT.h>

// DS18B20 setup
// data 5
// GPIO where the DS18B20 is connected to
const int oneWireBus = 5; 
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// DHT21 setup
#define DHTPIN 4 // what digital pin we're connected to
#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial mySerial(2, 3); // RX, TX

float h = 0;
float t = 0;
float f = 0;
float temperatureC = 0;
float temperatureF = 0;

#define light 6
#define pumb 10
#define fan 11
#define tmp 12

int light_state = 0;
int pumb_state = 0;
int fan_state = 0;
int tmp_state = 0;

volatile long time_;
volatile long time_temp = millis();
volatile long time_temp2 = millis();
unsigned int time_sensor  = 10000;

void setup() {
 
  Serial.begin(115200);  //Starts serial connection

  pinMode (light, OUTPUT);
  pinMode (pumb, OUTPUT); 
  pinMode (fan, OUTPUT);
  pinMode (tmp, OUTPUT);
  digitalWrite (pumb, LOW);
  digitalWrite (light, LOW);
  digitalWrite (fan, LOW);
  digitalWrite (tmp, LOW);
  
    // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?");

  // Start the DS18B20 sensor
  sensors.begin();
  // Start the DHT21 sensor
  dht.begin();
}


void readDS18B20(){
  sensors.requestTemperatures(); 
  temperatureC = sensors.getTempCByIndex(0);
  temperatureF = sensors.getTempFByIndex(0);
  /*
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  */
}

void readDHT21() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

/*  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
 */
}

void Json(String temperature,String humidity,String tWater)
 {
  String temp = "{";
  temp += "\"key\":";
  temp += 1234;
  temp += ",";
//  temp += "\"t\":";
//  temp += temperature;
//  temp += ",";
  temp += "\"h\":";
  temp += humidity;
//  temp += ",";
//  temp += "\"tW\":";
//  temp += tWater;
  temp += "}";
  mySerial.println(temp);
  Serial.println(temp);
 }

void readSensors(){
//  readDS18B20();
//  readDHT21();
  int value = analogRead(A0);    
  int percent = map(value, 0, 1023, 0, 100);
  Serial.print(percent);
  Serial.println('%');
  
  if ((unsigned long) (millis() - time_) > time_sensor ){
      time_ = millis();         
      Json(String(t), String(percent),String(temperatureC));
  }
}

void pumbRun(){
  if (pumb_state == 0){
    digitalWrite (pumb, LOW);
  }
  else {
    digitalWrite (pumb, HIGH);
  }
  Serial.println (pumb_state);
}

void lightRun(){
  if (light_state == 0){
    digitalWrite (light, LOW);
  }
  else {
    digitalWrite (light, HIGH);
  }
  Serial.println (light_state);
}

void fanRun(){
  if (fan_state == 0){
    digitalWrite (fan, LOW);
  }
  else {
    digitalWrite (fan, HIGH);
  }
  Serial.println (fan_state);
}

void tmpRun(){
  if (tmp_state == 0){
    digitalWrite (tmp, LOW);
  }
  else {
    digitalWrite (tmp, HIGH);
  }
  Serial.println (tmp_state);
}

void loop() {
  readSensors();
//  readTime();
  if(mySerial.available() > 0)
  {
    String temp = mySerial.readStringUntil('\n');
    Serial.println(temp);
    temp.trim();
    for (int i = 0; i < temp.length(); i++){
      if (temp[i] == 'p'){
        if (temp[i+1] == '1'){
          pumb_state = 1;
        }
        else if (temp[i+1] == '0'){
          pumb_state = 0;
        }
      }

      if (temp[i] == 'l'){
        if (temp[i+1] == '1'){
          light_state = 1;
        }
        else if (temp[i+1] == '0'){
          light_state = 0;
        }
      }

      if (temp[i] == 'f'){
        if (temp[i+1] == '1'){
          fan_state = 1;
        }
        else if (temp[i+1] == '0'){
          fan_state = 0;
        }
      }
      if (temp[i] == 't'){
        if (temp[i+1] == '1'){
          tmp_state = 1;
        }
        else if (temp[i+1] == '0'){
          tmp_state = 0;
        }
      }
    }
    
    delay(100);
  }
  pumbRun();
  lightRun();
  fanRun();
  tmpRun();
}
