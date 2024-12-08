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
        serial_port.Open(SerialPort::BAUD_115200,
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
	
	//This correlates with a read rectangle at 50,50 with a width of 40 and height of 40
	game_instance.add_list(1,50,50,40,20,ST7789_RED,0); //void add_list(uint8_t type, uint16_t x_start, uint16_t y_start, uint16_t w, uint16_t h, uint16_t color, uint8_t id);
	game_instance.add_list(1,100,90,40,20,ST7789_GREEN,0);
	game_instance.add_list(1,140,100,40,20,ST7789_GREEN,0);
	game_instance.add_list(1,180,100,40,20,ST7789_RED,0);
	game_instance.add_list(1,100,30,40,20,ST7789_GREEN,0);
	game_instance.add_list(1,20,180,40,20,ST7789_RED,0);
	game_instance.add_list(1,90,120,40,20,ST7789_GREEN,0);
	game_instance.add_list(1,200,140,40,20,ST7789_RED,0);
	game_instance.add_list(1,10,200,40,20,ST7789_GREEN,0);
	//game_instance.add_list(1,100-3,100,3,40,ST7789_RED,0);


	//game_instance.random_rect();

	char joystick_x = 5;
	char joystick_y = 5;

	game_instance.init_screen(serial_port);

	while(1)
	{
	game_instance.sendall(serial_port,joystick_x,joystick_y);
	//THIS IS THE DEAD SPACE the TIVA is now waiting for a uart transmission interrupt
	game_instance.moveall(joystick_x,joystick_y);
	
	//game_instance.random_rect();
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

