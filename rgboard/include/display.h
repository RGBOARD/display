//
// Created by evalentin on 09/03/25.
//

#ifndef DISPLAY_H
#define DISPLAY_H

#include "led-matrix.h"

#include <unistd.h>
#include <signal.h>
#include <math.h>

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;
void DrawOnCanvas(Canvas *canvas);
#endif //DISPLAY_H
