#ifndef BRIDGEALGORITHM_H
#define BRIDGEALGORITHM_H

#include "graph.h"
#include <vector>
#include <utility>
#include <string>

//Class ini untuk mencari bridge
class BridgeFinder{
    public:
        BridgeFinder(const Graph& graph);
        void findBridges();
        void printBridges() const;
        void exportToDot(const std::string& filename) const;
        void exportToPNG(const std::string& dotFile, const std::string& pngFile);

    private:
        const Graph& graph;
        int n, timer;
        std::vector<bool> visited;
        std::vector<int> disc, low;
        std::vector<std::pair<int, int>> bridges;

        void dfs(int u, int parent);
};

#endif