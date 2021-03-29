
#include "deadlock_detector.h"
#include "common.h"

#include <iostream>
#include <vector>
#include <string>

class FastGraph {
public:
    std::vector<std::vector<int>> adjacency;
    std::vector<int> outs;
} graph;

class NodesDictionary {
    int counter = 0;
    std::unordered_map<int, std::string> dict1;
    std::unordered_map<std::string, int> dict2;

public:
    int get(const std::string& w) {
        auto f = dict2.find(w);

        if (f == dict2.end()) {
            // New string
            dict1[counter] = w;
            dict2[w] = counter;
            counter++;
            return counter - 1;
        } else {
            // Known string
            return dict2[w];
        }
    }

    std::string get(int t) {
        if (t < 0 || t > dict1.size()) {
            return "";
        }
        return dict1[t];
    }

    int size() {
        return counter;
    }

} dictionary;

const std::string REQUEST = "->";
const std::string ASSIGN = "<-";


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
/// return Result with edge_index = -1 and empty cycle[].
///
Result detect_deadlock(const std::vector<std::string>& edges)
{

    Result result;

    // Handle edges
    for (int t = 0; t < edges.size(); t++) {
        std::string edge = edges[t];
        std::vector<std::string> tokens = split(edge);
        int t1 = dictionary.get("[P]" + tokens[0]); // [P]a, use prefix to differentiate process / resource
        int t2 = dictionary.get("[R]" + tokens[2]); // [R]a
        int from, to;
        std::string sign = tokens[1];

        if (sign == REQUEST) {
            // t1 -> t2
            from = t1;
            to = t2;
        } else if (sign == ASSIGN) {
            // t1 <- t2
            from = t2;
            to = t1;
        } else {
            // Invalid input
            continue;
        }

        // Sizing to number of nodes
        // TODO: Resize 2x
        int size = dictionary.size();
        graph.outs.resize(size);
        graph.adjacency.resize(size);

        // Record out and incoming
        graph.adjacency[to].push_back(from);
        graph.outs[from] ++;

        // All nodes with 0 outdegree
        std::vector<int> outs = graph.outs;
        std::vector<int> zeros;
        for (int t = 0; t < graph.outs.size(); t++) {
            if (graph.outs[t] == 0) {
                zeros.push_back(t);
            }
        }

        // Remove nodes with 0 outdegree from their adjacency
        while (!zeros.empty()) {
            int node = zeros.back();
            zeros.pop_back();
            for (int adj : graph.adjacency[node]) {
                outs[adj]--;

                if (outs[adj] == 0) {
                    zeros.push_back(adj);
                }
            }
        }

        // Find cycle
        for (int t = 0; t < outs.size(); t++) {
            if (outs[t] != 0) {
                std::string node = dictionary.get(t);
                if (node.find("[P]") != std::string::npos) {
                    // Only if it's a process and it has outs
                    node.replace(0, 3, "");
                    result.cycle.push_back(node);
                }
            }
        }

        // If there's cycle
        if (!result.cycle.empty()) {
            result.edge_index = t;
            break;
        }
    }

    // If no cycle
    if (result.cycle.empty()) {
        result.edge_index = -1;
    }


    return result;
}
