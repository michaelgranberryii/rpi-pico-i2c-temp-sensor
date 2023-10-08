/** 
 * @file push_button.hpp
 * @brief Provides fuctions for push button.
 * @author Michael Granberry
*/

#ifndef PUSHBUTTON_HPP
#define PUSHBUTTON_HPP

#include "pico/stdlib.h"
#include <iostream>

class PushButton {
    public:

        static const uint16_t DEBOUNCE_TIME = 150;
        static uint db_counter;
        static int8_t button_value;

        /**
         * @brief Constructor for push button
         * @param btn_pin selects a gpio pin for push button
        */
        PushButton(uint8_t btn_pin);

        /**
         * @brief Returns the button GPIO pin number
        */
        uint8_t get_pin();

    private:
        const uint8_t BTN_PIN;
};

#endif