/* esp-wroom-2に書き込むコード
    オムニホイール４個(正転逆転)　ギヤボックス１個(正転のみ)　サーボ(２値のみ)　
          B
  A〇ー　〇
   　　　｜
   |
   〇　－〇C
   D
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUDP.h>
#include <Servo.h>

IPAddress myIP(192, 168, 4, 1);
const char *ssid = "AoD_ESP_001";
const char *password = "AoDESP001";

byte WiFibuffer[1];
WiFiUDP UDP;

Servo servo;

const uint8_t servoPin = 0;

const uint8_t gearboxPin = 1;

const uint8_t omniPin_A1 = 2;
const uint8_t omniPin_A2 = 3;

const uint8_t omniPin_B1 = 4;
const uint8_t omniPin_B2 = 5;

const uint8_t omniPin_C1 = 12;
const uint8_t omniPin_C2 = 13;

const uint8_t omniPin_D1 = 13;
const uint8_t omniPin_D2 = 15;

boolean motorDirection = false;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.config(myIP, WiFi.gatewayIP(), WiFi.subnetMask());
  UDP.begin(100);

  servo.attach(servoPin);

  pinMode(servoPin, OUTPUT);
  pinMode(gearboxPin, OUTPUT);

  pinMode(omniPin_A1, OUTPUT);
  pinMode(omniPin_A2, OUTPUT);

  pinMode(omniPin_B1, OUTPUT);
  pinMode(omniPin_B2, OUTPUT);

  pinMode(omniPin_C1, OUTPUT);
  pinMode(omniPin_C2, OUTPUT);

  pinMode(omniPin_D1, OUTPUT);
  pinMode(omniPin_D2, OUTPUT);
}

void loop() {
  if (UDP.parsePacket() > 0) {
    receiveWiFi();
  }
}

void receiveWiFi() {
  UDP.read(WiFibuffer, 1);
  Serial.write(WiFibuffer[0]);
  UDP.flush();

}

void omniDrive(uint16_t velocity_x , uint16_t velocity_y) {//velosity_x, velosity_yで -1024～1024の範囲のx方向, y方向それぞれの速度を指定

  motorControl('A', velocity_y);
  motorControl('C', velocity_y);

  motorControl('B', velocity_x);
  motorControl('D', velocity_x);
}

void omniTurn(uint16_t velocity_turn) {
  //velosity_turnで-1024～1024の範囲の速度を指定
  //velosity_turnが正の値の時，右旋回．負の値のとき，左旋回
  motorControl('A', velocity_turn);
  motorControl('C', velocity_turn);
  motorControl('B', velocity_turn);
  motorControl('D', velocity_turn);
}

void motorControl(char omniName, int velocity) {
  //omniNameでA,B,C,Dのどのオムニのモーターかを，velosityで-1024～1024の範囲の速度を指定
  uint8_t HbridgeR = 255;
  uint8_t HbridgeL = 255;
  uint16_t pwm  = abs(velocity);
  boolean minus =  (velocity < 0) ? true : false;

  //minus =  !minus;をコメントアウトすることで各モーターの回転方向を反転させる
  switch (omniName) {
    case 'A':
      HbridgeR = omniPin_A1;
      HbridgeL = omniPin_A2;
      //minus =  !minus;
      break;
    case 'B':
      HbridgeR = omniPin_B1;
      HbridgeL = omniPin_B2;
      //minus =  !minus;
      break;
    case 'C':
      HbridgeR = omniPin_C1;
      HbridgeL = omniPin_C2;
      //minus =  !minus;
      break;
    case 'D':
      HbridgeR = omniPin_D1;
      HbridgeL = omniPin_D2;
      //minus =  !minus;
      break;
    default:
      break;
  }

  digitalWrite(HbridgeR , LOW);
  digitalWrite(HbridgeL , LOW);

  if (motorDirection != minus) delay(1); //モーターの回転方向が変わるときには1ms待つ(fetで信号が遅延するため)

  if (minus) {
    //モーターが逆転する処理(R→L方向への電流)
    analogWrite(HbridgeR , pwm);
  } else {
    //モーターが正転する処理(L→R方向への電流)
    analogWrite(HbridgeL , pwm);
  }
  motorDirection = minus;
}
