#include <WiFi.h>
#include <WiFiUdp.h>

const char ssid[] = "AoD_ESP_999"; // SSID
const char pass[] = "AoDESP999";  // password
const int localPort = 10000;      // ポート番号

const IPAddress ip(192, 168, 4, 1);       // IPアドレス(ゲートウェイも兼ねる)
const IPAddress subnet(255, 255, 255, 0); // サブネットマスク

WiFiUDP udp;

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, pass);           // SSIDとパスの設定
  delay(100);                        // 追記：このdelayを入れないと失敗する場合がある
  WiFi.softAPConfig(ip, ip, subnet); // IPアドレス、ゲートウェイ、サブネットマスクの設定

  Serial.print("AP IP address: ");
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);

  Serial.println("Starting UDP");
  udp.begin(localPort);  // UDP通信の開始(引数はポート番号)

  Serial.print("Local port: ");
  Serial.println(localPort);
}

void loop() {
  if (udp.parsePacket()) {
    Serial.println(udp.read()); // UDP通信で来た値を表示
    digitalWrite(2, HIGH);
    Serial.println("udp true");
  } else {
    digitalWrite(2, LOW);
//    Serial.println("udp false");
  }
}
