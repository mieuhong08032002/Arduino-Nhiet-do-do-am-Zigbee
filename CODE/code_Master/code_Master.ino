#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <ArduinoJson.h>
// Khai báo các chân RX/TX cho mô-đun Zigbee
const int zigbeeRX = 7;
const int zigbeeTX = 8;

SoftwareSerial zigbeeSerial(zigbeeRX, zigbeeTX);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  // Khởi tạo cổng serial cho mô-đun Zigbee
  zigbeeSerial.begin(9600);
  Serial.begin(9600);
}
float temperature[2];
void loop() {
  // Nhận dữ liệu
  if (zigbeeSerial.available()) {
    String receivedData = zigbeeSerial.readStringUntil('\n');
    Serial.print("Received data: ");
    Serial.println(receivedData);
    String jsonString = receivedData;
    StaticJsonDocument<128> jsonDocument;
    DeserializationError error = deserializeJson(jsonDocument, jsonString);
    if (error) {
      Serial.print("JSON parsing failed: ");
      Serial.println(error.c_str());
    } else {
      // Truy xuất các phần tử trong mảng JSON và gán vào mảng dữ liệu
      JsonArray dataJsonArray = jsonDocument.as<JsonArray>();

      for (int i = 0; i < 2; i++) {
        temperature[i] = dataJsonArray[i].as<float>();
      }
      //Hiển thị lên LCD
      lcd.setCursor(0, 0);
      lcd.print("Temp  : ");
      lcd.print(abs(temperature[0]));
      lcd.print((char)223);
      lcd.print("C ");
      lcd.setCursor(0, 1);
      lcd.print("Humid : ");
      lcd.print(temperature[1]);
      lcd.print("%H");
      delay(500);
    }
  }
}
