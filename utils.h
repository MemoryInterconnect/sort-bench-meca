/* utils.h */
#ifndef UTILS_H
#define UTILS_H

#include <time.h>

void populate_array_random(int *arr, int size, int flag);
void print_array(const int *arr, int size);
void print_time(clock_t tempo);
void print_comparison(unsigned int res);
float transfer_time(clock_t tempo);
void calculate_confidence(const double *comparisons, const float *times, int count,
                          unsigned int *compare_out, unsigned int *time_out,
                          double *mean_time, float *mean_comparison);
void print_result(unsigned int compare, unsigned int time, float mean_time, double mean_comparison);
int check_sorted(const int *arr, int size);

#endif
