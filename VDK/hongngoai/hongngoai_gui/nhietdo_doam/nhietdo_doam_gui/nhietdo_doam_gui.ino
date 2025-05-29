#include <IRremote.h>
#include <DHT.h>

#define DHTPIN 7       // Chân kết nối cảm biến DHT
#define DHTTYPE DHT11  // Loại cảm biến: DHT11 hoặc DHT22

DHT dht(DHTPIN, DHTTYPE);
IRsend irsend;

const int irPin = 3;  // Chân phát IR

void setup() {
  Serial.begin(9600);
  pinMode(irPin, OUTPUT);
  dht.begin();
}

void loop() {
  float temp = dht.readTemperature();   // Nhiệt độ (°C)
  float humi = dht.readHumidity();      // Độ ẩm (%)

  if (isnan(temp) || isnan(humi)) {
    Serial.println("Loi cam bien DHT!");
    delay(1000);
    return;
  }

  Serial.print("Nhiet do: ");
  Serial.print(temp);
  Serial.print(" *C, Do am: ");
  Serial.print(humi);
  Serial.println(" %");

  uint8_t command;

  if (temp > 40 || humi > 80) {
    command = 0x00;
    Serial.println("Nhiet do & do am cao - Gui 0x00");
  } else {
    command = 0x01;
    Serial.println("Nhiet do & do am binh thuong - Gui 0x01");
  }

  irsend.sendRC5(command, 12);  // Gửi IR với chuẩn RC5
  delay(2000);
}
