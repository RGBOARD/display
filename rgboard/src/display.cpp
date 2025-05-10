//
// Created by evalentin on 09/03/25.
//

#include "../include/display.h"
#include <string>              // std::string
#include <sstream>             // std::stringstream, std::istringstream
#include <iostream>            // std::cerr, std::cout


using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

void HexToRGB(const std::string& hex, int& r, int& g, int& b);

void DrawDesignOnCanvas(Canvas* canvas, const Json::Value& pixel_data)
{
    constexpr int GRID_SIZE = 8; // pixel data is 8x scaled
    canvas->Clear();

    for (const auto& key : pixel_data.getMemberNames())
    {
        int x = 0, y = 0;
        char comma;

        std::stringstream coord_stream(key);
        coord_stream >> x >> comma >> y;

        // downscale
        x /= GRID_SIZE;
        y /= GRID_SIZE;

        const std::string hex_color = pixel_data[key].asString();
        int r, g, b;
        HexToRGB(hex_color, r, g, b);

        if (x >= 0 && x < canvas->width() && y >= 0 && y < canvas->height())
        {
            canvas->SetPixel(x, y, r, g, b);
        }
    }
}


// Convert hex string to RGB integers
void HexToRGB(const std::string& hex, int& r, int& g, int& b)
{
    if (hex.length() == 7 && hex[0] == '#')
    {
        r = std::stoi(hex.substr(1, 2), nullptr, 16);
        g = std::stoi(hex.substr(3, 2), nullptr, 16);
        b = std::stoi(hex.substr(5, 2), nullptr, 16);
    }
    else
    {
        r = g = b = 0; // fallback to black
    }
}
