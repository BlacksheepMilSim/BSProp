// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void theaterChase(uint32_t c, uint8_t waitNeo) {

  static long theaterTime;
  static int theaterCount;
  static int theaterSwitch;

  if((long)(theaterTime-millis())<=0){      
    if(theaterCount>12){
      theaterCount=0; 
    }    
    if(theaterSwitch==0){
      for (int i=1; i < strip.numPixels(); i=i+12) {
        strip.setPixelColor(theaterCount+i, c);    //turn every third pixel on
      }
      strip.show();
      theaterTime=millis()+(long)waitNeo;
    } 
    else {
      for (int i=0; i < strip.numPixels(); i=i+12) {
        strip.setPixelColor(theaterCount+i, 0);        //turn every third pixel off
      }
      theaterCount++;
    }
    theaterSwitch=!theaterSwitch;
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

