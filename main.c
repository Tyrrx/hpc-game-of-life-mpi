#include <stdio.h>

#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vec2.h"
#include "kernel.h"
#include "vtk.h"

#define calcIndex(width, x, y)  ((y)*(width) + (x))
#define N_DIMS 2

void printGrid(int rank, struct Vec2i grid_size, const int *field_buffer);

void initGrid(int rank, struct Vec2i grid_size, int *field_buffer);


int evolve(struct Vec2i *small, struct Vec2i *large, const int *field_buffer, int *field_buffer_swap,
           struct Kernel2d *kernel2D)
{
    int local_changes;
    for (int y = 1; y < (*small).x2 + 1; ++y) {
        for (int x = 1; x < (*small).x1 + 1; ++x) {

            int surroundings = 0;
            for (int i = 0; i < (*kernel2D).num_elements; ++i) {
                struct Vec2i element = (*kernel2D).elements[i];
                int xk = element.x1 + x;
                int yk = element.x2 + y;
                // todo optimize by pre calculation of the kernel offsets
                if (field_buffer[calcIndex((*large).x1, xk, yk)] == 1) {
                    surroundings++;
                }
            }
            long index = calcIndex((*large).x1, x, y);
            field_buffer_swap[index] = field_buffer[index];

            int alive = field_buffer[index];
            if (!alive && surroundings == 3) {
                field_buffer_swap[index] = 1;
                local_changes = 1;
            }
            // do nothing?
//                if (alive && (surroundings == 2 || surroundings == 3)) {
//                    field_buffer_swap[index] = 1;
//                }
            // ------------------------------------- die = changes
            if (alive && (surroundings == 0 || surroundings == 1 || surroundings > 3)) {
                field_buffer_swap[index] = 0;
                local_changes = 1;
            }
        }
    }
    return local_changes;
}

int main(int argc, char *argv[])
{

    // init mpi and get rank and size
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    struct Vec2i nxy = new_vec2i(25, 25);
    struct Vec2i pxy = new_vec2i(3, 3);

    if (argc > 2) {
        nxy.x1 = atoi(argv[1]);
        nxy.x2 = atoi(argv[2]);
        // todo read px and py from input
    }

    // check if the given Px and Py parameters match the actual size
    if (pxy.x1 * pxy.x2 != size) {
        printf("Size: %d does not match the given Px * Py: %d", size, pxy.x1 * pxy.x2);
        return 1;
    }

    // init the communicator for the grid
    MPI_Comm communicator;
    int n_dims = N_DIMS;
    int dims[N_DIMS] = {pxy.x1, pxy.x2};
    int periodic[N_DIMS] = {true, true};
    MPI_Cart_create(MPI_COMM_WORLD, n_dims, dims, periodic, false, &communicator);


    struct Vec2i full_field_size = multiply(pxy, nxy);
    struct Vec2i Nxy = add(nxy, new_vec2i(2, 2));
    const int large_sizes[] = {Nxy.x1, Nxy.x2};

    const int size_ghost_y[] = {1, Nxy.x2};
    const int size_ghost_x[] = {Nxy.x1, 1};

    const int start_left_inner[] = {1, 0};
    const int start_right_inner[] = {Nxy.x1 - 2, 0};

    const int start_left_ghost[] = {0, 0};
    const int start_right_ghost[] = {Nxy.x1 - 1, 0};

    const int start_top_inner[] = {0, 1};
    const int start_bottom_inner[] = {0, Nxy.x2 - 2};

    const int start_top_ghost[] = {0, 0};
    const int start_bottom_ghost[] = {0, Nxy.x2 - 1};


    // define the layer types and commit them
    MPI_Datatype left_inner;
    MPI_Type_create_subarray(2, large_sizes, size_ghost_y, start_left_inner, MPI_ORDER_FORTRAN, MPI_INT, &left_inner);
    MPI_Type_commit(&left_inner);

    MPI_Datatype right_inner;
    MPI_Type_create_subarray(2, large_sizes, size_ghost_y, start_right_inner, MPI_ORDER_FORTRAN, MPI_INT, &right_inner);
    MPI_Type_commit(&right_inner);

    MPI_Datatype left_ghost;
    MPI_Type_create_subarray(2, large_sizes, size_ghost_y, start_left_ghost, MPI_ORDER_FORTRAN, MPI_INT, &left_ghost);
    MPI_Type_commit(&left_ghost);

    MPI_Datatype right_ghost;
    MPI_Type_create_subarray(2, large_sizes, size_ghost_y, start_right_ghost, MPI_ORDER_FORTRAN, MPI_INT, &right_ghost);
    MPI_Type_commit(&right_ghost);

    MPI_Datatype top_inner;
    MPI_Type_create_subarray(2, large_sizes, size_ghost_x, start_top_inner, MPI_ORDER_FORTRAN, MPI_INT, &top_inner);
    MPI_Type_commit(&top_inner);

    MPI_Datatype bottom_inner;
    MPI_Type_create_subarray(2, large_sizes, size_ghost_x, start_bottom_inner, MPI_ORDER_FORTRAN, MPI_INT,
                             &bottom_inner);
    MPI_Type_commit(&bottom_inner);

    MPI_Datatype top_ghost;
    MPI_Type_create_subarray(2, large_sizes, size_ghost_x, start_top_ghost, MPI_ORDER_FORTRAN, MPI_INT, &top_ghost);
    MPI_Type_commit(&top_ghost);

    MPI_Datatype bottom_ghost;
    MPI_Type_create_subarray(2, large_sizes, size_ghost_x, start_bottom_ghost, MPI_ORDER_FORTRAN, MPI_INT,
                             &bottom_ghost);
    MPI_Type_commit(&bottom_ghost);


    // left right top bottom
    const int send_counts[] = {1, 1, 1, 1};
    const MPI_Aint send_dsp[] = {0, 0, 0, 0};
    const int rec_counts[] = {1, 1, 1, 1};
    const MPI_Aint rec_dsp[] = {0, 0, 0, 0};

    // group send types and receive types
    MPI_Datatype send_types[] = {left_inner, right_inner, top_inner, bottom_inner};
    MPI_Datatype rec_types[] = {left_ghost, right_ghost, top_ghost, bottom_ghost};


    // init buffers
    int *field_buffer = calloc(Nxy.x1 * Nxy.x2, sizeof(int));
    int *field_buffer_swap = calloc(Nxy.x1 * Nxy.x2, sizeof(int));


    // get coords in topology
    int coords[n_dims];
    MPI_Cart_coords(communicator, rank, n_dims, coords);
    // calculate the absolute position inside the grid
    struct Vec2i absolute_origin = multiply(nxy, new_vec2i(coords[0], coords[1])); // todo 2d topology: 2 dims


    struct Kernel2d kernel2D = create_kernel();


    if (rank == 0) {
        field_buffer[Nxy.x1 * 2 + 4] = 1;
        field_buffer[Nxy.x1 * 3 + 4] = 1;
        field_buffer[Nxy.x1 * 4 + 4] = 1;
        field_buffer[Nxy.x1 * 4 + 3] = 1;
        field_buffer[Nxy.x1 * 3 + 2] = 1;
    }

    //initGrid(rank, nxy, field_buffer);


    // -------------------------------------------------------------------- game loop
    for (int step = 0; step < 10; ++step) {
        int local_changes = 0;
        int global_changes = 0;

        //writeVTK2(step, field_buffer, "golmpi", rank, absolute_origin, nxy, Nxy);
        writeSingleFile(field_buffer, rank, communicator, step, nxy, Nxy, absolute_origin, full_field_size, "golmpi");

        // exchange ghost layers

        MPI_Request request[4 * n_dims];
        MPI_Status status[4 * n_dims];
        int positive, negative;
        int req = 0;
        for (int dim = 0, side = 0; dim < n_dims; ++dim) {
            MPI_Cart_shift(communicator, dim, 1, &positive, &negative);
            MPI_Isend(field_buffer, send_counts[side], send_types[side], positive, 0, communicator, &request[req++]);
            MPI_Irecv(field_buffer, rec_counts[side], rec_types[side], positive, 0, communicator, &request[req++]);
            ++side;
            MPI_Isend(field_buffer, send_counts[side], send_types[side], negative, 0, communicator, &request[req++]);
            MPI_Irecv(field_buffer, rec_counts[side], rec_types[side], negative, 0, communicator, &request[req++]);
            ++side;
            MPI_Waitall(4, &request[4 * dim], &status[4 * dim]);
        }


//        MPI_Neighbor_alltoallw(field_buffer, send_counts, send_dsp, send_types, field_buffer, rec_counts, rec_dsp,
//                               rec_types, communicator);

        local_changes = evolve(&nxy, &Nxy, field_buffer, field_buffer_swap, &kernel2D);
        //printGrid(rank, nxy, field_buffer);

        // swap buffers
        int *temp = field_buffer;
        field_buffer = field_buffer_swap;
        field_buffer_swap = temp;
        MPI_Allreduce(&local_changes, &global_changes, 1, MPI_INT, MPI_SUM, communicator);
        if (global_changes == 0) {
            printf("Rank:%d Step:%d exited with no changes\n", rank, step);
            //break;
        }
    }
    // -------------------------------------------------------------------- end game loop

    MPI_Finalize();
    return 0;
}

void initGrid(int rank, struct Vec2i grid_size, int *field_buffer)
{
    for (int y = 1; y < grid_size.x2 + 1; ++y) {
        for (int x = 1; x < grid_size.x1 + 1; ++x) {
            field_buffer[calcIndex(grid_size.x1 + 2, x, y)] = rank;
        }
    }
}

void printGrid(int rank, struct Vec2i grid_size, const int *field_buffer)
{
    char out[grid_size.x1 * grid_size.x2 + 2000], *put = out;
    put += sprintf(put, "R%d-----------------------------------------------\n", rank);
    for (int y = 0; y < grid_size.x2 + 2; ++y) {
        for (int x = 0; x < grid_size.x1 + 2; ++x) {
            put += sprintf(put, "%d", field_buffer[calcIndex(grid_size.x1 + 2, x, y)]);
        }
        put += sprintf(put, "\n");
    }
    printf(out);
}
