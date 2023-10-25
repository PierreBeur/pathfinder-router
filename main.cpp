#include <iostream>
#include <vector>
#include <utility> // pair

using namespace std;

struct Problem {
    // signals to be routed given as pairs of source and sink nodes (s_i, t_ij)
    vector<pair<int, int>> signals;
    // intrinsic delay of each node, assumed to be the same for all nodes (d_n)
    double node_delay = 1.0;
    // directed graph given as an adjacency list (G = (V, E))
    vector<vector<int>> adj_list;

    Problem(vector<pair<int, int>> s, vector<vector<pair<int, int>>> pp) : signals(s) {
        // add definite nodes
        adj_list.resize(pp.size());
        // construct full adjacency list from partial paths
        // loop over all starting nodes
        for (int start = 0; start < pp.size(); start++) {
            // loop over all partial paths from starting node
            for (int partial_path = 0; partial_path < pp[start].size(); partial_path++) {
                int dest = pp[start][partial_path].first;
                int length = pp[start][partial_path].second;
                // construct full path
                vector<int> full_path;
                for (int i = 0; i < length - 1; i++) {
                    int n = adj_list.size() + i;
                    full_path.push_back(n);
                }
                full_path.push_back(dest);
                // add full path to adjacency list
                adj_list[start].push_back(full_path[0]);
                for (int i = 0; i < length - 1; i++) {
                    adj_list.push_back({full_path[i + 1]});
                }
            }
        }
    }
    
    size_t size() {
        return adj_list.size();
    }
};

int main() {
    // example problems

    vector<pair<int, int>> signals = {
    /*S1->D1*/  {0, 6},
    /*S2->D2*/  {1, 7},
    /*S3->D3*/  {2, 8},
    };

    // first order congestion
    vector<vector<pair<int, int>>> first_order_partial_paths = {
    /*S1*/  {{3, 2}, {4, 1}},
    /*S2*/  {{3, 3}, {4, 1}, {5, 4}},
    /*S3*/  {{4, 1}, {5, 3}},
    /*A */  {{6, 2}, {7, 3}},
    /*B */  {{6, 1}, {7, 1}, {8, 1}},
    /*C */  {{7, 4}, {8, 3}},
    /*D1*/  {},
    /*D2*/  {},
    /*D3*/  {},
    };
    Problem prob1(signals, first_order_partial_paths);

    // second order congestion
    vector<vector<pair<int, int>>> second_order_partial_paths = {
    /*S1*/  {{3, 2}, {4, 1}},
    /*S2*/  {{4, 2}, {5, 1}},
    /*S3*/  {{5, 1}},
    /*A */  {{6, 2}},
    /*B */  {{6, 1}, {7, 2}},
    /*C */  {{7, 1}, {8, 1}},
    /*D1*/  {},
    /*D2*/  {},
    /*D3*/  {},
    };
    Problem prob2(signals, second_order_partial_paths);

    return 0;
}
