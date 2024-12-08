#ifndef MMAP
#define MMAP
#include <SerialPort.h>
#include <SerialStream.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdio>
#include <cstdint>





#define ST7789_BLACK                0x0000
#define ST7789_RED                  0x001F
#define ST7789_BLUE                 0xF800
#define ST7789_GREEN                0x07E0
#define ST7789_WHITE		    0xFFFF
#define X_MAX			    239
#define Y_MAX			    319

class Mmap
{
private:
bool check_valid_x(uint16_t x_start, uint16_t x_width,uint16_t y_start, uint16_t y_height, int8_t move);
bool check_valid_y(uint16_t x_start, uint16_t x_width,uint16_t y_start, uint16_t y_height, int8_t move);
public:
uint16_t player_x_start;
uint16_t player_y_start;
uint16_t player_w;
uint16_t player_h;
int gps_class;
struct to_transmit
{
	uint8_t type;
	uint16_t x_start;
	uint16_t y_start;
	uint16_t w;
	uint16_t h;
	uint16_t color;
	uint8_t id;
};
uint8_t bitmap_mmap[153600];

std::vector<to_transmit> linked_list;
std::vector<to_transmit> clear_list;
uint16_t num_objects;
Mmap(int gps_class);
void moveall(char x, char y);
void sendall(SerialPort& serial_port,char& joystick_x, char& joystick_y);
void send256(SerialPort& serial_port);
void add_list(uint8_t type, uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color, uint8_t id);
void add_clear_list(uint8_t type, uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color, uint8_t id);
void i2screen(SerialPort& serial_port,uint8_t type,uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color);
void inc_rect();
bool check_collision();
void random_rect();
void collision();
void generate_object_gps(int gps_location);
};
#endif
