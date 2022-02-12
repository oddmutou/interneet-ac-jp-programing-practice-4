#include <SPI.h>
#include <WiFiNINA.h>

// 接続先WiFiアクセスポイントのSSIDとパスワード
#define SECRET_WIFI_SSID "XXXXX"
#define SECRET_WIFI_PASS "XXXXX"

// WiFiまわりのステータス。初期値はアイドル状態
int status = WL_IDLE_STATUS;
// WiFi client
WiFiClient client;

// 圧力(1~1024)
int pressure_level = 1;
// 開けっ放しのカウント
int count_with_open = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(9600);

  // WiFiモジュールが確認できるまで待つ
  if (WiFi.status() == WL_NO_MODULE) {
    while (true);
  }

  // ファームウェアバージョン確認
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");

  }

  // WiFiアクセスポイントへ接続
  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_WIFI_SSID);

    status = WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_PASS);
    if (status != WL_CONNECTED) {
      // WiFiアクセスポイントへの接続ができなければ10秒待つ
      delay(10000);
    }
  }
}

void loop() {
  // 1秒待つ。ついでに内蔵LEDを明滅させ動作していることをわかりやすく
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);

  pressure_level = 1024 - analogRead(1);
  Serial.println(pressure_level);

  if (pressure_level > 100) {
    // 圧力が100以上の場合は閉じているとする
    // 開けっ放しカウントをクリア
    count_with_open = 0;
  } else {
    // 開けっ放しカウントをインクリメント
    count_with_open++;

    // もし開けっ放しカウントが5を超えた場合アラート送信
    if (count_with_open > 5) {
      // アラートを送信
      requestToServer();

      // アラートを送信した場合、内蔵LEDを付けて5秒待つ(LEDつけっぱなしにしてアラート送信したことをわかりやすく)
      digitalWrite(LED_BUILTIN, HIGH);
      delay(5000);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

void requestToServer() {
  // インターネット上のサーバーにリクエストを送信
  if (client.connect("XXX.XXX.XXX.XXX", 8089)) {
    client.println("POST /index.php HTTP/1.1");
    client.println("Connection: close");
    client.println();
  }
}
