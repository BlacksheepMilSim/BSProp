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

//Theatre-style crawling lights.
void theaterChase(uint32_t c, byte wait) {
  for (int j=0; j<1; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 12; q++) {
      for (int i=1;i < strip.numPixels(); i=i+12) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(NEO_TIME);
     
      for (int i=0; i < strip.numPixels(); i=i+12) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
