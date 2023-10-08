/** 
 * @file push_button.cpp
 * @brief Implements fuctions that initialize, debounce, and poll the push button.
 * @author Michael Granberry
*/

#include "../inc/push_button.hpp"

// Constructor for push button
PushButton::PushButton(uint8_t btn_pin)
    :BTN_PIN(btn_pin) {
        gpio_init(BTN_PIN);
        gpio_set_dir(BTN_PIN, GPIO_IN);
        gpio_pull_down(BTN_PIN);
    }


 uint8_t PushButton::get_pin() {
    return BTN_PIN;
}