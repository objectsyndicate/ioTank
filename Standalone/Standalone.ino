#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h> // SPIFS
#include "DHTesp.h"
#include <Wire.h> //SPI
#include "Max44009.h" // Lux
#include <MCP3008.h>
#include <ArduinoJson.h>


#define CS_PIN D8
#define CLOCK_PIN D5
#define MOSI_PIN D7
#define MISO_PIN D6
MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);
DHTesp dht;
Max44009 myLux(0x4A);

String T1w;
String T2w;
String Hw;
String UVw;
String Lw;

void checkAndShortenFile(const char* filePath) {
    File file = SPIFFS.open(filePath, "r");
    if (!file) {
        Serial.println("Failed to open the file for reading");
        return;
    }

    // If the file size is not over 50KB, nothing to do
    if (file.size() <= 100000) {
        file.close();
        return;
    }

    size_t totalObjects = 0;
    size_t nestedLevel = 0;
    char ch;

    // Count the number of objects
    while (file.available()) {
        ch = (char)file.read();
        if (ch == '{') nestedLevel++;
        if (ch == '}') nestedLevel--;

        if (nestedLevel == 0 && ch == '}') {
            totalObjects++;
        }
    }

    // Go back to the start of the file
    file.seek(0);

    size_t targetObjects = totalObjects / 2;
    size_t currentObjects = 0;
    nestedLevel = 0;

    // Traverse until halfway point (in terms of objects)
    while (file.available() && currentObjects < targetObjects) {
        ch = (char)file.read();

        if (ch == '{') nestedLevel++;
        if (ch == '}') nestedLevel--;

        if (nestedLevel == 0 && ch == '}') {
            currentObjects++;
        }
    }

    // Ensure that we're positioned at the start of a new object
    while (file.available() && ((ch = (char)file.read()) != '{'));

    if (!file.available()) {
        Serial.println("Error: Unexpected end of file.");
        file.close();
        return;
    }

    // Create a temporary file to write the second half of the data
    File tempFile = SPIFFS.open("/temp.txt", "w");
    if (!tempFile) {
        Serial.println("Failed to open temp file for writing");
        file.close();
        return;
    }

    // Write the data to the temp file
    tempFile.write(ch);
    while (file.available()) {
        ch = (char)file.read();
        tempFile.write(ch);
    }

    file.close();
    tempFile.close();

    // Remove the original file
    if (!SPIFFS.remove(filePath)) {
        Serial.println("Error: Failed to remove the original file.");
        return;
    }

    // Rename the temporary file to the original file's name
    if (!SPIFFS.rename("/temp.txt", filePath)) {
        Serial.println("Error: Failed to rename the temp file.");
    }
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
#define STASSID "---"
#define STAPSK  "---"
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


String htmlData = R"raw(
<!DOCTYPE html><html lang='en'><head> <meta charset='UTF-8'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <title>Data Visualization</title> <style>body{font-family: Arial, sans-serif;}canvas{border: 1px solid black; margin-top: 20px; width: 100%;}#container div{margin-bottom: 15px;}</style></head><body><div id='container'> <div> <label>t1:</label> <span id='t1Key'></span> <canvas id='t1Canvas' width='1600' height='440'></canvas> </div><div> <label>t2:</label> <span id='t2Key'></span> <canvas id='t2Canvas' width='1600' height='440'></canvas> </div><div> <label>h:</label> <span id='hKey'></span> <canvas id='hCanvas' width='1600' height='440'></canvas> </div><div> <label>uv:</label> <span id='uvKey'></span> <canvas id='uvCanvas' width='1600' height='440'></canvas> </div><div> <label>l:</label> <span id='lKey'></span> <canvas id='lCanvas' width='1600' height='440'></canvas> </div></div><script>fetch("/json").then((t=>t.text())).then((t=>JSON.parse("["+t.slice(0,-1)+"]"))).then((t=>{const e=document.body.clientWidth,l=440,o=e/(t.length-1),n={t1:"red",t2:"orange",h:"blue",uv:"purple",l:"gray"};["t1","t2","h","uv","l"].forEach((i=>{let a=Math.max(...t.map((t=>t[i]||0))),r=Math.min(...t.map((t=>t[i]||0)));[r,a]=((t,e)=>{const l=.1*(e-t||1);return[t-l,e+l]})(r,a);let h=400/(a-r);document.getElementById(i+"Key").textContent="Min: "+r.toFixed(2)+", Max: "+a.toFixed(2);let f=document.getElementById(i+"Canvas").getContext("2d");for(let t=0;t<=10;t++){let o=l-40*t-20;f.strokeStyle="lightgray",f.beginPath(),f.moveTo(0,o),f.lineTo(e,o),f.stroke();let n=r+t*(a-r)/10;f.font="10px Arial",f.fillStyle="black",f.fillText(n.toFixed(2),5,o-5)}f.strokeStyle=n[i],f.beginPath(),t.forEach(((e,a)=>{let x=o*a,c=l-(e[i]-r)*h-20;f.fillStyle=n[i],f.fillRect(x-2,c-2,4,4),a%25!=0&&a!=t.length-1||(f.font="bold 12px Arial",f.fillStyle="black",f.fillText(e[i].toFixed(2),x+5,c-5),f.font="10px Arial",f.fillStyle="blue",f.fillText(new Date(1e3*e.utc).toISOString().substr(11,5),x-10,435)),0===a?f.moveTo(x,c):f.lineTo(x,c)})),f.stroke()}))}));</script>
)raw";


  server.send(200, "text/html", htmlData);
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
  delay(100);
  server.on("/", handleRoot);

server.on("/json", []() {
  File file = SPIFFS.open("/h24", "r+"); // Open the file for read/update
  server.streamFile(file, "text/plain");
  file.close();
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
  dht.setup(D4, DHTesp::DHT11); // Connect DHT sensor to GPIO 17
  timeClient.begin();
  digitalWrite(LED_BUILTIN, HIGH);
} // end setup 

long postPreviousMillis = 0;        // will store last time LED was updated
long readInterval = 1000;           // interval at which to read sensors 1 seconds
int count = 0; // how many times has it ran, if its 96 rotate log

void loop(void) {

unsigned long currentMillis = millis(); // NOW

 // Every X read
  if(currentMillis - postPreviousMillis > readInterval) {
    postPreviousMillis = currentMillis;
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  timeClient.update();

  String t = String(timeClient.getEpochTime());
  //Serial.println(t);

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

  float Hw = dht.getHumidity();
  float T2w = dht.getTemperature();
   delay(dht.getMinimumSamplingPeriod());
   
if (isnan(Hw)) {
  Hw = 0.0;
}

if (isnan(T2w)) {
  T2w = 0.0;
} 

T1w = steinhart;
// Serial.print("Temp 1 (C):    ");
// Serial.println(T1w);


// Serial.print("Temp 2 (C):    ");
// Serial.println(T2w);


// Serial.print("Humidity:    ");
// Serial.println(Hw);

UVw = uvIntensity;
// Serial.print("UV Index:    ");
// Serial.println(UVw);
Lw = myLux.getLux();
//Serial.print("Light (lux):    ");
// Serial.println(Lw);
//------------------------------------------------------------------

  File m24 = SPIFFS.open("/h24", "a+"); 
  //Serial.printf("File size: %.2f MB\n", (float)m24.size() / (1024 * 1024));
  String json = "{\"t1\":" + T1w + ", \"t2\":" + T2w + ", \"h\":" + Hw + ", \"uv\":" + UVw + ", \"l\":" + Lw + ", \"utc\":" + t + "},";
  m24.print(json);
  m24.close();

checkAndShortenFile("/h24");

  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  } //  end time
  
  server.handleClient();
  MDNS.update();
} // end loop
