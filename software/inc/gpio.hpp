#ifndef GPIO_HPP
#define GPIO_HPP

#include "pico/stdlib.h"
#include "hardware/i2c.h"

class Gpio {
    public:
        Gpio();
        static const uint8_t number_of_buttons = 7;
        static const uint8_t BTN0 = 16;
        static const uint8_t BTN1 = 17;
        static const uint8_t BTN2 = 18;
        static const uint8_t BTN3 = 19;
        static const uint8_t BTN4 = 20;
        static const uint8_t BTN5 = 21;
        static const uint8_t BTN6 = 22;

        static const uint8_t number_of_leds = 2;
        static const uint8_t LED0 = 15;
        static const uint8_t LED1 = 14;

        static const uint8_t SCl = 5;
        static const uint8_t SDL = 4;

        static const uint8_t ALERT = 13;
        
    private:
    

};

#endif