//
// Created by davidr on 21.11.21.
//

#ifndef HPC_GAME_OF_LIFE_VEC2_H
#define HPC_GAME_OF_LIFE_VEC2_H

struct Vec2i {
    int x1;
    int x2;
};

struct Vec2i new_vec2i(int x1, int x2);

struct Vec2i multiply(struct Vec2i a, struct Vec2i b);
struct Vec2i add(struct Vec2i a, struct Vec2i b);

struct Vec2d {
    double x1;
    double x2;
};

struct Vec2d new_vec2d(double x1, double x2);


#endif //HPC_GAME_OF_LIFE_VEC2_H
