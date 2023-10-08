/** 
 * @file led.cpp
 * @brief Implements fuctions that initialize leds and turns them on/off.
 * @author Michael Granberry
*/

#include "../inc/led.hpp"

// LED constructor
Led::Led(uint8_t led_pin)
    :LED_PIN(led_pin) {
        gpio_init(LED_PIN);
        gpio_set_dir(LED_PIN, GPIO_OUT);
}

// Turns on LED
void Led::turn_on_led() {
        gpio_put(LED_PIN, 1); // Sets GPIO pin HIGH (1)
}

// Turns off LED
void Led::turn_off_led() {
        gpio_put(LED_PIN, 0); // Sets GPIO pin LOW (0)
}

// Toggle LED
 void Led::toggle_led() {
        turn_on_led();
        sleep_ms(LED_SLEEP_TIME_mSec);
        turn_off_led();
        sleep_ms(LED_SLEEP_TIME_mSec);
 }


uint8_t Led::get_pin() {
        return LED_PIN;
}