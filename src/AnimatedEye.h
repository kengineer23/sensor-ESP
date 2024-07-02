#ifndef ANIMATED_EYE_H
#define ANIMATED_EYE_H

#include "TFT_eSPI.h"

class AnimatedEye{
    public:
        AnimatedEye(int x, int y);  //Constructor

        void init();
        void animate();
        void startup();
    
    private:
    TFT_eSPI tft;   //TFT display object

    int eyeX, eyeY; //top-left corner coordinates of a bounding box
    int pupilX, pupilY;

    const int eyewidth = 160;
    const int eyeheight = 120;
    const int irisradius = 30;
    const int pupilradius = 10;
    const uint16_t irisColor = TFT_BLUE;
    const uint16_t pupilColor = TFT_BLACK;
    const uint16_t scleraColor = TFT_WHITE;

    void drawEye(int irisX, int irisY);
};

#endif // !ANIMATED_EYE_H