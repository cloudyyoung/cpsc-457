// this is the file you need to edit

#include "fatsim.h"
#include <cstdio>
#include <stack>
#include <iostream>
#include <map>
#include <algorithm>

// reimplement this function
std::vector<long> fat_check(const std::vector<long>& fat)
{
  std::vector<bool> visited = std::vector<bool>(fat.size());
  std::vector<bool> cycle = std::vector<bool>(fat.size());
  std::vector<long> destination = std::vector<long>(fat.size());
  std::vector<long> depth = std::vector<long>(fat.size());
  std::map<long, long> destination_max;

  for (long int each = 0; each < long(fat.size()); each++) {

    if (visited[each]) {
      continue;
    }

    // Record searched nodes
    std::vector<long> search;
    std::vector<bool> search_visited = std::vector<bool>(fat.size());
    long at = each;
    long to_destination = -1;
    long to = 0;
    long to_depth = 0;
    bool to_cycle = false;

    while (true) {

      // Push to history and mark visited for this search
      search.push_back(at);
      search_visited[at] = true;

      // Get pointing to
      to = fat[at];

      if (to == -1) {
        // Pointing to -1
        to_depth = 0;
        to_cycle = false;
        to_destination = at;
        break;
      } else if (cycle[to] || search_visited[to]) {
        // Pointing to a cycle
        to_cycle = true;
        break;
      } else if (visited[to]) {
        // Find a visited node, take its depth
        to_depth = depth[to];
        to_cycle = false;
        to_destination = destination[to];
        break;
      }

      at = to;
    }

    // Backward marking each searched node
    for (long t = search.size() - 1; t >= 0; t--) {
      long each = search[t];
      visited[each] = true;
      destination[each] = to_destination;

      if (to_cycle) {
        cycle[each] = true;
      } else {
        to_depth++;
        depth[each] = to_depth;
      }
    }

    if (depth[each] > destination_max[to_destination]) {
      destination_max[to_destination] = depth[each];
    }

  }

  // Search for depth
  std::vector<long> size;
  for (auto each = destination_max.begin(); each != destination_max.end(); each++) {
    if (each->second > 0) {
      size.push_back(each->second);
    }
  }
  std::sort(size.begin(), size.end());

  return size;
}

