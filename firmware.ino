#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include "DHT.h"

#define DHTPIN 13     //Pin to connect DHT22
#define DHTTYPE DHT22
#define pwmPin 5   //PWM pin to connect MH-z19b
// Pins to connect BMP280 (SPI interface)
#define BMP_SCK 9        
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10

Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

DHT dht(DHTPIN, DHTTYPE);

int prevPWMVal = LOW;
long th, tl, h, l, ppm;

void setup() {
  Serial.begin(9600);
  pinMode(pwmPin, INPUT);
  dht.begin();
  bme.begin();
}

void loop() {
  long tt = millis();
  int currPWMVal = digitalRead(pwmPin);

  if (currPWMVal == HIGH) {
    if (currPWMVal != prevPWMVal) {
      h = tt;
      tl = h - l;
      prevPWMVal = currPWMVal;
    }
  }  else {
    if (currPWMVal != prevPWMVal) {
      l = tt;
      th = l - h;
      prevPWMVal = currPWMVal;
      ppm = 5000 * (th - 2) / (th + tl - 4);
      Serial.println(String(ppm) + " " + bme.readPressure() + " "+ dht_data());  //When readed co2 PPM value, read other sensors' data and send them to serial port 
    }
  }
}

String dht_data ()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    return "0 0";
  }
  return String(t) + " " + String(h);
}

