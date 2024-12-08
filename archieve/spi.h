#ifndef FUNCT_ME
#define FUNCT_ME
#include <SerialPort.h>
#include <SerialStream.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdio>
#include <cstdint>
void SetWindow(SerialPort& serial_port,int16_t x, int16_t y, int16_t w, int16_t h);
#endif
