# Cloud

Repository containing source code for a DIY cloud lamp using an arduino, Adafruit NeoPixel LED strips, and an Adafruit Soudboard.

## Hardware
 - [Adafruit NePixel Strip](https://www.adafruit.com/product/1138?gad_source=1&gclid=CjwKCAjw48-vBhBbEiwAzqrZVKkQDqYc6kzDuyQdu53pMRFMe_L1XK2hu9rpzA8hVS0tBXyKfn_SThoCbesQAvD_BwE)
 - [Adafruit Sound Board](https://www.adafruit.com/product/2220)

## NEOPIXEL BEST PRACTICES
 Skipping these may work OK on your workbench but can fail in the field
 - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
 - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
 - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
 - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS connect GROUND (-) first, then +, then data.
 - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip, a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.

## Sound FX
Sound effects are located in the FX directory and should be saved to the Adafruit soundboard.

## Cloud Modes

The cloud uses a state machine and has several modes it can be placed in.

### OFF

No lights or sound.

### ON

Lights are on.

### Lightning

Randomly flashes LED lights in one of four distinct patterns to emulate lightning and plays thunder sound effects on the board.

### Acid

Cycles hues through the colorstrip.

### Fade

Fades colors in and out.

### Twinkle

Rappidly switches random lights on in off to make a dancing light pattern.

### Breathe

Slowly dims/brightens the cloud.

### Asymetric Breathe

Similar to breathe, but timing is offset for different clouds.

