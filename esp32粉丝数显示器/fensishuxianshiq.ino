#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include <WiFi.h>
#include <Ticker.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h
const char* ssid     = "";//wifi的ssid
const char* password = "";//wifi的密码

const char* userId = "";//自己b站的UID
uint8_t refreshing = 0;//
const char* streamId   = "....................";
const char* privateKey = "....................";
uint8_t count = 0;//起到计时的作用
Ticker ticker;//声明一个系统变量
HTTPClient http;
uint32_t requestBNumber(const char* userId);
void refreshBNumber();
void showMessage(String message);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
  ticker.attach(1, []() {
    count ++;  
  });
  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}

void loop() {

  if(count % 5 == 0) {
    if(refreshing != count) {
      refreshing = count;
      refreshBNumber();
    }
  }
    

    display.display();
    delay(10);
}


uint32_t requestBNumber(const char* userId) {
  int httpCode;
  http.begin("api.bilibili.com", 80, "/x/relation/stat?vmid=" + String(userId));//请求API接口，接口地址api.bilibili.com，路径/x/relation/stat?vmid，userId，你的账号id
  httpCode = http.GET();//接下来就会返回一个数据
  if(httpCode == HTTP_CODE_OK) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, http.getString());
    JsonObject response = doc.as<JsonObject>();
    return response["data"]["follower"].as<uint32_t>();//首先获取dat变量下的follower，然后转成uint32类型，然后返回
  } else {
    Serial.println(http.getString());
    delay(3000);
    Serial.printf("start retry....");

    return requestBNumber(userId);
  }
}



void refreshBNumber() {
  uint32_t number = requestBNumber(userId);
  Serial.println(number);
  showMessage(String(number));
}


void showMessage(String message) {
  display.clear();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 18, message);
  display.display();
}
