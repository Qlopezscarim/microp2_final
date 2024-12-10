#include <iostream>
#include <cstdint>
#include <vector>
#include "Mmap.h"

#define Y_D 320
#define X_D 240


//uint16_t old_bitmap[Y_D][X_D];
//uint16_t new_bitmap[Y_D][X_D];



//void node_check(to_transmit& curr_rect);

void Mmap::node_check(to_transmit& curr_rect)
{
	//try to expand horizontally:
	uint16_t index_h = curr_rect.h;
	uint16_t index_x = curr_rect.x_start+curr_rect.w;
	bool expand_x = true;
	//std::cout << "\n Start of new recurstion: " << index_x;
	if(index_x >= X_D)
	{
		expand_x = false; //need to ensure we aren't trying to expand into outside the array
	}
	else
	{
		for(int j=curr_rect.y_start ; j<curr_rect.y_start+index_h ; j++)
		{
			std::cout << "\n We are checking x: " << index_x << "\tWe are checking y: " << j;
			if(old_bitmap[j][index_x] != curr_rect.color) //cannot expand in x-direction
			{
				expand_x = false;
				break;
			}
		}
	}

	//at this point we know if we can expand in the horizontal direction or not:
	if(expand_x == true)
        {
                //in this case we can expand in the x-direction
                curr_rect.w = curr_rect.w + 1;
                //we now need to delete in the old bitmap those values:
                for(int j=curr_rect.y_start ; j<curr_rect.y_start+index_h ; j++)
                {
			std::cout << "WE CLEARED THE LOCATION: " << index_x << "\t" << "y: " << j;
			old_bitmap[j][index_x] = 0; //no longer want to make rectangles out of these guys - they have been absoarbed!
		}
		//we have now expanded in the x-direction - now we should see if we can expand again!
		node_check(curr_rect);
		return; //don't want to make a disgusting stack overflow!
        }

	//IF we have reached this point we cannot expand in the x-direction, and should now check the y-direction!
	uint16_t y_index = curr_rect.y_start+curr_rect.h;
	uint16_t x_1 = curr_rect.x_start;
	uint16_t x_2 = curr_rect.w + x_1;
	bool expand_y = true;
	if(y_index >= Y_D)
	{
		expand_y = false;
	}
	else
	{
	for(int x = x_1; x < x_2; x++)
		{
			if(old_bitmap[y_index][x] != curr_rect.color)
			{
				expand_y = false;
				break;
			}
		}
	}
	//at this point we have decided if we want to expand the rectangle or not vertially
	if(expand_y == true)
	{
		//in this case we can expand in the x-direction
                curr_rect.h = curr_rect.h + 1;
                //we now need to delete in the old bitmap those values:
                for(int x=x_1 ; x<x_2 ; x++)
                {
                        old_bitmap[y_index][x] = 0; //no longer want to make rectangles out of these guys - they have been absoarbed!
                }
                //we have now expanded in the x-direction - now we should see if we can expand again!
                node_check(curr_rect);
                return; //don't want to make a disgusting stack overflow!
	}
	return;
}


void Mmap::rectangulize()
{
	for(int i=0;i<Y_D;i++)
	{
		for(int j=0;j<X_D;j++)
		{
			if(old_bitmap[i][j] != 0)
			{
				std::cout << "We have executed graph theory" << std::endl;
				to_transmit blank;
				blank.x_start = j;
				blank.y_start = i;
				blank.h = 1;
				blank.w = 1;
				blank.color = old_bitmap[i][j];
				node_check(blank);
				master_list.push_back(blank);
			}
		}
	}
}

void Mmap::display_trans(to_transmit to_output)
{
	std::cout << "\nx_start: " << to_output.x_start << "\ty_start: " << to_output.y_start << "\tw: " << to_output.w << "\th: " << to_output.h << "\tcolor: " << to_output.color;
}

void Mmap::rect_to_bitmap(std::vector<to_transmit> a_conv, int choice)
{
	if(choice == 0)
	{
		for(auto element: a_conv)
		{
			uint16_t x_1 = element.x_start;
			uint16_t y_1 = element.y_start;
			uint16_t y_2 = y_1+element.h;
			uint16_t x_2 = x_1+element.w;
			for(int i=y_1;i<y_2;i++)
			{
				for(int j=x_1;j<x_2;j++)
				{
					old_bitmap[i][j] = element.color;
				}
			}
		}
	}
	else if(choice == 1)
	{
	for(auto element: a_conv)
                {
                        uint16_t x_1 = element.x_start;
                        uint16_t y_1 = element.y_start;
                        uint16_t y_2 = y_1+element.h;
                        uint16_t x_2 = x_1+element.w;
			std::cout << "\n\n\n NEW ELEMENT : \tx_1: " << x_1 << "\tx_2:" << x_2 << "\ty_1:" << y_1 << "\ty_2:" << y_2;
			std::cout << "\ny_2 calc as: " << y_1 << " + " << element.h;
			std::cout << "\nx_s calc as: " << x_1 << " + " << element.w;
                        for(uint16_t i=y_1 ; i<y_2 ; i++)
                        {
                                for(uint16_t j=x_1 ; j < x_2 ; j++) //2,3,4
                                {
					std::cout << "\nModifying: " << "\ti: " << i << "\tj:" << j;
                                        new_bitmap[i][j] = element.color;
                                }
                        }
                }
	}
}

