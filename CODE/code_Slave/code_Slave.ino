 #include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 2 // what pin we're connected to
#define DHTTYPE           DHT11
DHT dht(DHTPIN, DHTTYPE);
float dataArray[2];
// Khai báo các chân RX/TX cho mô-đun Zigbee
const int zigbeeRX = 7;
const int zigbeeTX = 8;

SoftwareSerial zigbeeSerial(zigbeeRX, zigbeeTX);

void setup() {
  // Khởi tạo cổng serial cho mô-đun Zigbee
  zigbeeSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // Gửi dữ liệu
   float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  if (isnan(humi) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }
    dataArray[0] = temp;
  dataArray[1] = humi;
  float arraySize = sizeof(dataArray) / sizeof(dataArray[0]);

  // Tạo đối tượng JSON và đưa dữ liệu vào
  StaticJsonDocument<128> jsonDocument;
  JsonArray dataJsonArray = jsonDocument.to<JsonArray>();

  for (int i = 0; i < arraySize; i++) {
    dataJsonArray.add(dataArray[i]);
  }
  // Chuyển đổi thành chuỗi JSON
  String jsonData;
  serializeJson(dataJsonArray, jsonData);
  zigbeeSerial.println(jsonData);
  delay(1000);
}
