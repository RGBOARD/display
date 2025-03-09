//
// Created by evalentin on 08/03/25.
//

#define ROWS 32
#define COLS 64
#define PARALLEL 2
#define HARDWARE "regular"
#define GPIO_SLOWDOWN 4

#include  "display.h"


int main(int argc, char *argv[]){
    RGBMatrix::Options defaults;
    rgb_matrix::RuntimeOptions runtime_options;

    defaults.hardware_mapping = HARDWARE;  // or e.g. "adafruit-hat"
    defaults.rows = ROWS;
    defaults.cols = COLS;
    defaults.parallel = PARALLEL;

    runtime_options.gpio_slowdown = GPIO_SLOWDOWN;

    Canvas *canvas = RGBMatrix::CreateFromOptions(defaults, runtime_options);

    if (canvas == NULL)
        return 1;


    DrawOnCanvas(canvas);    // Using the canvas.

    // Animation finished. Shut down the RGB matrix.
    canvas->Clear();
    delete canvas;

    return 0;
    return 0;
}