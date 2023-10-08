#include "inc/temp_senor.hpp"


TempSensor::TempSensor(i2c_inst_t *i2c, int scl_gpio, int sdl_gpio, int data_freq) 
:I2C(i2c),SCL_GPIO(scl_gpio), SDL_GPIO(sdl_gpio), DATA_FREQ(data_freq){
    i2c_init(i2c, DATA_FREQ);
    gpio_set_function(SCL_GPIO, GPIO_FUNC_I2C);
    gpio_set_function(SDL_GPIO, GPIO_FUNC_I2C);
    temp_addr = scan_bus();
    printf("temp: %x\n", temp_addr);
}


bool TempSensor::reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

uint8_t TempSensor::scan_bus() {
    uint8_t i2c_addr;
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
 
    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }
 
        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.
 
        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr))
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(I2C, addr, &rxdata, 1, false);
 
        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
        if(ret == 1) { 
            i2c_addr = addr;
        }
    }
    return i2c_addr;
    
}

void TempSensor::write_config_reg(uint8_t data, uint8_t mask, uint8_t shift) {
    uint8_t reg = read_config_reg();
    reg &= mask;
    reg |= (data << shift);
    uint8_t buff[2] = {0x1, reg};
    i2c_write_blocking(I2C, temp_addr, buff, 2, false);
}


uint8_t TempSensor::read_config_reg() {
    uint8_t buff[1];
    uint8_t cs_ptr = 0x01;
    i2c_write_blocking(I2C, temp_addr, &cs_ptr, 1, false);
    i2c_read_blocking(I2C, temp_addr, buff, 1, false);
    return buff[0];
}


uint8_t TempSensor::read_config_reg_bits(uint8_t data, uint8_t mask, uint8_t shift) {
    uint8_t reg = read_config_reg();
    uint8_t masked_data = (reg >> shift) & mask;
    if (masked_data == data) {
        for (size_t i = 0; i < 4; i++) {
            leds[0].toggle_led();
        }
    } else {
        for (size_t i = 0; i < 4; i++) {
            leds[1].toggle_led();
        }
    }
    return masked_data;
}

int TempSensor::change_id(int addr) {
    uint8_t rxdata;
    int ret;
    ret = i2c_read_blocking(I2C, addr, &rxdata, 1, false);
    if (ret == 1) {
        temp_addr = addr;
        printf("[SUCCESS] Dev ID change to %x", addr);
        for (size_t i = 0; i < 4; i++) {
            leds[0].toggle_led();
        }
    } else {
        printf("[WARNING] Could not communicate with Dev ID %x", addr);
        for (size_t i = 0; i < 4; i++) {
            leds[1].toggle_led();
        }
    }
    return 0;
}

float TempSensor::read_temp_c() {
    sleep_ms(10);
    uint8_t cs_ptr = 0x00;
    uint8_t temp_buffer[2];
    uint16_t raw_temp;
    float temp;
    i2c_write_blocking(I2C, temp_addr, &cs_ptr, 1, false);
    i2c_read_blocking(I2C, temp_addr, temp_buffer, 2, false);
    temp_c = calculate_temp(temp_buffer[0], temp_buffer[1]);
    sleep_ms(READ_TEMP_mSec);
    return temp_c;
}

float TempSensor::read_temp_f() {
    temp_f = (temp_c * 9/5) + 32;
    return temp_f;
}

float TempSensor::get_temp_c() {
    return temp_c;
}

float TempSensor::get_temp_f() {
    return temp_f;
}

void TempSensor::read_limit_set_reg() {
    uint8_t buff[2];
    uint8_t cs_ptr = 0x03;
    i2c_write_blocking(I2C, temp_addr, &cs_ptr, 1, false);
    i2c_read_blocking(I2C, temp_addr, buff, 2, false);
    printf("\r");
    printf("\33[2K\r");
    printf("MAX value: %f", calculate_temp(buff[0], buff[1]));
}


void TempSensor::write_to_limit_set_reg(uint8_t msb_data, uint8_t lsb_data) {
    uint8_t buff[3] = {0x03, msb_data, lsb_data};
    i2c_write_blocking(I2C, temp_addr, buff, 3, false);
}


void TempSensor::read_hyst_reg() {
    uint8_t buff[2];
    uint8_t cs_ptr = 0x02;
    i2c_write_blocking(I2C, temp_addr, &cs_ptr, 1, false);
    i2c_read_blocking(I2C, temp_addr, buff, 2, false);
    printf("\r");
    printf("\33[2K\r");
    printf("MIN value: %f", calculate_temp(buff[0], buff[1]));
}


void TempSensor::write_to_hyst_reg(uint8_t msb_data, uint8_t lsb_data) {
    uint8_t buff[3] = {0x02, msb_data, lsb_data};
    i2c_write_blocking(I2C, temp_addr, buff, 3, false);
}



float TempSensor::calculate_temp(uint8_t msb, uint8_t lsb) {
    uint8_t mask = 0x80;
    float decimal = 0;
    int i = 1;
    if ((lsb & mask) != 0x00) {
        while ((lsb & mask) != 0x00) {
            decimal += std::pow(2, -(i));
            lsb = lsb << 1;
            i++;
        }
    }
    return static_cast<float>(msb) + decimal;
}