//#include <string.h>
//#include <LiquidCrystal_I2C.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncTCPbuffer.h>


#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

#include <SD.h>
#include <SPI.h>

#define SDCARD_ERROR_PIN 6
#define SERVER_ON 5 //to indicate whether the server is on or off
#define WIFI_PIN 7 //to indicate whether the wifi is connected or not
#define BAUDRATE 115200

String ssid = "verma's", pwd = "Strong@pass2";
File file;
AsyncWebServer server(80);

const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><ti2tle>ESP Console</title><style>.container{margin: 0;padding: 20vh;}
input{font-size: 20px;font-family: Times New Roman;font-style: bold;font-weight: bold;}
input[type=text]{width: 100%;height: 300px;background-color: black;color: green;border-style: solid;border-color: grey;border-width: 10px;border-radius: 4px;}
input[type=submit]{width: 100px;height: 50px;box-shadow: 5px 5px 5px grey;border-style: none;}
input[type=submit]:hover{width: 95px;height: 47px;box-shadow: 5px 5px 5px grey;}
</style><body><div class="container"><form>
<input style="" type="text" id="console" name="console"><br><br>
<input type="submit" value="run"></form><br><form>
<input  style="float: right; margin: 10px; margin-top: 0;" type="submit">&emsp; &emsp;<input style="float: right;  margin-top: 12px;" type="color" id="color" name="color" value="#ff0000">
</form></div></body><footer>
Made with &#128150; by Rahul!</footer></html>
)rawliteral";


void setup(){
  Serial.begin(BAUDRATE);
  setup_wifi(ssid, pwd, WIFI_PIN);
  

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/html", html);
  String inputmsg;
    if(request-> hasParam("console")){
      inputmsg = request->getParam("console") -> value();
      inputmsg = inputmsg.c_str();
      if((String)inputmsg[0] == "#"){
        if(inputmsg.substring(1, 8) == "ssid = "){
          ssid = inputmsg.substring(8, inputmsg.length() - 1); delay(1000);
          setup_wifi(ssid, pwd, WIFI_PIN);
        }
        if(inputmsg.substring(1, 7) == "pwd = "){
          pwd = inputmsg.substring(7, inputmsg.length() - 1); delay(1000);
          setup_wifi(ssid, pwd, WIFI_PIN);
        }
      }
      else if(inputmsg){
        if(inputmsg != inputmsg){
          if(!write_on_SDcard(inputmsg)){
            write_error_log_on_SDcard("SD card");
          }
        }
      }
    }
    else if(request->hasParam("color")){
      inputmsg = request->getParam("color") -> value();
      inputmsg = inputmsg.c_str();
      if(inputmsg){
        if(inputmsg != inputmsg){
          if(!write_on_SDcard(inputmsg)){
            write_error_log_on_SDcard("SD card");
          }
        }
      }
    }
    else{
      Serial.println("No message sent.");
    }
  });
  server.onNotFound(handle_NotFound);
  server.on("/help", handle_NotFound);
  server.begin();
}

void loop(){}

void setup_wifi(String ssid, String pwd, int WIFI_STATUS){
  pinMode(SERVER_ON, OUTPUT);
  WiFi.begin(ssid, pwd);
  while(WiFi.status() != WL_CONNECTED){
    digitalWrite(WIFI_STATUS, HIGH);
    delay(500);
  }
  Serial.print(WiFi.localIP());
  digitalWrite(WIFI_STATUS, LOW);
}


bool write_on_SDcard(String inputmsg, String device){
  file = SD.open("commands.txt", FILE_WRITE);
  if(file){
    file.print(device);file.print(inputmsg);file.println(",");
    file.close();
    return true;
  }
  else{
    return false;
  }
}

void write_error_log_on_SDcard(String error){
  file = SD.open("commands.txt", FILE_WRITE);
  if(file){
    file.println(error);
    file.close();
  }
  else{
    pinMode(SDCARD_ERROR_PIN, OUTPUT);
    digitalWrite(SDCARD_ERROR_PIN, HIGH);
  }
}

void handle_NotFound(AsyncWebServerRequest *request){
  String html = "<!DOCTYPE html><html>";
  html += "<body style=\"text-align: center; margin: auto;font-family:Comic Sans MS;\">";
  html += "<h1 style=\"font-size:200px;\">4&#128557;4<p style=\"font-size:30px;\">";
  html += "Oops! unable to found web page</p></h1></body></html>";
  request->send(404, "text/html", html);
}

void help(AsyncWebServerRequest *request){
  String html;
  request->send(404, "text/html", "You will get help here.");
}
