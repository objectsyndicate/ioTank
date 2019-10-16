#include <ESP8266WiFi.h> // Wifi stack
#include <FS.h> // SPIFS
#include <string.h> 
#include <dht.h> // DHT11
#include <WiFiClient.h> // POST
#include <ESP8266WebServer.h> // GET
#include <Wire.h> //SPI
#include <MAX44009.h> // Lux
#include "websites.h"
#include <MCP3008.h>

//The Arduino Map function but for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}



MAX44009 light;

#define CS_PIN D8
#define CLOCK_PIN D5
#define MOSI_PIN D7
#define MISO_PIN D6

MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);

ESP8266WebServer server(80);

 
//const char* host = "bots.herpien.com";
String TOKEN;
String URI;
String HOST;


// webserver strings.
String T1w;
String T2w;
String Hw;
String UVw;
String Lw;

int n = 288;  // modulus 100 just for the sake of example
int counter = 0;


#define DHT11_PIN D4

// which analog pin to connect
#define THERMISTORPIN A0
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 3
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3984
// the value of the 'other' resistor
#define SERIESRESISTOR 10000
int samples[NUMSAMPLES];

int readingIn = 0;
int settings_i = 0;         // incoming serial byte


float steinhart = 0;
float T2 = 0;
float H = 0;
String FC = "";
String S = "";

boolean sendcloud;

bool loadFromFlash(String path) {
  if(path.endsWith("/")) path += "index.html";

  int NumFiles = sizeof(files)/sizeof(struct t_websitefiles);
  
  for(int i=0; i<NumFiles; i++) {
    if(path.endsWith(String(files[i].filename))) {      
      _FLASH_ARRAY<uint8_t>* filecontent;
      String dataType = "text/plain";
      unsigned int len = 0;
      
      dataType = files[i].mime;
      len = files[i].len;


      server.setContentLength(len);
      server.send(200, files[i].mime, "");
      
      filecontent = (_FLASH_ARRAY<uint8_t>*)files[i].content;
      
      filecontent->open();
      
      WiFiClient client = server.client();
      client.write(*filecontent, 100);
      
      return true;
    }
  }
  
  return false;
}

void handleNotFound() {
     
  // try to find the file in the flash
  if(loadFromFlash(server.uri())) return;
  
  String message = "File Not Found\n\n";
  message += "URI..........: ";
  message += server.uri();
  message += "\nMethod.....: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments..: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  message += "\n";
  message += "FreeHeap.....: " + String(ESP.getFreeHeap()) + "\n";
  message += "ChipID.......: " + String(ESP.getChipId()) + "\n";
  message += "FlashChipId..: " + String(ESP.getFlashChipId()) + "\n";
  message += "FlashChipSize: " + String(ESP.getFlashChipSize()) + " bytes\n";
  message += "getCycleCount: " + String(ESP.getCycleCount()) + " Cycles\n";
  message += "Milliseconds.: " + String(millis()) + " Milliseconds\n";
  server.send(404, "text/plain", message);
}




void handleJSON() {   

  String json = "{\"t1\":" + T1w + " , \"t2\":" + T2w + " , \"h\":" + Hw + ", \"uv\":" + UVw + ", \"l\":" + Lw + "}";
  server.send ( 200,  "application/javascript", json );
} // JSON end


void handle24() {

  File m = SPIFFS.open("/h24", "r");
  String line = m.readString();
  server.send ( 200,  "text/*", line );
  m.close();
} //

void handle48() {

  File m = SPIFFS.open("/h48", "r");
  String line = m.readString();
  server.send ( 200,  "text/*", line );
  m.close();
} //

void handle72() {

  File m = SPIFFS.open("/h72", "r");
  String line = m.readString();
  server.send ( 200,  "text/*", line );
  m.close();
} //

void clearHistory() {

  File m24 = SPIFFS.open("/h24", "w+");
  delay(1);
  m24.close();

  File c24 = SPIFFS.open("/c24", "w+");
  delay(1);
  c24.close();

  File m48 = SPIFFS.open("/h48", "w+");
  delay(1);
  m48.close();
  
  File m72 = SPIFFS.open("/h72", "w+");
  delay(1);
  m72.close();

  counter = 0;

  server.send ( 200,  "text/*", "erased" );
  
} //


void setup() {
  Serial.begin(115200);          //  setup serial



//Define local vars for reading of file 
  char* ssid     = "";
  char* password = "";


  // always use this to "mount" the filesystem
  bool result = SPIFFS.begin();
  delay(10);

  File f = SPIFFS.open("/f", "r");
  // get the String from the FS into a char,
  String line = f.readString();
  char str[200];
  // We got to strip the unicode from byte 1
  String strip = line;
  strip.replace("\ufeff", "");
  strip.replace("\r", "");
  //Serial.println(strip);
  strip.toCharArray(str, 200);
  //Serial.println(str);

  char *token;
  char *rest = str;
  while ((token = strtok_r(rest, "\n", &rest)))
  {
    delay(10);
    ++settings_i;
    if (settings_i == 1) {
      ssid = token;
    }
    if (settings_i == 2) {
      password = token;
    }
    if (settings_i == 3) {
      if (token[0] == 'T') {
        sendcloud = true;}
        
      if (token[0] == 'F') {
        sendcloud = false;}      
    }

    if (settings_i == 4) {
     TOKEN = token;
    }

    if (settings_i == 5) {
     HOST = token;

    }
    if (settings_i == 6) {
     URI = token;
    }

  }

  f.close();
  // file MUST be closed ASAP.
  delay(10);





  Serial.begin(115200); // FS closes serial, must reopen


  //connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (WiFi.status() == WL_DISCONNECTED) {
      //Serial.println("Attempting to connect.");
    }
    if (WiFi.status() == WL_NO_SSID_AVAIL) {
      //Serial.println("Check the SSID.");
    }
    if (WiFi.status() == WL_CONNECT_FAILED) {
      //Serial.println("Connection failed, check password.");
    }
    if (WiFi.status() == WL_CONNECTION_LOST) {
      //Serial.println("Connection Lost");
    }
    if (WiFi.status() == WL_CONNECTED) {
      //Serial.print("Connected to ");
     // Serial.println(ssid);
    }
  }

  //Serial.println("");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
  light.begin(); // enable lux sensor
  
  server.on("/h24", handle24);
  server.on("/h48", handle48);
  server.on("/h72", handle72);
  
  server.on("/clear", clearHistory);

  server.on("/json", handleJSON);
  server.onNotFound(handleNotFound);
  server.begin();
  //Serial.println("HTTP server started");





// check the file size of c24 and divide it to count lines

  File c24 = SPIFFS.open("/c24", "r");
  int bytesize = c24.size();
  Serial.println(bytesize);

  counter = (bytesize / 3);
  Serial.println(counter);

  c24.close();


  
} // SETUP END



void pushSensor(long currentMillis) {


  String PostData = "{\"t1\":" + T1w + " , \"t2\":" + T2w + " , \"h\":" + Hw + ", \"uv\":" + UVw + ", \"l\":" + Lw + "}";
  //Serial.println(PostData);




// Every 288s rename file 1 to file 2, file 2 to file 3, file 3->7
// create file 1 (w+)
counter++;

//Serial.println(counter);

// reset file if 288
if (counter == 288)
{

SPIFFS.remove("/h72");
delay(1);

SPIFFS.rename("/h48", "/h72");
delay(1);

SPIFFS.rename("/h24", "/h48");
delay(1);

File m = SPIFFS.open("/h24", "w+");
delay(1);
m.close();

File c24 = SPIFFS.open("/c24", "w+");
delay(1);
c24.close();


}// endif

// reset counter if 288
if (counter >= n)
{
counter = 0;
}//endif

  File m = SPIFFS.open("/h24", "a+");
  m.println(PostData);
      delay(10);

  m.close();

  File c = SPIFFS.open("/c24", "a+");
  c.println('A');
      delay(10);

  c.close();


 if (sendcloud == true) {

    //Serial.println(HOST);
   // Serial.println(URI);
   // Serial.println(TOKEN);
    char host[50];
    HOST.toCharArray(host, 50);

  // Use WiFiClient class to create TCP connections
  WiFiClientSecure client;
 // WiFiClient client;

  const int httpPort = 443;
  //const int httpPort = 8000;


  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }


  // This will send the request to the server
  client.println("POST "+URI+" HTTP/1.1"); // API URI
  client.println("Host: "+HOST); // HOST
  client.println("Cache-Control: no-cache"); // all data is new data
  client.println("Authorization: Token "+TOKEN); //
  client.println("Content-Type: application/json"); // sets JSON type
  client.print("Content-Length: "); //Required
  client.println(PostData.length());
  client.println();
  client.println(PostData);
  //Serial.println(PostData.length());
  


  while (client.available() == 0) {

    if (millis() - currentMillis > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
    
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    // Serial.print(line);
  }
  // Serial.println();
  // Serial.println("closing connection");

    }// IF sendcloud true END
  

} // END





// Variables will change:
long postPreviousMillis = 0;        // will store last time LED was updated
long readPreviousMillis = 0;        // will store last time LED was updated
 
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.

long postInterval = 300000;           // interval at which to send POST 5 minutes)

//DEBUG
//long postInterval = 30000;           // interval at which to send POST 5 minutes)

long readInterval = 300;           // interval at which to read sensors 1 seconds


void loop()
{

  unsigned long currentMillis = millis(); // NOW

 // Every 10 seconds read
  if(currentMillis - postPreviousMillis > readInterval) {
    postPreviousMillis = currentMillis;



int uvi = averageAnalogRead(A0);    // read the input pin

float uviVoltage= uvi * (3.3 / 1023.0);

//int uvLevel = averageAnalogRead(UVOUT);
//int refLevel = averageAnalogRead(REF_3V3);
//float outputVoltage = 3.3 / refLevel * uvLevel;

//float uviVoltage = 3.3 / 3.3 * uvi;


float uvIntensity = mapfloat(uviVoltage, 0.96, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level





  uint8_t i;
  float average;

  // take N samples in a row, with a slight delay
  for (i = 0; i < NUMSAMPLES; i++) {
    samples[i] = adc.readADC(0);
    delay(10);
  }

  // average all the samples out
  average = 0;
  for (i = 0; i < NUMSAMPLES; i++) {
    average += samples[i];
  }
  average /= NUMSAMPLES;

  // Serial.print("Average analog reading ");
  // Serial.println(average);
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  // Serial.print("Thermistor resistance ");
  // Serial.println(average);

  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C


// UUID
//Serial.println(WiFi.localIP());

// DHT 11 on and read
dht DHT;
int chk = DHT.read11(DHT11_PIN);
H = DHT.humidity, 1;
T2 = DHT.temperature, 1;


// Set global vars
T1w = steinhart;
// Serial.print("Temp 1 (C):    ");
// Serial.println(T1w);

 
T2w = T2;
// Serial.print("Temp 2 (C):    ");
// Serial.println(T2w);

 
Hw = H;
// Serial.print("Humidity:    ");
// Serial.println(Hw);

 
UVw = uvIntensity;
// Serial.print("UV Index:    ");
// Serial.println(UVw);
 
Lw = light.get_lux();
// Serial.print("Light (lux):    ");
// Serial.println(Lw);

  } // END READ


  if(currentMillis - readPreviousMillis > postInterval) {
    readPreviousMillis = currentMillis;
    
  pushSensor(currentMillis);
  delay(10);
  
  } // END POST


server.handleClient();
}
