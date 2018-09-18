/*the code brink any GPIO pin except IO13, IO15, RX, TX,
  and the pins used SDIO Slave(IO6, 7, 8, 9, 10, 11)
*/
const int pin1 = 2;
const int pin2 = 0;
const int pin3 = 4;
const int pin4 = 16;
const int pin5 = 17;
const int pin6 = 5;
//const int pin7 = 18;
const int pin8 = 19;
const int pin9 = 21;
const int pin10 = 22;
const int pin11 = 23;

const int pin12 = 12;
const int pin13 = 14;
const int pin14 = 27;
const int pin15 = 26;
const int pin16 = 25;
const int pin17 = 33;
//const int pin18 = 32;
//const int pin19 = 35;
//const int pin20 = 34;


void setup() {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
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
}

void loop() {
  brink(pin1);
  brink(pin2);
  brink(pin3);
  brink(pin4);
  brink(pin5);
  brink(pin6);
  brink(pin8);
  brink(pin9);
  brink(pin10);
  brink(pin11);

  brink(pin12);
  brink(pin13);
  brink(pin14);
  brink(pin15);
  brink(pin16);
  brink(pin17);
}

void brink(int pin) {
  digitalWrite(pin, HIGH);
  delay(300);
  digitalWrite(pin, LOW);
  delay(300);
}

