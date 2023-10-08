#ifndef MENU_HPP
#define MENU_HPP

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/temp_senor.hpp"
#include <iostream>
#include <cstring>
class Menu {
    public:
        Menu(TempSensor temp_sensor);

        void print_main_menu();
        void print_menu_0();
        void print_menu_1();
        void print_menu_2();
        void print_menu_3();
        void print_menu_4();

        void select_main_menu(int8_t &sel);
        void select_menu_0_options(int8_t &sel);
        void select_menu_1_options(int8_t &sel);
        void select_menu_2_options(int8_t &sel);
        void select_menu_3_options(int8_t &sel);
        void select_menu_4_options(int8_t &sel);
        void select_menu_5_options(int8_t &sel);

        unsigned char *readLine();

    private:
        static const uint8_t str_size = 5;
        TempSensor TEMP_SENSOR;
        uint8_t str[str_size];
        
        

};

#endif