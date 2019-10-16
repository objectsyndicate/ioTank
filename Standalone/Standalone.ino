#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h> // SPIFS
// Stuff--------------------------------------
//#include <dht.h> // DHT11
#include <Wire.h> //SPI
#include <MAX44009.h> // Lux
#include <MCP3008.h>
MAX44009 light;
#define CS_PIN D8
#define CLOCK_PIN D5
#define MOSI_PIN D7
#define MISO_PIN D6
MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);

String T1w;
String T2w;
String Hw;
String UVw;
String Lw;

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
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
// WIFI --------------------------------------------
#ifndef STASSID
#define STASSID ".........."
#define STAPSK  ".........."
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = 13;
String line;

const long utcOffsetInSeconds = 0; //UTC
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void handleRoot() {
  digitalWrite(led, 1);
  String home = "<!DOCTYPE html> <html lang='en'> <head> <meta charset='utf-8'> <meta http-equiv='X-UA-Compatible' content='IE=edge'> <title>ioTank</title> <meta name='viewport' content='width=device-width, initial-scale=1'> <link rel='stylesheet' type='text/css' href='https://rawgit.com/gionkunz/chartist-js/master/dist/chartist.min.css'> <style> #chart1 { height: 300px; width: 100%; stroke: #000; } #chart1 .ct-point{ stroke: #000; } #chart1 .ct-line{ stroke: FireBrick ; } #chart2 { height: 300px; width: 100%; stroke: #000; } #chart2 .ct-point{ stroke: #000; } #chart2 .ct-line{ stroke: DarkBlue; } #chart3 { height: 300px; width: 100%; stroke: #000; } #chart3 .ct-point{ stroke: #000; } #chart3 .ct-line{ stroke: Indigo; } </style> </head> <body> <h2>Temp Probe</h2> <div class='t-chart' id='chart1'></div> <h2>Lux</h2> <div class='l-chart' id='chart2'></div> <h2>UV</h2> <div class='uv-chart' id='chart3'></div> <script src='https://code.jquery.com/jquery-3.4.1.min.js' integrity='sha256-CSXorXvZcTkaix6Yvo6HppcZGetbYMGWSFlBw8HfCJo=' crossorigin='anonymous'></script> <script type='text/javascript' src='https://rawgit.com/gionkunz/chartist-js/master/dist/chartist.js'></script> <script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/moment.js/2.24.0/moment.min.js'></script> <script> var time = []; var lux = []; var uv = []; $.ajax({ method: 'GET', url: '/json', processData: false, dataType: 'json', contentType: 'application/json', success: function(response) { response.forEach(responseFunction); function responseFunction(item, index) { time.push({x: new Date(item['utc']*1000), y: item['t1']}); lux.push({x: new Date(item['utc']*1000), y: item['uv']}); uv.push({x: new Date(item['utc']*1000), y: item['l']}); }; var chart = new Chartist.Line('.t-chart', { series: [ { name: 'Time', data: time }] }, { axisX: { type: Chartist.FixedScaleAxis, divisor: 5, labelInterpolationFnc: function(value) { return moment(value).format('MMM D'); } } }); var chart2 = new Chartist.Line('.l-chart', { series: [{ name: 'Lux', data: lux }] }, { axisX: { type: Chartist.FixedScaleAxis, divisor: 5, labelInterpolationFnc: function(value) { return moment(value).format('MMM D'); } } }); var chart3 = new Chartist.Line('.uv-chart', { series: [{ name: 'UV', data: uv }] }, { axisX: { type: Chartist.FixedScaleAxis, divisor: 5, labelInterpolationFnc: function(value) { return moment(value).format('MMM D'); } } }); } }); </script> </body> </html> ";
  server.send(200, "text/html", home);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/json", []() {
    unsigned int len = line.length();
    //Serial.print(line);
    line.remove(len-1, 1) ; //remove the last comma
    //Serial.print(line);
    server.send(200, "application/json", "["+line+"]");
  });


  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
SPIFFS.begin();

  File m24 = SPIFFS.open("/h24", "w+");
  delay(1);
  m24.close();
  Wire.begin();
  light.begin(); // enable lux sensor

  timeClient.begin();
} // end setup 



long postPreviousMillis = 0;        // will store last time LED was updated
long readInterval = 3000;           // interval at which to read sensors 1 seconds
int count = 0; // how many times has it ran, if its 96 rotate log

void loop(void) {

unsigned long currentMillis = millis(); // NOW


 // Every 10 seconds read
  if(currentMillis - postPreviousMillis > readInterval) {
    postPreviousMillis = currentMillis;
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  timeClient.update();

  String t = String(timeClient.getEpochTime());
  Serial.println(t);

//----------READ----------------------------------------------------

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

/*
dht DHT;
int chk = DHT.read11(DHT11_PIN);
H = DHT.humidity, 1;
T2 = DHT.temperature, 1;
// Set global vars
*/
T1w = steinhart;
 Serial.print("Temp 1 (C):    ");
 Serial.println(T1w);

/*
T2w = T2;
// Serial.print("Temp 2 (C):    ");
// Serial.println(T2w);
Hw = H;
// Serial.print("Humidity:    ");
// Serial.println(Hw);
*/

UVw = uvIntensity;
 Serial.print("UV Index:    ");
 Serial.println(UVw);
Lw = light.get_lux();
 Serial.print("Light (lux):    ");
 Serial.println(Lw);
//------------------------------------------------------------------

  File m24 = SPIFFS.open("/h24", "a+");

  String json = "{\"t1\":" + T1w + " , \"utc\":" + t + " , \"uv\":" + UVw + ", \"l\":" + Lw + "},";




  m24.print(json);
  delay(10);
  m24.close();
  count += 1;

  File m = SPIFFS.open("/h24", "r");

  
  line = m.readString();
  Serial.println(count);
  
  if (count>144){ // if history goes over 1mb, nuke it
    
  File m24 = SPIFFS.open("/h24", "w+");
  count=0;
  delay(1);
  
    }
  m.close();
  
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH

  }

  
  server.handleClient();
  MDNS.update();
} // end loop
