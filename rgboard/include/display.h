//
// Created by evalentin on 09/03/25.
//

#ifndef DISPLAY_H
#define DISPLAY_H

#include "led-matrix.h"

#include <jsoncpp/json/json.h> // JSON parsing

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;
void DrawDesignOnCanvas(Canvas* canvas, const Json::Value& pixel_data);
#endif //DISPLAY_H
