#include "Mmap.h"
#include <SerialPort.h>
#include <SerialStream.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdio>
#include <cstdint>
#define DELAY 5000

Mmap::Mmap(int gps_class)
{
	gps_class = gps_class;
	for(int i=0; i<153600; i++)
        {
                bitmap_mmap[i] = 0xff;
        }
}

void Mmap::inc_rect()
{
	uint16_t y_start = 0;
	while(y_start <= 319)
	{
	uint32_t in_arr = linked_list.size();
	in_arr = in_arr-1; //accounting for clearing rect.
	uint16_t x_start = in_arr*10;
	y_start = 0;
	while(x_start > 239)
	{
		y_start = y_start+10;
		std::cout << "\ny_start should be increased: " << y_start;
		x_start = x_start - 239;
	}
	add_list(1,x_start,y_start,10,10,ST7789_RED,1);
	}
}

void Mmap::random_rect()
{
	//this function assumes two are already in there!
	//linked_list.pop_back();

	std::srand(time(0));
	uint16_t color = abs(std::rand())%0xFFFFFF;
	std::cout << "\nRandom number is " << std::string(1,char(color)) << std::flush;
	std::srand(time(0)+1);
	uint8_t x_start = abs(std::rand())%239;
	std::srand(time(0)+2);
	uint8_t y_start = abs(std::rand())%319;
	std::srand(time(0)+3);
	uint8_t w = abs(std::rand())%(239-x_start);
	std::srand(time(0)+4);
	uint8_t h = abs(std::rand())%(319-y_start);
	add_list(1,x_start,y_start,w,h,color,1);
	
	
	
}

void Mmap::add_list(uint8_t type, uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color, uint8_t id)
{
	to_transmit add_node;
	add_node.type = type;
	add_node.x_start = x_start;
	add_node.y_start = y_start;
	add_node.w = w;
	add_node.h = h;
	add_node.color = color;
	add_node.id = id;
	if(id == 0)
	{
		player_x_start = x_start;
		player_y_start = y_start;
		player_w = w;
		player_h = h;
	}
	linked_list.push_back(add_node);
}

void Mmap::add_clear_list(uint8_t type, uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color, uint8_t id)
{
	to_transmit add_node;
	add_node.type = type;
        add_node.x_start = x_start;
        add_node.y_start = y_start;
        add_node.w = w;
        add_node.h = h;
        add_node.color = color;
        add_node.id = id;
        if(id == 0)
        {
		player_x_start = x_start;
                player_y_start = y_start;
                player_w = w;
                player_h = h;
        }
        clear_list.push_back(add_node);
}

void Mmap::add_update_list(uint8_t type, uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color, uint8_t id)
{
        to_transmit add_node;
	add_node.type = type;
        add_node.x_start = x_start;
        add_node.y_start = y_start;
        add_node.w = w;
        add_node.h = h;
        add_node.color = color;
        add_node.id = id;
        if(id == 0)
        {
                player_x_start = x_start;
		player_y_start = y_start;
                player_w = w;
                player_h = h;
        }
        update_list.push_back(add_node);
}

void Mmap::moveall(char x, char y)
{
	int8_t joystick_x = x - '0' - 5;
	int8_t joystick_y = y - '0' - 5;

	//std::cout << "\n\n joystick_x: " << joystick_x << " " << x;
	//std::cout << "\n\n joystick_y " <<  joystick_y << " " << y <<std::endl;
	
	
	for (auto& element : linked_list)
	{
		//can make this more efficient by only running if joystick certian value ig
		if(element.id == 0) //if the element is intended to be moved
		{
			//joystick_x = joystick_x - 5;
			//joystick_y = joystick_y - 5;
			if(abs(joystick_x) >= 2)
			{
				if(check_valid_x(element.x_start,element.w,element.y_start,element.h,element.color,-joystick_x))
				{
					element.x_start = element.x_start - joystick_x;//joystick is inverted
					//at this point we have moved the element and need to get rid of the old element 
				}
			}
			if(abs(joystick_y) >= 2)
			{
				if(check_valid_y(element.x_start,element.w,element.y_start,element.h,element.color,joystick_y))
				{
					element.y_start = element.y_start + joystick_y; //this one is not inverted
				}
			}
		}
	}
}

void Mmap::init_screen(SerialPort& serial_port)
{
	for (const auto& element : linked_list)
        {
                i2screen(serial_port,element.type,element.x_start,element.y_start,element.w,element.h,element.color);
                std::vector<unsigned char> received_data(3);
                serial_port.Read(received_data, received_data.size());
        }
}

void Mmap::sendall(SerialPort& serial_port, char& joystick_x, char& joystick_y)
{
	if(clear_list.size() == 0 && update_list.size() == 0)
	{
		std::cout << "GOOD WE ENTERED HERE" << std::endl;
		i2screen(serial_port,1,0,0,0,0,ST7789_BLACK);
		std::cout << "Getting joystick: " << std::endl;
		std::vector<unsigned char> received_data(3);
                serial_port.Read(received_data, received_data.size());
                joystick_x = received_data[1];
                joystick_y = received_data[2];
	}
	while(clear_list.size() != 0)
	//for(auto element:clear_list)
	{
		auto element = clear_list.back();
		i2screen(serial_port,element.type,element.x_start,element.y_start,element.w,element.h,element.color);
                std::cout << "\nELEMENT INFO: x_start, y_start,w,h: " << element.x_start << ", " << element.y_start << ", " << element.w << ", " << element.h << std::endl;
                std::vector<unsigned char> received_data(3);
                serial_port.Read(received_data, received_data.size());
                joystick_x = received_data[1];
                joystick_y = received_data[2];
		clear_list.pop_back();
	}
	while(update_list.size() != 0)
	//for (const auto& element : update_list) 
	{
		auto element = update_list.back();
        	i2screen(serial_port,element.type,element.x_start,element.y_start,element.w,element.h,element.color);
		std::cout << "\n NOT CLEARED: ELEMENT INFO: x_start, y_start,w,h: " << element.x_start << ", " << element.y_start << ", " << element.w << ", " << element.h << "Color: " << element.color  << std::endl;
		std::vector<unsigned char> received_data(3);
        	serial_port.Read(received_data, received_data.size());
		joystick_x = received_data[1];
		joystick_y = received_data[2];
		update_list.pop_back();
		//moveall(received_data[1],received_data[2]);
    	}
}

void Mmap::send256(SerialPort& serial_port)
{
	for(int i=0; i<153600; i++)
	{
		serial_port.Write(std::string(1,char(bitmap_mmap[i])));
	}
}

void Mmap::i2screen(SerialPort& serial_port,uint8_t type,uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color)
{

//const std::string type_high = std::string(1,char((type>>8 & 0xFF)));
const std::string type_low = std::string(1,char((type>>0 & 0xFF)));

const std::string x_high = std::string(1,char((x_start>>8 & 0xFF)));
const std::string x_low = std::string(1,char((x_start>>0 & 0xFF)));

const std::string y_high = std::string(1,char((y_start>>8 & 0xFF)));
const std::string y_low = std::string(1,char((y_start>>0 & 0xFF)));

const std::string w_high = std::string(1,char((w>>8 & 0xFF)));
const std::string w_low = std::string(1,char((w>>0 & 0xFF)));

const std::string h_high = std::string(1,char((h>>8 & 0xFF)));
const std::string h_low = std::string(1,char((h>>0 & 0xFF)));

const std::string color_high = std::string(1,char((color>>8 & 0xFF)));
const std::string color_low = std::string(1,char((color>>0 & 0xFF)));

serial_port.Write(type_low);
usleep(DELAY);
serial_port.Write(x_low);
usleep(DELAY);
serial_port.Write(x_high);
usleep(DELAY);
serial_port.Write(y_low);
usleep(DELAY);
serial_port.Write(y_high);
usleep(DELAY);
serial_port.Write(w_low);
usleep(DELAY);
serial_port.Write(w_high);
usleep(DELAY);
serial_port.Write(h_low);
usleep(DELAY);
serial_port.Write(h_high);
usleep(DELAY);
serial_port.Write(color_low);
usleep(DELAY);
serial_port.Write(color_high);
usleep(DELAY);

//std::cout << "\n" << type_low;
}
bool Mmap::check_collision()
{

for (auto it = linked_list.begin(); it != linked_list.end(); ++it)
        {
                to_transmit* element = &(*it);
                //std::cout << "Current color: " << element.color;
                if(element->id != 0 && element->id != 2)
                {
                        //if(element->x_start + x_i < 240 && element->x_start - x_i > 0 && abs(x_i) > 1)
                        uint16_t xs = element->x_start;
			uint16_t ys = element->y_start;
			uint16_t w = element->w;
			uint16_t h = element->h;
			if(xs == player_x_start)
			{
				std::cout << "Kill this: " << xs;
				std::cout << "Player place: " << player_x_start;
				collision();
				return 1;
			}
                }
	}
	return 0;
}

void Mmap::collision()
{
	while(linked_list.size() != 0)
	{
		linked_list.pop_back();
	}
	add_list(2,0,0,248,318,ST7789_BLUE,2);
}

void Mmap::generate_object_gps(int gps_location)
{
	//while(linked_list.size() != 2)
	//{
	//	linked_list.pop_back();
	//}
	//linked_list.pop_back();


	std::cout << "\nGenerating an object according to GPS";
	if((abs(gps_class)%78) == 0)
	//if(1)
	{
		std::cout << "\nGPS Location " << gps_location << "Correlates to a masterball";
                add_list(1,100,100,50,50,ST7789_BLUE,5);
	}
	else if((abs(gps_class)%2) == 0)
	{
		std::cout << "\nGPS Location " << gps_location << "Correlates to a bush";
                add_list(1,100-60,100,50,50,ST7789_GREEN,5);
	}
	else if((abs(gps_class)%3) == 0)
	{
		std::cout << "\nGPS Location " << gps_location << "Correlates to a pokeball";
		add_list(1,100-60,100,50,50,ST7789_RED,5);
		//add_list(1,120-60,150,20,10,ST7789_RED,5);
		//add_list(1,112-60,119,22,11,ST7789_WHITE,5);
	}
	else
	{
		std::cout << "\nYou beat the odds and nothing spawned";
	}

}

bool Mmap::check_valid_x(uint16_t x_start, uint16_t x_width,uint16_t y_start, uint16_t y_height, uint16_t color, int8_t move)
{
	bool to_return = true;
	//move left
	if(move < 0)
	{
		if(x_start + move < 0)
		{
			to_return = false;
		}
		else
		{
			//we have decided to move left and thus must clear right side:
			add_clear_list(1,x_start+x_width+move,y_start,abs(move),y_height,ST7789_BLACK,2);
			//we must also draw the new location on the left
			add_update_list(1,x_start+move,y_start,abs(move),y_height,color,2);
		}
	}
	//move right
	if(move > 0)
	{
		//std::cout << "\n\n VALUE I WANT RIGHT NOW " << x_start + x_width + move;
		//std::cout << "\n XSTART IS " << x_start;
		//std::cout << "\n X_WIDTH IS " << x_width;
		//std::cout << "\n MOVE IS " << move;
		if(x_start + x_width + move > X_MAX)
		{
			to_return = false;
		}
		else
		{
			//we have decided to move right and thus must clear left side:
                        add_clear_list(1,x_start,y_start,abs(move),y_height,ST7789_BLACK,2);
			//we now must also draw the new location on the right
			add_update_list(1,x_start+x_width,y_start,abs(move),y_height,color,2);
		}
	}
	return to_return;
}
bool Mmap::check_valid_y(uint16_t x_start, uint16_t x_width,uint16_t y_start, uint16_t y_height, uint16_t color, int8_t move)
{
	bool to_return = true;
        if(move < 0)
        {
                if(y_start + move < 0)
                {
                        to_return = false;
                }
		else
		{
			//we have decide to move down and thus must clear top
			add_clear_list(1,x_start,y_start+y_height+move,x_width,abs(move),ST7789_BLACK,2);
			//we now must also draw the new location on the bottom
			add_update_list(1,x_start,y_start+move,x_width,abs(move),color,2);
		}
        }
        if(move > 0)
        {
                if(y_start + y_height + move > Y_MAX)
                {
                        to_return = false;
                }
		else
		{
			//we have decided to move up and thus must clear bottom
			add_clear_list(1,x_start,y_start,x_width,abs(move),ST7789_BLACK,2);
			//we now must also draw the new location on the top
			add_update_list(1,x_start,y_start+y_height,x_width,abs(move),color,2);
		}
        }
        return to_return;
}
