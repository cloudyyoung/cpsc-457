// this is the file you need to edit
// -------------------------------------------------------------------------------------
// you don't have to use any of the code below, but you can

#include "memsim.h"
#include <iostream>
#include <cassert>

// I suggest you implement the simulator as a class. This is only a suggestion.
// Feel free to ignore it.
struct Simulator {
  Simulator(int64_t page_size) {
    // constructor
  }
  void allocate(int tag, int size) {
    // Pseudocode for allocation request:
    // - search through the list of partitions from start to end, and
    //   find the largest partition that fits requested size
    //     - in case of ties, pick the first partition found
    // - if no suitable partition found:
    //     - get minimum number of pages from OS, but consider the
    //       case when last partition is free
    //     - add the new memory at the end of partition list
    //     - the last partition will be the best partition
    // - split the best partition in two if necessary
    //     - mark the first partition occupied, and store the tag in it
    //     - mark the second partition free
  }
  void deallocate(int tag) { 
    // Pseudocode for deallocation request:
    // - for every partition
    //     - if partition is occupied and has a matching tag:
    //         - mark the partition free
    //         - merge any adjacent free partitions
  }
  MemSimResult getStats() {
    // let's guess the result... :)
    MemSimResult result;
    result.max_free_partition_size = 123;
    result.max_free_partition_address = 321;
    result.n_pages_requested = 111;
    return result;
  }
  void check_consistency() {
    // make sure the sum of all partition sizes in your linked list is
    // the same as number of page requests * page_size

    // make sure your addresses are correct

    // make sure the number of all partitions in your tag data structure + 
    // number of partitions in your free blocks is the same as the size 
    // of the linked list

    // make sure that every free partition is in free blocks

    // make sure that every partition in free_blocks is actually free

    // make sure that none of the partition sizes or addresses are < 1
  }
};


// re-implement the following function
// ===================================
// parameters:
//    page_size: integer in range [1..1,000,000]
//    requests: array of requests
// return:
//    some statistics at the end of simulation
MemSimResult mem_sim(int64_t page_size, const std::vector<Request> & requests)
{
  Simulator sim(page_size);
  for (const auto & req : requests) {
    if (req.tag < 0) {
      sim.deallocate(-req.tag);
    } else {
      sim.allocate(req.tag, req.size);
    }
    sim.check_consistency();
  }
  return sim.getStats();
}
