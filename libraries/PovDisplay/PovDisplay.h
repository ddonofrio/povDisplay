/*--------------------------------------------------------------------
  This file is part of the PovDisplay library.

  PovDisplay is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  PovDisplay library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with PovDisplay library.  If not, see
  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

#ifndef _ADAFRUIT_NEOMATRIX_H_
    #include <Adafruit_NeoPixel.h>
#endif

#ifndef _POV_DISPLAY_H_
#define _POV_DISPLAY_H_

constexpr auto RPM_SAMPLE = 60;
constexpr auto MIN_RPM = 60;

class PovDisplay {

 public:
     PovDisplay(
         int8_t height,
         int8_t width,
         Adafruit_NeoPixel* ledStrip,
         float radius,
         float verticalPixelSize,
         float verticalPixelBorder,
         float displayTriggerDistance
     );
     void setVideoBuffer(uint8_t* screen);
     void swapVideoBuffer();
     void screenDrawTrigger();
     void loopTrigger();

 private:
     float verticalPixelSize;
     float verticalPixelBorder;
     float displayTriggerDistance;
     uint8_t width;
     uint8_t height;
     Adafruit_NeoPixel *ledStrip;
     uint8_t *screen_0;
     uint8_t* screen_1;
     uint8_t* activeScreen;
     uint8_t* bufferScreen;
     bool isZeroScreenActive;
     bool isIntialized;
     float radius;
     float currentSpeed;
     unsigned long displayTriggerDelay;
     unsigned long displayPaintingDelay;
     unsigned long displayInterDelay;
     unsigned long lastTimeTriggered;
     unsigned int rpm;
     unsigned int rpmSampleCount;
     unsigned long rpmSampleTimer;
     unsigned long loopMaxTime;
     unsigned long loopMaxTimeAux;
     unsigned long loopLastTime;
     unsigned int fps;
     uint8_t drawingState;
     uint8_t drawingColumn;
     void draw();
     void init();
};

#endif // _POV_DISPLAY_H_
