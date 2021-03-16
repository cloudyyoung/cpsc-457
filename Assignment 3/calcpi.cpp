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
#include "calcpi.h"

struct Task {
    int start_row;
    int end_row;
    int count;
    int r;
};
Task tasks[256];

uint64_t count_pixels(int r, int n_threads) {
    int row_amount = std::ceil(r / n_threads);

    uint64_t count = 0;
    // for (double x = 1; x <= r; x++)
    //     for (double y = 0; y <= r; y++)
    //         if (x * x + y * y <= rsq) count++;

    for (int t = 0; t < n_threads; t++) {
        Task task = tasks[t];

        task.r = r;
        task.start_row = row_amount * t;
        task.end_row = row_amount * (t + 1) - 1;

        if (task.end_row > r) {
            task.end_row = r;
        }

        std::thread t(count_pixels_thread, t);
        t.join();
    }

    for (Task task : tasks) {
        count += task.count;
    }

    return count * 4 - 3;
}

void count_pixels_thread(int t) {
    Task task = tasks[t];
    double rsq = double(task.r) * task.r; // r^2

    for (double x = task.start_row; x <= task.end_row; x++) {
        for (double y = 0; y <= task.r; y++) {
            if (x * x + y * y <= rsq) {
                task.count++;
            }
        }
    }
}