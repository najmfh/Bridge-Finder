#include "bridgeAlgorithm.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <cstdlib>

BridgeFinder::BridgeFinder(const Graph& g) : graph(g), n(g.getNumNodes()), timer(0){
    visited.assign(n+1, false);
    disc.assign(n+1, -1);
    low.assign(n+1, -1);
    bridges.clear();
}

void BridgeFinder::dfs(int u, int parent){
    visited[u] = true;
    disc[u] = low[u] = timer++;

    const auto& adj = graph.getAdjList();

    for (int v : adj[u]) {
        if (v == parent) continue;
        if (!visited[v]) {
            dfs(v, u);
            low[u] = std::min(low[u], low[v]);
            if (low[v] > disc[u]){
                bridges.emplace_back(std::min(u, v), std::max(u,v));
            }
        } else {
            low[u] = std::min(low[u], disc[v]);
        }
    }
}

void BridgeFinder::findBridges(){
    timer = 0;
    std::fill(visited.begin(), visited.end(), false);
    std::fill(disc.begin(), disc.end(), -1);
    std::fill(low.begin(), low.end(), -1);
    bridges.clear();

    for (int i = 1; i <= n; ++i){
        if (!visited[i]) {
            dfs(i, -1);
        }
    }
}

void BridgeFinder::printBridges() const{
    if (bridges.empty()){
        std::cout << "Tidak ada bridge dalam graph ini.\n";
    } else{
        std::cout << "Bridge ditemukan!\n";
        for (const auto& [u, v] : bridges) {
            std::cout << " " << u << " - " << v << "\n"; 
        }
    }
}

void BridgeFinder::exportToDot(const std::string& filename) const {
    std::ofstream fout(filename);
    fout << "graph G {\n";

    std::set<std::pair<int,int>> bridgeSet(bridges.begin(), bridges.end());
    const auto& adj = graph.getAdjList();

    std::vector<std::vector<bool>> printed(n + 1, std::vector<bool>(n + 1, false));

    for (int u = 1; u <= n; ++u) {
        for (int v : adj[u]) {
            if (printed[u][v] || printed[v][u]) continue;
            printed[u][v] = printed[v][u] = true;

            fout << "  " << u << " -- " << v;
            auto e = std::minmax(u, v);
            if (bridgeSet.count(e)) {
                fout << " [color=red, penwidth=2.0]";
            }
            fout << ";\n";
        }
    }
    fout << "}\n";
    fout.close();
}

void BridgeFinder::exportToPNG(const std::string& dotFile, const std::string& pngFile) {
    std::string command = "dot -Tpng " + dotFile + " -o " + pngFile;
    int result = system(command.c_str());

    if (result != 0) {
        std::cerr << "Failed to convert visual graph.\n";
    } else {
        std::cout << "Successfully convert to: " << pngFile << "\n";
    }
}