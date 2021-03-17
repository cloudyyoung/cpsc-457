// =============================================================================
// You must modify this file and then submit it for grading to D2L.
// =============================================================================

#include "detectPrimes.h"
#include <pthread.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <atomic>
#include <unordered_map>

using namespace std;

pthread_barrier_t barrier;

int64_t n_threads;
std::vector<int64_t> nums;
std::vector<int64_t> result;

bool finished;

int num_index;
int64_t n;
bool is_threading;
atomic<bool> is_prime;
int64_t lower;
int64_t upper;
int64_t part;
unordered_map<int64_t, bool> cache;

void skip_threading(bool is) {
    if (is_threading) {
        is_threading = false;
        is_prime.store(is, memory_order_release);
    }
}

void* is_prime_thread(void* id) {

    int thread_id = long(id);

    while (true) {
        // Serial #1
        // Update global variables after retrive next number to detect
        int ret1 = pthread_barrier_wait(&barrier);
        if (ret1 == PTHREAD_BARRIER_SERIAL_THREAD) {
            // Get processing number
            if (num_index >= (int)nums.size()) {
                finished = true;
            } else {
                n = nums[num_index];
                is_threading = true;
                is_prime.store(true, memory_order_release);

                // Find if result already has current number
                auto search = cache.find(n);
                if (search != cache.end()) {
                    cout << "find cache" << endl;
                    skip_threading(cache[n]);
                }

                // Handle trivial cases
                if (n < 2) skip_threading(false);
                if (n <= 3) skip_threading(true); // 2 and 3 are primes
                if (n % 2 == 0) skip_threading(false); // handle multiples of 2
                if (n % 3 == 0) skip_threading(false); // handle multiples of 3

                // Try to divide n by every number 5 .. sqrt(n)
                lower = 5;
                upper = sqrt(n);
                part = ceil(double(upper - lower) / n_threads);

                // Pad part to 6
                part = (part > 6) ? part - (part % 6) + 6 : 6;

                // Other cases
                if (upper < lower) skip_threading(true); // Prime, but small numbers, like 13, 17, 19
                if (upper == lower) skip_threading(false); // It's 25, not prime
                if (n % upper == 0 || n % (upper + 2) == 0) skip_threading(false); // Upper bound is the divisor

                num_index++;
            }
        }

        // Parallel
        // Threads work on their own workloads
        pthread_barrier_wait(&barrier);
        if (finished) {
            break;
        } else if (is_threading) {
            // Calculate draft start and end
            int64_t start = lower + part * thread_id;
            int64_t end = start + part;

            // Limit range
            if (start > upper) start = upper;
            if (start < lower) start = lower;
            if (end > upper) end = upper;


            // Loop checking prime
            for (int64_t t = start; t <= end; t += 6) {
                if (!is_prime.load()) {
                    break;
                }
                if (n % t == 0 || n % (t + 2) == 0) {
                    is_prime.store(false, memory_order_release);
                    break;
                }
            }
        }

        // Serial #2
        int ret2 = pthread_barrier_wait(&barrier);
        if (ret2 == PTHREAD_BARRIER_SERIAL_THREAD) {
            if (is_prime.load()) {
                result.push_back(n);
            }

            cache.insert({ n, is_prime.load() });
        }
    }

    pthread_exit(0);
}

// This function takes a list of numbers in nums[] and returns only numbers that
// are primes.
//
// The parameter n_threads indicates how many threads should be created to speed
// up the computation.
// -----------------------------------------------------------------------------
// You will most likely need to re-implement this function entirely.
// Note that the current implementation ignores n_threads. Your multithreaded
// implementation must use this parameter.
std::vector<int64_t>
detect_primes(const std::vector<int64_t>& nums, int n_threads)
{
    // Initialzie global variables
    ::nums = nums;
    ::n_threads = n_threads;
    finished = false;
    num_index = 0;

    // Initialize barrier and threads list
    pthread_barrier_init(&barrier, NULL, n_threads);
    pthread_t threads[n_threads];

    // Start threads
    for (int t = 0; t < n_threads; t++) {
        pthread_create(&threads[t], NULL, is_prime_thread, (void*)(long)t);
    }

    // Join threads
    for (int t = 0; t < n_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    // Destroy barrier
    pthread_barrier_destroy(&barrier);

    return result;
}
