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
				//std::cout << "\n\n\n\n\nWe have executed graph theory\n---------------------------------------------------------------\n\n\n" << std::endl;
				//std::cout << "i: " << i << "j: " <<j << "value: " << old_bitmap[i][j];
				to_transmit blank;
				blank.type = 1;
				blank.x_start = j;
				blank.y_start = i;
				blank.h = 1;
				blank.w = 1;
				blank.color = old_bitmap[i][j]; //get the color from the most updated bitmap
				node_check(blank);
				blank.color = new_bitmap[i][j];
				update_list.push_back(blank); //update the list of changes
			}
		}
	}
}

void Mmap::display_trans(to_transmit to_output)
{
	std::cout << "\nx_start: " << to_output.x_start << "\ty_start: " << to_output.y_start << "\tw: " << to_output.w << "\th: " << to_output.h << "\tcolor: " << to_output.color << std::endl;
}

void Mmap::rect_to_bitmap(std::vector<to_transmit> a_conv, int choice) //clears for both - get rid of this if need be though!
{
	if(choice == 0)
	{
		for(int i=0;i<Y_D;i++)
        	{
                	for(int j=0;j<X_D;j++)
                	{
                        	old_bitmap[i][j] = 0;
                	}
        	}
		//done clearing
		for(auto element: a_conv)
		{
			uint16_t x_1 = element.x_start;
			uint16_t y_1 = element.y_start;
			//check for overflow
			if(x_1 >1000)
			{x_1 = 0;}
			if(y_1 > 1000)
			{y_1 = 0;}
			uint16_t y_2 = y_1+element.h;
			uint16_t x_2 = x_1+element.w;
			for(int i=y_1;i<y_2;i++)
			{
				for(int j=x_1;j<x_2;j++)
				{
					if(i<Y_D && j<X_D)
					{old_bitmap[i][j] = element.color;}
				}
			}
		}
	}
	else if(choice == 1)
	{

		for(int i=0;i<Y_D;i++)
        	{
                	for(int j=0;j<X_D;j++)
                	{
                        	new_bitmap[i][j] = 0;
                	}
        	}
	//done clearing
	for(auto element: a_conv)
                {
                        uint16_t x_1 = element.x_start;
                        uint16_t y_1 = element.y_start;
                        uint16_t y_2 = y_1+element.h;
                        uint16_t x_2 = x_1+element.w;
                        for(uint16_t i=y_1 ; i<y_2 ; i++)
                        {
                                for(uint16_t j=x_1 ; j < x_2 ; j++) //2,3,4
                                {
					if(i<Y_D && j<X_D)
                                        {new_bitmap[i][j] = element.color;}
                                }
                        }
                }
	}
}

void Mmap::diff()
{
	for(int i=0;i<Y_D;i++)
        {
                for(int j=0;j<X_D;j++)
                {
			uint16_t old_value = old_bitmap[i][j];
			uint16_t new_value = new_bitmap[i][j];
			//std::cout << "old value:\t" << old_bitmap[i][j];
                        //std::cout << "new value:\t" << new_bitmap[i][j] << std::endl;
                        old_bitmap[i][j] = old_bitmap[i][j] - new_bitmap[i][j];
			/*if(old_bitmap[i][j] != 0)
			{
				std::cout << "Diff: \t\t" << old_bitmap[i][j] <<"old value:\t\t" << old_value;
                        	std::cout << "new value:\t\t" << new_value << std::endl;
			}*/
		}
       }
}

void Mmap::clear_diff()
{
        for(int i=0;i<Y_D;i++)
        {
                for(int j=0;j<X_D;j++)
                {
                        new_bitmap[i][j] = old_bitmap[i][j] - new_bitmap[i][j];
			//if old_bitmap is a zero and new_bitmap[i][j] is not zero make it magic number
			if(new_bitmap[i][j] != 0 && old_bitmap[i][j] == 0)
			{
				new_bitmap[i][j] = MAGIC;
			}
                }
       }
}

void Mmap::print_portion_bitmap(int choice,uint16_t x_start,uint16_t y_start, uint16_t w, uint16_t h)
{
	uint16_t x_1 = x_start;
	uint16_t x_2 = x_start+w;
	uint16_t y_1 = y_start;
	uint16_t y_2 = y_start+h;
	std::cout << "\nStart of bitmap: type:" << choice << std::endl;
	if(choice == 0)
	{
		for(int i = y_1;i<y_2;i++)
		{
			for(int j=x_1;j<x_2;j++)
			{
				std::cout << old_bitmap[i][j] << "\t";
			}
			std::cout << "\n";
		}
	}
	if(choice == 1)
        {
                for(int i = y_1;i<y_2;i++)
                {
                        for(int j=x_1;j<x_2;j++)
                        {
                                std::cout << new_bitmap[i][j] << "\t";
                        }
                        std::cout << "\n";
                }
        }

}

void Mmap::clear_rectangulize()
{
        for(int i=0;i<Y_D;i++)
        {
                for(int j=0;j<X_D;j++)
                {
                        if(new_bitmap[i][j] != 0)
                        {
                                //std::cout << "\n\n\n\n\nWe have executed CLEAR theory\n---------------------------------------------------------------\n\n\n" << std::endl;
                                to_transmit blank;
                                blank.x_start = j;
                                blank.y_start = i;
                                blank.h = 1;
                                blank.w = 1;
				blank.type = 1;
                                blank.color = new_bitmap[i][j]; //get the color from the most updated bitmap
                                clear_node_check(blank);
                                blank.color = ST7789_BLACK;
                                clear_list.push_back(blank); //update the list of changes
                        }
                }
        }
}

void Mmap::clear_node_check(to_transmit& curr_rect)
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
                        if(new_bitmap[j][index_x] != curr_rect.color) //cannot expand in x-direction
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
                        new_bitmap[j][index_x] = 0; //no longer want to make rectangles out of these guys - they have been absoarbed!
                }
                //we have now expanded in the x-direction - now we should see if we can expand again!
                clear_node_check(curr_rect);
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
                        if(new_bitmap[y_index][x] != curr_rect.color)
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
                        new_bitmap[y_index][x] = 0; //no longer want to make rectangles out of these guys - they have been absoarbed!
                }
                //we have now expanded in the x-direction - now we should see if we can expand again!
                clear_node_check(curr_rect);
                return; //don't want to make a disgusting stack overflow!
        }
        return;
}


void Mmap::win_diff()
{
        for(int i=0;i<Y_D;i++)
        {
                for(int j=0;j<X_D;j++)
                {
                        uint16_t old_value = old_bitmap[i][j];
                        uint16_t new_value = new_bitmap[i][j];
                        if(i<120 && i>99 && j<200 && j>40)
			{
				//inside gradient
                        	old_bitmap[i][j] = old_bitmap[i][j] - new_bitmap[i][j];
			}
			else
			{
				old_bitmap[i][j] = 0;
			}
                }
       }
}
