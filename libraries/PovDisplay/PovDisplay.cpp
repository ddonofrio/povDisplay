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

#include <PovDisplay.h>

PovDisplay::PovDisplay(
    int8_t height, 
    int8_t width, 
    Adafruit_NeoPixel *ledStrip,
    float radius,
    float verticalPixelSize,
    float verticalPixelBorder,
    float displayTriggerDistance
    ) 
{
    this->height = height;
    this->width = width;
    this->ledStrip = ledStrip;
    this->radius = radius;
    this->verticalPixelSize = verticalPixelSize;
    this->verticalPixelBorder = verticalPixelBorder;
    this->displayTriggerDistance = displayTriggerDistance;
    screen_0 = NULL;
    screen_1 = NULL;
    init();
}

void PovDisplay::init() {
    if (!screen_0) {
        uint8_t screen_0_aux[height * width * 3];
        uint8_t screen_1_aux[height * width * 3];
        this->screen_0 = screen_0_aux;
        this->screen_1 = screen_1_aux;
    }
    memset(screen_0, 0, sizeof screen_0);
    memset(screen_1, 0, sizeof screen_1);
    isZeroScreenActive = false;
    swapVideoBuffer();
    ledStrip->begin(); // Turn off all the leds in the strip.
    ledStrip->show(); 
    rpmSampleCount = 0;
    currentSpeed = 0;
    displayTriggerDelay = 0;
    displayPaintingDelay = 0;
    displayInterDelay = 0;
    rpm = 0;
    loopMaxTime = 0;
    loopLastTime = 0;
    isIntialized = true;
    drawingState = 0;
}

void PovDisplay::swapVideoBuffer() {
    if (isZeroScreenActive) {
        activeScreen = screen_1;
        bufferScreen = screen_0;
        isZeroScreenActive = false;
    } else {
        activeScreen = screen_0;
        bufferScreen = screen_1;
        isZeroScreenActive = true;
    }
}

void PovDisplay::screenDrawTrigger() {
    if (!isIntialized) return;
    lastTimeTriggered = micros();
    if (rpmSampleCount < RPM_SAMPLE) {
        if (rpmSampleCount == 0) {
            rpmSampleTimer = micros();
        }
        rpmSampleCount++;
    } else {
        unsigned long time = micros() - rpmSampleTimer;
        rpm = rpmSampleCount / time * 1000000 * 60;
        loopMaxTime = loopMaxTimeAux;
        currentSpeed = radius * rpm * 0.10472;
        displayTriggerDelay = 1 / currentSpeed * displayTriggerDistance;
        displayPaintingDelay = 1 / currentSpeed * verticalPixelSize;
        displayInterDelay = 1 / currentSpeed * verticalPixelBorder;
        rpmSampleCount = 0;
        loopLastTime = 0;
    }
    if (rpm >= MIN_RPM) {
        drawingState = 1;
        drawingColumn = 0;
        draw();
    }
}

void PovDisplay::loopTrigger() {
    if (drawingState) {
        draw();
        return;
    }
    if (!isIntialized) return;
    if (loopLastTime) {
        int currentTime = micros() - loopLastTime;
        if (loopMaxTimeAux < currentTime)
            loopMaxTimeAux = currentTime;
    }
    loopLastTime = micros();
}

void PovDisplay::draw() {
    cli(); // Disabling interrupts.
    long waitTime;
    while (true) {
        switch (drawingState) {
            case 1: // Waiting for configured displayTriggerDistance is traveled by led stripe to start drawing.
                waitTime = lastTimeTriggered + displayTriggerDelay - micros();
                if (waitTime >= 0) {
                    break;
                } else {
                    drawingState = 2;
                }
                //No break optimization.
            case 2: // Drawing current column
                for (uint8_t pixel = 0; pixel < height; pixel++) {
                    ledStrip->setPixelColor(pixel, activeScreen[drawingColumn * height + pixel * 3],
                        activeScreen[drawingColumn * height + pixel * 3 + 1],
                        activeScreen[drawingColumn * height + pixel * 3 + 2]
                        );
                }
                ledStrip->show();
                drawingColumn++;
                drawingState = 3;
                //No break optimization.
            case 3: // Waiting for configured displayPaintingDistance is traveled by led stripe to stop drawing.
                waitTime = lastTimeTriggered + displayPaintingDelay - micros();
                if (waitTime >= 0) {
                    break;
                }
                else {
                    drawingState = 4;
                }
                //No break optimization.
            case 4: // Paint pixel Border
                ledStrip->begin(); // Turn off all the leds in the strip.
                ledStrip->show();
                drawingState = 5;
                //No break optimization.
            case 5: // Waiting for configured Pixel Border Distance is traveled by led stripe to start drawing next column.
                waitTime = lastTimeTriggered + displayInterDelay - micros();
                if (waitTime >= 0) {
                    break;
                }
                else {
                    drawingState = 6;
                }
                //No break optimization.
            case 6: // Repeat from 2 for the next column or exit if it was the last one. 
                drawingColumn++;
                if (width > drawingColumn) {
                    drawingState = 2; // continue painting the next column.
                }
                else {
                    drawingState = 0;
                    drawingColumn = 0;
                }
                break;
        }
        if (waitTime > loopMaxTime || !drawingState) {
            break;
        } else {
            continue; // active wait
        }
    }
    loopLastTime = 0; // as we were drawing we do not want to count this loop's time.
    sei(); // Enabling interrupts again.
}

void PovDisplay::setVideoBuffer(uint8_t* screen) {
    memcpy(this->bufferScreen, screen, sizeof(this->bufferScreen));
}