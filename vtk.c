//
// Created by davidr on 10.12.21.
//

#include "vtk.h"

void writeSingleFile(const int *field_buffer, int rank, MPI_Comm comm, int timestep, struct Vec2i nxy, struct Vec2i Nxy,
                     struct Vec2i origin, struct Vec2i field_size, char prefix[10])
{
    char filename[2024];

    double deltax = 1.0;

    snprintf(filename, sizeof(filename), "%s-%05d.vti", prefix, timestep);

    char buffer[2048];
    int headerLength = 0;
    headerLength += snprintf(buffer + headerLength, 50, "<?xml version=\"1.0\"?>\n");
    headerLength += snprintf(buffer + headerLength, 512,
                             "<VTKFile type=\"ImageData\" version=\"0.1\" byte_order=\"LittleEndian\" header_type=\"UInt64\">\n");
    headerLength += snprintf(buffer + headerLength, 512,
                             "<ImageData WholeExtent=\"%d %d %d %d %d %d\" Origin=\"0 0 0\" Spacing=\"%le %le %le\">\n",
                             0,
                             field_size.x1, 0, field_size.x2, 0, 0, deltax, deltax, 0.0);
    headerLength += snprintf(buffer + headerLength, 512, "<CellData Scalars=\"%s\">\n", prefix);

    headerLength += snprintf(buffer + headerLength, 512,
                             "<DataArray type=\"UInt64\" Name=\"%s\" format=\"appended\" offset=\"0\"/>\n", prefix);
    headerLength += snprintf(buffer + headerLength, 30, "</CellData>\n");
    headerLength += snprintf(buffer + headerLength, 30, "</ImageData>\n");
    headerLength += snprintf(buffer + headerLength, 60, "<AppendedData encoding=\"raw\">\n");
    headerLength += snprintf(buffer + headerLength, 2, "_");

    MPI_File file;
    MPI_File_open(comm, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

    long n_nxy = field_size.x1 * field_size.x2 * sizeof(int);

    if (rank == 0) {
        MPI_File_write_at(file, 0, buffer, (int) (headerLength), MPI_CHAR, MPI_STATUS_IGNORE);
        MPI_File_write_at(file, headerLength, (unsigned char *) &n_nxy, sizeof(long), MPI_CHAR, MPI_STATUS_IGNORE);
    }

    headerLength += sizeof(long);

    int nxy_sizes[] = {nxy.x1, nxy.x2};
    int Nxy_sizes[] = {Nxy.x1, Nxy.x2};
    int field_sizes[] = {field_size.x1, field_size.x2};
    int start_field[] = {origin.x1, origin.x2};

    MPI_Datatype full_field_filetype;
    {
        MPI_Type_create_subarray(2, field_sizes, nxy_sizes, start_field, MPI_ORDER_FORTRAN, MPI_INT, &full_field_filetype);
        MPI_Type_commit(&full_field_filetype);
    }

    MPI_Datatype local_field_filetype;
    {
        int start_indices[2] = {1, 1};
        MPI_Type_create_subarray(2, Nxy_sizes, nxy_sizes, start_indices, MPI_ORDER_FORTRAN, MPI_INT, &local_field_filetype);
        MPI_Type_commit(&local_field_filetype);
    }

    MPI_File_set_view(file, headerLength, MPI_INT, full_field_filetype, "native", MPI_INFO_NULL);
    MPI_File_write_all(file, field_buffer, 1, local_field_filetype, MPI_STATUS_IGNORE);

    if (rank == 0) {
        char buf[100];
        int l = snprintf(buf, 100, "\n</AppendedData>\n</VTKFile>\n");
        MPI_File_write_at(file, headerLength + n_nxy, buf, l, MPI_CHAR, MPI_STATUS_IGNORE);
    }
    MPI_File_close(&file);
}

