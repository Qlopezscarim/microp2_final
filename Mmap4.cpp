#include <iostream>
#include <cstdint>
#include <vector>
#include "Mmap.h"


void Mmap::save_context()
{
	while(update_list.size() != 0)
	{update_list.pop_back();}
	while(clear_list.size() != 0)
        {clear_list.pop_back();}
	while(linked_list.size() != 0)
        {
                auto element = linked_list.back();
       		context_list.push_back(element);
                linked_list.pop_back();
	}

}

void Mmap::restore_context()
{
	while(update_list.size() != 0)
        {update_list.pop_back();}
	while(clear_list.size() != 0)
        {clear_list.pop_back();}
	while(context_list.size() != 0)
        {
                auto element = context_list.back();
                linked_list.push_back(element);
                context_list.pop_back();
        }
}

void Mmap::encounter(SerialPort& serial_port)
{
	//add_list(//red rectangle!
	
	//SimpleWeed(90,100,1);
	Gradient(40,100,1);

	//THIS MUST BE THE MOST RECENT ITEM!
	//gen random x_position
	std::srand(time(0)+1);
        uint16_t rand_x = abs(std::rand())%155;
	bool left = true;
	
	add_list(0,rand_x+40,100,3,30,ST7789_BLUE,0);
	
	
	char sw_1 = '0';
        char sw_2 = '0';
	
	bool initialize = true;

	while(1)
	{
		sendall(serial_port,sw_1,sw_2);       //sends updated list and gets joystick input
        	//THIS IS THE DEAD SPACE the TIVA is now waiting for a uart transmission interrupt
		//can cheack to break here
		if(initialize)
		{
			initialize = !initialize;
			indicate_bitmap(serial_port,0);
		}
		else
		{
			mov_rect(left);
        		rect_to_bitmap(linked_list, 1);     //updates the new bitmap to hold where everything newis
			
			
			win_diff();//stores non-zero values where there are changes in window into old_bitmap
			

                	rectangulize();                                   //puts in update_list all the things that need to be drawn

                	rect_to_bitmap(linked_list, 0);     //updating old bitmap
                	rect_to_bitmap(linked_list, 1);     //updates the new bitmap -- still need to clear old stuff!
			std::cout << "X_loc:\t" << linked_list.back().x_start;
			if(sw_1 == '1')
			{
                        	indicate_bitmap(serial_port,11);
				return;
			}
		}
	}
}

void Mmap::read_pokemon(uint8_t pokemon)
{
	std::string filename = "bitmaps/charizard_bitmap.txt";
	
	std::ifstream file(filename);
	if (!file)
	{
        std::cerr << "Error opening file" << std::endl;
        return;
    	}

    	uint16_t value;
	uint16_t x_index = 0;
	uint16_t y_index = 0;
    	while (file >> std::hex >> value) {
        	//std::cout << "Hex: 0x" << std::hex << value;
		new_bitmap[y_index][x_index] = value;
		x_index++;
		if(x_index == 240)
		{
			x_index=0;
			y_index++;
		}
    	}
}

void Mmap::indicate_bitmap(SerialPort& serial_port, int choice)
{
const std::string type_low = std::string(1,char((99>>0 & 0xFF)));

serial_port.Write(type_low);
usleep(DELAY);

std::string filename;

if(choice == 0)
{
	filename = "bitmaps/charizard_bitmap.txt";
}
else if(choice == 1)
{
	
}
else if(choice == 2)
{

}
else if(choice == 3)
{
}
else if(choice == 4)
{
}
else if(choice == 5)
{
}
else if(choice == 6)
{
}
else if(choice == 10)
{
	filename = "bitmaps/caught_bitmap.txt";//to be inserted
}
else if(choice == 11)
{
	filename = "bitmaps/escaped_bitmap.txt";
}

std::ifstream file(filename);
if (!file)
{
std::cerr << "Error opening file" << std::endl;
return;
}


uint16_t value;
while (file >> std::hex >> value) {
std::string color_high = std::string(1,char((value>>8 & 0xFF)));
std::string color_low = std::string(1,char((value>>0 & 0xFF)));
color_high = color_high + color_low;

retry_write(serial_port,color_high);
}


}

void Mmap::Gradient(uint16_t x_start, uint16_t y_start, float scale)
{
	int16_t h = 20;
	int16_t w = 160;
	int16_t middle = x_start+(w/2);
	for(int16_t x_1 = x_start ; x_1 <x_start+w ; x_1 = x_1+5)
	{
		float gradient = float(abs(x_1-middle)) / (w);
		std::cout << "\nGradient:\t "<< gradient << std::endl;
		uint16_t color = ST7789_RED*(1-gradient);
		add_list(2,x_1,y_start,5,h,color,0);
	}

}

void Mmap::mov_rect(bool& left)
{
	to_transmit old_rect = linked_list.back();
	int16_t displacement = 3;
	uint16_t x_start = old_rect.x_start;
	uint16_t y_start = old_rect.y_start;
	uint16_t w = old_rect.w;
	uint16_t h = old_rect.h;
	uint16_t color = old_rect.color;
	if(left)
	{
		if(x_start+w+displacement > 160+40)
		{
			left = !left;
		}
		else
		{
			to_transmit new_block;
			new_block.x_start = x_start + displacement;
        		new_block.y_start = old_rect.y_start;
        		new_block.w = old_rect.w;
        		new_block.h = old_rect.h;
        		new_block.color = old_rect.color;
			linked_list.pop_back();
			linked_list.push_back(new_block);
		}
	}
	else
	{
		if(x_start-displacement < 40)
		{
			left = !left;
		}
		{
			to_transmit new_block;
                        new_block.x_start = x_start - displacement;
                        new_block.y_start = old_rect.y_start;
                        new_block.w = old_rect.w;
                        new_block.h = old_rect.h;
                        new_block.color = old_rect.color;
                        linked_list.pop_back();
                        linked_list.push_back(new_block);
		}
	}
	
}
