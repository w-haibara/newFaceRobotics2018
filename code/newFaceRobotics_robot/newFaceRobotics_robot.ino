/* esp-wroom-2に書き込むコード
    オムニホイール４個(正転逆転)　ギヤボックス１個(正転のみ)　サーボ(２値のみ)　
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUDP.h>
#include <Servo.h>

IPAddress myIP(192, 168, 4, 1);
const char *ssid = "AoD_ESP_001";sudo
const char *password = "AoDESP001";

byte WiFibuff[1];
WiFiUDP UDP;

Servo servo;


const int servoPin = 0;

const int gearboxPin = 1;

const int omniPin_A1 = 2;
const int omniPin_A2 = 3;

const int omniPin_B1 = 4;
const int omniPin_B2 = 5;

const int omniPin_C1 = 12;
const int omniPin_C2 = 13;

const int omniPin_D1 = 13;
const int omniPin_D2 = 15;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.config(myIP, WiFi.gatewayIP(), WiFi.subnetMask());
  UDP.begin(100);

  servo.attach(servoPin);

  pinMode(servoPin, OUTPUT);
  pinMode(gearboxPin, OUTPUT);

  pinMode(omniPinA1, OUTPUT);
  pinMode(omniPinA2, OUTPUT);

  pinMode(omniPinB1, OUTPUT);
  pinMode(omniPinB2, OUTPUT);

  pinMode(omniPinC1, OUTPUT);
  pinMode(omniPinC2, OUTPUT);

  pinMode(omniPinD1, OUTPUT);
  pinMode(omniPinD2, OUTPUT);
}

void loop() {
}
