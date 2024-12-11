#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// 定义 LED 引脚
#define LED_PIN 8

// 定义新的 manufacturer_data 和厂商信息
uint8_t manufacturer_data[] = {
    0x02, 0x00, 0x01, 0x10, 0x1c, 0xa9, 0x28, 0x1e,
    0xb3, 0xbe, 0x4c, 0xd8, 0x9a, 0xf7, 0xcd, 0xb2,
    0xf5, 0xaa, 0x90, 0xf7, 0x5d, 0x0b, 0xa1, 0xe3,
    0xfb, 0xbc
};
uint16_t company_id = 0x012d; // 厂商数据为 0x2d 0x01 (小端顺序)

// 广播间隔（毫秒）
const unsigned long broadcast_interval = 2000; // 2 秒
unsigned long last_broadcast_time = 0;

BLEAdvertising *pAdvertising = nullptr;

// 初始化蓝牙广播
void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Advertising...");

  // 初始化 BLE 设备
  BLEDevice::init("ESP32 BLE Advertiser");

  // 创建 BLE 广播
  pAdvertising = BLEDevice::getAdvertising();

  pinMode(LED_PIN, OUTPUT); // 设置 LED 引脚为输出
}

void broadcastBLE() {
  // 设置 Manufacturer Data
  BLEAdvertisementData advertisementData;
  String manufacturerData = ""; // 使用 Arduino String 类型

  // 添加公司 ID（小端序）
  manufacturerData += (char)(company_id & 0xFF);        // 低字节：0x2d
  manufacturerData += (char)((company_id >> 8) & 0xFF); // 高字节：0x01

  // 添加 manufacturer_data
  for (size_t i = 0; i < sizeof(manufacturer_data); i++) {
    manufacturerData += (char)manufacturer_data[i];
  }

  advertisementData.setManufacturerData(manufacturerData); // 使用 Arduino String 类型
  pAdvertising->setAdvertisementData(advertisementData);

  // 启动广播
  pAdvertising->start();
  Serial.println("Broadcast sent!");

  // 停止广播（可选）
  delay(100); // 确保广播发送完成
  pAdvertising->stop();
}

void loop() {
  unsigned long current_time = millis();

  // 每隔 2 秒发送一次广播
  if (current_time - last_broadcast_time >= broadcast_interval) {
    last_broadcast_time = current_time;

    // LED 闪烁
    digitalWrite(LED_PIN, HIGH);
    delay(200); // 点亮 200ms
    digitalWrite(LED_PIN, LOW);

    // 发送广播
    broadcastBLE();
  }
}