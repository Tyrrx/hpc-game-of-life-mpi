//
// Created by davidr on 21.11.21.
//

#include "kernel.h"

struct Kernel2d create_kernel()
{
    const int num_elements = 8;
    struct Kernel2d kernel;
    kernel.num_elements = num_elements;
    kernel.elements = malloc(num_elements * sizeof(struct Vec2i));
    kernel.elements[0] = new_vec2i(-1, -1);
    kernel.elements[1] = new_vec2i(-1, 0);
    kernel.elements[2] = new_vec2i(-1, 1);
    kernel.elements[3] = new_vec2i(0, -1);
    kernel.elements[4] = new_vec2i(0, 1);
    kernel.elements[5] = new_vec2i(1, -1);
    kernel.elements[6] = new_vec2i(1, 0);
    kernel.elements[7] = new_vec2i(1, 1);
    return kernel;
}

struct Kernel1d translate_kernel_1d(struct Vec2i grid_size, struct Kernel2d kernel)
{
    struct Kernel1d kernel1d;
    kernel1d.elements = calloc(kernel.num_elements, sizeof(int));
    for (int i = 0; i < kernel.num_elements; ++i) {
        kernel1d.elements[i] = kernel.elements[i].x1 + kernel.elements[i].x2 * grid_size.x1;
    }
    return kernel1d;
}

void freeKernel1d(struct Kernel1d kernel)
{
    free(kernel.elements);
}

void freeKernel2d(struct Kernel2d kernel)
{
    free(kernel.elements);
}
