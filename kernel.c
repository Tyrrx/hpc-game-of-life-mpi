//
// Created by davidr on 21.11.21.
//

#include "kernel.h"

struct Kernel create_kernel() {
    const int num_elements = 8;
    struct Kernel kernel;
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
