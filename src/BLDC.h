/**
 * @file BLDC.h
 * @brief Header file for the BLDC class, which controls a Brushless DC motor.
 * 
 * This header file defines the `BLDC` class for controlling a Brushless DC (BLDC) motor
 * using PWM signals. It includes the class declaration, constructor, and methods for
 * speed control.
 */

#ifndef BLDC_H
#define BLDC_H

#include <Arduino.h>

/**
 * @class BLDC
 * @brief A class for controlling a Brushless DC (BLDC) motor using PWM.
 * 
 * The `BLDC` class provides methods to control the speed of a BLDC motor by adjusting
 * the PWM duty cycle. It handles the PWM configuration and provides an interface for
 * setting the motor speed.
 */
class BLDC {
public:
    /**
     * @brief Constructs a BLDC object and initializes the PWM channel.
     * 
     * The constructor initializes the PWM channel with the specified pin for controlling
     * the BLDC motor. It sets up the PWM frequency, resolution, and attaches the pin to
     * the PWM channel.
     * 
     * @param PWMPin The GPIO pin number connected to the PWM input of the motor driver.
     */
    BLDC(uint8_t PWMPin);
    
    void motor_init();
    /**
     * @brief Sets the speed of the BLDC motor.
     * 
     * This method adjusts the speed of the BLDC motor by setting the PWM duty cycle. The
     * speed is controlled by writing a value to the PWM channel.
     * 
     * @param motorspeed The desired speed of the motor, represented as a PWM duty cycle value.
     * 
     * @note The `motorspeed` value should be within the range defined by the PWM resolution.
     * 
     * @warning Ensure that the PWM frequency and resolution match the motor driver's specifications.
     */
    void speedcontrol(uint16_t motorspeed);

private:
    /** @brief PWM frequency for motor control in Hz. */
    const int PWMFreq = 20000;  ///< 20 kHz frequency.
    
    /** @brief PWM channel number used for motor control. */
    const int PWMChannel = 0;   ///< Channel 0 for PWM.
    
    /** @brief PWM resolution in bits. */
    const int PWMResolution = 10; ///< 11-bit resolution for PWM.
};

#endif //!BLDC_H
