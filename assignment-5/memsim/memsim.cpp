// this is the file you need to edit
// -------------------------------------------------------------------------------------
// you don't have to use any of the code below, but you can

#include "memsim.h"
#include <iostream>
#include <cassert>
#include <unordered_map>
#include <list>
#include <set>
#include <iterator>
#include <climits>

struct Partition {
  int tag;
  int64_t start;
  int64_t size;

  Partition(int64_t start, int64_t size, int tag = 0) {
    this->tag = tag;
    this->start = start;
    this->size = size;
  }

  bool occupied() {
    return this->tag != 0;
  }
};

typedef std::list<Partition>::iterator PartitionIterator;
typedef std::set<PartitionIterator>::iterator PartitionIteratorIterator;

struct PartitionCompare {
  bool operator()(const PartitionIterator& p1, const PartitionIterator& p2) const {
    if (p1->size == p2->size) {
      return p1->start < p2->start;
    } else {
      return p1->size > p2->size;
    }
  }
};

struct Simulator {
  int64_t page_size;
  int64_t page_requested;
  std::list<Partition> partitions;                                       // [Partition]
  std::set<PartitionIterator, PartitionCompare> unoccupied;              // [PartitionIterator]
  std::unordered_map<int, std::list<PartitionIterator>> tag_partitions;  // {Tag: [PartitionIterator]}

  Simulator(int64_t page_size) {
    this->page_size = page_size;
    this->page_requested = 0;
    this->partitions = std::list<Partition>();
    this->tag_partitions = std::unordered_map<int, std::list<PartitionIterator>>();
  }

  int64_t page_to_request(int64_t size) {
    int64_t page_size = this->page_size;
    int64_t remainder = size % page_size;

    if (remainder == 0) {
      return size / page_size;
    } else {
      return size / page_size + 1;
    }
  }

  void unoccupied_erase_by_partition_iterator(PartitionIterator partition_iterator) {
    PartitionIteratorIterator partition_iterator_iterator = this->unoccupied.find(partition_iterator);
    if (partition_iterator_iterator != this->unoccupied.end()) {
      this->unoccupied.erase(partition_iterator_iterator);
    }
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
      int64_t new_pages = this->page_to_request(size);
      Partition head = Partition(0, new_pages * this->page_size);
      this->page_requested += new_pages;
      this->partitions.push_back(head);
      this->unoccupied.insert(this->partitions.begin());
    }

    // Local variables
    bool has_suitable = false;
    PartitionIterator suitable;

    if (!this->unoccupied.empty()) { // Have unoccupied partitions, find largest suitable partition
      PartitionIteratorIterator max_unoccupied_iterator = this->unoccupied.begin();
      PartitionIterator max_unoccupied = *max_unoccupied_iterator;
      if (max_unoccupied->size >= size) {
        has_suitable = true;
        suitable = max_unoccupied;
        this->unoccupied.erase(max_unoccupied_iterator);
      }
    }

    // Split partiton OR expand by page
    if (!has_suitable) {
      // No suitable unoccupied partition, expand partition at the end
      PartitionIterator last = std::prev(this->partitions.end());

      // Last partition unoccupied, make its size to total last size
      if (!last->occupied()) {
        int64_t total_last_size = last->size;

        // Add pages
        int64_t new_pages = this->page_to_request(size - last->size);
        total_last_size += new_pages * this->page_size;
        this->page_requested += new_pages;

        // Occupy last partition
        this->unoccupied_erase_by_partition_iterator(last);
        last->tag = tag;
        last->size = size;
        this->tag_partitions[tag].push_back(last);
        total_last_size -= size;

        // Append Leftover size if there is any
        if (total_last_size > 0) {
          int64_t start = last->start + last->size;
          Partition new_last = Partition(start, total_last_size);
          this->partitions.push_back(new_last);
          this->unoccupied.insert(std::prev(this->partitions.end()));
        }

      } else {
        // Make new partition at the end by new page
        int64_t new_pages = this->page_to_request(size);
        this->page_requested += new_pages;
        int64_t total_last_size = new_pages * this->page_size;
        int64_t leftover_size = total_last_size - size;

        // New partition
        int64_t new_last_start = last->start + last->size;
        Partition new_last = Partition(new_last_start, size, tag);
        this->partitions.push_back(new_last);
        this->tag_partitions[tag].push_back(std::prev(this->partitions.end()));

        // Append leftover partition
        if (leftover_size > 0) {
          int64_t leftover_start = new_last_start + size;
          Partition leftover = Partition(leftover_start, leftover_size);
          this->partitions.push_back(leftover);
          this->unoccupied.insert(std::prev(this->partitions.end()));
        }

      }

    } else {
      // Split partition
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

        this->partitions.insert(std::next(suitable), leftover);
        this->unoccupied.insert(std::next(suitable));
      }
    }
  }

  void deallocate(int tag) {
    // Pseudocode for deallocation request:
    // - for every partition
    //     - if partition is occupied and has a matching tag:
    //         - mark the partition free
    //         - merge any adjacent free partitions

    std::list<PartitionIterator> partitions = this->tag_partitions[tag];

    for (PartitionIterator partition : partitions) {
      partition->tag = 0;

      // If has predecessor
      if (partition != this->partitions.begin()) {
        PartitionIterator predecessor = std::prev(partition);

        // And predecessor is unoccupied, merge with partition
        if (!predecessor->occupied()) {
          partition->start = predecessor->start;
          partition->size += predecessor->size;
          this->partitions.erase(predecessor);
          this->unoccupied_erase_by_partition_iterator(predecessor);
        }
      }

      // If has successor
      if (partition != std::prev(this->partitions.end())) {
        PartitionIterator successor = std::next(partition);

        // And and successor is unoccupied
        if (!successor->occupied()) {
          // Successor is unoccupied, merge with partition
          partition->size += successor->size;
          this->partitions.erase(successor);
          this->unoccupied_erase_by_partition_iterator(successor);
        }
      }

      this->unoccupied.insert(partition);
    }

    this->tag_partitions[tag].clear();
  }

  MemSimResult getStats() {
    MemSimResult result;

    if (this->unoccupied.empty()) {
      result.max_free_partition_size = 0;
      result.max_free_partition_address = 0;
    } else {
      PartitionIteratorIterator max_unoccupied_iterator = this->unoccupied.begin();
      PartitionIterator max_unoccupied = *max_unoccupied_iterator;
      result.max_free_partition_size = max_unoccupied->size;
      result.max_free_partition_address = max_unoccupied->start;
    }

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
