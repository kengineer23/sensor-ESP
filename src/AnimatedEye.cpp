#include "AnimatedEye.h"
//#include <SPI.h>  // Include any necessary libraries

AnimatedEye::AnimatedEye(int x, int y)
  : eyeX(x), eyeY(y) {
  // Randomize initial pupil position within the iris bounds
  pupilX = random(eyeX + irisradius, eyeX + eyewidth - irisradius);
  pupilY = random(eyeY + irisradius, eyeY + eyeheight - irisradius);
}

void AnimatedEye::init() {
  tft.setRotation(1); // Set rotation if needed
  tft.fillScreen(TFT_BLACK);
  drawEye(pupilX, pupilY); // Draw iris and pupil
}

void AnimatedEye::animate() {
  // Randomize new pupil position within the iris bounds
  pupilX = random(eyeX + irisradius, eyeX + eyewidth - irisradius);
  pupilY = random(eyeY + irisradius, eyeY + eyeheight - irisradius);
  drawEye(pupilX, pupilY);
  delay(500); // Adjust delay as needed
}

void AnimatedEye::startup(){
    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10,10);
    tft.setTextColor(TFT_WHITE);
    tft.print("Hello!");
    tft.setCursor(10,40);
    tft.print("I am ISAAC!");
}

void AnimatedEye::drawEye(int irisX, int irisY) {
  tft.fillEllipse(eyeX + eyewidth / 2, eyeY + eyeheight / 2, eyewidth / 2, eyeheight / 2, scleraColor);
  tft.fillCircle(irisX, irisY, irisradius, irisColor);
  tft.fillCircle(irisX, irisY, pupilradius, pupilColor);
}
