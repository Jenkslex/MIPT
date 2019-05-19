#include <stdio.h>
#include <x86intrin.h>

const int offset           = 4  * 1024 * 1024;
const int data_size        = 16 * 1024;
const int max_pieces_count = 32;


static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi = 0, lo = 0;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
}

void init_array(int *array, int pieces_count) {
    const int piece_size = data_size / pieces_count;

    for (int i = 0; i <= pieces_count; i++)
        for (int j = 0; j < piece_size; j++)
            array[i * offset + j] = (i == pieces_count - 1)? j + 1 : (i + 1) * offset + j;

    array[(pieces_count - 1) * offset + piece_size - 1] = 0;
}

long double process_array(int *array) {
    unsigned long long start = 0, end = 0;
    int nextIndex = 1;

    start = rdtsc();
    while (nextIndex)
        nextIndex = array[nextIndex];
    end = rdtsc();

    return ((long double) (end - start)) / data_size;
}

int main() {
    int* array = (int*) calloc(offset * max_pieces_count, sizeof(int));

    for (int pieces_count = 1; pieces_count <= max_pieces_count; pieces_count++) {
        init_array(array, pieces_count);
        long double averageTime = process_array(array);

        fprintf(stdout, "%d %Lf\n", pieces_count, averageTime);
    }

    free(array);
    return 0;
}
