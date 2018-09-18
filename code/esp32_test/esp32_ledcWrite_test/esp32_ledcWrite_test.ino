/*the code fade any GPIO pin except IO13, IO15, RX, TX, inputPin(IO34,35,36,37,38,39)
  and the pins used SDIO Slave(IO6,7,8,9,10,11)
*/
const uint8_t pin1 = 2;
const uint8_t pin2 = 0;
const uint8_t pin3 = 4;
const uint8_t pin4 = 16;
const uint8_t pin5 = 17;
const uint8_t pin6 = 5;
//const uint8_t pin7 = 18;
const uint8_t pin8 = 19;
const uint8_t pin9 = 21; //不安なところ　其の一
const uint8_t pin10 = 22;//不安なところ　其の二
const uint8_t pin11 = 23;

const uint8_t pin12 = 12;
const uint8_t pin13 = 14;
const uint8_t pin14 = 27;
const uint8_t pin15 = 26;//不安なところ　其の三
const uint8_t pin16 = 25;//不安なところ　其の四
const uint8_t pin17 = 33;
//const uint8_t pin18 = 32;
//const uint8_t pin19 = 35;
//const uint8_t pin20 = 34;

void fade(uint8_t pin) {
  uint8_t pwm = 0;
  const uint8_t ch = 0;
  const uint8_t delayTime = 2;
  ledcSetup(0, 490, 8);
  ledcAttachPin(pin, ch);
  while (pwm < 254) {
    ledcWrite(0, pwm);
    pwm++;
    delay(delayTime);
  }
  while (pwm > 0) {
    ledcWrite(0, pwm);
    pwm--;
    delay(delayTime);
  }
  ledcDetachPin(pin);
}

void setup() {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  //pinMode(pin7, OUTPUT);
  pinMode(pin8, OUTPUT);
  pinMode(pin9, OUTPUT);
  pinMode(pin10, OUTPUT);
  pinMode(pin11, OUTPUT);

  pinMode(pin12, OUTPUT);
  pinMode(pin13, OUTPUT);
  pinMode(pin14, OUTPUT);
  pinMode(pin15, OUTPUT);
  pinMode(pin16, OUTPUT);
  pinMode(pin17, OUTPUT);
  //pinMode(pin18, OUTPUT);
  //pinMode(pin19, OUTPUT);
  //pinMode(pin20, OUTPUT);
}

void loop() {
  fade(pin1);
  fade(pin2);
  fade(pin3);
  fade(pin4);
  fade(pin5);
  fade(pin6);
  //fade(pin7);
  fade(pin8);
  fade(pin9);
  fade(pin10);
  fade(pin11);

  fade(pin12);
  fade(pin13);
  fade(pin14);
  fade(pin15);
  fade(pin16);
  fade(pin17);
  //fade(pin18);
  //fade(pin19);
  //fade(pin20);
}
