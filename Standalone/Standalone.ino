#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h> // SPIFS
// Stuff--------------------------------------
#include "DHTesp.h"
#include <Wire.h> //SPI
#include "Max44009.h" // Lux
#include <MCP3008.h>
Max44009 myLux(0x4A);
#define CS_PIN D8
#define CLOCK_PIN D5
#define MOSI_PIN D7
#define MISO_PIN D6
MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);
DHTesp dht;

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
#define STASSID "..."
#define STAPSK  "..."
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
<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Data Visualization</title>
    <style>
        body { font-family: Arial, sans-serif; }
        canvas { border: 1px solid black; margin-top: 20px; width: 100%; }
        #container div { margin-bottom: 15px; }
    </style>
</head>
<body>
<div id='container'>
    <div>
        <label>t1:</label> <span id='t1Key'></span>
        <canvas id='t1Canvas' width='1600' height='440'></canvas>
    </div>
    <div>
        <label>t2:</label> <span id='t2Key'></span>
        <canvas id='t2Canvas' width='1600' height='440'></canvas>
    </div>
    <div>
        <label>h:</label> <span id='hKey'></span>
        <canvas id='hCanvas' width='1600' height='440'></canvas>
    </div>
    <div>
        <label>uv:</label> <span id='uvKey'></span>
        <canvas id='uvCanvas' width='1600' height='440'></canvas>
    </div>
    <div>
        <label>l:</label> <span id='lKey'></span>
        <canvas id='lCanvas' width='1600' height='440'></canvas>
    </div>
</div>
<script>
    fetch('/json')
    .then(response => response.json())
    .then(data => {
        const canvasWidth = document.body.clientWidth;
        const canvasHeight = 440;
        const padding = 20;

        // Adjusting the width for plotting by subtracting a few pixels
        const xSlotWidth = canvasWidth / (data.length - 1);

        const adjustForConstantValues = (min, max) => {
            const offset = 0.1 * (max - min || 1);
            return [min - offset, max + offset];
        };

        const colors = {
            t1: 'red',
            t2: 'orange',
            h: 'blue',
            uv: 'purple',
            l: 'gray'
        };

        ['t1', 't2', 'h', 'uv', 'l'].forEach(metric => {
            let maxVal = Math.max(...data.map(item => item[metric] || 0));
            let minVal = Math.min(...data.map(item => item[metric] || 0));
            
            [minVal, maxVal] = adjustForConstantValues(minVal, maxVal);
            
            let yScale = (canvasHeight - 2 * padding) / (maxVal - minVal);
            document.getElementById(metric + 'Key').textContent = 'Min: ' + minVal.toFixed(2) + ', Max: ' + maxVal.toFixed(2);
            let canvas = document.getElementById(metric + 'Canvas');
            let ctx = canvas.getContext('2d');
            
            ctx.strokeStyle = colors[metric];
            data.forEach((point, index) => {
                let x = xSlotWidth * index;
                let y = canvasHeight - (point[metric] - minVal) * yScale - padding;

                ctx.fillStyle = colors[metric];
                ctx.fillRect(x-2, y-2, 4, 4);

                ctx.font = 'bold 12px Arial';
                ctx.fillStyle = 'black';
                ctx.fillText(point[metric].toFixed(2), x + 5, y - 5);

                if (index % 5 == 0 || index == data.length - 1) {
                    ctx.font = '10px Arial';
                    ctx.fillStyle = 'blue';
                    ctx.fillText(new Date(point.utc * 1000).toISOString().substr(11, 5), x - 10, canvasHeight - 5);
                }

                if(index === 0) ctx.moveTo(x, y);
                else ctx.lineTo(x, y);
            });
            ctx.stroke();
        });
    });
</script>
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

  if (!file) {
    server.send(500, "text/plain", "Failed to open the file.");
    return;
  }

  // Check and replace the last comma with ]
  file.seek(file.size() - 1); // Move to the last character
  char finalChar = file.read();
  if (finalChar == ',') {
    file.seek(file.size() - 1);
    file.print("]");
  }

  // Reset the file position to start to ensure the full content is streamed
  file.seek(0);
  
  server.streamFile(file, "application/json");
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
 Serial.print("Temp 1 (C):    ");
 Serial.println(T1w);


 Serial.print("Temp 2 (C):    ");
 Serial.println(T2w);


 Serial.print("Humidity:    ");
 Serial.println(Hw);

UVw = uvIntensity;
 Serial.print("UV Index:    ");
 Serial.println(UVw);
Lw = myLux.getLux();
 Serial.print("Light (lux):    ");
 Serial.println(Lw);
//------------------------------------------------------------------




File m24 = SPIFFS.open("/h24", "r+");  // Open for reading and updating
if (!m24) {
    Serial.println("Failed to open file for appending");
    return;
}
if (m24.size() == 0) {  // If the file is empty
    m24.print("[");
}
m24.seek(m24.size() - 1);  // Move to the last character
char lastChar = m24.read();
if (lastChar == ']') {
    m24.seek(m24.size() - 1);  // Move one character back to overwrite ]
    m24.print(",");
}

String json = "{\"t1\":" + T1w + ", \"t2\":" + T2w + ", \"h\":" + Hw + ", \"uv\":" + UVw + ", \"l\":" + Lw + ", \"utc\":" + t + "}";

// Add the new JSON data
m24.print(json);  // where newData is your new JSON string

// Always ensure the data ends with a ]
m24.print("]");

m24.close();


  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  } //  end time
  
  server.handleClient();
  MDNS.update();
} // end loop
