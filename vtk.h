//
// Created by davidr on 10.12.21.
//

#ifndef HPC_GAME_OF_LIFE_MPI_VTK_H
#define HPC_GAME_OF_LIFE_MPI_VTK_H

#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vec2.h"

void writeSingleFile(const int *field_buffer, int rank, MPI_Comm comm, int timestep, struct Vec2i nxy, struct Vec2i Nxy,
                     struct Vec2i origin, struct Vec2i field_size, char prefix[10]);

void writeVTK2(long time_step, const int *data, const char prefix[1024], int rank, struct Vec2i origin,
               struct Vec2i small_size, struct Vec2i large_size);

#endif //HPC_GAME_OF_LIFE_MPI_VTK_H
