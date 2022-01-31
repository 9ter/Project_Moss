#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(5, 4);

const char* ssid = "";
const char* password = "";

const char* host = "192.168.1.83";
const int httpPort = 4000;
WiFiClient Client;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup() {
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
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

  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  if (Client.connect(host, httpPort)) {
    Serial.print("Connecting to ");
    Serial.println(host);
  } else {
    Serial.println("Connection failed");
    return;
  }

}//end setup
uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}
void loop() {
  delay(1000);
  // Use WiFiClient class to create TCP connections


  /*  String delta = client_send("Hu_tao,Ayaka");
    Serial.println(delta);*/



}

String client_send(String data) {
  String delta;
  String url = "/suisei?data=" + data;
  // This will send the request to the server
  //Client.print(String("GET ") + url +" HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  Client.print("GET ");
  Client.print(url);
  Client.println(" HTTP/1.1");
  Client.print("Host: ");
  Client.println(host);
  Client.println("Connection: close");
  Client.println();
  delay(500);

  // Read all the lines of the reply from server and print them to Serial
  while (Client.available()) {
    String line = Client.readStringUntil('\r');
    Serial.println(line);
    delta = line;
  }

  return delta;

}
