#include <SerialPort.h>
#include <SerialStream.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
//#include "spi.h"
#include "Mmap.h"

long random_gps;

int main() {
    using namespace LibSerial;

    //srand((unsigned) time(NULL));
    //random_gps = std::randint();
    //random_gps = random.randint(1, 100)
    //std::cout << "\n" << random_gps;

    const std::string serial_port_name = "/dev/ttyO4"; // Replace with your UART port
    const int baud_rate = 9600;


        // Create and open the serial port.
        SerialPort serial_port(serial_port_name);
        serial_port.Open(SerialPort::BAUD_1000000,
                         SerialPort::CHAR_SIZE_8,
                         SerialPort::PARITY_NONE,
                         SerialPort::STOP_BITS_1,
                         SerialPort::FLOW_CONTROL_NONE);

        // Check if the serial port is open.
        if (!serial_port.IsOpen()) {
            std::cerr << "Failed to open the serial port." << std::endl;
            return 1;
        }

        // Write data to the serial port.
	Mmap game_instance = Mmap(random_gps);
	
	game_instance.add_update_list(3,1,0,0,0,0,0);
	//This correlates with a read rectangle at 50,50 with a width of 40 and height of 40
	/*game_instance.add_list(1,50,50,40,20,ST7789_RED,0); //void add_list(uint8_t type, uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color, uint8_t id);
	game_instance.add_list(1,100,90,40,20,ST7789_GREEN,0);
	game_instance.add_list(1,140,100,40,20,ST7789_GREEN,0);
	game_instance.add_list(1,180,100,40,20,ST7789_RED,0);
	game_instance.add_list(1,100,30,40,20,ST7789_GREEN,0);
	game_instance.add_list(1,20,180,40,20,ST7789_RED,0);
	game_instance.add_list(1,90,120,40,20,ST7789_GREEN,0);
	game_instance.add_list(1,200,140,40,20,ST7789_RED,0);*/
	//game_instance.add_list(1,10,200,40,20,ST7789_GREEN,0);
	//game_instance.add_list(1,10,10,5,5,ST7789_GREEN,0);
	//game_instance.add_list(1,100-3,100,3,40,ST7789_RED,0);
	
	//game_instance.add_list(1,20,20,50,50,ST7789_GREEN,0);
	//game_instance.add_list(1,25,25,10,10,ST7789_RED,0);

	/*// Draw the white semicircle (bottom half)
	game_instance.add_list(1, 50, 50, 40, 20, 0xFFFF, 0); // White: 0xFFFF

	// Draw the red semicircle (top half)
	game_instance.add_list(1, 50, 30, 40, 20, 0xF800, 0); // Red: 0xF800

	// Draw the black horizontal stripe
	game_instance.add_list(1, 50, 48, 40, 4, 0x0000, 0); // Black: 0x0000

	// Draw the outer black circle (button outline)
	game_instance.add_list(1, 65, 45, 10, 10, 0x0000, 0); // Black: 0x0000

	// Draw the inner white circle (button)
	game_instance.add_list(1, 67, 47, 6, 6, 0xFFFF, 0); // White: 0xFFFF*/
	
	//game_instance.Pokeball(50,50,.5);
	//game_instance.Trainer(100,100,1);
	
	//game_instance.Charizard(50, 50, 1);
	//game_instance.Diglett(50,50,1);
	//game_instance.Terrain2(50,50,1);
	//game_instance.RockyTerrain(50,50,1);

	/*for(int i=0;i<3;i++)
	{
		for(int j=0;j<7;j++)
		{
			game_instance.GTerrain(i*100,j*50,1,i);
		}
	}*/
	game_instance.Background();
	//game_instance.GTerrain(0,0,3,1);
	game_instance.Terrain2((240/2)-15-21, (320/2)-5-16, 1);
	game_instance.SimpleWeed(50,50,1);

	//game_instance.Trainer(100,100,.8);
	//game_instance.TrainerWalkRight(100,100,.8);
	//game_instance.add_list(1,10,200,40,20,ST7789_BLUE,0);
        //game_instance.add_list(1,40,40,50,50,ST7789_GREEN,0);
        //game_instance.add_list(1,100-3,100,20,40,ST7789_RED,0);


	//game_instance.add_list(1,20,20,50,50,ST7789_GREEN,0);


	//game_instance.random_rect();

	char joystick_x = 5;
	char joystick_y = 5;

	game_instance.rect_to_bitmap(game_instance.linked_list, 1); //updating new bitmap
	//game_instance.rect_to_bitmap(game_instance.linked_list, 0); //updating old bitmap

        game_instance.rect_to_bitmap(game_instance.linked_list, 1);     //updates the new bitmap to hold where everything new is
        std::cout << "\nMOVING WITH INPUT 0,5: " << std::endl;
        //game_instance.print_portion_bitmap(0,6,6,14,14);
        //game_instance.print_portion_bitmap(1,6,6,14,14);


        game_instance.diff();                                           //stores non-zero values where there are changes into old_bitmap

        std::cout << "\nAFTER RUNNING DIFF: " << std::endl;
        //game_instance.print_portion_bitmap(0,6,6,14,14);
        //game_instance.print_portion_bitmap(1,6,6,14,14);

        game_instance.rectangulize();                                   //puts in update_list all the things that need to be drawn
        game_instance.rect_to_bitmap(game_instance.linked_list, 0);     //updating old bitmap
        game_instance.rect_to_bitmap(game_instance.linked_list, 1);     //updates the new bitmap -- still need to clear old stuff!
        std::cout << "\nAFTER RECTUANGULARIZATION AND RESET: " << std::endl;
        //game_instance.print_portion_bitmap(0,6,6,14,14);
        //game_instance.print_portion_bitmap(1,6,6,14,14);

	/*std::cout << "\nTRYING TO PRINT UPDATE_LIST: " << std::endl;

        for(auto element: game_instance.update_list)
        {
                game_instance.display_trans(element);
        }

        std::cout << "\nTRING TO PRINT CLEAR_LIST: " << std::endl;

        for(auto element: game_instance.clear_list)
        {
                game_instance.display_trans(element);
        }

	while(1)
	{
	}
	*/

	//initialization finished - send to screen now!
	while(1)
	{
	game_instance.sendall(serial_port,joystick_x,joystick_y); 	//sends updated list and gets joystick input
	//THIS IS THE DEAD SPACE the TIVA is now waiting for a uart transmission interrupt
	game_instance.moveall(joystick_x,joystick_y); 			//Moves in linked_list all to_transmit blocks
	game_instance.check_collision(joystick_x,joystick_y);

	game_instance.rect_to_bitmap(game_instance.linked_list, 1);     //updates the new bitmap to hold where everything new is
	//std::cout << "\nMOVING WITH INPUT 0,5: " << std::endl;
	//game_instance.print_portion_bitmap(0,6,6,14,14);
        //game_instance.print_portion_bitmap(1,6,6,14,14);


	game_instance.diff(); 						//stores non-zero values where there are changes into old_bitmap
	
	//std::cout << "\nAFTER RUNNING DIFF: " << std::endl;
	//game_instance.print_portion_bitmap(0,6,6,14,14);
        //game_instance.print_portion_bitmap(1,6,6,14,14);

	game_instance.rectangulize(); 					//puts in update_list all the things that need to be drawn
	game_instance.rect_to_bitmap(game_instance.linked_list, 0); 	//updating old bitmap
	game_instance.rect_to_bitmap(game_instance.linked_list, 1);     //updates the new bitmap -- still need to clear old stuff!
	game_instance.GDirection(joystick_x,joystick_y);
	game_instance.Check_TB();

	game_instance.update_trainer();

	//std::cout << "\nAFTER RECTUANGULARIZATION AND RESET: " << std::endl;
        //game_instance.print_portion_bitmap(0,6,6,14,14);
        //game_instance.print_portion_bitmap(1,6,6,14,14);
	/*game_instance.clear_diff();
	std::cout << "\nAFTER CLEAR_DIFF: " << std::endl;
        game_instance.print_portion_bitmap(0,6,6,14,14);
        game_instance.print_portion_bitmap(1,6,6,14,14);
	game_instance.clear_rectangulize();
	std::cout << "\nAFTER CLEAR_RECTUANGULARIZATION: " << std::endl;
        game_instance.print_portion_bitmap(0,6,6,14,14);
        game_instance.print_portion_bitmap(1,6,6,14,14);
	game_instance.rect_to_bitmap(game_instance.linked_list, 0);     //updates the new bitmap -- everything should be equal again!
        game_instance.rect_to_bitmap(game_instance.linked_list, 1);     //updates the new bitmap -- everything should be equal again!
	std::cout << "\n AFTER FULL RESET: " << std::endl;
	game_instance.print_portion_bitmap(0,6,6,14,14);
        game_instance.print_portion_bitmap(1,6,6,14,14);*/
	}

	std::cout << "\nTRYING TO PRINT UPDATE_LIST: " << std::endl;
	
	for(auto element: game_instance.update_list)
	{
		game_instance.display_trans(element);
	}
	
	std::cout << "\nTRING TO PRINT CLEAR_LIST: " << std::endl;
	
	for(auto element: game_instance.clear_list)
        {
                game_instance.display_trans(element);
        }

	while(1)
	{
	}
	
	game_instance.sendall(serial_port,joystick_x,joystick_y);       //sends updated list and gets joystick input

	std::cout << "WE FINISHED NICELY" << std::endl;

	while(1)
	{
		
	}

        // Read data from the serial port.
        //std::string received_data;
        //serial_port.Read(received_data, 256); // Read up to 256 bytes.
        //std::cout << "Data received from the serial port: " << received_data << std::endl;
	std::vector<unsigned char> received_data(256);
	serial_port.Read(received_data, received_data.size());

        // Print received data.
        for (auto byte : received_data) {
            std::cout << byte;
        }

    return 0;
}

