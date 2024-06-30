#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <PubSubClient.h>

/*WIFI和MQTT信息*/
#define Wifi_SRC 2 // 选择WIFI源，1为华为，2为xiaoyan
#if Wifi_SRC == 1
const char* ssid     = "HUAWEI-B91J5S"; // 无线网络的SSID
const char* password = "Xiang936!"; // 无线网络的密码
#elif Wifi_SRC == 2
const char* ssid     = "XIAOYAN"; // 无线网络的SSID
const char* password = "88888888"; // 无线网络的密码
#endif
const char* mqtt_server = "broker.emqx.io"; // MQTT服务器的地址
WiFiClient espClient; // 创建一个WiFi客户端实例
PubSubClient client(espClient); // 创建一个PubSub客户端实例，使用上面创建的WiFi客户端


// LED灯信息
#define NUM_LEDS 1 // 定义LED的数量
#define DATA_PIN 48 // 定义数据引脚
CRGB leds[NUM_LEDS]; // 定义LED数组


// // 回调函数，当消息到达时调用
// void callback(char* topic, byte* payload, unsigned int length) {
//   // 打印收到的消息
//   for (int i = 0; i < length; i++) {
//     Serial.print((char)payload[i]);

//   }
//   Serial.println();
// }

// 重新连接函数
void reconnect(void) {
  // 循环直到重新连接成功
  while (!client.connected()) {
    // Serial.print("Attempting MQTT connection...");
    // 创建一个随机的客户端ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // 尝试连接
    if (client.connect(clientId.c_str())) {
      // Serial.println("connected");
      // 一旦连接成功，发布一个公告...
      client.publish("xiaoyan", "xiaoyan nihao");
      // ...并重新订阅
      client.subscribe("xiaoyan");
    } else {
      // Serial.print("failed, rc=");
      // Serial.print(client.state());
      // Serial.println(" try again in 5 seconds");
      // 等待5秒后重试
      delay(5000);
    }
  }
}

//wifi连接函数
void Set_Wifi()
{
    // 初始化并连接到指定的Wi-Fi网络
    WiFi.begin(ssid, password);
    // 等待WiFi连接
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        // Serial.print(".");
    }
    FastLED.setBrightness(100);

  //如果连上就变成绿色
    leds[0] = CRGB::Green;
  //显示
    FastLED.show();

    // Serial.println("");
    // Serial.println("WiFi connected");
    // Serial.println("IP address: ");
    // Serial.println(WiFi.localIP());
}

void MQTT_Connect_Init(void)
{
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    // Serial2.begin(115200);
    // tft初始化
    delay(3000);
    FastLED.setBrightness(100);
    // 把第0个灯设置成红色
    leds[0] = CRGB::Red;
    // 显示
    FastLED.show();
    Set_Wifi();
    // 设置MQTT端口和回调函数
    client.setServer(mqtt_server, 1883);
 //   client.setCallback(callback);
}