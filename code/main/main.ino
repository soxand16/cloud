// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_Soundboard.h"

// Neopixel Setup -- runs once at startup ----------------------------------

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    12

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  19

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// SFX Board Serial Setup -- runs once at startup --------------------------

// Choose any two pins that can be used with SoftwareSerial to RX & TX
#define SFX_TX 11
#define SFX_RX 10

// Connect to the RST pin on the Sound Board
#define SFX_RST 9

// You can also monitor the ACT pin for when audio is playing!

// we'll be using software serial
SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);

// pass the software serial to Adafruit_soundboard, the second
// argument is the debug port (not used really) and the third 
// arg is the reset pin
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);
// can also try hardware serial with
// Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial1, NULL, SFX_RST);

#define BTN 2

// Cloud start/stops
#define CLD0_SRT 0
#define CLD1_SRT 6
#define CLD2_SRT 11
#define CLD3_SRT 15

enum MODE
{
  OFF,
  ON,
  LIGHTNING,
  ACID,
  FADE,
  TWINKLE,
  BREATHE,
  ASYM_BREATHE
};

enum LIGHTNING_TYPE
{
  ROLLING,
  TRAVELING,
  ALTERNATING,
  GROUP
};

volatile unsigned int nextMode = 0;
unsigned char mode = ASYM_BREATHE;
unsigned char lightning;
int cloud_config; 
unsigned long next_lightning; // time for next lightning



// setup() function -- runs once at startup --------------------------------

void setup()
{
  // softwareserial at 9600 baud
  ss.begin(9600);

  // Initialize NeoPixel
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  // Initialize button pin
  pinMode(BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN), changeState, RISING);
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop()
{
  switch(mode)
  {  
    case OFF:
      // If button pressed
      if(nextMode)
      {
        strip.clear();
        strip.show();
        mode = ON;
        nextMode = 0;
      }
      break;
    case ON:
      // If button pressed
      strip.fill(strip.Color(127, 127, 127));
      strip.show();
      if(nextMode)
      {
        strip.clear();
        strip.show();
        mode = LIGHTNING;
        next_lightning = millis();
        nextMode = 0;
      }
      break;
    case LIGHTNING:
      if(next_lightning < millis())
      {
        sfx.playTrack(random(7));
        lightning = random(4);
        switch(lightning)
        {
          case ROLLING:
            rollingLightning();
            break;
          case TRAVELING:
            travelingLightning();
            break;
          case ALTERNATING:
            alternatingLightning();
            break;
          case GROUP:
            groupLightning();
            break;
        }
        next_lightning = millis() + random(7000, 10000);
      }
      // If button pressed
      if(nextMode)
      {
        strip.clear();
        strip.show();
        mode = ACID;
        nextMode = 0;
      }
      break;
    case ACID:
      acid();
      // If button pressed
      if(nextMode)
      {
        strip.clear();
        strip.show();
        mode = FADE;
        nextMode = 0;
      }
      break;
    case FADE:
      fade();
      // If button pressed
      if(nextMode)
      {
        strip.clear();
        strip.show();
        mode = TWINKLE;
        nextMode = 0;
      }
      break;
    case TWINKLE:
      twinkle();
      // If button pressed
      if(nextMode)
      {
        strip.clear();
        strip.show();
        mode = BREATHE;
        nextMode = 0;
      }
      break;
    case BREATHE:
      breathe();
      // If button pressed
      if(nextMode)
      {
        strip.clear();
        strip.show();
        mode = ASYM_BREATHE;
        nextMode = 0;
      }
      break;
    case ASYM_BREATHE:
      asym_breathe();
      // If button pressed
      if(nextMode)
      {
        strip.clear();
        strip.show();
        mode = OFF;
        nextMode = 0;
      }
      break;
  }   

//  acid(10);
}

void changeState()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
    nextMode = 1;
  }
  last_interrupt_time = interrupt_time;
}

void fillCloud(int cloud, uint32_t color)
{
  switch(cloud)
  {
    case 0:
      strip.fill(color, CLD0_SRT, CLD1_SRT - CLD0_SRT);
      break;
    case 1:
      strip.fill(color, CLD1_SRT, CLD2_SRT - CLD1_SRT);
      break;
    case 2:
      strip.fill(color, CLD2_SRT, CLD3_SRT - CLD2_SRT);
      break;
    case 3:
      strip.fill(color, CLD3_SRT, strip.numPixels() - CLD3_SRT);
    break;
  }
}

void fillClouds(int clouds, uint32_t color)
{
  if(clouds & 0b0001)
  {
    fillCloud(0, color);
  }
  if(clouds & 0b0010)
  {
    fillCloud(1, color);
  }
  if(clouds & 0b0100)
  {
    fillCloud(2, color);
  }
  if(clouds & 0b1000)
  {
    fillCloud(3, color);
  }
}

void rollingLightning()
{
  int a = 2 * random(2) - 1;
  int pix = random(strip.numPixels());
  for(int i=0; i<9; i++)
  {
    strip.setPixelColor((pix - 3*a) % strip.numPixels(), strip.Color(0, 0, 0));
    strip.setPixelColor(pix, strip.Color(127, 127, 127));
    strip.show();
    pix += a;
    if(pix < 0 or pix > strip.numPixels() - 1)
    {
      a *= -1;
      pix += 2*a;
    }
    waitUntil(millis() + random(10, 150));
    if(nextMode)
    {
      return;
    }
  }
  strip.clear();
  strip.show();
}

void travelingLightning()
{
  int cloud = random(4);
  int num_flashes = random(2, 5);
  for(int i=0; i<num_flashes; i++)
  {
    fillCloud(cloud, strip.Color(127, 127, 127));
    strip.show();
    waitUntil(millis() + random(200, 600));
    if(nextMode)
    {
      strip.clear();
      strip.show();
      return;
    }
    cloud = (cloud + random(1, 4)) % 4;
    strip.clear();
  }
  strip.show();
}

void alternatingLightning()
{
  int clouds = random(1, 16);
  int num_flashes = random(2, 5);
  for(int i=0; i<num_flashes; i++)
  {
    if(i % 2 == 0)
    {
      fillClouds(clouds, strip.Color(127, 127, 127));
    }
    else
    {
      fillClouds(clouds ^ 0b1111, strip.Color(127, 127, 127));
    }
    strip.show();
    waitUntil(millis() + random(100, 300));
    strip.clear();
    strip.show();
    waitUntil(millis() + random(100, 300));
    if(nextMode)
    {
      return;
    }
  }
}

void groupLightning()
{
  int clouds = random(1, 16);
  int num_flashes = random(2, 5);
  for(int i=0; i<num_flashes; i++)
  {
    fillClouds(clouds, strip.Color(127, 127, 127));
    strip.show();
    waitUntil(millis() + random(100, 300));
    strip.clear();
    strip.show();
    waitUntil(millis() + random(100, 300));
    if(nextMode)
    {
      return;
    }
  }
}

void acid()
{
  // Hue of first pixel runs 1 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 65536. Adding 256 to firstPixelHue each time
  // means we'll make 65536/256 = 256 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 65536; firstPixelHue += 256)
  {
    for(int i=0; i<strip.numPixels(); i++) // For each pixel in strip...
    { 
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (20 * i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      // If button pressed
      if(nextMode)
      {
        return;
      }
    }
    strip.show(); // Update strip with new contents
    if(nextMode)
    {
      return;
    }
    delay(10);  // Pause for a moment
  }
}

void fade()
{
  for(int hue=0; hue<65536L; hue+=256)
  {
    fillCloud(0, strip.ColorHSV(hue));
    fillCloud(1, strip.gamma32(strip.ColorHSV(hue + 16634L)));
    fillCloud(2, strip.gamma32(strip.ColorHSV(hue + 33268L)));
    fillCloud(3, strip.gamma32(strip.ColorHSV(hue + 49902L)));
    strip.show();
    delay(50);
    if(nextMode)
    {
      return;
    }
  }
}

void twinkle()
{
  for(int i=0; i<3; i++)
  {
    for(int j=i; j<strip.numPixels(); j+=3)
    {
      strip.setPixelColor(j, strip.Color(127, 127, 127));
      if(nextMode)
      {
        return;
      }
    }
    strip.show();
    strip.clear();
    waitUntil(millis() + random(75, 126));
  }
 
}

void breathe()
{
  for(int i=20; i<128; i++)
  {
    strip.fill(strip.Color(i, i, i));
    strip.show();
    waitUntil(millis() + 25);
    if(nextMode)
    {
      return;
    }
  }
  for(int i=127; i>20; i--)
  {
    strip.fill(strip.Color(i, i, i));
    strip.show();
    waitUntil(millis() + 25);
    if(nextMode)
    {
      return;
    }
  }
}

void asym_breathe()
{
  for(int i=-40; i<41; i++)
  {
    fillClouds(0b0101, strip.Color(64 + i, 64 + i, 64 + i));
    fillClouds(0b1010, strip.Color(64 - i, 64 - i, 64 - i));
    strip.show();
    waitUntil(millis() + 25);
    if(nextMode)
    {
      return;
    }
  }
  for(int i=40; i>-41; i--)
  {
    fillClouds(0b0101, strip.Color(64 + i, 64 + i, 64 + i));
    fillClouds(0b1010, strip.Color(64 - i, 64 - i, 64 - i));
    strip.show();
    waitUntil(millis() + 25);
    if(nextMode)
    {
      return;
    }
  }
}

void waitUntil(unsigned long t)
{
  while(millis() < t)
  {
    if(nextMode)
    {
      break;
    }
  }
}
