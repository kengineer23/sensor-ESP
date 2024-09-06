/**
 * @file BLDC.cpp
 * @brief Implementation of the BLDC class for controlling a BLDC motor.
 * 
 * This file contains the implementation of the `BLDC` class, which provides methods for
 * controlling the speed of a Brushless DC (BLDC) motor using PWM signals.
 */

#include "BLDC.h"

/**
 * @brief Constructs a BLDC object and initializes the PWM channel.
 * 
 * This constructor initializes the PWM channel used to control the BLDC motor. It sets up
 * the PWM frequency, resolution, and attaches the PWM pin to the specified channel.
 * 
 * @param PWMPin The GPIO pin number connected to the PWM input of the motor driver.
 */
BLDC::BLDC(uint8_t PWMPin) {
    ledcSetup(PWMChannel, PWMFreq, PWMResolution);  ///< Set up the PWM properties: channel, frequency, and resolution.
    ledcAttachPin(PWMPin, PWMChannel);               ///< Attach the PWM pin to the specified PWM channel.
}

void BLDC::motor_init(){
    ledcWrite(PWMChannel, 1020);
}
/**
 * @brief Sets the speed of the BLDC motor.
 * 
 * This method sets the speed of the BLDC motor by adjusting the PWM duty cycle. The speed
 * is controlled by writing a value to the PWM channel.
 * 
 * @param motorspeed The desired speed of the motor, represented as a PWM duty cycle value.
 * 
 * @note The value of `motorspeed` should be within the range defined by the PWM resolution.
 * 
 * @warning Ensure that the PWM frequency and resolution are configured correctly to match
 *          the motor driver's specifications.
 */

void BLDC::speedcontrol(uint16_t motorspeed) {
    ledcWrite(PWMChannel, motorspeed);   ///< Write the speed value to the PWM channel to control the motor speed.
    Serial.println("Motor speed changed"); ///< Print a message to the serial monitor indicating that the motor speed has been changed.
}
