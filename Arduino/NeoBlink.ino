#define USB_CFG_DEVICE_NAME     'D','i','g','i','B','l','i','n','k'
#define USB_CFG_DEVICE_NAME_LEN 9
#include <Adafruit_NeoPixel.h>
 
#define PIN 1
#define STRIPSIZE 10
#include <DigiUSB.h>
byte in = 0;
int Blue = 0;
int Red = 0;
int Green = 0;
int id=0;

int next = 0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
    strip.begin();
    strip.setBrightness(255);  // Lower brightness and save eyeballs!
    strip.show(); // Initialize all pixels to 'off'
    DigiUSB.begin();
}


void loop() {
     DigiUSB.refresh();
     if (DigiUSB.available() > 0) {
       in = 0;
       
       in = DigiUSB.read();
       if (next == 0){
         if(in == 115){
           next = 1;
           DigiUSB.println("Start");
         }
       }
       else if (next == 1){
            id = in;
            next = 2;
       }
       else if (next == 2){
            Red = in;
            next = 3;
       }
       else if (next == 3){
            Green = in;
            next = 4;
       }
       else if (next == 4){
            Blue = in;
            next = 0;            
            strip.setPixelColor(id, Red, Green, Blue);            
            strip.show();
       }
        
     }   
}


