// you need to modify this file

#include "scheduler.h"
#include "common.h"
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <climits>

void print(std::queue<int> q, std::string name)
{
    std::cout << name << ":  [";
    while (!q.empty()) {
        std::cout << q.front() << ",";
        q.pop();
    }
    std::cout << "]" << std::endl;
}

void print(std::vector<int64_t> q, std::string name)
{
    std::cout << name << ": [";
    for (int64_t each : q) {
        std::cout << each << ", ";
    }
    std::cout << "]" << std::endl;
}


void print(std::vector<int> q, std::string name)
{
    std::cout << name << ": [";
    for (int64_t each : q) {
        std::cout << each << ", ";
    }
    std::cout << "]" << std::endl;
}

// this is the function you should edit
//
// runs Round-Robin scheduling simulator
// input:
//   quantum = time slice
//   max_seq_len = maximum length of the reported executing sequence
//   processes[] = list of process with populated IDs, arrival_times, and bursts
// output:
//   seq[] - will contain the execution sequence but trimmed to max_seq_len size
//         - idle CPU will be denoted by -1
//         - other entries will be from processes[].id
//         - sequence will be compressed, i.e. no repeated consecutive numbers
//   processes[]
//         - adjust finish_time and start_time for each process
//         - do not adjust other fields
//
void simulate_rr(
    int64_t quantum,
    int64_t max_seq_len,
    std::vector<Process>& processes,
    std::vector<int>& seq
) {
    // Clear
    seq.clear();

    // No process
    if (processes.empty()) {
        return;
    }

    // Initialize local variables
    int64_t time = 0;
    int lining = 0;
    bool burst_jump = false;
    std::queue<int> queue;
    std::vector<int64_t> burst(processes.size());
    std::vector<bool> start(processes.size());
    std::vector<bool> finish(processes.size());


    while (true) {
        // Queue arrived processes
        while (lining < int(processes.size()) && processes[lining].arrival_time <= time) {
            queue.push(lining);
            burst[lining] = processes[lining].burst;
            start[lining] = false;
            finish[lining] = false;
            // std::cout << "  - queue new process " << lining << ", arrival " << processes[lining].arrival_time << std::endl;
            lining++;
        }

        // std::cout << "time:   " << time << std::endl;
        // print(queue, "queue");
        // print(burst, "burst");
        // std::cout << "lining: " << lining << "/" << processes.size() << std::endl;

        burst_jump = false;

        if (!queue.empty()) {
            // Current
            int current = queue.front();
            int queue_size = int(queue.size());
            queue.pop();
            if (seq.back() != current || seq.empty()) {
                // std::cout << "  - seq back " << seq.back() << ", current: " << current << std::endl;
                seq.push_back(current);
                // print(seq, "seq");
            }

            // Process start
            if (!start[current]) {
                processes[current].start_time = time;
                start[current] = true;
            }

            if (queue_size == 1 && lining == int(processes.size())) {
                // Current is the last process
                time += burst[current];
                burst[current] = 0;

                // std::cout << "  - last process jump " << std::endl;
            } else if (queue_size == 1 && time + burst[current] <= processes[lining].arrival_time) {
                // Current alone, finishes before/at next process arrives
                processes[current].finish_time = time + burst[current];
                finish[current] = true;
                burst[current] = 0;

                // std::cout << "  - single process finish jump" << std::endl;
            } else if (queue_size == 1 && quantum <= processes[lining].arrival_time - time) {
                // Currect alone, half jump to next process arrives
                int64_t jump = ((processes[lining].arrival_time - time) / quantum + 1) * quantum;
                burst[current] -= jump;
                time += jump;

                // std::cout << "  - single process half jump" << std::endl;
            } else {
                //  Burst jump or Regular one-quantum step
                // std::cout << "current: " << current << std::endl;

                // Copy queue to vector for later
                std::queue<int> _queue = queue;
                std::vector<int> queue_v;
                queue_v.push_back(current);
                while (!_queue.empty()) {
                    queue_v.push_back(_queue.front());
                    _queue.pop();
                }
                // print(queue_v, "queue_v");

                // Get jump steps, find minimum process burst in queue
                int64_t min = LLONG_MAX;
                for (int b : queue_v) {
                    if (burst[b] < min) {
                        min = burst[b];
                        // std::cout << "    - min " << b << ": " << min << " -> " << burst[b] << std::endl;
                    }
                }

                // In case future processes arrive, also consider it with min
                // If there is future processes arriving, diving min with current queue size,
                // so that every queued process burst same amount of time and reach next process arrival
                if (lining < int(processes.size())) {
                    min = std::min(min, processes[lining].arrival_time - time);
                    min /= queue_v.size();
                    // std::cout << "    - burst jump process arrive " << lining << ": " << processes[lining].arrival_time - time << std::endl;
                    // std::cout << "    - burst min / queue_v.size: " << min << std::endl;
                }

                // Make sure min is padded with quantum
                min -= min % quantum;
                // std::cout << "    - burst min %% quantum: " << min << std::endl;
                // std::cout << "  - burst min: " << min << std::endl;


                // Whether make a burst jump (aka the hardest hint) or regular one-quantum step
                if (min >= quantum) {
                    // Burst jumping is available, namely every queued process can at least 
                    // burst one quantum amount of time. Otherwise there might be more processes
                    // arriving during less than a quantum of time, which should go regular step.
                    burst_jump = true;
                    // std::cout << "  - burst jump " << std::endl;

                    // Record start time and declare new queue
                    int64_t start_time = time;
                    std::queue<int> queue_new;

                    // Burst every process in queue by min
                    for (int b : queue_v) {
                        time += min;
                        burst[b] -= min;

                        // Requeue if not burst out
                        if (burst[b] != 0) {
                            queue_new.push(b);
                        }

                        // std::cout << "      - queue current: " << b << ", burst: " << burst[b] << std::endl;
                    }
                    queue = queue_new;
                    // std::cout << "      - ";
                    // print(queue_new, "queue_new");

                    // Assign finish time for processes
                    int index = 0;
                    for (int b : queue_v) {
                        // Process start
                        if (!start[b]) {
                            processes[b].start_time = start_time + quantum * index;
                            start[b] = true;
                        }

                        // Process finish
                        if (burst[b] == 0 && !finish[b]) {
                            processes[b].finish_time = time - quantum * (queue_v.size() - 1) + quantum * index;
                            finish[b] = true;
                        }

                        index++;
                    }

                    // Populate seq list
                    int64_t times = min / quantum;
                    for (int64_t t = 0; t < times && int64_t(seq.size()) <= max_seq_len; t++) {
                        for (int b : queue_v) {
                            if (seq.back() != b) {
                                seq.push_back(b);
                            }
                        }
                    }
                } else {
                    // Regular one-quantum steps
                    // std::cout << "  - regular one-quantum burst" << std::endl;
                    if (burst[current] >= quantum) {
                        burst[current] -= quantum;
                        time += quantum;
                    } else { // finish early
                        time += burst[current];
                        burst[current] = 0;
                    }
                }
            }

            // Process finish
            // (burst jump handles itself seperately above)
            if (!burst_jump) {
                if (burst[current] == 0 && !finish[current]) {
                    processes[current].finish_time = time;
                    finish[current] = true;
                } else {
                    // Queue any newly arrived process during the burst
                    while (lining < int(processes.size()) && processes[lining].arrival_time < time) {
                        queue.push(lining);
                        burst[lining] = processes[lining].burst;
                        start[lining] = false;
                        finish[lining] = false;
                        // std::cout << "  - queue 2 new process " << lining << ", arrival " << processes[lining].arrival_time << std::endl;
                        lining++;
                    }

                    // Queue back if process not done
                    if (burst[current] != 0) {
                        queue.push(current);
                    }
                }
            }

        } else {
            // Queue empty, jump to next lining if there are more processes
            seq.push_back(-1);
            if (lining < int(processes.size())) {
                time = processes[lining].arrival_time;
            }
        }

        // std::cout << "time:   " << time << std::endl;
        // print(queue, "queue");
        // print(burst, "burst");
        // std::cout << "lining: " << lining << "/" << processes.size() << std::endl;
        // std::cout << std::endl;

        // Finished all processes
        if (queue.empty() && int(processes.size()) == lining) {
            break;
        }

    }

    seq.resize(std::min(max_seq_len, int64_t(seq.size())));

}