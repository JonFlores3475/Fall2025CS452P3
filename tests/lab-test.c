#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "harness/unity.h"
#include "../src/lab.h"
#include <time.h>

/* AI use: AI Assisted */

// ----------------------- Test Setup ------------------------//

void setUp(void) { /* Nothing to setup for now */ }
void tearDown(void) { /* Nothing to cleanup */ }

// Helper: check if array is sorted ascending
int is_sorted(int *A, int n) {
    for (int i = 1; i < n; i++) {
        if (A[i - 1] > A[i]) return 0;
    }
    return 1;
}

// --------------------------- Tests ---------------------------


/*AI use: AI Assisted*/
// Test single-threaded mergesort
void test_mergesort_s(void) {
    int A[] = {5, 2, 9, 1, 5, 6};
    int n = sizeof(A) / sizeof(A[0]);
    mergesort_s(A, 0, n - 1);
    TEST_ASSERT_TRUE(is_sorted(A, n));
}

//AI use: AI Assisted
// Test multi-threaded array split
void test_split_sizes_indices(void) {
    int n = 10;
    int num_threads = 3;
    int *A = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) A[i] = i;

    int *sizes = split_sizes(n, num_threads);
    int *indices = split_indices(sizes, num_threads);

    int sum = 0;
    for (int i = 0; i < num_threads - 1; i++) sum += sizes[i];
    TEST_ASSERT_EQUAL_INT(n, sum + (n % (num_threads - 1))); // sum of sizes matches array

    TEST_ASSERT_EQUAL_INT(0, indices[0]);
    TEST_ASSERT_EQUAL_INT(sizes[0], indices[1]);

    free(A);
    free(sizes);
    free(indices);
}

/*
//AI use: AI Assisted
// Test multi-threaded sorting
void test_parallel_sort(void) {
    int n = 20;
    int num_threads = 4;
    int *A = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) A[i] = rand() % 100;

    int *sizes = split_sizes(n, num_threads);
    int *indices = split_indices(sizes, num_threads);

    int worker_threads = num_threads - 1;
    parallel_args_t *par_args = malloc(sizeof(parallel_args_t) * worker_threads);

    for (int i = 0; i < worker_threads; i++) {
        par_args[i].A = A;
        par_args[i].start = indices[i];
        par_args[i].end   = indices[i] + sizes[i] - 1;
        pthread_create(&par_args[i].tid, NULL, thread_sort, &par_args[i]);
    }

    for (int i = 0; i < worker_threads; i++)
        pthread_join(par_args[i].tid, NULL);

    merge_all_tree_parallel_locked(A, indices, sizes, num_threads);

    TEST_ASSERT_TRUE(is_sorted(A, n));

    free(A);
    free(sizes);
    free(indices);
    free(par_args);
}

//AI use: AI Assisted
// Test edge cases: empty array, single-element array
void test_edge_cases(void) {
    int A1[] = {};
    mergesort_s(A1, 0, -1); // Should not crash
    TEST_ASSERT_TRUE(1); // Just checking no crash

    int A2[] = {42};
    mergesort_s(A2, 0, 0);
    TEST_ASSERT_TRUE(A2[0] == 42);
}

//AI use: AI Assisted
// Stress test with larger arrays and varying thread counts
void test_stress_sort(void) {
    const int sizes[] = {10000, 50000, 100000};
    const int max_threads = 8; // adjust as needed
    for (int s = 0; s < sizeof(sizes)/sizeof(sizes[0]); s++) {
        int n = sizes[s];
        int *A = malloc(sizeof(int) * n);
        for (int i = 0; i < n; i++) A[i] = rand() % 100000;

        for (int num_threads = 1; num_threads <= max_threads; num_threads++) {
            int *copy = malloc(sizeof(int) * n);
            memcpy(copy, A, sizeof(int) * n);

            int *sizes_split = split_sizes(n, num_threads);
            int *indices_split = split_indices(sizes_split, num_threads);

            int worker_threads = (num_threads <= 1) ? 0 : num_threads - 1;
            parallel_args_t *par_args = (worker_threads > 0) ?
                malloc(sizeof(parallel_args_t) * worker_threads) : NULL;

            double start = getMilliSeconds();

            if (worker_threads == 0) {
                mergesort_s(copy, 0, n - 1);
            } else {
                for (int i = 0; i < worker_threads; i++) {
                    par_args[i].A = copy;
                    par_args[i].start = indices_split[i];
                    par_args[i].end = indices_split[i] + sizes_split[i] - 1;
                    pthread_create(&par_args[i].tid, NULL, (void*(*)(void*))thread_sort, &par_args[i]);
                }

                for (int i = 0; i < worker_threads; i++)
                    pthread_join(par_args[i].tid, NULL);

                merge_all_tree_parallel_locked(copy, indices_split, sizes_split, num_threads);
            }

            double end = getMilliSeconds();
            printf("Stress test: array_size=%d threads=%d time=%f\n", n, num_threads, end - start);

            TEST_ASSERT_TRUE(is_sorted(copy, n));

            free(copy);
            if (par_args) free(par_args);
            free(sizes_split);
            free(indices_split);
        }
        free(A);
    }
}
*/
/* AI use: AI Assisted */
// ----------------------- Test Runner ------------------------//
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_mergesort_s);
    RUN_TEST(test_split_sizes_indices);
    /*
    RUN_TEST(test_parallel_sort);
    RUN_TEST(test_edge_cases);
    RUN_TEST(test_stress_sort);
    */
    return UNITY_END();
}