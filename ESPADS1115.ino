
#include <Wire.h>
#include<ADS1115_lite.h>
#include <TFT_eSPI.h> 
#include "fonts.h"
#include "flmuke.h"

ADS1115_lite ads(ADS1115_DEFAULT_ADDRESS);  // 0x48 addr pin connected to GND


const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;

int backlight[5] = {10,30,60,160,220};
byte br=3;

TFT_eSPI tft = TFT_eSPI(); 


long raw = 0;


double Volts=0;
double resolution=0;

void setup() {

    pinMode(0,INPUT_PULLUP);
    pinMode(35,INPUT_PULLUP);
   
    tft.init();
    tft.setRotation(0);
    tft.setSwapBytes(true);
    tft.fillScreen(TFT_ORANGE);
    tft.pushImage(0,0,240,240,fluke);
    tft.setFreeFont(&DSEG7_Classic_Bold_34);
    tft.setTextColor(TFT_BLACK,0xA5F7);
    tft.drawString("0.145",56,58);

    ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
    ledcAttachPin(17, pwmLedChannelTFT);
    ledcWrite(pwmLedChannelTFT, backlight[br]);
    delay(10); 
  ads_config();

    

}



void loop() {

 tft.fillRect(50,56,164,40,0xA5F7);
 
 
  raw=ads_read();
  resolution=4096.00/32752.00;
  delay(200);
  float result=(raw*resolution)/1000.00;
   
  tft.drawFloat(result,3,60,58); 
 

delay(50);
}


void ads_config(){
  ads.setGain(ADS1115_REG_CONFIG_PGA_4_096V); // GAIN_ONE and resolution to ± 4.096V
  ads.setSampleRate(ADS1115_REG_CONFIG_DR_8SPS); // Set to the fastest MODE 860Samples per sec
}

int16_t ads_read(){ 
  ads.setMux(ADS1115_REG_CONFIG_MUX_SINGLE_0);  // Single mode input on AIN0 (AIN0 - GND)
  ads.triggerConversion();  // Triggered mannually 
  return ads.getConversion();  // returns int16_t value
}
