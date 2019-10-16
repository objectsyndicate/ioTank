# ioTank
IoT (internet of things) environment monitor based on [Esp8266](https://espressif.com/en/products/hardware/esp8266ex/overview) [(NodeMCU)](https://github.com/nodemcu/nodemcu-devkit-v1.0), [Arduino](https://www.arduino.cc/), [Jquery](https://jquery.com/), [Django REST](http://www.django-rest-framework.org/), [Fritzing](http://fritzing.org/home/), the third industrial revolution, and the [Hammond 1551k project box](http://www.hammondmfg.com/pdf/1551K.pdf). Designed in Colorado by [Object Syndicate](https://objectsyndicate.com).

<img class='img-responsive' src="https://www.objectsyndicate.com/wp-content/uploads/2017/11/IMAG0343-2.jpg">

## Build instructions
- Order PCB using 'Gerber' files provided.
- Place components into the PCB as labeled and solder. 
- Flash 'update firmware' with the windows flasher via USB or Android flasher with an OTC cable.

[![ioTank Video](https://img.youtube.com/vi/SeW17QTGf-k/0.jpg)
](https://www.youtube.com/watch?v=SeW17QTGf-k)

## Useful for,
- Horticulture
- Aquaculture
- Herpetoculture
- Permaculture
- Mycology / Fungiculture

## What can it do for environmental monitoring?
ioTank captures real time (every 300ms) sensor readings for temperature, humidity, and light. It plots them on a graph which can be read on any HTML5/JavaScript compatible browser (so any smart phone and all modern PCs/macs). Since it is WiFi enabled it will connect to a router or a device using Internet sharing. You do need a PC/Mac/Android to set the device's WiFi SSID/password and flash firmware. Once the SSID/pass is set up it will operate autonomously and will connect to the access point and act like any other WiFi device. IoTank is designed to store it's info every 5 minutes send it to a central server for long term storage and data processing. We provide a SaaS for cloud storage, [ObjectSyndicate.com](https://objectsyndicate.com). There is also an open source Django server published under the Apache 2.0 license [Crop🌱Watch](https://github.com/objectsyndicate/Crop-Watch). You are also free to build your own using Django REST or other web languages. With Crop🌱Watch it becomes possible to interface with industrial PLC for automation. 


## What can it do for web developers/coders?
ioTank provides a complete user experience to bootstrap your Django IoT project (or other REST based framework). The code covers out of box registration, HTTP POST via REST token to a cloud service, and on board webserver with real-time and past plotted graphs (powered by [jQuery flot](http://www.flotcharts.org/)).
User flashed configuration using esptool and mkspiffs backends. Placing Wifi config in a serial flasher forces a bit more security as the device takes no HTTP POSTs as well as allows serial communication for a secure place to store the device UUID for registration. Flasher also allows for the enable/disable of the cloud POST option.
Off the shelf parts reduces cost and allows for rapid prototyping and market exploration. Remaining in a small, USB form factor and outsourcing things like AC power control(WEMO) and network routing to other open source devices reduces regulation problems typical of bringing an appliance to market.
The source code can of course be adapted to any industry or hobby, as you may add/remove and change sensors.

## Monitors the following stats,
* Internal temperature
* Waterproof temperature probe
* Humidity
* Lux
* UV Index

Can be configured with a variety of other sensors Soil Humidity, more Temp probes, RGB sensor, etc. Expanding the 1 ADC typically available to the ESP8266 to 9 with the addition of the MCP3008.

## Materials required to build,
* 1 NodeMCU v1.0 (CP2102)
* 1 MCP3008
* 1 GY-ML8511 (blue PCB)
* 1 GY-49 (purple PCB)
* 1 DHT11
* 1 NTC 10k thermistor 3' lead.
* 1 Hammond 1551k
* 2 10k through-hole resistors.


### Notes
The Flasher is built with [SharpDevelop](http://www.icsharpcode.net/opensource/sd/Default.aspx) It uses [esptool-ck](https://github.com/igrr/esptool-ck), [mkspiffs](https://github.com/igrr/mkspiffs) and [putty](http://www.putty.org/). 

The Fritzing NodeMCU PCB/SVG data is a [fork](https://github.com/squix78/esp8266-fritzing-parts), I added vias of a larger size as a quick hack since the holes are too small in production. 

The PCB shape was built using Inkscape, following tutorials on the listed sites. As of this writing one should include 2.5mm holes in the route just to be safe, see the included Fritzing and Gerber files for a working production model. 

The ESP8266 is a cheap chip and arduino open source. Although this gives increased availability and simplifies design the stack has limitations. Most notably the difficulty of implementing HTTPS as a server on chip. What it can do by itself is still a lot, but not enough for some users or for 100% security concerns. These can mostly be overcome with the addition of a decent router. We suggest anything which runs DD-WRT. This will allow for encrypted VPN connections and MAC address based static IP assignment of the DHCP server. I have addressed any security concern I could think of and have included or excluded many things based on what I felt was the best decision always placing security first and assuming the end user would be using a regular router, with anyone possibly on the LAN with 0 configuration beyond that. We are very open to exploit reports and any ideas from the security community on how to best improve the stack or questions on why a given decision was made(or can change). Part of our transparency process for security is to publish the firmware so it is in the public and can be scrutinized and improved upon.

Much of the code examples I used to write this are published on Github, Adafruit, Sparkfun, Fritzing and Arduino forums, but not all of it. Such as the way I decided to serve large files from the ESP8266, that came from a [post on esp8266.com](http://www.esp8266.com/viewtopic.php?f=32&t=3780). It is highly encouraged to reference those sources. 

### Drivers
The driver for the USB to UART chip is provided by [Sillabs](http://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) as the NodeMCU uses the CP2102 USB to UART. The Flasher application provided here is used to configure WiFi username/password and to enable cloud service on devices flashed with the cloud enabled (prototypes we have demoed and handed out at trade shows don't have cloud enabled.)
