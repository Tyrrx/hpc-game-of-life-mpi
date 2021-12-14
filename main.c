#include <stdio.h>

#include <mpi.h>
#include <stdbool.h>
#include <stdlib.h>
#include "vec2.h"
#include "kernel.h"
#include "vtk.h"

#define calcIndex(width, x, y)  ((y)*(width) + (x))

int evolve(struct Vec2i *nxy, struct Vec2i *Nxy, const int *field_buffer, int *field_buffer_swap,
           struct Kernel2d *kernel2D)
{
    int local_changes;
    for (int y = 1; y < (*nxy).x2 + 1; ++y) {
        for (int x = 1; x < (*nxy).x1 + 1; ++x) {

            int surroundings = 0;
            for (int i = 0; i < (*kernel2D).num_elements; ++i) {
                struct Vec2i element = (*kernel2D).elements[i];
                int xk = element.x1 + x;
                int yk = element.x2 + y;
                // todo optimize by pre calculation of the kernel offsets
                if (field_buffer[calcIndex((*Nxy).x1, xk, yk)] == 1) {
                    surroundings++;
                }
            }
            long index = calcIndex((*Nxy).x1, x, y);
            field_buffer_swap[index] = field_buffer[index];

            int alive = field_buffer[index];
            if (!alive && surroundings == 3) {
                field_buffer_swap[index] = 1;
                local_changes = 1;
            }
            // ------------------------------------- die = changes
            if (alive && (surroundings == 0 || surroundings == 1 || surroundings > 3)) {
                field_buffer_swap[index] = 0;
                local_changes = 1;
            }
        }
    }
    return local_changes;
}

void init_mpi_data_types(struct Vec2i *nxy, const struct Vec2i *process_origin, const struct Vec2i *full_field_size,
                         const struct Vec2i *Nxy, MPI_Datatype *left_inner, MPI_Datatype *right_inner,
                         MPI_Datatype *left_ghost, MPI_Datatype *right_ghost, MPI_Datatype *top_inner,
                         MPI_Datatype *bottom_inner, MPI_Datatype *top_ghost, MPI_Datatype *bottom_ghost,
                         MPI_Datatype *full_field_filetype, MPI_Datatype *field_filetype)
{
    const int full_field_sizes[] = {(*full_field_size).x1, (*full_field_size).x2};
    const int nxy_sizes[] = {(*nxy).x1, (*nxy).x2};
    const int Nxy_sizes[] = {(*Nxy).x1, (*Nxy).x2};

    const int size_ghost_y[] = {1, (*Nxy).x2};
    const int size_ghost_x[] = {(*Nxy).x1, 1};

    const int start_left_inner[] = {1, 0};
    const int start_right_inner[] = {(*Nxy).x1 - 2, 0};

    const int start_left_ghost[] = {0, 0};
    const int start_right_ghost[] = {(*Nxy).x1 - 1, 0};

    const int start_top_inner[] = {0, 1};
    const int start_bottom_inner[] = {0, (*Nxy).x2 - 2};

    const int start_top_ghost[] = {0, 0};
    const int start_bottom_ghost[] = {0, (*Nxy).x2 - 1};

    const int start_full_field[] = {(*process_origin).x1, (*process_origin).x2};
    const int start_field[2] = {1, 1};

    // define the layer types and commit them
    int order = MPI_ORDER_FORTRAN;
    MPI_Datatype old_type = MPI_INT;
    MPI_Type_create_subarray(2, Nxy_sizes, size_ghost_y, start_left_inner, order, old_type, left_inner);
    MPI_Type_commit(left_inner);
    MPI_Type_create_subarray(2, Nxy_sizes, size_ghost_y, start_right_inner, order, old_type, right_inner);
    MPI_Type_commit(right_inner);
    MPI_Type_create_subarray(2, Nxy_sizes, size_ghost_y, start_left_ghost, order, old_type, left_ghost);
    MPI_Type_commit(left_ghost);
    MPI_Type_create_subarray(2, Nxy_sizes, size_ghost_y, start_right_ghost, order, old_type, right_ghost);
    MPI_Type_commit(right_ghost);
    MPI_Type_create_subarray(2, Nxy_sizes, size_ghost_x, start_top_inner, order, old_type, top_inner);
    MPI_Type_commit(top_inner);
    MPI_Type_create_subarray(2, Nxy_sizes, size_ghost_x, start_bottom_inner, order, old_type, bottom_inner);
    MPI_Type_commit(bottom_inner);
    MPI_Type_create_subarray(2, Nxy_sizes, size_ghost_x, start_top_ghost, order, old_type, top_ghost);
    MPI_Type_commit(top_ghost);
    MPI_Type_create_subarray(2, Nxy_sizes, size_ghost_x, start_bottom_ghost, order, old_type, bottom_ghost);
    MPI_Type_commit(bottom_ghost);
    MPI_Type_create_subarray(2, full_field_sizes, nxy_sizes, start_full_field, order, old_type,
                             full_field_filetype);
    MPI_Type_commit(full_field_filetype);
    MPI_Type_create_subarray(2, Nxy_sizes, nxy_sizes, start_field, order, old_type, field_filetype);
    MPI_Type_commit(field_filetype);
}

int main(int argc, char *argv[])
{

    // init mpi and get rank and size
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // set default sizes
    struct Vec2i nxy = new_vec2i(25, 25);
    struct Vec2i pxy = new_vec2i(3, 3);
    int time_steps = 0;

    if (argc > 5) {
        time_steps = atoi(argv[1]);
        nxy.x1 = atoi(argv[2]);
        nxy.x2 = atoi(argv[3]);
        pxy.x1 = atoi(argv[4]);
        pxy.x2 = atoi(argv[5]);
        // todo read px and py from input
    }

    // check if the given Px and Py parameters match the actual size
    if (pxy.x1 * pxy.x2 != size) {
        printf("Size: %d does not match the given Px * Py: %d", size, pxy.x1 * pxy.x2);
        return 1;
    }

    const struct Vec2i full_field_size = multiply(pxy, nxy);
    const struct Vec2i Nxy = add(nxy, new_vec2i(2, 2));

    // -------------------------------- init the communicator for the grid
    MPI_Comm communicator;
    const int n_dims = 2;
    const int dims[] = {pxy.x1, pxy.x2};
    const int periodic[] = {true, true};
    MPI_Cart_create(MPI_COMM_WORLD, n_dims, dims, periodic, false, &communicator);


    // -------------------------------- get coords in topology
    int coords[n_dims];
    MPI_Cart_coords(communicator, rank, n_dims, coords);
    // calculate the absolute position inside the grid
    const struct Vec2i process_origin = multiply(nxy, new_vec2i(coords[0], coords[1]));

    // -------------------------------- define and init mpi types
    MPI_Datatype left_inner;
    MPI_Datatype right_inner;
    MPI_Datatype left_ghost;
    MPI_Datatype right_ghost;
    MPI_Datatype top_inner;
    MPI_Datatype bottom_inner;
    MPI_Datatype top_ghost;
    MPI_Datatype bottom_ghost;
    MPI_Datatype full_field_filetype;
    MPI_Datatype field_filetype;
    init_mpi_data_types(&nxy, &process_origin, &full_field_size, &Nxy, &left_inner, &right_inner, &left_ghost,
                        &right_ghost, &top_inner,
                        &bottom_inner, &top_ghost, &bottom_ghost, &full_field_filetype, &field_filetype);


    // -------------------------------- define send and rec arrays
    const int send_counts[] = {1, 1, 1, 1};
    const int rec_counts[] = {1, 1, 1, 1};
    // group send types and receive types
    MPI_Datatype send_types[] = {left_inner, right_inner, top_inner, bottom_inner};
    MPI_Datatype rec_types[] = {left_ghost, right_ghost, top_ghost, bottom_ghost};


    // -------------------------------- init buffers
    int *field_buffer = calloc(Nxy.x1 * Nxy.x2, sizeof(int));
    int *field_buffer_swap = calloc(Nxy.x1 * Nxy.x2, sizeof(int));

    // -------------------------------- init kernel
    struct Kernel2d kernel2D = create_kernel();

    if (rank == 0) {
        field_buffer[Nxy.x1 * 2 + 4] = 1;
        field_buffer[Nxy.x1 * 3 + 4] = 1;
        field_buffer[Nxy.x1 * 4 + 4] = 1;
        field_buffer[Nxy.x1 * 4 + 3] = 1;
        field_buffer[Nxy.x1 * 3 + 2] = 1;
    }

    // -------------------------------- game loop
    for (int step = 0; step < time_steps; ++step) {
        int local_changes = 0;
        int global_changes = 0;

        //writeMultipleFiles(step, field_buffer, "golmpi", rank, process_origin, nxy, Nxy);
        writeSingleFile(field_buffer, rank, communicator, step, full_field_size, "golmpi", full_field_filetype,
                        field_filetype);

        // -------------------------------- exchange ghost layers
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


        // -------------------------------- evolve
        local_changes = evolve(&nxy, &Nxy, field_buffer, field_buffer_swap, &kernel2D);

        // -------------------------------- swap buffers
        int *temp = field_buffer;
        field_buffer = field_buffer_swap;
        field_buffer_swap = temp;

        // -------------------------------- exit if no changes
        MPI_Allreduce(&local_changes, &global_changes, 1, MPI_INT, MPI_SUM, communicator);
        if (global_changes == 0) {
            printf("Rank:%d Step:%d exited with no changes\n", rank, step);
            break;
        }
    }
    // -------------------------------- finish
    MPI_Finalize();
    return 0;
}