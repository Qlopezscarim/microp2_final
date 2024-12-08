#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>

// Callback function to write data to a file
size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

// Function to fetch a Stamen map tile and save it as an image
void fetch_tile(int zoom, int x, int y, const std::string& style) {
    // Construct the URL for the tile
    std::string url = "https://a.tile.stamen.com/" + style + "/" + std::to_string(zoom) + "/" + std::to_string(x) + "/" + std::to_string(y) + ".png";

    // Initialize libcurl
    CURL* curl;
    FILE* fp;
    CURLcode res;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        fp = fopen("tile.png", "wb");
        if (fp == nullptr) {
            std::cerr << "Failed to open file for writing\n";
            return;
        }

        // Set the URL to fetch
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        // Perform the request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "Error fetching tile: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Tile saved as tile.png\n";
        }

        // Clean up
        fclose(fp);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

int main() {
    // Example: Fetch tile at zoom level 10, x=512, y=384, using the "toner" style
    fetch_tile(10, 512, 384, "toner");
    return 0;
}

