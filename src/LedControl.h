/**
 * @file LedControl.h
 * @brief Header file for the LedControl class.
 * 
 * This header file defines the `LedControl` class, which manages the color of an RGB LED.
 * It includes the class declaration, member variables, and method declarations for controlling
 * the LED color by adjusting the brightness levels of its red, green, and blue components.
 */

#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "stdint.h"
#include "Arduino.h"

/**
 * @class LedControl
 * @brief A class for controlling the color of an RGB LED.
 * 
 * The `LedControl` class provides methods for changing the color of an RGB LED by adjusting
 * the PWM values of the red, green, and blue color channels. It includes methods for setting
 * the brightness of each color component.
 */
class LedControl {
    public:
        /**
         * @brief Constructs a LedControl object.
         * 
         * The constructor initializes the `LedControl` object, setting up the pin assignments
         * for controlling the RGB LED.
         */
        LedControl(); ///< Constructor
        
        void setpins(); ///< Set LED pins.
        /**
         * @brief Changes the color of the RGB LED.
         * 
         * This method sets the color of the RGB LED by adjusting the PWM values for the red,
         * green, and blue color channels. The brightness of each color component is set using
         * the `analogWrite` function.
         * 
         * @param red The brightness level of the red color component (0-255).
         * @param green The brightness level of the green color component (0-255).
         * @param blue The brightness level of the blue color component (0-255).
         */
        void changecolor(uint8_t red, uint8_t green, uint8_t blue); ///< Change LED color.

    private:
        uint8_t redpin = 5;   ///< GPIO pin number connected to the red color component.
        uint8_t greenpin = 18; ///< GPIO pin number connected to the green color component.
        uint8_t bluepin = 19;  ///< GPIO pin number connected to the blue color component.
};

#endif // !LED_CONTROL_H
