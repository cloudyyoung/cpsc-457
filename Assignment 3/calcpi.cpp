// ======================================================================
// You must modify this file and then submit it for grading to D2L.
// ======================================================================
//
// count_pi() calculates the number of pixels that fall into a circle
// using the algorithm explained here:
//
// https://en.wikipedia.org/wiki/Approximations_of_%CF%80
//
// count_pixels() takes 2 paramters:
//  r         =  the radius of the circle
//  n_threads =  the number of threads you should create
//
// Currently the function ignores the n_threads parameter. Your job is to
// parallelize the function so that it uses n_threads threads to do
// the computation.

#include <thread>
#include <iostream>
#include <cmath>
#include <vector>
#include "calcpi.h"

struct Task {
    int start_row;
    int end_row;
    uint64_t count;
};
Task tasks[256];
std::thread threads[256];
int r;
double rsq;


void count_pixels_thread(Task& task) {
    // Counting
    // Here we only calculate 1/4 of the circle, the right bottom corner
    uint64_t count = 0;
    int start_row = task.start_row;
    int end_row = task.end_row;
    int r = ::r;
    double rsq = ::rsq;

    for (double x = start_row; x < end_row; x++) {
        for (double y = 1; y <= r; y++) {
            if (x * x + y * y <= rsq) {
                count++;
            }
        }
    }

    task.count = count;
}

uint64_t count_pixels(int r, int n_threads) {

    // If radius is 0 or a negative number (invalid), there is no circle, return 0
    if (r <= 0) {
        return 0;
    }

    // Over the limit of number of threads
    if (n_threads > 256) {
        n_threads = 256;
    }

    // Initialize local variables
    int row_amount = std::ceil(double(r) / n_threads);
    uint64_t count = 0;
    double rsq = double(r) * r; // r^2

    // The number of threads is greater than radius, each thread will handle less than 1 row, 
    // so we force each thread to handle minimum 1 row.
    if (row_amount < 1) {
        row_amount = 1;
    }

    // Launch threads and assign tasks
    for (int t = 0; t < n_threads; t++) {
        // Set radius, start and end row; (start, end]
        ::r = r;
        ::rsq = rsq;
        int start_row = row_amount * t;
        int end_row = start_row + row_amount;

        // If start or end row is greater than radius, set to radius
        if (start_row > r) start_row = r;
        if (end_row > r) end_row = r;

        // Set to struct
        Task& task = tasks[t];
        task.start_row = start_row;
        task.end_row = end_row;

        // Append to threads list
        threads[t] = std::thread(count_pixels_thread, std::ref(task));
    }

    // Join threads and count
    for (int t = 0; t < n_threads; t++) {
        threads[t].join();
        count += tasks[t].count;
    }

    return count * 4 + 1;
}

// ./calcpi 10 2
// ./calcpi 2 2
// ./calcpi 100000 16
// ./calcpi 50000 16