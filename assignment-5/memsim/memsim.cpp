// this is the file you need to edit
// -------------------------------------------------------------------------------------
// you don't have to use any of the code below, but you can

#include "memsim.h"
#include <iostream>
#include <cassert>
#include <unordered_map>
#include <list>

// I suggest you implement the simulator as a class. This is only a suggestion.
// Feel free to ignore it.
struct Partition {
  int tag;
  int64_t start;
  int64_t size;
  Partition* predecessor;
  Partition* successor;

  Partition(int64_t start, int64_t size) {
    this->tag = NULL;
    this->start = start;
    this->size = size;
    this->predecessor = NULL;
    this->successor = NULL;
  }

  bool occupied() {
    return this->tag != NULL;
  }
};

struct Simulator {
  int64_t page_size;
  int64_t page_requested;
  std::list<Partition> partitions;                                  // [Partition]
  std::list<Partition*> unoccupied;                                 // [Partition Pointer]
  std::unordered_map<int, std::list<Partition*>> tag_partitions;    // Tag: [Partition Pointer]

  Simulator(int64_t page_size) {
    this->page_size = page_size;
    this->page_requested = 0;
    this->partitions = std::list<Partition>();
    this->unoccupied = std::list<Partition*>();
  }

  void allocate(int tag, int64_t size) {
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

    // If just started
    if (partitions.empty()) {
      int64_t new_pages = size / this->page_size + 1;
      Partition head = Partition(0, new_pages * this->page_size);
      this->page_requested += new_pages;

      this->partitions.push_back(head);
      this->unoccupied.push_back(&head);
    }

    // Local variables
    bool has_suitable = false;
    Partition* suitable;

    if (!unoccupied.empty()) { // Have unoccupied partitions, find largest suitable partition
      int64_t suitable_size = 0;

      for (Partition* each : unoccupied) {
        if (each->size > suitable_size) {
          suitable_size = each->size;
          suitable = each;
          has_suitable = true;
        }
      }
    }

    // If tag_partition for tag is not intialized, then initialize
    if (!this->tag_partitions.contains(tag)) {
      this->tag_partitions[tag] = std::list<Partition*>();
    }

    // Split partiton OR expand by page
    if (!has_suitable) {
      // No suitable unoccupied partition, expand partition at the end
      Partition last = *this->partitions.end();
      int64_t total_last_size = 0;

      // Last partition unoccupied, make its size to total last size
      if (last.tag == NULL) {
        total_last_size += last.size;
      }

      // Add pages
      int64_t new_pages = (size - last.size) / this->page_size + 1;
      total_last_size += new_pages * this->page_size;
      this->page_requested += new_pages;

      // Occupy last partition
      last.tag = tag;
      last.size = size;
      tag_partitions[tag].push_back(&last);
      total_last_size -= size;

      // Append Leftover size if there is any
      if (total_last_size > 0) {
        int64_t start = last.start + last.size;
        Partition new_last = Partition(start, total_last_size);

        this->partitions.push_back(new_last);
        this->unoccupied.push_back(&new_last);

        last.successor = &new_last; // connects
        new_last.predecessor = &last;
      }

    } else { // Split partition

      int64_t total_suitable_size = suitable->size;
      int64_t leftover_size = total_suitable_size - size;

      // Occupy partition
      suitable->tag = tag;
      suitable->size = size;
      this->tag_partitions[tag].push_back(suitable);

      // Append leftover partition
      if (leftover_size > 0) {
        int64_t leftover_start = suitable->start + size;
        Partition leftover = Partition(leftover_start, leftover_size);

        leftover.predecessor = suitable; // connects
        leftover.successor = suitable->successor;
        if (leftover.successor != NULL) {
          (*leftover.successor).predecessor = &leftover;
        }
        suitable->successor = &leftover;

        this->unoccupied.push_back(&leftover);
      }

      // Remove occupied partition from occupied
      this->unoccupied.remove(suitable);
    }
  }

  void deallocate(int tag) {
    // Pseudocode for deallocation request:
    // - for every partition
    //     - if partition is occupied and has a matching tag:
    //         - mark the partition free
    //         - merge any adjacent free partitions

    std::list<Partition*> partitions = this->tag_partitions[tag];
    for (Partition* partition : partitions) {
      partition->tag = NULL;
      this->unoccupied.push_back(partition);

      // If has predecessor and predecessor is unoccupied
      Partition* predecessor = partition->predecessor;
      if (predecessor != NULL && predecessor->tag == NULL) {
        // Predecessor is unoccupied, merge with partition
        partition->start = predecessor->start;
        partition->size += predecessor->size;

        Partition* predecessor_predecessor = predecessor->predecessor;
        predecessor_predecessor->successor = partition;
        partition->predecessor = predecessor_predecessor;

        this->unoccupied.remove(predecessor);
      }

      // If has successor and successor is unoccupied
      Partition* successor = partition->successor;
      if (successor != NULL && successor->tag == NULL) {
        // Successor is unoccupied, merge with partition
        partition->size += successor->size;

        Partition* successor_successor = successor->successor;
        successor_successor->predecessor = partition;
        partition->successor = successor_successor;

        this->unoccupied.remove(successor);
      }
    }

    this->tag_partitions[tag].clear();
  }

  MemSimResult getStats() {
    MemSimResult result;

    int64_t max_size = 0;
    int64_t max_start = 0;
    for (Partition* each : unoccupied) {
      int64_t each_size = each->size;
      int64_t each_start = each->start;
      if (each_size >= max_size && each_start < max_start) {
        max_size = each_size;
        max_start = each_start;
      }
    }
    result.max_free_partition_size = max_size;
    result.max_free_partition_address = max_start;

    result.n_pages_requested = this->page_requested;
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
MemSimResult mem_sim(int64_t page_size, const std::vector<Request>& requests)
{
  Simulator sim(page_size);
  for (const auto& req : requests) {
    if (req.tag < 0) {
      sim.deallocate(-req.tag);
    } else {
      sim.allocate(req.tag, req.size);
    }
    sim.check_consistency();
  }
  return sim.getStats();
}
