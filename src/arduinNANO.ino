#include <SPI.h>
#include <SD.h>

#include <FastLED.h>

#define BAUDRATE 115200
#define NUM_LEDS 59
#define LED_DATA_PIN 6
#define SDCARD_ERROR_PIN 5

CRGB leds[NUM_LEDS];
File file;

int semaphore = 2; //if negative block else work
bool status = true;
String error = "error";
String data;
unsigned long int fileptr;

void setup() {
  Serial.begin(BAUDRATE);

  pinMode(SDCARD_ERROR_PIN, OUTPUT);

  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);

  //initialize sd card
  
}

void loop() {
  //1. read SD card and get some data
  //2. prepare data
  //3. perform operation on data
  if(read_SDcard()){
    int info = prepare(data);
    status = perform_operations_with(info, status);
  }
}

bool read_SDcard(){
  file = SD.open("commands.txt");
  if(file){
    file.seek(fileptr);
    data = file.readString();
    fileptr = file.position();
    file.close();
    return true;
  }
  else{
    write_error_log_on_SDcard("SD card");
    return false;
  }
}

int prepare(String data){
  return 0;
}

bool perform_operations_with(int info, bool status){
  if(!status){
    if(semaphore > 0){
      semaphore = semaphore - 1;
      status = perform_operations_with(info, status);
    }
    return false;
  }
  else{
    semaphore = 2;
    Serial.println("performing operations");
    return true;
  }
}

void write_error_log_on_SDcard(String error){
  file = SD.open("commands.txt", FILE_WRITE);
  if(file){
    file.println(error);
    file.close();
    digitalWrite(SDCARD_ERROR_PIN, LOW);
  }
  else{
    digitalWrite(SDCARD_ERROR_PIN, HIGH);
  }
}

int *hex_to_RGB(String hex){
  int color[3];
  color[0] = atoi(hex[1]) + atoi(hex[2]) + 51;
  color[1] = atoi(hex[3]) + atoi(hex[4]) + 51;
  color[2] = atoi(hex[5]) + atoi(hex[6]) + 51;

  return color;
}
