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

#include <stdexcept>


#define COMP 2


void Mmap::retry_write(SerialPort& serial_port, const std::string& data) {
    	int loops = 0;
	bool write_occured = false;
	while (loops<2) 
	{
	loops++;
        try {
            serial_port.Write(data);  // Attempt to write the entire data
	    write_occured = true;
            break;  // Exit the loop on success
		}
        catch (const std::runtime_error& e) 
		{
            		if (errno == EINTR) 
			{
				usleep(100);
                		continue;  // Retry on interruption
				//std::cerr << "\n -----------BAD" << std::endl;
         		}
            	throw std::runtime_error("Serial port write failed: " + std::string(e.what()));
        	}
    	}
	
	if(write_occured == false)
	{
	retry_write(serial_port,data);
	}
}


void Mmap::smart_read(SerialPort& serial_port,std::vector<unsigned char>& received_data)
{
	while(received_data.size() != 4)
	{
		try
		{
			//std::vector<unsigned char> received_data(2);
			unsigned char data;
			data = serial_port.ReadByte(10000); // Timeout of 1000 ms
                	received_data.push_back(data);
		}
		catch (const SerialPort::ReadTimeout&) 
		{
			//assuming the uart is waiting ig?
			std::string nothing = "0";
			retry_write(serial_port,nothing);
		}

	}
}



Mmap::Mmap(int gps_class, std::string& User_in)
{
	User = User_in;
	gps_class = gps_class;
	for(int i=0; i<Y_D; i++)
	{
		for(int j=0; j<X_D; j++)
		{
			old_bitmap[i][j] = ST7789_BLACK;
			new_bitmap[i][j] = ST7789_BLACK;
		}
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

void Mmap::update_bitmap()
{
	//depreciated
	for(int i=0; i<240; i++)
        {
                for(int j=0; j<320; j++)
                {
                        new_bitmap[i][j] = ST7789_BLACK;
                }
        }
	for(auto element:linked_list)
	{
		//need to get start and end indicies and shift starts by one because indexes
		uint16_t x_start = element.x_start - 1;
		uint16_t x_end = x_start+element.w;
		uint16_t y_start = element.y_start - 1;
		uint16_t y_end = y_start+element.h;
		if(x_start < 0)
		{x_start = 0;}
		if(y_start < 0)
		{y_start = 0;}
		if(x_end > 239)
		{x_end = 239;}
		if(y_end > 319)
		{y_end = 319;}
		for(int i=x_start; i<x_end; i++)
		{
			for(int j=y_start; j<y_end; j++)
			{
				new_bitmap[i][j] = element.color;
			}
		}
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

	joystick_x = -joystick_x;
	joystick_y = -joystick_y;

	//std::cout << "\n\n joystick_x: " << joystick_x << " " << x;
	//std::cout << "\n\n joystick_y " <<  joystick_y << " " << y <<std::endl;
	bool valid_move = 1;

	for(auto element : linked_list)
	{
		bool x_valid = check_valid_x(joystick_x, element);
		bool y_valid = check_valid_y(joystick_y, element);
		//std::cout << "\nY_valid:\t" << y_valid << "X_VALID:\t" <<x_valid<<std::endl;
		bool el_valid_move = x_valid | y_valid;
		if(el_valid_move == 0)
		{
			valid_move = 0;
		}
	}

//	while(!valid_move)

	//handle displacement:
	if(abs(joystick_x) > 2 && valid_move)
	{x_displacement = x_displacement + joystick_x;}
	if(abs(joystick_y) > 2 && valid_move)
        {y_displacement = y_displacement + joystick_y;}
	
	for (auto& element : linked_list)
	{
		//can make this more efficient by only running if joystick certian value ig
		if(element.id == 0 || element.id == 3 || element.id == 7) //if the element is intended to be moved
		{
			//joystick_x = joystick_x - 5;
			//joystick_y = joystick_y - 5;
			if(abs(joystick_x) >= 2)
			{
				if(element.id != 2 && valid_move)
				{
					element.x_start = element.x_start - joystick_x;//joystick is inverted
					//at this point we have moved the element and need to get rid of the old element 
				}
			}
			if(abs(joystick_y) >= 2)
			{
				if(element.id != 2 && valid_move)
				{
					element.y_start = element.y_start + joystick_y; //this one is not inverted
				}
			}
		}
	}
}

void Mmap::init_screen(SerialPort& serial_port)
{
}

void Mmap::sendall(SerialPort& serial_port, char& joystick_x, char& joystick_y,char& sw1, char&sw2)
{
	if(clear_list.size() == 0 && update_list.size() == 0)
	{
		i2screen(serial_port,1,0,0,0,0,999);
		std::cout << "\nELEMENT INFO: x_start, y_start,w,h: \t" << 1 << "\t, " << 0 << "\t, " << 99;
		std::vector<unsigned char> received_data;//(2);
                //serial_port.Read(received_data, received_data.size());
		smart_read(serial_port,received_data);
                joystick_x = received_data[0];
                joystick_y = received_data[1];
		sw1 = received_data[2];
		sw2 = received_data[3];
	}
	while(clear_list.size() != 0)
	//for(auto element:clear_list)
	{
		auto element = clear_list.back();
		i2screen(serial_port,element.type,element.x_start,element.y_start,element.w,element.h,element.color);
                std::cout << "\nELEMENT INFO: x_start, y_start,w,h: \t" << element.x_start << "\t, " << element.y_start << "\t, " << element.w << "\t, " << element.h << std::endl;
                std::vector<unsigned char> received_data;//(2);
                //serial_port.Read(received_data, received_data.size());
		smart_read(serial_port,received_data);
                joystick_x = received_data[0];
                joystick_y = received_data[1];
		sw1 = received_data[2];
                sw2 = received_data[3];
		clear_list.pop_back();
	}
	while(update_list.size() != 0)
	//for (const auto& element : update_list) 
	{
		auto element = update_list.back();
        	i2screen(serial_port,element.type,element.x_start,element.y_start,element.w,element.h,element.color);
		std::cout << "\n NOT CLEARED: ELEMENT INFO: x_start, y_start,w,h: \t" << element.x_start << "\t, " << element.y_start << "\t, " << element.w << "\t, " << element.h << "Color: " << element.color  << std::endl;
		std::vector<unsigned char> received_data;//(2);
        	//serial_port.Read(received_data, received_data.size());
		smart_read(serial_port,received_data);
		joystick_x = received_data[0];
		joystick_y = received_data[1];
		sw1 = received_data[2];
                sw2 = received_data[3];
		update_list.pop_back();
    	}
}

void Mmap::send256(SerialPort& serial_port)
{
}

void Mmap::i2screen(SerialPort& serial_port,uint8_t type,uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color)
{

//const std::string type_high = std::string(1,char((type>>8 & 0xFF)));
std::string type_low = std::string(1,char((type>>0 & 0xFF)));

std::string x_high = std::string(1,char((x_start>>8 & 0xFF)));
std::string x_low = std::string(1,char((x_start>>0 & 0xFF)));

std::string y_high = std::string(1,char((y_start>>8 & 0xFF)));
std::string y_low = std::string(1,char((y_start>>0 & 0xFF)));

std::string w_high = std::string(1,char((w>>8 & 0xFF)));
std::string w_low = std::string(1,char((w>>0 & 0xFF)));

std::string h_high = std::string(1,char((h>>8 & 0xFF)));
std::string h_low = std::string(1,char((h>>0 & 0xFF)));

std::string color_high = std::string(1,char((color>>8 & 0xFF)));
std::string color_low = std::string(1,char((color>>0 & 0xFF)));

//std::string to_send = type_low+x_low+x_high+y_low+y_high+w_low+w_high+h_low+h_high+color_low+color_high;

//serial_port.Write(to_send);
//usleep(DELAY);
retry_write(serial_port,type_low);
retry_write(serial_port,x_low);
retry_write(serial_port,x_high);
retry_write(serial_port,y_low);
retry_write(serial_port,y_high);
retry_write(serial_port,w_low);
retry_write(serial_port,w_high);
retry_write(serial_port,h_low);
retry_write(serial_port,h_high);
retry_write(serial_port,color_low);
retry_write(serial_port,color_high);

}
bool Mmap::check_collision(char x, char y)
{
        int8_t joystick_x = x - '0' - 5;
        int8_t joystick_y = y - '0' - 5;

        joystick_x = -joystick_x;
        joystick_y = -joystick_y;

        //std::cout << "\n\n joystick_x: " << joystick_x << " " << x;
        //std::cout << "\n\n joystick_y " <<  joystick_y << " " << y <<std::endl;
	bool is_collision = false;
	for(auto& element: linked_list)
	{
        	bool x_valid = check_x_collision(joystick_x,element);
        	bool y_valid = check_y_collision(joystick_y,element);
        	bool collision_occured = x_valid && y_valid;
		if(collision_occured)
		{
		is_collision = true;
		element.id = 7;
		}
		//element.id = 2; //no longer interactable
	}
	if(is_collision)
	{std::cout << "COLLISION OCCURED!\n" <<std::endl;}
	return is_collision;

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

bool Mmap::check_valid_x(int8_t move, to_transmit element)
{
	bool to_return = true;
	move = -move;
	uint16_t x_1 = element.x_start;
	uint16_t x_2 = element.x_start+element.w;
	for(uint16_t x_c=x_1; x_c<x_2;x_c++)
	{
		if(x_c + move > (240/2)-15+COMP && x_c + move < ((240/2)-15+25)-COMP && element.id != 2 && element.id == 0)
		{
			to_return = false;
		}
		else
		{
				//std::cout << "This set passed x_c:\t" << x_c << "\tMove::\t" << static_cast<int>(move) << "\tstart:\t" << (240/2)-16 << std::endl;
				//we have decided to move left and thus must clear right side:
				//add_clear_list(1,x_start+x_width+move,y_start,abs(move),y_height,ST7789_BLACK,2);
				//we must also draw the new location on the left
				//add_update_list(1,x_start+move,y_start,abs(move),y_height,color,2);
		}
	}
	return to_return;
}
bool Mmap::check_valid_y(int8_t move, to_transmit element)
{
	bool to_return = true;
	uint16_t y_1 = element.y_start;
        uint16_t y_2 = element.y_start+element.h;
        for(uint16_t y_c=y_1; y_c<y_2;y_c++)
	{
                if(y_c + move > (320/2)-5+COMP && y_c + move < (320/2)-5+25-COMP && element.id != 2 && element.id == 0)
                {
                        to_return = false;
                }
                else
                {
                        	//we have decided to move left and thus must clear right side:
                        	//add_clear_list(1,x_start+x_width+move,y_start,abs(move),y_height,ST7789_BLACK,2);
                        	//we must also draw the new location on the left
                        	//add_update_list(1,x_start+move,y_start,abs(move),y_height,color,2);
                }
	}
        return to_return;
}

bool Mmap::check_x_collision(int8_t move, to_transmit element)
{
        bool to_return = false;
        move = -move;
        uint16_t x_1 = element.x_start;
        uint16_t x_2 = element.x_start+element.w;
        for(uint16_t x_c=x_1; x_c<x_2;x_c++)
        {
        	if(x_c + move > (240/2)-15+COMP && x_c + move < ((240/2)-15+25)-COMP && element.id == 3)
                {
                        to_return = true;
                }
                else
                {
                                //std::cout << "This set passed x_c:\t" << x_c << "\tMove::\t" << static_cast<int>(move) << "\tstart:\t" << (240/2)-16 << std::$
                                //we have decided to move left and thus must clear right side:
                                //add_clear_list(1,x_start+x_width+move,y_start,abs(move),y_height,ST7789_BLACK,2);
                                //we must also draw the new location on the left
                                //add_update_list(1,x_start+move,y_start,abs(move),y_height,color,2);
                }
        }
        return to_return;
}

bool Mmap::check_y_collision(int8_t move, to_transmit element)
{
        bool to_return = false;
                uint16_t y_1 = element.y_start;
                uint16_t y_2 = element.y_start+element.h;
                for(uint16_t y_c=y_1; y_c<y_2;y_c++)
                {
                        if(y_c + move > (320/2)-5+COMP && y_c + move < (320/2)-5+25-COMP && element.id == 3)
                        {
                                to_return = true;
                        }
                        else
                        {
                                //we have decided to move left and thus must clear right side:
                                //add_clear_list(1,x_start+x_width+move,y_start,abs(move),y_height,ST7789_BLACK,2);
                                //we must also draw the new location on the left
                                //add_update_list(1,x_start+move,y_start,abs(move),y_height,color,2);
                        }
                }
        return to_return;
}
