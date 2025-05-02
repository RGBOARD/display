//
// Created by evalentin on 08/03/25.
//

#define ROWS 32
#define COLS 64
#define PARALLEL 2
#define HARDWARE "regular"
#define GPIO_SLOWDOWN 4

#include "queue-client.h"

int main(int argc, char *argv[]){
    auto client = QueueClient("abc", "123");
    return 0;
}