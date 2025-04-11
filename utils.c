/* utils.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "utils.h"

void populate_array_random(int *arr, int size, int flag) {
    for (int i = 0; i < size; i++) {
        if (flag == 1) {
            arr[i] = rand() % (10 * (size + 1));
        } else if (flag == 2) {
            arr[i] = i + 1;
        } else if (flag == 3) {
            arr[i] = size * 2 - i;
        } else if (flag == 4) {
            arr[i] = rand() % 10;
        }
    }
}

void print_array(const int *arr, int size) {
    for (int i = 0; i < size && i < 20; i++) {
        printf("%d\t", arr[i]);
        if ((i + 1) % 5 == 0) printf("\n");
    }
    if (size > 20) printf("...\n");
    printf("\n");
}

void print_time(clock_t tempo) {
    printf("%f\n", (float)tempo / CLOCKS_PER_SEC);
}

void print_comparison(unsigned int res) {
    printf("%u\n", res);
}

float transfer_time(clock_t tempo) {
    return (float)tempo / CLOCKS_PER_SEC;
}

void calculate_confidence(const double *comparisons, const float *times, int count,
                          unsigned int *compare_out, unsigned int *time_out,
                          double *mean_time, float *mean_comparison) {
    double sum_comp = 0, var_comp = 0;
    float sum_time = 0, var_time = 0;
    for (int i = 0; i < count; i++) {
        sum_comp += comparisons[i];
        sum_time += times[i];
    }
    double avg_comp = sum_comp / count;
    float avg_time = sum_time / count;

    for (int i = 0; i < count; i++) {
        var_comp += pow(comparisons[i] - avg_comp, 2);
        var_time += pow(times[i] - avg_time, 2);
    }
    var_comp /= (count - 1);
    var_time /= (count - 1);

    *mean_comparison = (float)avg_comp;
    *mean_time = avg_time;
    *compare_out = (unsigned int)(1536.64 * var_comp / pow(avg_comp, 2));
    *time_out = (unsigned int)(1536.64 * var_time / pow(avg_time, 2));
}

void print_result(unsigned int compare, unsigned int time, float mean_time, double mean_comparison) {
    printf("The mean value of time : %f\n", mean_time);
    printf("The mean value of comparisons: %f\n", mean_comparison);
}

