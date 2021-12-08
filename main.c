#include <stdio.h>

#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vec2.h"
#include "kernel.h"

#define calcIndex(width, x, y)  ((y)*(width) + (x))

void printGrid(int rank, struct Vec2i *grid_size, const int *field_buffer);

void initGrid(int rank, struct Vec2i grid_size, int *field_buffer);

void writeVTK2(const long time_step, const int *data, const char prefix[1024], int rank, struct Vec2i origin,
               struct Vec2i small_size, struct Vec2i large_size)
{
    char filename[2048];


    const int offsetX = origin.x1;
    const int offsetY = origin.x2;
    const float delta_x = 1.0;
    const long nxy = small_size.x1 * small_size.x2 * sizeof(float);

    snprintf(filename, sizeof(filename), "%s-%d-%05ld%s", prefix, rank, time_step, ".vti");
    FILE *fp = fopen(filename, "w");

    fprintf(fp, "<?xml version=\"1.0\"?>\n");
    fprintf(fp, "<VTKFile type=\"ImageData\" version=\"0.1\" byte_order=\"LittleEndian\" header_type=\"UInt64\">\n");
    fprintf(fp, "<ImageData WholeExtent=\"%d %d %d %d %d %d\" Origin=\"0 0 0\" Spacing=\"%le %le %le\">\n", offsetX,
            offsetX + small_size.x1, offsetY, offsetY + small_size.x2, 0, 0, delta_x, delta_x, 0.0);
    fprintf(fp, "<CellData Scalars=\"%s\">\n", prefix);
    fprintf(fp, "<DataArray type=\"Float32\" Name=\"%s\" format=\"appended\" offset=\"0\"/>\n", prefix);
    fprintf(fp, "</CellData>\n");
    fprintf(fp, "</ImageData>\n");
    fprintf(fp, "<AppendedData encoding=\"raw\">\n");
    fprintf(fp, "_");
    fwrite((unsigned char *) &nxy, sizeof(long), 1, fp);

    for (int y = 0; y < small_size.x2; y++) {
        for (int x = 0; x < small_size.x1; x++) {
            float value = (float) data[calcIndex(large_size.x1, x + 1, y)];
            fwrite((unsigned char *) &value, sizeof(float), 1, fp);
        }
    }

    fprintf(fp, "\n</AppendedData>\n");
    fprintf(fp, "</VTKFile>\n");
    fclose(fp);
}

int main(int argc, char *argv[])
{
    // init mpi and get rank and size
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    // init the communicator for the grid
    MPI_Comm communicator;
    int dims[] = {size};
    int periodic[] = {true};
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periodic, false, &communicator);


    // define the local gird sizes
    struct Vec2i small = new_vec2i(25, 25);

    // calc the local grid size with ghosts
    struct Vec2i large = add(small, new_vec2i(2, 0));

    // calc the grid buffer size (with ghosts)
    const int large_size = large.x1 * large.x2;

    // the size of the grid with the ghost layers left and right
    const int large_sizes[] = {large.x1, large.x2};

    // the size of the ghost layer x and y
    const int size_ghost_y[] = {1, large.x2};

    // the start position of the left inner layer that will be sent to the left ghost layer
    const int start_left_inner[] = {1, 0};

    // the start position of the right inner layer that will be sent to the right ghost layer
    const int start_right_inner[] = {small.x1, 0};

    // start position of the left ghost layer inside the grid buffer
    const int start_left_ghost[] = {0, 0};

    // start position of the right ghost layer inside the grid buffer
    const int start_right_ghost[] = {small.x1 + 1, 0};


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


    // left right top bottom
    const int send_counts[] = {1, 1};
    const MPI_Aint send_dsp[] = {0, 0};
    const int rec_counts[] = {1, 1};
    const MPI_Aint rec_dsp[] = {0, 0};


    // group send types and receive types
    MPI_Datatype send_types[] = {left_inner, right_inner};
    MPI_Datatype rec_types[] = {left_ghost, right_ghost};


    // init buffers
    int *field_buffer = calloc(large_size, sizeof(int));
    int *field_buffer_swap = calloc(large_size, sizeof(int));


    //initGrid(rank, small, field_buffer);


    // create the kernel
    const struct Kernel2d kernel2d = create_kernel();
    const struct Kernel1d kernel = translate_kernel_1d(large, kernel2d);

    if(rank == 0) {
        field_buffer[large.x1 + 3] = 1;
        field_buffer[large.x1 * 2 + 3] = 1;
        field_buffer[large.x1 * 3 + 3] = 1;
        field_buffer[large.x1 * 3 + 2] = 1;
        field_buffer[large.x1 * 2 + 1] = 1;
    }

    // do the steps
    for (int step = 0; step < 200; ++step) {
        // define buffers that track the changes (spawn/die)
        int local_changes = 0;
        int global_changes = 0;

        // send the layers
        // todo async send and iterate over inner fields then wait for all completed and iterate over the ghost for better communication hiding
        // todo use ISend and IRecv

        // get coords in topology
        int coords[1];
        MPI_Cart_coords(communicator, rank, 1, coords);
        // calculate the absolute position inside the grid
        struct Vec2i absolute_origin = multiply(small, new_vec2i(coords[0], 0));
        writeVTK2(step, field_buffer, "golmpi", rank, absolute_origin, small, large);

        // exchange ghost layers
        MPI_Neighbor_alltoallw(field_buffer, send_counts, send_dsp, send_types, field_buffer, rec_counts, rec_dsp,
                               rec_types, communicator);

        // iterate over y first for cache alignment
        for (int y = 0; y < small.x2; ++y) {

            // calculate the y0 position inside the 1d array
            const int iy = large.x2 * y;

            for (int x = 0; x < small.x1; ++x) {
                int surroundings = 0;

                // shift the x position by one and add to y0
                const int ixy = iy + x + 1;

                // count the surroundings
                for (int i = 0; i < kernel.num_elements; ++i) {
                    const int ixy_k = ixy + kernel.elements[i];
                    if (field_buffer[(ixy_k + large_size) % large_size] == 1) {
                        surroundings++;
                    }
                }

                // copy the old value
                field_buffer_swap[ixy] = field_buffer[ixy];

                const int alive = field_buffer[ixy];
                // ------------------------------------- spawn = changes
                if (!alive && surroundings == 3) {
                    field_buffer_swap[ixy] = 1;
                    local_changes = 1;
                }
                // do nothing?
                if (alive && (surroundings == 2 || surroundings == 3)) {
                    field_buffer_swap[ixy] = 1;
                }
                // ------------------------------------- die = changes
                if (alive && (surroundings == 0 || surroundings == 1 || surroundings > 3)) {
                    field_buffer_swap[ixy] = 0;
                    local_changes = 1;
                }

            }
        }

        // swap buffers
        int *temp = field_buffer;
        field_buffer = field_buffer_swap;
        field_buffer_swap = temp;

        // distribute local changes to all ranks and exit if none
        // todo works as a barrier?
        MPI_Barrier(communicator);
        MPI_Allreduce(&local_changes, &global_changes, 1, MPI_INT, MPI_SUM, communicator);
        if (global_changes == 0) {
            printf("Rank:%d Step:%d exited with no changes\n", rank, step);
            break;
        }
        //printGrid(rank, &small, field_buffer);

    }

//    MPI_Type_free(&left_ghost);
//    MPI_Type_free(&left_inner);
//    MPI_Type_free(&right_ghost);
//    MPI_Type_free(&right_inner);
//    free(field_buffer);
//    free(field_buffer_swap);
//    freeKernel1d(kernel);
//    freeKernel2d(kernel2d);
    MPI_Finalize();
    return 0;
}

void initGrid(int rank, struct Vec2i grid_size, int *field_buffer)
{
    for (int y = 0; y < grid_size.x2; ++y) {
        for (int x = 1; x < grid_size.x1 + 1; ++x) {
            if (y == x - 1) {
                field_buffer[calcIndex(grid_size.x1, x, y)] = 1;
            }
        }
    }
}

void printGrid(int rank, struct Vec2i *grid_size, const int *field_buffer)
{
    char out[2000], *put = out;
    put += sprintf(put, "R%d-----------------------------------------------\n", rank);
    for (int y = 0; y < (*grid_size).x2; ++y) {
        for (int x = 0; x < (*grid_size).x1 + 2; ++x) {
            put += sprintf(put, "%d", field_buffer[calcIndex((*grid_size).x1 + 2, x, y)]);
        }
        put += sprintf(put, "\n");
    }
    printf(out);
}
