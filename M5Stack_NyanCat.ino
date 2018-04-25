#include <M5Stack.h>
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

extern uint8_t n1_img[];
extern uint8_t n2_img[];
extern uint8_t n3_img[];
extern uint8_t n4_img[];
int anime = 0;

void setup(void) {
  M5.begin();
  M5.Lcd.setBrightness(200);
  
  file = new AudioFileSourceSD("/NyanCat.mp3");
  id3 = new AudioFileSourceID3(file);
  out = new AudioOutputI2S(0, 1); // Output to builtInDAC
  out->SetOutputModeMono(true);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);
  xTaskCreatePinnedToCore(task1, "task1", 4096, NULL, 2, NULL, 0);
}

void loop() {
  M5.update();
  if(anime == 0){
    M5.Lcd.drawJpg(n1_img, 6803, 10, 0);
  }else if(anime == 1){
    M5.Lcd.drawJpg(n2_img, 6764, 10, 0);
  }else if(anime == 2){
    M5.Lcd.drawJpg(n3_img, 7002, 10, 0);
  }else if(anime == 3){
    M5.Lcd.drawJpg(n4_img, 7051, 10, 0);
  }
  anime++;

  if(anime >= 4){
    anime = 0;
  }
}

void task1(void * pvParameters) {
  for(;;) {
    if (mp3->isRunning()) {
      if (!mp3->loop()) mp3->stop();
    } else {
      Serial.printf("MP3 done\n");
      delay(1000);
    }
  }
}
