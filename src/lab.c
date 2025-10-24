#include <stdlib.h>
#include <sys/time.h>
#include "lab.h"
#include <pthread.h>

/*
 * AI use: AI Assisted
 */
typedef struct
{
  int *A;
  int start;
  int mid;
  int end;
  pthread_mutex_t *merge_lock; // pointer to global lock
} merge_task_t;

/*
 * @brief Thread function for sorting
 *
 * @param arg The argument passed to the thread
 * @return NULL
 * AI use: AI Assisted
 */
void *thread_sort(void *arg)
{
  parallel_args_t *p = (parallel_args_t *)arg;
  mergesort_s(p->A, p->start, p->end);
  return NULL;
}

// GCOVR_EXCL_START

/**
 * @brief Worker function for merging two sorted subarrays with locking
 *
 * @param arg The argument passed to the thread
 * @return NULL
 * AI use: AI Assisted
 */
static void *merge_worker(void *arg)
{
  merge_task_t *task = (merge_task_t *)arg;

  // Minimal lock: protects merge operation
  pthread_mutex_lock(task->merge_lock);
  merge_s(task->A, task->start, task->mid, task->end);
  pthread_mutex_unlock(task->merge_lock);

  return NULL;
}

// GCOVR_EXCL_STOP

/**
 * @brief Standard insertion sort that is faster than merge sort for small array's
 *
 * @param A The array to sort
 * @param p The starting index
 * @param r The ending index
 */
static void insertion_sort(int A[], int p, int r)
{
  int j;

  for (j = p + 1; j <= r; j++)
  {
    int key = A[j];
    int i = j - 1;
    while ((i > p - 1) && (A[i] > key))
    {
      A[i + 1] = A[i];
      i--;
    }
    A[i + 1] = key;
  }
}

void mergesort_s(int A[], int p, int r)
{
  if (r - p + 1 <= INSERTION_SORT_THRESHOLD)
  {
    insertion_sort(A, p, r);
  }
  else
  {
    int q = (p + r) / 2;
    mergesort_s(A, p, q);
    mergesort_s(A, q + 1, r);
    merge_s(A, p, q, r);
  }
}

void merge_s(int A[], int p, int q, int r)
{
  int *B = (int *)malloc(sizeof(int) * (size_t)(r - p + 1));

  int i = p;
  int j = q + 1;
  int k = 0;
  int l;

  /* as long as both lists have unexamined elements */
  /*  this loop keeps executing. */
  while ((i <= q) && (j <= r))
  {
    if (A[i] < A[j])
    {
      B[k] = A[i];
      i++;
    }
    else
    {
      B[k] = A[j];
      j++;
    }
    k++;
  }

  /* now only at most one list has unprocessed elements. */
  if (i <= q)
  {
    /* copy remaining elements from the first list */
    for (l = i; l <= q; l++)
    {
      B[k] = A[l];
      k++;
    }
  }
  else
  {
    /* copy remaining elements from the second list */
    for (l = j; l <= r; l++)
    {
      B[k] = A[l];
      k++;
    }
  }

  /* copy merged output from array B back to array A */
  k = 0;
  for (l = p; l <= r; l++)
  {
    A[l] = B[k];
    k++;
  }

  free(B);
}

/**
 * @brief Computes the sizes for each split given the total size and number of threads
 *
 * @param A The array to split
 * @param n The size of the array
 * @param num_threads The number of threads
 * @return int* An array of sizes for each split
 * AI use: AI Assisted
 */
int *split_sizes(int n, int num_threads)
{
  int working_threads = (num_threads <= 1) ? 1 : num_threads - 1;

  int *sizes = malloc(sizeof(int) * (size_t)working_threads);
  if (working_threads == 1)
  {
    sizes[0] = n; // single-thread mode
    return sizes;
  }

  int chunk_remainder = n % working_threads;
  int avg_chunk_size = n / working_threads;

  for (int i = 0; i < working_threads; i++)
  {
    sizes[i] = (i == working_threads - 1) ? avg_chunk_size + chunk_remainder : avg_chunk_size;
  }
  return sizes;
}

/**
 * @brief Computes the starting indices for each split given the sizes
 *
 * @param sizes The sizes of each split
 * @param num_threads The number of threads
 * @return int* An array of starting indices for each split
 * AI use: AI Assisted
 */
int *split_indices(int *sizes, int num_threads)
{
  int num_splits = (num_threads <= 1) ? 1 : num_threads - 1;
  int *indices = malloc(sizeof(int) * (size_t)num_splits);
  indices[0] = 0;
  for (int i = 1; i < num_splits; i++)
  {
    indices[i] = indices[i - 1] + sizes[i - 1];
  }
  return indices;
}

/**
 * @brief Splits the array into num_threads parts and fills in par_args
 *
 * @param A The array to split
 * @param indices The starting indices of each split
 * @param num_threads The number of threads
 * @return int** An array of pointers to the start of each subarray
 * AI use: AI Assisted
 */
int **split_arrays_even(int *A, int *indices, int num_threads)
{
  int worker_threads = num_threads - 1;

  int **subarrays = malloc(sizeof(int *) * (size_t)worker_threads);
  if (!subarrays)
    return NULL;

  for (int i = 0; i < worker_threads; i++)
  {
    subarrays[i] = &A[indices[i]];
  }

  return subarrays;
}

/**
 * @brief Splits the array into subarray with the remainder after even splits
 *
 * @param A The array to split
 * @param indices The starting indices of each split
 * @param num_threads The number of threads
 * @return int** An array of pointers to the start of each subarray
 * AI use: AI Assisted
 */
int **split_arrays_remainder(int *A, int *indices, int num_threads)
{
  int worker_threads = num_threads - 1;

  int **remainder_subarray = malloc(sizeof(int *));
  if (!remainder_subarray)
    return NULL;

  remainder_subarray[0] = &A[indices[worker_threads - 1]];
  return remainder_subarray;
}

// GCOVR_EXCL_START

/**
 * @brief Multi-threaded mergesort
 *
 * @param A The array to sort
 * @param n The size of the array
 * @param num_threads The number of threads to use
 * AI use: AI Assisted
 */
void mergesort_mt(int *A, int n, int num_threads)
{
  if (num_threads < 2)
  {
    mergesort_s(A, 0, n - 1);
    return;
  }

  int worker_threads = num_threads - 1;
  parallel_args_t *par_args = malloc(sizeof(parallel_args_t) * (size_t)worker_threads);

  int *sizes = split_sizes(n, num_threads);
  int *indices = split_indices(sizes, num_threads);
  int **subarrays = split_arrays_even(A, indices, num_threads);

  // Launch worker threads
  for (int i = 0; i < worker_threads; i++)
  {
    par_args[i].A = subarrays[i];
    par_args[i].start = 0;
    par_args[i].end = sizes[i] - 1;
    pthread_create(&par_args[i].tid, NULL, parallel_mergesort, &par_args[i]);
  }

  // Wait for all workers
  for (int i = 0; i < worker_threads; i++)
  {
    pthread_join(par_args[i].tid, NULL);
  }

  // Merge all sorted segments in the main thread
  merge_all_tree_parallel_locked(A, indices, sizes, num_threads);

  free(sizes);
  free(indices);
  free(subarrays);
}
// GCOVR_EXCL_STOP

// GCOVR_EXCL_START

/**
 * @brief Merges all sorted segments in a tree-like fashion using multiple threads with locking
 *
 * @param A The array to merge
 * @param indices The starting indices of each sorted segment
 * @param sizes The sizes of each sorted segment
 * @param num_threads The number of threads used for sorting
 * AI use: AI Assisted
 */
void merge_all_tree_parallel_locked(int *A, int *indices, int *sizes, int num_threads)
{
  int worker_threads = num_threads - 1;
  int level_size = worker_threads;

  pthread_mutex_t merge_lock = PTHREAD_MUTEX_INITIALIZER;

  int *new_indices = malloc((size_t)level_size * sizeof(int));
  int *new_sizes = malloc((size_t)level_size * sizeof(int));

  while (level_size > 1)
  {
    int merge_pairs = level_size / 2;

    pthread_t *threads = malloc(sizeof(pthread_t) * (size_t)merge_pairs);
    merge_task_t *tasks = malloc(sizeof(merge_task_t) * (size_t)merge_pairs);

    int new_count = 0;
    int t = 0;

    for (int i = 0; i < level_size; i += 2)
    {
      if (i + 1 < level_size)
      {
        int start = indices[i];
        int mid = indices[i] + sizes[i] - 1;
        int end = indices[i + 1] + sizes[i + 1] - 1;

        tasks[t].A = A;
        tasks[t].start = start;
        tasks[t].mid = mid;
        tasks[t].end = end;
        tasks[t].merge_lock = &merge_lock;

        pthread_create(&threads[t], NULL, merge_worker, &tasks[t]);
        t++;

        new_indices[new_count] = start;
        new_sizes[new_count] = sizes[i] + sizes[i + 1];
        new_count++;
      }
      else
      {
        // Odd one out — carry forward
        new_indices[new_count] = indices[i];
        new_sizes[new_count] = sizes[i];
        new_count++;
      }
    }

    // Wait for all merges at this level
    for (int j = 0; j < merge_pairs; j++)
    {
      pthread_join(threads[j], NULL);
    }

    // Prepare next level
    for (int j = 0; j < new_count; j++)
    {
      indices[j] = new_indices[j];
      sizes[j] = new_sizes[j];
    }

    free(threads);
    free(tasks);
    level_size = new_count;
  }

  free(new_indices);
  free(new_sizes);
  pthread_mutex_destroy(&merge_lock);
}

// GCOVR_EXCL_STOP


// GCOVR_EXCL_START
/**
 * @brief The function that is called by each thread to sort their chunk
 *
 * @param args see struct parallel_args
 * @return void* always NULL
 * AI use: AI Assisted
 */
void *parallel_mergesort(void *args)
{
  parallel_args_t *parg = (parallel_args_t *)args;
  mergesort_s(parg->A, parg->start, parg->end);
  return NULL;
}

// GCOVR_EXCL_STOP

double getMilliSeconds()
{
  struct timeval now;
  gettimeofday(&now, (struct timezone *)0);
  return (double)now.tv_sec * 1000.0 + (double)now.tv_usec / 1000.0;
}