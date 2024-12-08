#include <SerialPort.h>
#include <SerialStream.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include "spi.h"
#include "Mmap.h"

int main() {
    using namespace LibSerial;

    srand((unsigned) time(NULL));
    int random_gps = rand();
    std::cout << random_gps;
	return 0;

    const std::string serial_port_name = "/dev/ttyO4"; // Replace with your UART port
    const int baud_rate = 9600;

    try {
        // Create and open the serial port.
        SerialPort serial_port(serial_port_name);
        serial_port.Open(SerialPort::BAUD_115200,
                         SerialPort::CHAR_SIZE_8,
                         SerialPort::PARITY_NONE,
                         SerialPort::STOP_BITS_1,
                         SerialPort::FLOW_CONTROL_NONE);

        // Configure the serial port.
        //serial_port.SetBaudRate(BaudRate::BAUD_9600);
        //serial_port.SetCharacterSize(CharacterSize::CHAR_SIZE_8);
        //serial_port.SetParity(Parity::PARITY_NONE);
        //serial_port.SetStopBits(StopBits::STOP_BITS_1);
        //serial_port.SetFlowControl(FlowControl::FLOW_CONTROL_NONE);

        // Check if the serial port is open.
        if (!serial_port.IsOpen()) {
            std::cerr << "Failed to open the serial port." << std::endl;
            return 1;
        }

        // Write data to the serial port.
	Mmap game_instance = Mmap();
	game_instance.add_list(2,0,0,248,318,ST7789_BLACK,2);
	game_instance.add_list(1,100,125,10,20,ST7789_BLUE,0);
	game_instance.add_list(1,50,50,30,30,ST7789_RED,1);
	game_instance.add_list(1,75,30,5,5,ST7789_GREEN,1);
        const std::string data_to_write = "Hello, UART!";
	const std::string data_to_write_i = std::string(1,char(0x2A));
	while(1)
	{
	game_instance.sendall(serial_port);
	game_instance.check_collision();

        //std::cout << "Data written to the serial port: " << data_to_write << std::endl;
	//std::vector<unsigned char> received_data(3);
	//serial_port.Read(received_data, received_data.size());
	//std::cout << "\nI got this back: ";
	//for(int i=0; i<3;i++)
	//{ 
	//	std::cout << received_data[i];
	//}
	//game_instance.moveall(received_data[1],received_data[2]);
//	usleep(1000000);
	}
        // Wait a moment for the response.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

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

    } catch (const std::exception& ex) {
        // Catch and handle exceptions.
        std::cerr << "An error occurred: " << ex.what() << std::endl;
        return 1;
    }

        // Close the serial port.
    //serial_port.Close();
    return 0;
}

