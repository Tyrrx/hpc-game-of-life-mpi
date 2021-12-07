//
// Created by davidr on 21.11.21.
//

#ifndef HPC_GAME_OF_LIFE_KERNEL_H
#define HPC_GAME_OF_LIFE_KERNEL_H

#include <stdlib.h>

#include "vec2.h"

struct Kernel2d {
    int num_elements;
    struct Vec2i *elements;
};

struct Kernel1d {
    int num_elements;
    int *elements;
};

struct Kernel2d create_kernel();
struct Kernel1d translate_kernel_1d(struct Vec2i grid_size, struct Kernel2d kernel);
void freeKernel1d(struct Kernel1d kernel);
void freeKernel2d(struct Kernel2d kernel);

#endif //HPC_GAME_OF_LIFE_KERNEL_H
