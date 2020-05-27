#include "esp_camera.h"
#include <WiFi.h>
#include "WiFiUdp.h"

const char *ssid = "***";  //要连接的wifi或热点名字最好不要有特殊符号和中文
const char *password = "***";//wifi或热点的密码

WiFiUDP  udp;                      //创建UDP对象
unsigned int localUdpPort = 1234; //本地端口号
uint8_t aaa='0';
camera_fb_t * fb;
IPAddress ip;

#define led       4

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35 //D7
#define Y8_GPIO_NUM       34 //D6
#define Y7_GPIO_NUM       39 //D5
#define Y6_GPIO_NUM       36 //D4
#define Y5_GPIO_NUM       21 //D3
#define Y4_GPIO_NUM       19 //D2
#define Y3_GPIO_NUM       18 //D1
#define Y2_GPIO_NUM        5 //D0
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22





esp_err_t camera_init() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    if(psramFound()){

    config.frame_size = FRAMESIZE_SVGA; //调整分辨率     
    /*FRAMESIZE_QQVGA,    // 160x120
    FRAMESIZE_QQVGA2,   // 128x160
    FRAMESIZE_QCIF,     // 176x144
    FRAMESIZE_HQVGA,    // 240x176
    FRAMESIZE_QVGA,     // 320x240
    FRAMESIZE_CIF,      // 400x296
    FRAMESIZE_VGA,      // 640x480
    FRAMESIZE_SVGA,     // 800x600
    FRAMESIZE_XGA,      // 1024x768
    FRAMESIZE_SXGA,     // 1280x1024
    FRAMESIZE_UXGA,     // 1600x1200
    FRAMESIZE_QXGA,     // 2048*1536 */

    config.jpeg_quality = 10;
    config.fb_count = 4;
  }
    //摄像头初始化
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.print("Camera Init Failed");
        return err;
    }
    sensor_t * s = esp_camera_sensor_get();
    //图像处理
    if (s->id.PID == OV2640_PID) {
            s->set_vflip(s, 1); //图像翻转
            s->set_brightness(s, 1); //图像亮度
            s->set_contrast(s, 1); //图像对比度
    }
    
    Serial.print("Camera Init OK");
    return ESP_OK;
}

void wifi_init(void)
{
    WiFi.mode(WIFI_STA);
    
    WiFi.setSleep(false); //关闭STA模式下wifi休眠，提高响应速度
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("Connected");

    Serial.print("IP Address:");
    
    Serial.println(WiFi.localIP());
    pinMode(led, OUTPUT);
    if(udp.begin(localUdpPort))
    {
        Serial.println("UDP connected");
        udp.print("Hello Server!"); 
    }
    digitalWrite(led, LOW);
}


void setup() {
    Serial.begin(115200);
    wifi_init(); // WiFi初始化
    camera_init(); // 摄像头初始化
    udp.begin(localUdpPort); //启用UDP监听以接收数据
    

    Serial.println("sys is running!");
}

void loop() 
{
    
            fb = esp_camera_fb_get();
            if (!fb)
            {
                Serial.print( "Camera capture failed\n");
            }
            else
            { 
                
                int packetSize = udp.parsePacket();
                if (packetSize){
                    char buf[4];
                    udp.read(&aaa,4);
                    ip = udp.remoteIP();
                    Serial.println(ip);
                }                
                if(aaa=='1'){
                    digitalWrite(led, HIGH);
                    //Serial.println("on");
                }
                else
                {
                    digitalWrite(led, LOW);
                    //Serial.println("off");
                }
                if(1){
                    Serial.println((uint8_t)fb->buf);
                    udp.beginPacket("192.168.0.7", 777);//这里ip和端口改为客户端显示的ip和设定的端口，一定要对应！
                    udp.write((uint8_t*)fb->buf,fb->len);                
                    udp.endPacket();
                }
                esp_camera_fb_return(fb);
            }

}
