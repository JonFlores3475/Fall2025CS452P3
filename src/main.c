#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lab.h"

#ifndef TEST


/**
 * @brief Main function to run multi-threaded mergesort
 *
 * @param argc Argument count
 * @param argv Argument values
 * @return int Exit status
 * AI use: AI Assisted
 */
int main(int argc, char **argv) {
    if (argc < 3) {
        printf("usage: %s <array_size> <num_threads>\n", argv[0]);
        return 1;
    }

    int array_size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    if (num_threads < 1) num_threads = 1;  // enforce minimum

    // Allocate and initialize array
    int *A = malloc(sizeof(int) * (size_t)array_size);
    for (int i = 0; i < array_size; i++)
        A[i] = rand() % 10000;

    // Split array
    int *sizes   = split_sizes(array_size, num_threads);
    int *indices = split_indices(sizes, num_threads);

    int worker_threads = (num_threads <= 1) ? 0 : num_threads - 1;
    parallel_args_t *par_args = (worker_threads > 0) ?
        malloc(sizeof(parallel_args_t) * (size_t)worker_threads) : NULL;

    double start = getMilliSeconds();

    if (worker_threads == 0) {
        // Single-thread mode
        mergesort_s(A, 0, array_size - 1);
    } else {
        // Multi-thread mode
        for (int i = 0; i < worker_threads; i++) {
            par_args[i].A = A;
            par_args[i].start = indices[i];
            par_args[i].end   = indices[i] + sizes[i] - 1;
            pthread_create(&par_args[i].tid, NULL, thread_sort, &par_args[i]);
        }

        for (int i = 0; i < worker_threads; i++) {
            pthread_join(par_args[i].tid, NULL);
        }

        merge_all_tree_parallel_locked(A, indices, sizes, num_threads);
    }

    double end = getMilliSeconds();

    printf("%d %f\n", num_threads, end - start);

    free(A);
    free(sizes);
    free(indices);
    if (par_args) free(par_args);

    return 0;
}
#endif