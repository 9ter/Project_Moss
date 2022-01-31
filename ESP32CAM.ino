// ESP32 รับข้อมูล id จากserver ถ้าสำเร็จจะถ่ายรูปส่งไลน์

#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>

#include "esp_camera.h"
#include "esp_system.h"

hw_timer_t *timer = NULL;
void IRAM_ATTR resetModule() {
  ets_printf("reboot\n");
  esp_restart();
}

#include <TridentTD_LineNotify.h>

#define LINE_TOKEN  "fWNWozkY1Om4vmT9cn4h4zIiuyyvFbYgdVftG802Eow"


const char* ssid = "Monday_2.4";
const char* password = "0924077881";


const char* host = "192.168.1.83";
const int httpPort = 4000;

String suisei = "Suisei";
WiFiClient Client;
HTTPClient http;


// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

const int Led_Flash = 4;
const int trigPin = 12;
const int echoPin = 13;
boolean startTimer = false;
unsigned long time_now = 0;
int time_capture = 0;



void setup() {
  ///////////////////////////////////////////setwifi/////////////////////////////
  Serial.begin(9600);
  delay(10);
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

  /////////////////////////////////////////setupcamera/////////////////////////////////////
  while (!Serial) {
    ;
  }
  pinMode(Led_Flash, OUTPUT);

  LINE.setToken(LINE_TOKEN);

  timer = timerBegin(0, 80, true); //timer 0, div 80Mhz
  timerAttachInterrupt(timer, &resetModule, true);
  timerAlarmWrite(timer, 20000000, false); //set time in us 15s
  timerAlarmEnable(timer); //enable interrupt

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

  if (psramFound()) {
    // FRAMESIZE_ +
    //QQVGA/160x120//QQVGA2/128x160//QCIF/176x144//HQVGA/240x176
    //QVGA/320x240//CIF/400x296//VGA/640x480//SVGA/800x600//XGA/1024x768
    //SXGA/1280x1024//UXGA/1600x1200//QXGA/2048*1536
    config.frame_size = FRAMESIZE_SXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  ////////////////////////////////////////////////
  

}

void loop() {


  /////////////////////camera///////////////////////////



  ////////////////////////////send to server/////////////////////
  // Use WiFiClient class to create TCP connections
  
  /* String delta = http_send("Req,ID");
    Serial.println(delta);*/

  /* if (delta == "CAP" ) {
     Camera_capture();
     delay(300);
     delta = http_send("Req,ID,NOCAP");
    }else {}

  */
  if (Client.connect(host, httpPort)) {
    Serial.print("Connecting to ");
    Serial.println(host);
  } else {
    Serial.println("Connection failed");
    return;
  }

  String si = http_send("req");
  Serial.println(si);
  delay(1000);
}

String http_send(String data_) {
  String url = "/suisei?data=" + data_;
  String delta;
  http.begin("http://192.168.1.83:4000" + url); //HTTP
  int httpCode = http.GET();
  if (httpCode > 0) {
    //Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      delta = payload;
      Serial.println(delta);
      if (delta == "YES") {
        Camera_capture();
        delay(200);
      }

    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  } http.end();

  return delta ;
}
void Camera_capture() {  /////////// ถ่าย
  digitalWrite(Led_Flash, HIGH);
  delay(200);
  digitalWrite(Led_Flash, LOW);
  delay(200);
  digitalWrite(Led_Flash, HIGH);
  camera_fb_t * fb = NULL;
  delay(200);
  // Take Picture with Camera
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }
  digitalWrite(Led_Flash, LOW);
  Send_line(fb->buf, fb->len);
  esp_camera_fb_return(fb);
  // Serial.println("Going to sleep now");
  // esp_deep_sleep_start();
  // Serial.println("This will never be printed");

}

void Send_line(uint8_t *image_data, size_t   image_size) {
  LINE.notifyPicture("DETECT!!", image_data, image_size);
}
