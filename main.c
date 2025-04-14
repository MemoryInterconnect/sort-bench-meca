/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include "sorting_algorithms.h"
#include "utils.h"

#define MMAP_OFFSET 0x200000000
#define MMAP_DEVICE "/dev/mem"

void print_usage(const char *prog_name) {
    printf("Usage: %s <algorithm> <size> <array_type> <runs> <use_mmap> [threads]\n", prog_name);
    printf("  <algorithm>   : 1=insertion, 2=merge, 3=quick, 5=bucket, 6=bucket_pthreads\n");
    printf("  <size>        : size of the array\n");
    printf("  <array_type>  : 1=random, 2=sorted, 3=reverse, 4=duplicates\n");
    printf("  <runs>        : number of runs to average results\n");
    printf("  <use_meca>    : 0=malloc, 1=mmap to /dev/mem at 0x%lx\n", (unsigned long)MMAP_OFFSET);
    printf("  [threads]     : required only for bucket_pthreads (algorithm 6)\n");
}

int main(int argc, char *argv[]) {
    if (argc < 6) {
        print_usage(argv[0]);
        return 1;
    }

    unsigned int choice = atoi(argv[1]);
    unsigned int size = atoi(argv[2]);
    unsigned int flag = atoi(argv[3]);
    unsigned int numRun = atoi(argv[4]);
    int use_mmap = atoi(argv[5]);
    unsigned int num_threads = 1;

    if (choice == 6 && argc < 7) {
        fprintf(stderr, "Error: number of threads required for bucket_pthreads.\n");
        print_usage(argv[0]);
        return 1;
    } else if (choice == 6) {
        num_threads = atoi(argv[6]);
    }

    unsigned int compare = 0, time = 0, numComparisons = 0;
    double meanTime;
    float meanCompare;
    clock_t tempo;

    int *array = NULL;
    int fd = -1;

    if (use_mmap) {
        fd = open(MMAP_DEVICE, O_RDWR | O_SYNC);
        if (fd < 0) {
            perror("open /dev/mem");
            return 1;
        }
        array = (int *)mmap(NULL, size * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, MMAP_OFFSET);
        if (array == MAP_FAILED) {
            perror("mmap");
            close(fd);
            return 1;
        }
    } else {
        array = malloc(size * sizeof(int));
        if (!array) {
            fprintf(stderr, "malloc failed.\n");
            return 1;
        }
    }

    double *comparison_data = malloc(numRun * sizeof(double));
    float *time_data = malloc(numRun * sizeof(float));
    if (!comparison_data || !time_data) {
        fprintf(stderr, "Memory allocation failed.\n");
        if (!use_mmap) free(array);
        else {
            munmap(array, size * sizeof(int));
            close(fd);
        }
        return 1;
    }
    for (int i = 0; i < numRun; i++) {
        populate_array_random(array, size, flag);

        switch (choice) {
            case 1:
		if( i == 0 ) 
			printf("insertion_sort: size=%.1f kB array_type=%d runs=%d use_meca=%d\n", ((float)size)/256, flag, numRun, use_mmap);
		printf("\rrun = %d/%d", i+1, numRun);
		fflush(stdout);
                insertion_sort(array, size, &numComparisons, &tempo);
                break;
            case 2:
		if( i == 0 ) 
			printf("merge_sort: size=%.1f kB array_type=%d runs=%d use_meca=%d\n", ((float)size)/256, flag, numRun, use_mmap);
		printf("\rrun = %d/%d", i+1, numRun);
		fflush(stdout);
                merge_sort(array, size, &numComparisons, &tempo);
                break;
            case 3:
		if( i == 0 ) 
			printf("quick_sort: size=%.1f kB array_type=%d runs=%d use_meca=%d\n", ((float)size)/256, flag, numRun, use_mmap);
		printf("\rrun = %d/%d", i+1, numRun);
		fflush(stdout);
                quick_sort(array, size, &numComparisons, &tempo);
                break;
/*            case 4:
		if( i == 0 ) 
			printf("quick_sort_optimized: size=%d kB array_type=%d runs=%d use_meca=%d\n", size>>8, flag, numRun, use_mmap);
		printf("\rrun = %d/%d", i+1, numRun);
		fflush(stdout);
                quick_sort_optimized(array, size, &numComparisons, &tempo);
                break;
*/
            case 5:
		if( i == 0 ) 
			printf("bucket_sort: size=%.1f kB array_type=%d runs=%d use_meca=%d\n", ((float)size)/256, flag, numRun, use_mmap);
		printf("\rrun = %d/%d", i+1, numRun);
		fflush(stdout);
                bucket_sort(array, size, &numComparisons, &tempo);
                break;
            case 6:
		if( i == 0 ) 
			printf("bucket_sort_pthreads: size=%.1f kB array_type=%d runs=%d use_meca=%d\n", ((float)size)/256, flag, numRun, use_mmap);
		printf("\rrun = %d/%d", i+1, numRun);
		fflush(stdout);
                bucket_sort_pthreads(array, size, &numComparisons, &tempo, num_threads);
                break;
            default:
                fprintf(stderr, "Invalid algorithm choice.\n");
                print_usage(argv[0]);
                if (!use_mmap) free(array);
                else {
                    munmap(array, size * sizeof(int));
                    close(fd);
                }
                free(comparison_data);
                free(time_data);
                return 1;
        }

        comparison_data[i] = numComparisons;
        time_data[i] = transfer_time(tempo);
        numComparisons = 0;
    }

    printf("\r");

    check_sorted(array, size);

    calculate_confidence(comparison_data, time_data, numRun, &compare, &time, &meanTime, &meanCompare);
    print_result(compare, time, meanTime, meanCompare);

    if (use_mmap) {
        munmap(array, size * sizeof(int));
        close(fd);
    } else {
        free(array);
    }

    free(comparison_data);
    free(time_data);

    return 0;
}

