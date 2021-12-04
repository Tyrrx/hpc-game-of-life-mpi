//
// Created by davidr on 04.12.21.
//

#include "game.h"

//
// Created by davidr on 19.11.21.
//

void writeVTK2(const long time_step, const int *data, const char prefix[1024], const int w, const int h, struct Vec2i thread_origin, struct Vec2i part_size, int thread_num) {
    char filename[2048];
    int x, y;

    const int offsetX = thread_origin.x1;
    const int offsetY = thread_origin.x2;
    const float delta_x = 1.0;
    const long nxy = w * h * sizeof(float);

    snprintf(filename, sizeof(filename), "%s-%d-%05ld%s", prefix, thread_num, time_step, ".vti");
    FILE *fp = fopen(filename, "w");

    fprintf(fp, "<?xml version=\"1.0\"?>\n");
    fprintf(fp, "<VTKFile type=\"ImageData\" version=\"0.1\" byte_order=\"LittleEndian\" header_type=\"UInt64\">\n");
    fprintf(fp, "<ImageData WholeExtent=\"%d %d %d %d %d %d\" Origin=\"0 0 0\" Spacing=\"%le %le %le\">\n", offsetX,
            offsetX + w, offsetY, offsetY + h, 0, 0, delta_x, delta_x, 0.0);
    fprintf(fp, "<CellData Scalars=\"%s\">\n", prefix);
    fprintf(fp, "<DataArray type=\"Float32\" Name=\"%s\" format=\"appended\" offset=\"0\"/>\n", prefix);
    fprintf(fp, "</CellData>\n");
    fprintf(fp, "</ImageData>\n");
    fprintf(fp, "<AppendedData encoding=\"raw\">\n");
    fprintf(fp, "_");
    fwrite((unsigned char *) &nxy, sizeof(long), 1, fp);

    for (y = offsetY; y < offsetY + part_size.x2; y++) {
        for (x = offsetX; x < offsetX + part_size.x1; x++) {
            float value = data[calcIndex(h, x, y)];
            fwrite((unsigned char *) &value, sizeof(float), 1, fp);
        }
    }

    fprintf(fp, "\n</AppendedData>\n");
    fprintf(fp, "</VTKFile>\n");
    fclose(fp);
}


void show(const int *const current_field, const int w, const int h) {
    printf("\033[H");
    int x, y;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) printf(current_field[calcIndex(w, x, y)] ? "\033[07m  \033[m" : "  ");
        //printf("\033[E");
        printf("\n");
    }
    fflush(stdout);
}


void evolve(
        const long time_step,
        const int *const current_field,
        int *const new_field,
        const struct Vec2i field_size,
        const struct Vec2i part_size,
        const struct Vec2i threads,
        const struct Kernel kernel) {
    const int x_len = part_size.x1;
    const int y_len = part_size.x2;
    const int num_threads = threads.x1 * threads.x2;

    // todo change
    {
        const int this_thread = 0; //omp_get_thread_num(); todo change

        const int x_pos = this_thread % threads.x1;
        const int y_pos = this_thread / threads.x1;

        const int x_start = x_pos * x_len;
        const int y_start = y_pos * y_len;
        const int x_end = x_start + x_len;
        const int y_end = y_start + y_len;


//        printf("---------------------\n");
//        printf("%d\n", this_thread);
//        printf("x_start %d\n", x_start);
//        printf("x_end %d\n", x_end);
//        printf("y_start %d\n", y_start);
//        printf("y_end %d\n", y_end);
        writeVTK2(time_step, current_field, "gol", field_size.x1, field_size.x2, new_vec2i(x_start, y_start), part_size, this_thread); // todo fix

        for (int y = y_start; y < y_end; ++y) {
            for (int x = x_start; x < x_end; ++x) {
                const int surroundings = calc_surroundings(current_field, field_size.x1, field_size.x2, x, y, kernel);
                apply_rules(current_field, new_field, field_size.x1, field_size.x2, y, x, surroundings);
            }
        }
    }
}

void apply_rules(
        const int *const current_field,
        int *const new_field,
        const int w,
        const int h,
        const int y,
        const int x,
        const int surroundings) {
    int i = calcIndex(w, x, y);
    new_field[i] = current_field[i];
    const int alive = is_alive(current_field, w, h, x, y);
    if (!alive && surroundings == 3) {
        new_field[i] = 1;
    }
    if (alive && (surroundings == 2 || surroundings == 3)) {
        new_field[i] = 1;
    }
    if (alive && (surroundings == 0 || surroundings == 1 || surroundings > 3)) {
        new_field[i] = 0;
    }
}

void filling(int *const current_field, const int w, const int h) {
    for (int i = 0; i < h * w; i++) {
        current_field[i] = (rand() < RAND_MAX / 1) ? 1 : 0; ///< init domain randomly
    }
}

void game(const long steps, const struct Vec2i part_size, const struct Vec2i threads) {
    struct Vec2i field_size = multiply(part_size, threads);
    int *current_field = calloc(field_size.x1 * field_size.x2, sizeof(int));
    int *new_field = calloc(field_size.x1 * field_size.x2, sizeof(int));
    const struct Kernel kernel = create_kernel();

    // filling(current_field, field_size.x1, field_size.x2);

    current_field[field_size.x1+3] = 1;
    current_field[field_size.x1*2+3] = 1;
    current_field[field_size.x1*3+3] = 1;
    current_field[field_size.x1*3+2] = 1;
    current_field[field_size.x1*2+1] = 1;

    for (long t = 0; t < steps; t++) {

        //show(current_field, field_size.x1, field_size.x2);
        evolve(t, current_field, new_field, field_size, part_size, threads, kernel);
        // printf("%ld timestep\n", t);
        // writeVTK2(t,current_field,"gol", field_size.x1, field_size.x2);
        // usleep(200000);
        int *temp = current_field;
        current_field = new_field;
        new_field = temp;
    }
    free(current_field);
    free(new_field);

}

int is_alive(const int *const field, const int w, const int h, const int x, const int y) {
    if (x > w || x < 0) {
        return 0;
    }
    if (y > h || y < 0) {
        return 0;
    }
    if (field[calcIndex(w, x, y)] == 1) {
        return 1;
    }
    return 0;
}

int calc_surroundings(const int *const field, const int w, const int h, const int x, const int y,
                      const struct Kernel kernel) {
    int num_surroundings = 0;
    for (int i = 0; i < kernel.num_elements; ++i) {
        const int x1 = x + kernel.elements[i].x1;
        const int y1 = y + kernel.elements[i].x2;
        if (is_alive(field, w, h, x1, y1) == 1) {
            num_surroundings = num_surroundings + 1;
        }
    }
    return num_surroundings;
}

