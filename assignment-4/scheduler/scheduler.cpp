// you need to modify this file

#include "scheduler.h"
#include "common.h"
#include <vector>
#include <queue>

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

    // No process
    if (processes.empty()) {
        return;
    }

    int64_t time = 0;
    int lining = 0;
    std::queue<int> queue;
    std::vector<int64_t> burst(processes.size());
    std::vector<bool> start(processes.size());

    while (true) {

        // Queue arrived process
        if (processes[lining].arrival_time <= time && queue.size() <= max_seq_len) {
            queue.push(lining);
            burst[lining] = processes[lining].burst;
            start[lining] = false;
            lining++;
        }

        // Handle current process
        if (!queue.empty()) {
            int current = queue.front();
            queue.pop();
            seq.push_back(current);

            if (!start[current]) {
                // Process start
                processes[current].start_time = time;
                start[current] = true;
            }

            burst[current] --;

            if (burst[current] == 0) {
                // Process finish
                processes[current].finish_time = time;
            } else {
                // Queue back
                queue.push(current);
            }
        } else {
            // Queue empty
            seq.push_back(-1);
        }

        if (queue.empty() && processes.size() == lining) {
            break;
        }


        time++;
    }

}