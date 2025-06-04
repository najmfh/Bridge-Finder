#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <set>
#include <fstream>

class Graph {
    public:
        Graph(int n = 0);                                           // variable graph
        void setNumNodes(int n);
        int getNumNodes() const;                                    //
        bool addEdge(int u, int v);                                 // input edge
        void clearEdges();
        void showGraph() const;                                     // menampilkan graph
        void saveToFile(const std::string& filename) const;
        void loadFromFile(const std::string& filename);
        const std::vector<std::vector<int>>& getAdjList() const;    //

    private:
        int numNodes;                                                      //
        std::vector<std::vector<int>> adj;                          //
        std::set<std::pair<int,int>> edgeSet;                       //
};

#endif