/** 
 * @file led.hpp
 * @brief Provides fuctions that control LEDs.
 * @author Michael Granberry
*/

#ifndef LED_HPP
#define LED_HPP

#include "pico/stdlib.h"
#include "pico/multicore.h"

class Led {
    public:
        
        /**
         * @brief Constructor for LED
         * @param led_pin selects a GPIO pin for LED
        */
        Led(uint8_t led_pin);

        /**
         * @brief Turns on LED
        */
        void turn_on_led();

        /**
         * @brief Turns off LED
        */
        void turn_off_led();

        /**
         * @brief Toggles LEDs
        */
        void toggle_led();

        /**
         * @brief Gets the gpio number
        */
        uint8_t get_pin();

        static const uint8_t ALERT_SLEEP_mSec = 62;
    private:
        const uint8_t LED_PIN;
        const uint16_t LED_SLEEP_TIME_mSec = 125;
       
};
#endif