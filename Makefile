all:
	config-pin p9.13 uart
	config-pin p9.11 uart
	g++ -std=c++11 -o uart_example -L/usr/lib/arm-linux-gnueabihf uart_example.cpp Mmap.cpp -lserial -lpthread
tile:
	g++ -o fetch_tile map_data.cpp -lcurl