////
//// Created by davidr on 04.12.21.
////
//
//#ifndef HPC_GAME_OF_LIFE_MPI_GAME_H
//#define HPC_GAME_OF_LIFE_MPI_GAME_H
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//
//#include <math.h>
//#include <sys/time.h>
//
//#include "kernel.h"
//
//#define calcIndex(width, x, y)  ((y)*(width) + (x))
//
//void evolve(long time_step, const int *current_field, int *new_field, struct Vec2i field_size, struct Vec2i part_size, struct Vec2i threads, struct Kernel2d kernel);
//
//int is_alive(const int *field, int w, int h, int x, int y);
//
//int calc_surroundings(const int *field, int w, int h, int x, int y, struct Kernel2d kernel);
//
//void apply_rules(const int *current_field, int *new_field, int w, int h, int y, int x, int surroundings);
//
//void game(const long steps, struct Vec2i part_size, struct Vec2i threads);
//
//#endif //HPC_GAME_OF_LIFE_MPI_GAME_H
