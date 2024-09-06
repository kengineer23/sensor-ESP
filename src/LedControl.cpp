/**
 * @file LedControl.cpp
 * @brief Implementation of the LedControl class for managing RGB LED colors.
 * 
 * This file contains the implementation of the `LedControl` class, which provides methods
 * for changing the color of an RGB LED by controlling its red, green, and blue components.
 */

#include "LedControl.h"

/**
 * @brief Constructs a LedControl object.
 * 
 * The constructor initializes the `LedControl` object. It does not take any parameters
 * or perform any actions during construction.
 */
LedControl::LedControl() {}

/**
 * @brief Changes the color of the RGB LED.
 * 
 * This method sets the color of the RGB LED by adjusting the PWM values for the red, green,
 * and blue color channels. The `analogWrite` function is used to control the brightness of
 * each color component.
 * 
 * @param red The brightness level of the red color component (0-255).
 * @param green The brightness level of the green color component (0-255).
 * @param blue The brightness level of the blue color component (0-255).
 */
void LedControl::setpins() {
    pinMode(redpin, OUTPUT);   ///< Set the red color pin as an output.
    pinMode(greenpin, OUTPUT); ///< Set the green color pin as an output.
    pinMode(bluepin, OUTPUT);  ///< Set the blue color pin as an output.
    Serial.println("LED pins set"); ///< Output message indicating pin setup.

    analogWrite(redpin, 0);
    analogWrite(greenpin, 0);
    analogWrite(bluepin, 0);
}

void LedControl::changecolor(uint8_t red, uint8_t green, uint8_t blue) {
    analogWrite(redpin, red);   ///< Set the red color component brightness.
    analogWrite(greenpin, green); ///< Set the green color component brightness.
    analogWrite(bluepin, blue);  ///< Set the blue color component brightness.
    Serial.println("LED color changed"); ///< Output message indicating color change.
}
