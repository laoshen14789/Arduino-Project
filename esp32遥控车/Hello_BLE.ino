/* *****************************************************************
 *
 * Download latest Blinker library here:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * 
 * Blinker is a cross-hardware, cross-platform solution for the IoT. 
 * It provides APP, device and server support, 
 * and uses public cloud services for data transmission and storage.
 * It can be used in smart home, data monitoring and other fields 
 * to help users build Internet of Things projects better and faster.
 * 
 * Make sure installed 2.5.0 or later ESP8266/Arduino package,
 * if use ESP8266 with Blinker.
 * https://github.com/esp8266/Arduino/releases
 * 
 * Make sure installed 1.0.2 or later ESP32/Arduino package,
 * if use ESP32 with Blinker.
 * https://github.com/espressif/arduino-esp32/releases
 * 
 * Docs: https://doc.blinker.app/
 *       https://github.com/blinker-iot/blinker-doc/wiki
 * 
 * *****************************************************************
 * 
 * Blinker 库下载地址:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * Blinker 是一套跨硬件、跨平台的物联网解决方案，提供APP端、设备端、
 * 服务器端支持，使用公有云服务进行数据传输存储。可用于智能家居、
 * 数据监测等领域，可以帮助用户更好更快地搭建物联网项目。
 * 
 * 如果使用 ESP8266 接入 Blinker,
 * 请确保安装了 2.5.0 或更新的 ESP8266/Arduino 支持包。
 * https://github.com/esp8266/Arduino/releases
 * 
 * 如果使用 ESP32 接入 Blinker,
 * 请确保安装了 1.0.2 或更新的 ESP32/Arduino 支持包。
 * https://github.com/espressif/arduino-esp32/releases
 * 
 * 文档: https://doc.blinker.app/
 *       https://github.com/blinker-iot/blinker-doc/wiki
 * 
 * *****************************************************************/

#define BLINKER_BLE

#include <Blinker.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier

BlinkerButton Button1("Front");
BlinkerNumber Number1("num-abc");
BlinkerButton Button2("Left");
BlinkerButton Button3("Right");
BlinkerButton Button4("Back");
int front=0,left=0,right=0,back=0;
int counter = 0;

void button1_callback(const String & state)
{
    BLINKER_LOG("Front ", state);
    if(state=="press") front=1; else front=0;
    //digitalWrite(12,1);
    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    
}

void button2_callback(const String & state)
{
    BLINKER_LOG("Left ", state);
    if(state=="press") left=1; else left=0;
    //digitalWrite(14,1);
    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    
}

void button3_callback(const String & state)
{
    BLINKER_LOG("Right ", state);
    if(state=="press") right=1; else right=0;
    //digitalWrite(27,1);
    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    
}

void button4_callback(const String & state)
{
    BLINKER_LOG("Back ", state);
    if(state=="press") back=1; else back=0;
    //digitalWrite(26,1);
    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    
}
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    BLINKER_LOG("123456");
    //counter++;
    //Number1.print(counter);
}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    
    //pinMode(LED_BUILTIN, OUTPUT);
    //digitalWrite(LED_BUILTIN, HIGH);
    
    Blinker.begin();
    //Blinker.attachData(dataRead);
    pinMode(12,OUTPUT);
    pinMode(14,OUTPUT);
    pinMode(27,OUTPUT);
    pinMode(26,OUTPUT);
    digitalWrite(12,0);
    digitalWrite(14,0);
    digitalWrite(26,0);
    digitalWrite(27,0);
    Button1.attach(button1_callback);
    Button2.attach(button2_callback);
    Button3.attach(button3_callback);
    Button4.attach(button4_callback);
}

void loop()
{
    Blinker.run();
    if(front==1) digitalWrite(12,1); else digitalWrite(12,0);
    if(left==1) digitalWrite(14,1); else digitalWrite(14,0);
    if(right==1) digitalWrite(27,1); else digitalWrite(27,0);
    if(back==1) digitalWrite(26,1); else digitalWrite(26,0);

}

    
