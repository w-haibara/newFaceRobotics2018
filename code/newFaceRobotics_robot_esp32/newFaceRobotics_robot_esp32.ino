/* esp-wroom-32に書き込むコード
    オムニホイール４個(正転逆転)　ギヤボックス１個(正転のみ)　サーボ(２値のみ)　
         B
  A〇ー　〇
   　　　｜
   |
   〇　－〇C
   D
*/
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUDP.h>

IPAddress myIP(192, 168, 4, 1);
const char *ssid = "AoD_ESP_999";
const char *password = "AoDESP999";

WiFiUDP UDP;

const uint8_t servoPin = 2;

const uint8_t gearboxPin = 0;

const uint8_t omniPin_A1 = 4;
const uint8_t omniPin_A2 = 16;

const uint8_t omniPin_B1 = 17;
const uint8_t omniPin_B2 = 5;

const uint8_t omniPin_C1 = 19;
const uint8_t omniPin_C2 = 21;

const uint8_t omniPin_D1 = 22;
const uint8_t omniPin_D2 = 23;

boolean motorDirection = false;

const uint8_t pwmChannel = 0;

void servoWrite(uint8_t deg) {
  //ledcWrite();
}

void motorControl(char omniName, int8_t velocity) {
  //omniNameでA,B,C,Dのどのオムニのモーターかを，velosityで-1024～1024の範囲の速度を指定
  if (velocity < 0 && velocity > 1024) {
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

    // digitalWrite(HbridgeR , LOW);
    // digitalWrite(HbridgeL , LOW);

    if (motorDirection != minus) delay(1); //モーターの回転方向が変わるときには1ms待つ(fetで信号が遅延するため)

    if (minus) {
      //モーターが逆転する処理(R→L方向への電流)
      //analogWrite(HbridgeR , pwm);
    } else {
      //モーターが正転する処理(L→R方向への電流)
      //analogWrite(HbridgeL , pwm);
    }
    motorDirection = minus;
  }
}
void omniTurn(int16_t velocity_turn) {
  //velosity_turnで-1024～1024の範囲の速度を指定
  //velosity_turnが正の値の時，右旋回．負の値のとき，左旋回．
  if (velocity_turn < 0 && velocity_turn > 1024) {
    motorControl('A', velocity_turn);
    motorControl('C', velocity_turn);
    motorControl('B', velocity_turn);
    motorControl('D', velocity_turn);
  }
}

void omniDrive(int16_t velocity_x , int16_t velocity_y) {
  //velosity_x, velosity_yで -1024～1024の範囲のx方向, y方向それぞれの速度を指定
  if (velocity_x < 0 && velocity_x > 1024 && velocity_x < 0 && velocity_x > 1024) {
    motorControl('A', velocity_y);
    motorControl('C', velocity_y);

    motorControl('B', velocity_x);
    motorControl('D', velocity_x);
  }
}

void receiveWiFi(byte* firstRead, byte* secondRead) {
  byte WiFibuffer[2];

  UDP.read(WiFibuffer, 1);
  *firstRead = WiFibuffer[0];
  Serial.write(WiFibuffer[0]);
  digitalWrite(0, HIGH);
  delay(100);
  digitalWrite(0, LOW);
  UDP.read(WiFibuffer, 1);
  *secondRead = WiFibuffer[1] - 128;

  UDP.flush();
}

void readWiFibuffer(byte* button1, byte* button2, byte* button3) {
  if (UDP.parsePacket() > 0) {
    byte firstRead = 0;
    byte secondRead = 0;
    receiveWiFi(&firstRead, &secondRead);
    if (firstRead == secondRead) {
      *button1 = firstRead >> 3; //x
      *button2 = (firstRead & B00000100) >> 2; //y
      *button3 = (firstRead & B00000010) >> 1; //z
    } else {
      *button1 = 0;
      *button2 = 0;
      *button3 = 0;
    }
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.config(myIP, WiFi.gatewayIP(), WiFi.subnetMask());
  UDP.begin(100);

  ledcSetup(0, 490, 8);
  ledcAttachPin(servoPin, 0);

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
  delay(10);//WiFiの通信に余裕を持たせるためのdelay

  byte  button_drive = 0;
  byte  button_servo = 0;
  byte  button_gearbox = 0;
  uint8_t buttonNum = 0;

  const uint8_t pwm_turn = 500;
  const uint8_t pwm_drive = 500;

  const uint8_t servo_start = 0;

  const uint8_t servo_end = 90;

  readWiFibuffer(&button_drive, &button_servo, &button_gearbox);

  //駆動系の処理
  switch (button_drive) {
    case 1://左旋回
      //buttonNum = 13;
      omniTurn(-pwm_turn);
      break;
    case 2://右旋回
      //buttonNum = 14;
      omniTurn(pwm_turn);
      break;
    case 3://↑前
      //buttonNum = 1;
      omniDrive(0, pwm_drive);
      break;
    case 4://↓後
      //buttonNum = 2;
      omniDrive(0, -pwm_drive);
      break;
    case 5://→右
      //buttonNum = 3;
      omniDrive(pwm_drive, 0);
      break;
    case 6://←左
      //buttonNum = 4;
      omniDrive(-pwm_drive, 0);
      break;
    case 7://↗右斜め上
      //buttonNum = 5;
      omniDrive(pwm_drive, pwm_drive);
      break;
    case 8://↖左斜め上
      //buttonNum = 6;
      omniDrive(-pwm_drive, pwm_drive);
      break;
    case 9://➘右斜め下
      //buttonNum = 7;
      omniDrive(pwm_drive, -pwm_drive);
      break;
    case 10://↙左斜め下
      //buttonNum = 8;
      omniDrive(-pwm_drive, -pwm_drive);
      break;
    default:
      break;
  }

  //制御系の処理
  if (button_servo) {
    //サーボの処理
    //servo.write(servo_end);
  } else {
    //servo.write(servo_start);
  }

  if (button_gearbox) {
    //ギヤボックスの処理
    digitalWrite(gearboxPin, HIGH);
  }
}
