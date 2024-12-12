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
	
	
	char joystick_x = 5;
	char joystick_y = 5;
	char sw_1 = '0';
        char sw_2 = '0';
	
	bool initialize = true;

	int rand_pokemon = 0;

	while(1)
	{
		sendall(serial_port,joystick_x,joystick_y,sw_1,sw_2);       //sends updated list and gets joystick input
        	//THIS IS THE DEAD SPACE the TIVA is now waiting for a uart transmission interrupt
		//can cheack to break here
		if(initialize)
		{
			initialize = !initialize;
			std::srand(time(0));
                        rand_pokemon = (abs(std::rand())%10);
			indicate_bitmap(serial_port,rand_pokemon);
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
				//std::srand(time(0));
        			//uint16_t rand_pokemon = 1 + (abs(std::rand())%9);
				int16_t loc = linked_list.back().x_start;
				uint16_t distance = abs(loc-120);
				uint16_t odds = 100-distance; //should be 100 to 20
				uint16_t random = std::rand()%101;
				std::cout << "Odds of catch: " << odds << std::endl;
				if(random > odds)
				{
                        		indicate_bitmap(serial_port,11);
				}
				else
				{
					//caught it:
					write_to_file(rand_pokemon);
					indicate_bitmap(serial_port,10);
				}
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
	filename = "bitmaps/nidoran_bitmap.txt";
}
else if(choice == 1)
{
	filename = "bitmaps/charizard_bitmap.txt";
}
else if(choice == 2)
{
	filename = "bitmaps/luc_bitmap.txt";
}
else if(choice == 3)
{
	filename = "bitmaps/nidoran_bitmap.txt";
}
else if(choice == 4)
{
	filename = "bitmaps/pikachu_bitmap.txt";
}
else if(choice == 5)
{
	filename = "bitmaps/monkey_bitmap.txt";
}
else if(choice == 6)
{
	filename = "bitmaps/monkey_2_bitmap.txt";
}
else if(choice == 7)
{
	filename = "bitmaps/mewtwo_bitmap.txt";
}
else if(choice == 8)
{
	filename = "bitmaps/slowbro_bitmap.txt";
}
else if(choice == 9)
{
	filename = "bitmaps/eevee_bitmap.txt";
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

void Mmap::write_to_file(int rand_pokemon)
{
        std::ofstream file("users/" + std::string("defualt") + ".txt", std::ios::app);
        if (!file) {
                         std::cerr << "Unable to open or create file!" << std::endl;
                   }
        file << rand_pokemon << std::endl;
        file.close();

}

void Mmap::read_file(std::vector<int>& to_return)
{
	// Open the file in read mode
    std::ifstream file("users/" + std::string("defualt") + ".txt");
    if (!file) {
        std::cerr << "Unable to open file for reading!" << std::endl;
        return;  // Exit the function if the file can't be opened
    }

    int value;
    // Read the file line by line and add each value to the vector
    while (file >> value) {
        to_return.push_back(value);
    }

    // Close the file
    file.close();
}


void Mmap::pokedex(SerialPort& serial_port)
{
        //THIS MUST BE THE MOST RECENT ITEM!

	std::vector<int> pokemon_caught;
	read_file(pokemon_caught);
	


        char joystick_x = 5;
        char joystick_y = 5;
        char sw_1 = '0';
        char sw_2 = '0';

        bool initialize = true;

	if(pokemon_caught.size() <= 0)
	{
		return;
	}

	int index = 0;
        int curr_pokemon = pokemon_caught[0];
	int old_pokemon = -1;
	

        while(1)
        {
                sendall(serial_port,joystick_x,joystick_y,sw_1,sw_2);       //sends updated list and gets joystick input
                //THIS IS THE DEAD SPACE the TIVA is now waiting for a uart transmission interrupt
                //can cheack to break here
                if(initialize)
                {
                        initialize = !initialize;
                        if(curr_pokemon != old_pokemon)
			{
                        	indicate_bitmap(serial_port,curr_pokemon);
			}
			old_pokemon = curr_pokemon;
                }
                else
                {
                        rect_to_bitmap(linked_list, 1);     //updates the new bitmap to hold where everything newis


                        win_diff();//stores non-zero values where there are changes in window into old_bitmap


//                        rectangulize();                                   //puts in update_list all the things that need to be drawn

                        rect_to_bitmap(linked_list, 0);     //updating old bitmap
                        rect_to_bitmap(linked_list, 1);     //updates the new bitmap -- still need to clear old stuff!
                        
			if(sw_1 == '1')
                        {
                                return;
                        }
			
			if(abs(joystick_x) > 2)
			{
				nav_pokedex(index,joystick_x,pokemon_caught.size());
				curr_pokemon = pokemon_caught[index];
				initialize = true;
			}
                }
        }
}

void Mmap::nav_pokedex(int& index,char joystick_x_in, int size_pokedex)
{
	int8_t joystick_x = joystick_x_in - '0' - 5;
	bool move_right;
	if(joystick_x < 0 && abs(joystick_x) > 2)
	{
		move_right = false;
	}
	else if(abs(joystick_x) > 2)
	{
		move_right = true;
	}

	if(move_right)
	{
		if(index + 1 < size_pokedex)
		{
			index = index + 1;
		}
	}

	if(!move_right)
	{
		if(index - 1 >= 0)
		{
			index = index - 1;
		}
	}

}
