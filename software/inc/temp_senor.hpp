#ifndef TEMP_SENSOR_HPP
#define TEMP_SENSOR_HPP

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/gpio.hpp"
#include "inc/led.hpp"
#include <iostream>
#include <cmath>
class TempSensor {
    public:
        TempSensor(i2c_inst_t *i2c, int scl_gpio, int sdl_gpio, int data_freqz);
        static bool alert;

        bool reserved_addr(uint8_t addr);
        uint8_t scan_bus();
        
        uint8_t read_config_reg();
        uint8_t read_config_reg_bits(uint8_t data, uint8_t mask, uint8_t shift);
        void write_config_reg(uint8_t data, uint8_t mask, uint8_t shift);

        int change_id(int addr);

        float read_temp_c();
        float read_temp_f();

        void read_limit_set_reg();
        void write_to_limit_set_reg(uint8_t msb_data, uint8_t lsb_data);
        void read_hyst_reg();
        void write_to_hyst_reg(uint8_t msb_data, uint8_t lsb_data);


        float calculate_temp(uint8_t msb, uint8_t lsb);

        float get_temp_f();
        float get_temp_c();

    private:
        float temp_f;
        float temp_c;
        const int DATA_FREQ;
        const int SCL_GPIO;
        const int SDL_GPIO;
        const int READ_TEMP_mSec = 500;
        i2c_inst_t *I2C;
        Led leds[Gpio::number_of_leds] {
            Led(Gpio::LED0),
            Led(Gpio::LED1)
        };
        uint8_t temp_addr;

};

#endif