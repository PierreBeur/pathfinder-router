#include <iostream>
#include <vector>
#include <utility> // pair

using namespace std;

/*
example problems
weighted directed graphs are given as adjacency lists (G = (V, E))
*/
namespace ex {
    struct Prob {
        int node_count;
        vector<pair<int, int>> signals;
        vector<vector<pair<int, double>>> adj_list;

        Prob(vector<pair<int, int>> s, vector<vector<pair<int, double>>> al) {
            signals = s;
            adj_list = al;
            node_count = al.size();
        }
    };

    // signals to be routed given as pairs of source and sink nodes (s_i, t_i)
    vector<pair<int, int>> signals = {
    /*S1->D1*/  {0, 6},
    /*S2->D2*/  {1, 7},
    /*S3->D3*/  {2, 8},
    };

    // first order congestion
    vector<vector<pair<int, double>>> first_order_adj_list = {
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
    Prob ex1(signals, first_order_adj_list);

    // second order congestion
    vector<vector<pair<int, double>>> second_order_adj_list = {
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
    Prob ex2(signals, second_order_adj_list);
}

int main() {
    return 0;
}
