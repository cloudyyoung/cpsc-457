// this is the ONLY file you should edit and submit to D2L

#include "deadlock_detector.h"
#include "common.h"

class FastGraph {
    std::vector<std::vector<int>> adj_list;
    std::vector<int> out_counts;
} graph;

/// this is the function you need to (re)implement
///
/// parameter edges[] contains a list of request- and assignment- edges
///   example of a request edge, process "p1" resource "r1"
///     "p1 -> r1"
///   example of an assignment edge, process "XYz" resource "XYz"
///     "XYz <- XYz"
///
/// You need to process edges[] one edge at a time, and run a deadlock
/// detection after each edge. As soon as you detect a deadlock, your function
/// needs to stop processing edges and return an instance of Result structure
/// with edge_index set to the index that caused the deadlock, and cycle set
/// to contain with names of processes that are in the deadlock.
///
/// To indicate no deadlock was detected after processing all edges, you must
/// return Result with edge_index = -1 and empy cycle[].
///
Result detect_deadlock(const std::vector<std::string>& edges)
{
    Result result;

    

    return result;
}
