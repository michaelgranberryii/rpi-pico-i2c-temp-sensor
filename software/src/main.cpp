#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "inc/gpio.hpp"
#include "inc/menu.hpp"
#include "inc/push_button.hpp"
#include "inc/led.hpp"
#include "inc/temp_senor.hpp"
#include <iostream>

bool TempSensor::alert = false;
uint PushButton::db_counter = 0;
int8_t PushButton::button_value = -1;
    PushButton push_buttons[Gpio::number_of_buttons] {
    PushButton(Gpio::BTN0),
    PushButton(Gpio::BTN1),
    PushButton(Gpio::BTN2),
    PushButton(Gpio::BTN3),
    PushButton(Gpio::BTN4),
    PushButton(Gpio::BTN5),
    PushButton(Gpio::BTN6),
};

bool repeating_timer_callback(struct repeating_timer *t) {
    PushButton::db_counter++;
    return PushButton::db_counter;
}

void button_callback(uint gpio, uint32_t event) {
    if (gpio == Gpio::ALERT) {
        TempSensor::alert = true;
    } else {
        if (PushButton::db_counter >= PushButton::DEBOUNCE_TIME) {
            PushButton::button_value = gpio-Gpio::BTN0;
            PushButton::db_counter = 0;
        }
    }
}

void core1() {
    Led alert_led(Gpio::LED1);
    while (true)
    {
        if(PushButton::button_value == 5) {
            TempSensor::alert = false;
            PushButton::button_value = -1;
        }
        std::cout << "\r"; // I have to add this, or else the code below isn't executed. It's really weird. 
        if(TempSensor::alert) {            
            alert_led.turn_on_led();
            sleep_ms(Led::ALERT_SLEEP_mSec);
            alert_led.turn_off_led();
            sleep_ms(Led::ALERT_SLEEP_mSec);

        }
        tight_loop_contents();
    }
}

int main() {
    stdio_init_all();
    int data_freq = 400000;
    TempSensor temp_sensor(i2c0, Gpio::SCl, Gpio::SDL, data_freq);
    temp_sensor.write_config_reg(0,0,0);
    Menu menu(temp_sensor);
    multicore_launch_core1(core1);
    gpio_set_irq_enabled_with_callback(Gpio::BTN0, GPIO_IRQ_EDGE_RISE, true, &button_callback);
    gpio_set_irq_enabled(Gpio::BTN1, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(Gpio::BTN2, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(Gpio::BTN3, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(Gpio::BTN4, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(Gpio::BTN5, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(Gpio::BTN6, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(Gpio::ALERT, GPIO_IRQ_EDGE_FALL, true);
    struct repeating_timer timer;
    add_repeating_timer_ms(1, repeating_timer_callback, nullptr, &timer);
    sleep_ms(1000);

    while (true)
    {
        menu.select_main_menu(PushButton::button_value);
        sleep_ms(500);
        tight_loop_contents();
    }
    

    return 0;
}