//
// Created by davidr on 21.11.21.
//

#ifndef HPC_GAME_OF_LIFE_KERNEL_H
#define HPC_GAME_OF_LIFE_KERNEL_H

#include <stdlib.h>

#include "vec2.h"

struct Kernel {
    int num_elements;
    struct Vec2i *elements;
};

struct Kernel create_kernel();

#endif //HPC_GAME_OF_LIFE_KERNEL_H
