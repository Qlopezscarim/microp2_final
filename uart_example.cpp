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




int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <your_string>" << std::endl;
        return 1; // Exit with error code
    }
    using namespace LibSerial;

    //srand((unsigned) time(NULL));
    //random_gps = std::randint();
    //random_gps = random.randint(1, 100)
    //std::cout << "\n" << random_gps;

    const std::string serial_port_name = "/dev/ttyO4"; // Replace with your UART port
    const int baud_rate = 9600;


        // Create and open the serial port.
        SerialPort serial_port(serial_port_name);
        serial_port.Open(SerialPort::BAUD_115200,
                         SerialPort::CHAR_SIZE_8,
                         SerialPort::PARITY_NONE,
                         SerialPort::STOP_BITS_1,
                         SerialPort::FLOW_CONTROL_HARD);

        // Check if the serial port is open.
        if (!serial_port.IsOpen()) {
            std::cerr << "Failed to open the serial port." << std::endl;
            return 1;
        }

        // Write data to the serial port.
	std::string name = argv[1];
	Mmap game_instance = Mmap(random_gps,name);

	//game_instance.start_screen(serial_port);

	/*int rand_pokemon = 9;

	std::ofstream file("users/" + std::string("defualt") + ".txt", std::ios::app);
        if (!file) {
                         std::cerr << "Unable to open or create file!" << std::endl;
                   }
        file << rand_pokemon << std::endl;
        file.close();*/
	
	game_instance.add_update_list(3,1,0,0,0,0,0);
	game_instance.Background();
	game_instance.Terrain2((240/2)-15-21, (320/2)-5-16, 1);
	game_instance.SimpleWeed(90,100,1);
	game_instance.SimpleWeed(130,30,1);


	char joystick_x = 5;
	char joystick_y = 5;
	char sw2 = '0';
	char sw1 = '0';

	//safe to clear context here!
	game_instance.save_context();
        game_instance.restore_context();

	game_instance.rect_to_bitmap(game_instance.linked_list, 1); //updating new bitmap

        game_instance.diff();                                           //stores non-zero values where there are changes into old_bitmap

        game_instance.rectangulize();                                   //puts in update_list all the things that need to be drawn
        game_instance.rect_to_bitmap(game_instance.linked_list, 0);     //updating old bitmap
        game_instance.rect_to_bitmap(game_instance.linked_list, 1);     //updates the new bitmap -- still need to clear old stuff!
        std::cout << "\nAFTER RECTUANGULARIZATION AND RESET: " << std::endl;
	
//	game_instance.save_context();
//	game_instance.restore_context();


	//initialization finished - send to screen now!
	while(1)
	{
	game_instance.sendall(serial_port,joystick_x,joystick_y,sw1,sw2); 	//sends updated list and gets joystick input
	//THIS IS THE DEAD SPACE the TIVA is now waiting for a uart transmission interrupt
	game_instance.moveall(joystick_x,joystick_y); 			//Moves in linked_list all to_transmit blocks
	bool is_encounter = game_instance.check_collision(joystick_x,joystick_y);

	if(is_encounter == true)
	{
		game_instance.save_context();
		game_instance.encounter(serial_port);
        	game_instance.restore_context();
	}
	else if(sw2 == '1')
	{
		game_instance.save_context();
		game_instance.pokedex(serial_port);
		game_instance.restore_context();
	}

	game_instance.rect_to_bitmap(game_instance.linked_list, 1);     //updates the new bitmap to hold where everything new is


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
	
	//game_instance.sendall(serial_port,joystick_x,joystick_y);       //sends updated list and gets joystick input

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

