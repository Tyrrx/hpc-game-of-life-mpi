//
// Created by davidr on 04.12.21.
//


#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm communicator;
    int dims[] = { size };
    int periodic[] = { true };
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periodic, false, &communicator);

    int coords[1];
    MPI_Cart_coords(communicator, rank, 1, coords);

    int neighbor, source;
    MPI_Cart_shift(communicator, 0, 1, &source, &neighbor);
    printf("Rank %d Koordinaten %d Source %d Nachbar %d\n", rank, coords[0], source, neighbor);

    MPI_Finalize();
    return 0;
}
