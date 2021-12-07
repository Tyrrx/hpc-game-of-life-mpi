//
// Created by davidr on 21.11.21.
//

#include "vec2.h"

struct Vec2i new_vec2i(const int x1, const int x2) {
    struct Vec2i element;
    element.x1 = x1;
    element.x2 = x2;
    return element;
}

struct Vec2d new_vec2d(double x1, double x2) {
    struct Vec2d result;
    result.x1 = x1;
    result.x2 = x2;
    return result;
}

struct Vec2i multiply(struct Vec2i a, struct Vec2i b) {
    struct Vec2i result;
    result.x1 = a.x1 * b.x1;
    result.x2 = a.x2 * b.x2;
    return result;
}

struct Vec2i add(struct Vec2i a, struct Vec2i b)
{
    struct Vec2i result;
    result.x1 = a.x1 + b.x1;
    result.x2 = a.x2 + b.x2;
    return result;
}
