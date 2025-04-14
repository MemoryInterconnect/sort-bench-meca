/* sorting_algorithms.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include "sorting_algorithms.h"

static void exchange(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void insertion_sort(int *arr, int size, unsigned int *numComparisons, clock_t *tempo) {
    clock_t start = clock();
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
            (*numComparisons)++;
        }
        arr[j + 1] = key;
        (*numComparisons)++;
    }
    *tempo = clock() - start;
}

static void merge(int *arr, int *temp, int left, int mid, int right, unsigned int *numComparisons) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        (*numComparisons)++;
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (int l = left; l <= right; l++) arr[l] = temp[l];
}

static void merge_sort_recursive(int *arr, int *temp, int left, int right, unsigned int *numComparisons) {
    if (left < right) {
        int mid = (left + right) / 2;
        merge_sort_recursive(arr, temp, left, mid, numComparisons);
        merge_sort_recursive(arr, temp, mid + 1, right, numComparisons);
        merge(arr, temp, left, mid, right, numComparisons);
    }
}

void merge_sort(int *arr, int size, unsigned int *numComparisons, clock_t *tempo) {
    clock_t start = clock();
    int *temp = (int *)malloc(size * sizeof(int));
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    merge_sort_recursive(arr, temp, 0, size - 1, numComparisons);
    free(temp);
    *tempo = clock() - start;
}

static int partition(int *arr, int low, int high, unsigned int *numComparisons) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        (*numComparisons)++;
        if (arr[j] <= pivot) {
            i++;
            exchange(&arr[i], &arr[j]);
        }
    }
    exchange(&arr[i + 1], &arr[high]);
    return i + 1;
}

static void quick_sort_recursive(int *arr, int low, int high, unsigned int *numComparisons) {
    if (low < high) {
        int pi = partition(arr, low, high, numComparisons);
        quick_sort_recursive(arr, low, pi - 1, numComparisons);
        quick_sort_recursive(arr, pi + 1, high, numComparisons);
    }
}

void quick_sort(int *arr, int size, unsigned int *numComparisons, clock_t *tempo) {
    clock_t start = clock();
    quick_sort_recursive(arr, 0, size - 1, numComparisons);
    *tempo = clock() - start;
}

static int median_of_three(int *arr, int low, int high, unsigned int *numComparisons) {
    int mid = low + (high - low) / 2;
    if (arr[mid] > arr[high]) { exchange(&arr[mid], &arr[high]); (*numComparisons)++; }
    if (arr[low] > arr[high]) { exchange(&arr[low], &arr[high]); (*numComparisons)++; }
    if (arr[mid] > arr[low])  { exchange(&arr[mid], &arr[low]);  (*numComparisons)++; }
    return arr[low];
}

static int partition_optimized(int *arr, int low, int high, unsigned int *numComparisons) {
    int pivot = median_of_three(arr, low, high, numComparisons);
    int i = low + 1;
    int j = high;
    while (i < j) {
        while (i <= high && arr[i] < pivot) { i++; (*numComparisons)++; }
        while (j >= low + 1 && arr[j] > pivot) { j--; (*numComparisons)++; }
        if (i < j) { exchange(&arr[i], &arr[j]); i++; j--; }
    }
    exchange(&arr[low], &arr[j]);
    return j;
}

static void quick_sort_optimized_recursive(int *arr, int low, int high, unsigned int *numComparisons) {
    int i, j, key, pivotIndex;

    while (low < high) {
        if (high - low <= 10) {
            for (i = low + 1; i <= high; i++) {
                key = arr[i];
                j = i - 1;
                while (j >= low && arr[j] > key) {
                    arr[j + 1] = arr[j];
                    j--;
                    (*numComparisons)++;
                }
                arr[j + 1] = key;
                (*numComparisons)++;
            }
            break;
        }
        pivotIndex = partition_optimized(arr, low, high, numComparisons);
        quick_sort_optimized_recursive(arr, low, pivotIndex - 1, numComparisons);
        low = pivotIndex + 1;
    }
}

void quick_sort_optimized(int *arr, int size, unsigned int *numComparisons, clock_t *tempo) {
    clock_t start = clock();
    quick_sort_optimized_recursive(arr, 0, size - 1, numComparisons);
    *tempo = clock() - start;
}

void bucket_sort(int *arr, int size, unsigned int *numComparisons, clock_t *tempo) {
    clock_t start = clock();

    int min = arr[0], max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }

    int bucketCount = (int)ceil(size / 2.0);
    int range = (max - min + 1);
    float step = (float)range / bucketCount;

    int **buckets = (int **)malloc(bucketCount * sizeof(int *));
    int *bucketSizes = (int *)calloc(bucketCount, sizeof(int));
    int *bucketCapacities = (int *)malloc(bucketCount * sizeof(int));

    for (int i = 0; i < bucketCount; i++) {
        bucketCapacities[i] = 10;
        buckets[i] = (int *)malloc(bucketCapacities[i] * sizeof(int));
    }

    for (int i = 0; i < size; i++) {
        int index = (int)((arr[i] - min) / step);
        if (index >= bucketCount) index = bucketCount - 1;
        if (bucketSizes[index] >= bucketCapacities[index]) {
            bucketCapacities[index] *= 2;
            buckets[index] = (int *)realloc(buckets[index], bucketCapacities[index] * sizeof(int));
        }
        buckets[index][bucketSizes[index]++] = arr[i];
    }

    int idx = 0;
    for (int i = 0; i < bucketCount; i++) {
        if (bucketSizes[i] > 0) {
            unsigned int localComparisons = 0;
            clock_t dummy;
            insertion_sort(buckets[i], bucketSizes[i], &localComparisons, &dummy);
            *numComparisons += localComparisons;
            for (int j = 0; j < bucketSizes[i]; j++) {
                arr[idx++] = buckets[i][j];
            }
        }
        free(buckets[i]);
    }

    free(buckets);
    free(bucketSizes);
    free(bucketCapacities);

    *tempo = clock() - start;
}

typedef struct {
    int **buckets;
    int *bucketSizes;
    int start;
    int end;
    unsigned int comparisons;
} ThreadArgs;

void *threaded_insertion_sort(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    for (int i = args->start; i < args->end; i++) {
        if (args->bucketSizes[i] > 0) {
            unsigned int local = 0;
            clock_t dummy;
            insertion_sort(args->buckets[i], args->bucketSizes[i], &local, &dummy);
            args->comparisons += local;
        }
    }
    return NULL;
}

void bucket_sort_pthreads(int *arr, int size, unsigned int *numComparisons, clock_t *tempo, unsigned int num_threads) {
    clock_t start = clock();

    int min = arr[0], max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }

    int bucketCount = (int)ceil(size / 2.0);
    int range = (max - min + 1);
    float step = (float)range / bucketCount;

    int **buckets = (int **)malloc(bucketCount * sizeof(int *));
    int *bucketSizes = (int *)calloc(bucketCount, sizeof(int));
    int *bucketCapacities = (int *)malloc(bucketCount * sizeof(int));

    for (int i = 0; i < bucketCount; i++) {
        bucketCapacities[i] = 10;
        buckets[i] = (int *)malloc(bucketCapacities[i] * sizeof(int));
    }

    for (int i = 0; i < size; i++) {
        int index = (int)((arr[i] - min) / step);
        if (index >= bucketCount) index = bucketCount - 1;
        if (bucketSizes[index] >= bucketCapacities[index]) {
            bucketCapacities[index] *= 2;
            buckets[index] = (int *)realloc(buckets[index], bucketCapacities[index] * sizeof(int));
        }
        buckets[index][bucketSizes[index]++] = arr[i];
    }

    pthread_t threads[num_threads];
    ThreadArgs args[num_threads];
    int bucketsPerThread = (bucketCount + num_threads - 1) / num_threads;

    for (int i = 0; i < num_threads; i++) {
        args[i].buckets = buckets;
        args[i].bucketSizes = bucketSizes;
        args[i].start = i * bucketsPerThread;
        args[i].end = (i + 1) * bucketsPerThread;
        if (args[i].end > bucketCount) args[i].end = bucketCount;
        args[i].comparisons = 0;
        pthread_create(&threads[i], NULL, threaded_insertion_sort, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        *numComparisons += args[i].comparisons;
    }

    int idx = 0;
    for (int i = 0; i < bucketCount; i++) {
        for (int j = 0; j < bucketSizes[i]; j++) {
            arr[idx++] = buckets[i][j];
        }
        free(buckets[i]);
    }

    free(buckets);
    free(bucketSizes);
    free(bucketCapacities);

    *tempo = clock() - start;
}

