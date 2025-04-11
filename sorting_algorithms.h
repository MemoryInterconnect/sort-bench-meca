/* sorting_algorithms.h */
#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include <time.h>

void insertion_sort(int *arr, int size, unsigned int *numComparisons, clock_t *tempo);
void merge_sort(int *arr, int size, unsigned int *numComparisons, clock_t *tempo);
void quick_sort(int *arr, int size, unsigned int *numComparisons, clock_t *tempo);
void quick_sort_optimized(int *arr, int size, unsigned int *numComparisons, clock_t *tempo);
void bucket_sort(int *arr, int size, unsigned int *numComparisons, clock_t *tempo);
void bucket_sort_pthreads(int *arr, int size, unsigned int *numComparisons, clock_t *tempo, unsigned int num_threads);

#endif
