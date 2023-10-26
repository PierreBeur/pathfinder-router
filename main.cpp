#include <iostream>
#include <vector>
#include <utility> // pair
#include <queue> // priority_queue

using namespace std;

template <typename T>
using min_priority_queue = priority_queue<T, vector<T>, greater<T>>;

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
    
    size_t size() const {
        return adj_list.size();
    }
};

// check for nodes which are used by more than one signal
bool has_shared_resources(const vector<double>& node_signal_uses) {
    for (double n : node_signal_uses) {
        if (n > 1.0) {
            return true;
        }
    }
    return false;
}

void update_node_costs(vector<double>& costs, const vector<double>& base_costs, const vector<double>& congestion_history_costs, const vector<double>& signal_uses) {
    for (int n = 0; n < costs.size(); n++) {
        double b_n = base_costs[n];
        double h_n = congestion_history_costs[n];
        double p_n = signal_uses[n];
        costs[n] = (b_n + h_n) * p_n;
    }
}

vector<vector<int>> negotiated_congestion_router(const Problem& p) {
    vector<double> node_costs(p.size()); // c_n
    vector<double> node_base_costs(p.size(), p.node_delay); // b_n = d_n
    vector<double> node_congestion_history_costs(p.size(), 0.0); // h_n
    vector<double> node_signal_uses(p.size(), 1.0); // p_n

    vector<vector<int>> routing_trees(p.signals.size()); // RT_i

    // while shared resources exist (global router)
    bool first_iteration = true;
    int num_iterations = 8;
    while (num_iterations-- && (first_iteration || has_shared_resources(node_signal_uses))) {

        // update c_n for all nodes
        update_node_costs(node_costs, node_base_costs, node_congestion_history_costs, node_signal_uses);

        // loop over all signals i (signal router)
        for (int i = 0; i < p.signals.size(); i++) {
            pair<int, int> signal = p.signals[i];
            // rip up routing tree RT_i
            routing_trees[i].clear();
            routing_trees[i].push_back(signal.first); // add source node s_i to routing tree

            // loop until all sinks t_ij have been found
            bool all_sinks_found = false;
            while (!all_sinks_found) {
                // intialize priority queue PQ to RT_i at cost 0
                min_priority_queue<pair<double, int>> pq;
                for (int node : routing_trees[i]) {
                    pq.emplace(0.0, node);
                }
                vector<double> dist(p.size(), __DBL_MAX__);
                vector<int> backtrace(p.size(), -1);

                // loop until new sink t_ij is found
                while(!pq.empty()) {
                    double P_im = pq.top().first;
                    // remove lowest cost node m from PQ
                    int m = pq.top().second;
                    pq.pop();

                    // check if m is a sink t_ij
                    if (m == signal.second) {
                        // found new sink t_ij
                        break;
                    }

                    // loop over fanouts n of node m
                    for (int n : p.adj_list[m]) {
                        double cost = node_costs[n] + P_im;
                        if (cost < dist[n]) {
                            dist[n] = cost;
                            // add n to PQ at cost c_n + P_im
                            pq.emplace(cost, n);
                            // update backtrace
                            backtrace[n] = m;
                        }
                    }
                }
                // only one sink per signal
                all_sinks_found = true;
                // find path from t_ij to s_i using backtrace
                vector<int> reverse_path;
                int curr = signal.second; // t_ij
                while (curr != signal.first) {
                    reverse_path.push_back(curr);
                    curr = backtrace[curr];
                }
                // loop over nodes n in path t_ij to s_i (backtrace)
                while (!reverse_path.empty()) {
                    // add n to RT_i
                    routing_trees[i].push_back(reverse_path.back());
                    reverse_path.pop_back();
                }
            }
        }

        // update congestion
        for (double& su : node_signal_uses) {
            su = 0.0;
        }
        for (auto rt : routing_trees) {
            for (int node : rt) {
                node_signal_uses[node] += 1.0;
                node_congestion_history_costs[node] += 0.0;
            }
        }
        for (double& su : node_signal_uses) {
            su = max(1.0, su);
        }

        first_iteration = false;
    }

    return routing_trees;
}

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

    // display adjacency list in https://visualgo.net/en/graphds format
    cout << "Problem adjacency list: \n";
    cout << prob1.size() << endl;
    for (int n = 0; n < prob1.size(); n++) {
        cout << prob1.adj_list[n].size() << ' ';
        for (auto e : prob1.adj_list[n]) {
            cout << e << ' ';
        }
        cout << endl;
    }

    auto routing_trees = negotiated_congestion_router(prob1);

    // display results
    for (int i = 0; i < routing_trees.size(); i++) {
        cout << "Routing tree for signal " << i << ": \n";
        for (int n : routing_trees[i]) {
            cout << n << ' ';
        }
        cout << endl;
    }

    return 0;
}
