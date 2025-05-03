//
// Created by evalentin on 08/03/25.
//

#define ROWS 32
#define COLS 64
#define PARALLEL 2
#define HARDWARE "regular"
#define GPIO_SLOWDOWN 4

#include <unistd.h>

#include "queue-client.h"
#include "display.h"

int main(int argc, char* argv[])
{
    // Matrix configuration
    rgb_matrix::RGBMatrix::Options defaults;
    rgb_matrix::RuntimeOptions runtime_options;

    defaults.hardware_mapping = HARDWARE;
    defaults.rows = ROWS;
    defaults.cols = COLS;
    defaults.parallel = PARALLEL;

    runtime_options.gpio_slowdown = GPIO_SLOWDOWN;

    // Initialize matrix
    rgb_matrix::Canvas* canvas = rgb_matrix::CreateMatrixFromOptions(defaults, runtime_options);
    if (canvas == nullptr)
    {
        std::fprintf(stderr, "Failed to initialize matrix.\n");
        return 1;
    }

    // Authenticate with client
    QueueClient client("email", "password");

    // main loop: fetch + draw + wait
    while (true)
    {
        if (client.GetDesign())
        {
            const Json::Value& pixel_data = client.GetPixelData();
            int duration = client.GetDisplayDuration();

            DrawDesignOnCanvas(canvas, pixel_data);
            std::printf("Displaying design for %d seconds.\n", duration);
            sleep(duration);
        }
        else
        {
            std::fprintf(stderr, "Failed to get design. Retrying in 5 seconds...\n");
            sleep(5);
        }
    }

    return 0;
}
