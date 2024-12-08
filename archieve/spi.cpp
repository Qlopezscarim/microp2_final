#include "spi.h"
#include <SerialPort.h>
#include <SerialStream.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdio>
#include <cstdint>
#define DELAY 100000
void SetWindow(SerialPort& serial_port,int16_t x, int16_t y, int16_t w, int16_t h)
{

const std::string ST7789_CASET_ADDR = std::string(1,char(0x2A));
const std::string x_high = std::string(1,char((x>>8 & 0xFF)));
const std::string x_low = std::string(1,char((x>>0 & 0xFF)));

const std::string y_high = std::string(1,char((y>>8 & 0xFF)));
const std::string y_low = std::string(1,char((y>>0 & 0xFF)));

serial_port.Write(ST7789_CASET_ADDR);
usleep(DELAY);
serial_port.Write(x_high);
usleep(DELAY);
serial_port.Write(x_low);
usleep(DELAY);
}
