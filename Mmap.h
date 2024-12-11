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




#define MAGIC			    0xFFFE
#define ST7789_BLACK                0x0000
#define ST7789_RED                  0x001F
#define ST7789_BLUE                 0xF800
#define ST7789_GREEN                0x07E0
#define ST7789_WHITE		    0xFFFF
#define X_MAX			    239
#define Y_MAX			    319
#define Y_D 320
#define X_D 240


//uint16_t old_bitmap[Y_D][X_D];
//uint16_t new_bitmap[Y_D][X_D];

class Mmap
{
private:
bool check_valid_x(int8_t move);
bool check_valid_y(int8_t move);
bool check_x_collision(int8_t move);
bool check_y_collision(int8_t move);

public:
uint16_t player_x_start;
uint16_t player_y_start;
uint16_t player_w;
uint16_t player_h;
int32_t x_displacement = 0;
int32_t y_displacement = 0;
int gps_class;
std::string cur_direction = "F";
std::string old_direction = "B";
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
uint16_t old_bitmap[Y_D][X_D];
uint16_t new_bitmap[Y_D][X_D];

std::vector<to_transmit> linked_list;
std::vector<to_transmit> clear_list;
std::vector<to_transmit> update_list;
uint16_t num_objects;
Mmap(int gps_class);
void moveall(char x, char y);
void init_screen(SerialPort& serial_port);
void sendall(SerialPort& serial_port,char& joystick_x, char& joystick_y);
void send256(SerialPort& serial_port);
void add_list(uint8_t type, uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color, uint8_t id);
void add_clear_list(uint8_t type, uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color, uint8_t id);
void add_update_list(uint8_t type, uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color, uint8_t id);
void i2screen(SerialPort& serial_port,uint8_t type,uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color);
void update_bitmap();
void inc_rect();
bool check_collision(char x, char y);
void random_rect();
void collision();
void generate_object_gps(int gps_location);
//start of Mmap2
void node_check(to_transmit& curr_rect);
void rectangulize();
void rect_to_bitmap(std::vector<to_transmit> a_conv, int choice);
void display_trans(to_transmit to_output);
void diff();
void clear_diff();
void print_portion_bitmap(int choice,uint16_t x_start,uint16_t y_start, uint16_t w, uint16_t h);
void clear_rectangulize();
void clear_node_check(to_transmit& curr_rect);
//start of MMap3
void Pokeball(uint16_t x_start, uint16_t y_start, float scale);
void Trainer(uint16_t x_start, uint16_t y_start, float scale);
void Charizard(uint16_t x_start, uint16_t y_start, float scale);
void Diglett(uint16_t x_start, uint16_t y_start, float scale);
void Terrain(uint16_t x_start, uint16_t y_start, float scale);
void Terrain2(uint16_t x_start, uint16_t y_start, float scale);
void RockyTerrain(uint16_t x_start, uint16_t y_start, float scale);
void Check_TB();
void GTerrain(uint8_t to_spawn);
void GDirection(char joystick_x, char joystick_y);
void Background();
void TrainerWalkRight(uint16_t x_start, uint16_t y_start, float scale);
void update_trainer();
void SimpleWeed(uint16_t x_start, uint16_t y_start, float scale);
};
#endif
