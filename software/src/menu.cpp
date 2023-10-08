#include "inc/menu.hpp"
#include "inc/temp_senor.hpp"

Menu::Menu(TempSensor temp_sensor) 
    :TEMP_SENSOR(temp_sensor){

}

void Menu::print_main_menu() {
    printf("\033[H\033[J");
    printf("Main Menu\n");
    printf("[0] Scan I2C\n");
    printf("[1] ADC RES\n");
    printf("[2] Display ID\n");
    printf("[3] Alert Config\n");
    printf("[4] Read Temp\n");
}

void Menu::print_menu_0() {
    printf("\033[H\033[J");
    printf("Scan I2C\n");
    printf("[x] QUIT\n");
}

void Menu::print_menu_1() {
    printf("\033[H\033[J");
    printf("ADC RES\n");
    printf("[0] 9 bit or 0.5C\n");
    printf("[1] 10 bit or 0.25C\n");
    printf("[2] 11 bit or 0.125C\n");
    printf("[3] 12 bit or 0.0625C\n");
    printf("[x] Return to main\n\n");
}

void Menu::print_menu_2() {
    printf("\033[H\033[J");
    printf("Display ID\n");
    printf("[0] 0x48\n");
    printf("[1] 0x49\n");
    printf("[2] 0x4A\n");
    printf("[3] 0x4B\n");
    printf("[4] 0x4C\n");
    printf("[5] 0x4D\n");
    printf("[6] 0x4E\n");
    printf("[7] 0x4F\n");
    printf("[x] Return to main\n");
}

void Menu::print_menu_3() {
    printf("\033[H\033[J");
    printf("Alert Config\n");
    printf("[0] Set MAX Limit\n");
    printf("[1] Set MIN Limit\n");
    printf("[2] Show MAX Limit\n");
    printf("[3] Show MIN Limit\n");
    printf("[x] Return to main\n");
}

void Menu::print_menu_4() {
    printf("\033[H\033[J");
    printf("Read Temp\n");

}


void Menu::select_main_menu(int8_t &sel) {
    print_main_menu();
    printf("\r");
    switch (sel)
    {
    case 0:
        print_menu_0();
        sel = -1;
        select_menu_0_options(sel);
        sel = -1;
        break;

    case 1:
        print_menu_1();
        sel = -1;
        select_menu_1_options(sel);
        sel = -1;
        break;

    case 2:
        print_menu_2();
        sel = -1;
        select_menu_2_options(sel);
        sel = -1;
        break;

    case 3:
        print_menu_3();
        sel = -1;
        select_menu_3_options(sel);
        sel = -1;
        break;

    case 4:
        print_menu_4();
        sel = -1;
        select_menu_4_options(sel);
        sel = -1;
        break;
    }
}

void Menu::select_menu_0_options(int8_t &sel){
    TEMP_SENSOR.scan_bus();
    while (sel != 0) {
        printf("\r");
        switch (sel)
        {
        case 0:
            return;
        }
    }
}

void Menu::select_menu_1_options(int8_t &sel){
    while (sel != 4) {
        printf("\r");
        switch (sel)
        {
        case 0:
            TEMP_SENSOR.write_config_reg(0x0, 0x9F, 5);
            TEMP_SENSOR.read_config_reg_bits(0x0, 0x03, 5);
            sel = -1;
            break;

        case 1:
            TEMP_SENSOR.write_config_reg(0x1, 0x9F, 5);
            TEMP_SENSOR.read_config_reg_bits(0x1, 0x03, 5);
            sel = -1;
            break;

        case 2:
            TEMP_SENSOR.write_config_reg(0x2, 0x9F, 5);
            TEMP_SENSOR.read_config_reg_bits(0x2, 0x03, 5);
            sel = -1;
            break;

        case 3:
            TEMP_SENSOR.write_config_reg(0x3, 0x9F, 5);
            TEMP_SENSOR.read_config_reg_bits(0x3, 0x03, 5);
            sel = -1;
            break;

        case 4:
            return;

        }
    }
}

void Menu::select_menu_2_options(int8_t &sel){
    printf("\n");
    while (sel != 1) {
        char c = getchar();
        printf("\33[2K\r");
        printf("Enter value: ");
        

        switch (c)
        {
        case '0':
            TEMP_SENSOR.change_id(0x48);
            break;

        case '1':
            TEMP_SENSOR.change_id(0x49);
            break;

        case '2':
            TEMP_SENSOR.change_id(0x4A);
            break;

        case '3':
            TEMP_SENSOR.change_id(0x4B);
            break;

        case '4':
            TEMP_SENSOR.change_id(0x4C);
            break;

        case '5':

            TEMP_SENSOR.change_id(0x4D);
            break;

        case '6':
            TEMP_SENSOR.change_id(0x4D);
            break;

        case '7':
            TEMP_SENSOR.change_id(0x4F);
            break;

        case 'x':
            return;
            break;

        default:
            std::cout << "Enter a value 0 through 7\n";
            break;
        }
        
    }
}

unsigned char *Menu::readLine() {
    uint8_t u, *p;
    for (p = str, u = getchar(); u != '\r' && u != '\n' && p - str < str_size - 1; u = getchar())
        putchar(*p++ = u);
    *p = 0;
    return str;
}

void Menu::select_menu_3_options(int8_t &sel) {
    printf("\r");
    uint8_t msb;
    uint8_t lsb;
    
    while (sel != 4) {
        printf("\r");
        switch (sel)
        {
        case 0:
            // user input
            printf("\r");
            printf("\33[2K\r");
            printf("Enter MAX value with (XX.X), then press enter: \n");
            readLine();
            msb = ((str[0] - '0') * 10) + (str[1] - '0');
            lsb = (str[3] == '5') ? 0x80 : 0;
            TEMP_SENSOR.write_to_limit_set_reg(msb, lsb);
            sel = -1;
            break;

        case 1:
            //user input
            printf("\r");
            printf("\33[2K\r");
            printf("Enter MIN value with (XX.X), then press enter: \n");
            readLine();
            msb = ((str[0] - '0') * 10) + (str[1] - '0');
            lsb = (str[3] == '5') ? 0x80 : 0;
            TEMP_SENSOR.write_to_hyst_reg(msb, lsb);
            sel = -1;
            break;

        case 2:
            TEMP_SENSOR.read_limit_set_reg();
            sel = -1;
            break;

        case 3:
            TEMP_SENSOR.read_hyst_reg();
            sel = -1;
            break;

        case 4:
            return;
        }
    }
}

void Menu::select_menu_4_options(int8_t &sel) {
    while (sel != 0) {
        TEMP_SENSOR.read_temp_c();
        TEMP_SENSOR.read_temp_f();
        printf("\033[H\033[J");
        printf("   Temp C    |    Temp F   \n");
        printf("-------------+-------------\n");
        printf("   %0.4f   |    %0.4f    \n", TEMP_SENSOR.get_temp_c(), TEMP_SENSOR.get_temp_f());
        printf("\n[x] Return to main\n");
    }
}
