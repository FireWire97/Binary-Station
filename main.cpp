
#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include "Ds1302.h"

#define LED_PIN 6

#define LED_COUNT 40

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Ds1302 rtc_time(7, 8, 9);
Ds1302::DateTime today;

void setup() 
{
  today.year = 2020;
  today.month = 9;
  today.day = 13;
  today.dow = 7;
  today.hour = 16;
  today.minute = 30;
  today.second = 0;

  rtc_time.init();
  //rtc_time.setDateTime(&today);
  
  //if(Serial.available())
    Serial.begin(9600);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)  
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void displayTime()
{
  uint32_t colour;
  colour = strip.Color(21, 240, 29);

  //hours
  uint8_t hour = today.hour;

  strip.clear();

  if(hour > 15)
  {
    strip.setPixelColor(8, colour);
    strip.setPixelColor(9, colour);
    hour -= 16;
  }
  if(hour > 7)
  {
    strip.setPixelColor(6, colour);
    strip.setPixelColor(7, colour);
    hour -= 8;
  }
  if(hour > 3)
  {
    strip.setPixelColor(4, colour);
    strip.setPixelColor(5, colour);
    hour -= 4;
  }
  if(hour > 1)
  {
    strip.setPixelColor(2, colour);
    strip.setPixelColor(3, colour);
    hour -= 2;
  }
  if(hour == 1)
  {
    strip.setPixelColor(0, colour);
    strip.setPixelColor(1, colour);
  }



  //minutes
  uint8_t min = today.minute;
  colour = strip.Color(16, 175, 243);

  if(min > 31)
  {
    strip.setPixelColor(26, colour);
    strip.setPixelColor(27, colour);
    min -= 32;
  }
  if(min > 15)
  {
    strip.setPixelColor(24, colour);
    strip.setPixelColor(25, colour);
    min -= 16;
  }
  if(min > 7)
  {
    strip.setPixelColor(22, colour);
    strip.setPixelColor(23, colour);
    min -= 8;
  }
  if(min > 3)
  {
    strip.setPixelColor(20, colour);
    strip.setPixelColor(21, colour);
    min -= 4;
  }
  if(min > 1)
  {
    strip.setPixelColor(18, colour);
    strip.setPixelColor(19, colour);
    min -= 2;
  }
  if(min == 1)
  {
    strip.setPixelColor(16, colour);
    strip.setPixelColor(17, colour);
  }
  

  //seconds
  uint8_t sec = today.second;
  uint8_t led_nr = sec / 8;
  colour = strip.Color(243, 24, 16);

  for(int i =0; i <= led_nr; i++)
  {
    strip.setPixelColor(32 + i, colour);
  }

  strip.setBrightness(20);
  strip.show();
}

void loop()
{
  /*
  rtc_time.getDateTime(&today);
  Serial.print(today.hour);
  Serial.println(" hour");
  Serial.print(today.minute);
  Serial.println(" minute");
  Serial.print(today.second);
  Serial.println(" second");
  */
 rtc_time.getDateTime(&today);
  delay(100);
  displayTime();

/*
  uint8_t czas = 0;
  for(int i = 0; i < 61; i++)
  {
    displayTime(czas);
    czas++;
    delay(1000);
  }
  */
  /*
  // Fill along the length of the strip in various colors...
  colorWipe(strip.Color(255,   0,   0), 50); // Red
  colorWipe(strip.Color(  0, 255,   0), 50); // Green
  colorWipe(strip.Color(  0,   0, 255), 50); // Blue

  // Do a theater marquee effect in various colors...
  theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
  theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness

  rainbow(10);             // Flowing rainbow cycle along the whole strip
  theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
  */
}