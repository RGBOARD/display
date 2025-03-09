//
// Created by evalentin on 09/03/25.
//

#include "../include/display.h"

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;


void DrawOnCanvas(Canvas *canvas) {
    /*
     * Let's create a simple animation. We use the canvas to draw
     * pixels. We wait between each step to have a slower animation.
     */
    canvas->Fill(0, 255, 0);

    int center_x = canvas->width() / 2;
    int center_y = canvas->height() / 2;
    float radius_max = canvas->width() / 2;
    float angle_step = 1.0 / 360;
    for (float a = 0, r = 0; r < radius_max; a += angle_step, r += angle_step) {
        float dot_x = cos(a * 2 * M_PI) * r;
        float dot_y = sin(a * 2 * M_PI) * r;
        canvas->SetPixel(center_x + dot_x, center_y + dot_y, 0, 0, 40);
        usleep(1 * 1000); // wait a little to slow down things.
    }
}