#include <iostream>
#include <fstream>
#include <cmath>
#include <curl/curl.h>

// Callback function to write data received from CURL to a file
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::ofstream* outFile = (std::ofstream*)userp;
    outFile->write((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to download a tile from the OSM tile server
void downloadTile(int zoom, int x, int y, const std::string& savePath) {
    // OpenStreetMap tile URL
    std::string url = "https://tile.openstreetmap.org/" + 
                      std::to_string(zoom) + "/" + 
                      std::to_string(x) + "/" + 
                      std::to_string(y) + ".png";

    // Initialize libcurl
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL." << std::endl;
        return;
    }

    // Set CURL options
    std::ofstream outFile(savePath, std::ios::binary); // File to save the tile
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "Downloaded tile: " << savePath << std::endl;
    }

    // Clean up
    curl_easy_cleanup(curl);
    outFile.close();
}

// Function to convert latitude and longitude to tile indices
void latLonToTile(double lat, double lon, int zoom, int& x, int& y) {
    x = static_cast<int>(std::floor((lon + 180.0) / 360.0 * (1 << zoom)));
    y = static_cast<int>(std::floor((1.0 - std::log(std::tan(lat * M_PI / 180.0) + 
                              1.0 / std::cos(lat * M_PI / 180.0)) / M_PI) / 2.0 * (1 << zoom)));
}

int main() {
    // Example GPS coordinates (latitude and longitude)
    double latitude = 37.7749;  // San Francisco
    double longitude = -122.4194;

    // Zoom level
    int zoom = 10;

    // Convert GPS coordinates to tile indices
    int tileX, tileY;
    latLonToTile(latitude, longitude, zoom, tileX, tileY);

    // Construct save path for the downloaded tile
    std::string savePath = "tile_" + std::to_string(zoom) + "_" + 
                           std::to_string(tileX) + "_" + 
                           std::to_string(tileY) + ".png";

    // Download the tile
    downloadTile(zoom, tileX, tileY, savePath);

    return 0;
}

