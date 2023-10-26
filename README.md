# PathFinder Routing Algorithm Implementation

**Citation:**

> McMurchie, L., & Ebeling, C. (1995). *PathFinder: A Negotiation-Based Performance-Driven Router for FPGAs.* Proceedings of the 1995 ACM Third International Symposium on Field-Programmable Gate Arrays. (111–117). Association for Computing Machinery. [Link to the paper](https://dl.acm.org/doi/10.1145/201310.201328) and [PDF](https://dl.acm.org/doi/pdf/10.1145/201310.201328)

From the paper:

>PathFinder \[is\] an iterative algorithm that balances the competing goals of eliminating congestion and minimizing delay of critical paths in an iterative framework. In this framework, signals are allowed to share routing resources initially, but subsequently must negotiate with other signals to determine which signal needs the shared resource most. A timing analysis is performed every iteration to apply pressure continuously to routes that can potentially become critical if left unchecked. The emphasis of our approach is to adjust the costs of routing resources in a gradual, semi-equilibrium fashion to achieve an optimum distribution of resources.
>
>PathFinder is composed of two parts: a signal router, which routes one signal at a time using a shortest-path algorithm, and a global router, which calls the signal router to route all signals, adjusting the resource costs in order to achieve a complete routing. The signal router uses a breadth-first search to find the shortest path given a congestion cost and delay for each routing resource. The global router dynamically adjusts the congestion penalty of each routing resource based on the demands signals place on that resource. During the first iteration of the global router there is no cost for sharing routing resources and individual routing resources may be used by more than one signal. However, during subsequent iterations the penalty is gradually increased so that signals in effect negotiate for resources. Signals may use shared resources that are in high demand if all alternative routes utilize resources in even higher demand; other signals will tend to spread out and use resources in lower demand. The global router reroutes signals using the signal router until no more resources are shared.

This C++ program is my attempt at implementing the routing algorithm described in the PathFinder paper. The program currently includes a basic implementation of the Negotiated Congestion algorithm, and only routes signals with exactly one sink. I have been unable to get the algorithm to iteratively converge on a solution; instead, it is stuck flipping between several invalid solutions. The paper does not go into detail on how specific congestion penalties such as $p_n$ (related to the number of other signals presently using a routing resource $n$) are adjusted by the global router. Gradually increasing congestion penalties appropriately is crucial to the ability of the program to converge on a solution.

The program includes example problems from figures 1 and 2 in the paper. The arcs in the figures represent partial paths, weighted with their associated costs. The program takes weighted directed graphs in this form and transforms them into unweighted directed graphs by expanding each arc into a sequence of nodes with equivalent cost.

## Build and Usage
Modify `main.cpp` to select example problem.
```
g++ main.cpp
./a.out
```
## Example Output
Program run using example problem 1.
```
Problem adjacency list: 
25
2 9 4
3 10 4 12
2 4 15
2 17 18
3 6 7 8
2 20 23
0
0
0
1 3
1 11
1 3
1 13
1 14
1 5
1 16
1 5
1 6
1 19
1 7
1 21
1 22
1 7
1 24
1 8
Routing tree for signal 0:
0 4 6
Routing tree for signal 1:
1 4 7
Routing tree for signal 2:
2 4 8
```
This output shows routing failure, with all three signals using node $4$ (node $B$ from figure 1 in the paper). The problem adjacency list can be input to the unweighted directed graphing tool at [visualgo.net/en/graphds](https://visualgo.net/en/graphds) to visualize the node graph, as seen below:

![graph](https://github.com/PierreBeur/pathfinder-router/assets/97627312/3084d165-6b2f-43b0-be7f-c640d38af43b)

Nodes $0$, $1$, and $2$ are sources $S_1$, $S_2$, and $S_3$, respectively.  
Nodes $3$, $4$, and $5$ are nodes $A$, $B$, and $C$, respectively.  
Nodes $6$, $7$, and $8$ are sinks $D_1$, $D_2$, and $D_3$, respectively.  
The remaining nodes are generated from expanding partial paths into a sequence of nodes with equivalent cost.
