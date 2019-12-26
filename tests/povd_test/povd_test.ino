
#include <Adafruit_NeoPixel.h>
#include <ESP8266WebServer.h>

#include <PovDisplay.h>
#include <PovDWebServer.h>


PovDisplay* display;
Adafruit_NeoPixel* ledStrip;
ESP8266WebServer *ESPWebServer;

#define DISPLAY_PIN 0
#define DISPLAY_HEIGHT 16
#define DISPLAY_WIDTH 32
#define DISPLAY_RADIUS 0.1
#define DISPLAY_PIXEL_SIZE 0.001
#define DISPLAY_PIXEL_BORDER 0.005
#define DISPLAY_TRIGGER_DISTANCE 0.01

void setup() {

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

  Serial.begin(115200);
  Serial.print("\n\n\n\n\n\n\n\n\n\n");
  Serial.println("Starting program...");
  ledStrip = new Adafruit_NeoPixel(DISPLAY_HEIGHT, DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
  display = new PovDisplay(DISPLAY_HEIGHT, DISPLAY_WIDTH, ledStrip, DISPLAY_RADIUS, 
    DISPLAY_PIXEL_SIZE, DISPLAY_PIXEL_BORDER, DISPLAY_TRIGGER_DISTANCE);
  ESPWebServer = new ESP8266WebServer(80);

  Serial.println("Setup complete");
}

void loop() {
  // put your main code here, to run repeatedly:
  display->loopTrigger();
}
