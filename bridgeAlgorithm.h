#ifndef BRIDGEALGORITHM_H
#define BRIDGEALGORITHM_H

#include "graph.h"
#include <vector>
#include <utility> // Untuk std::pair
#include <string>  // Untuk std::string

// Class ini untuk mencari bridge
class BridgeFinder {
public:
    BridgeFinder(const Graph& graph); 
    
    void findBridgesDFS(); // Menjalankan algoritma Tarjan (DFS)
    void findBridgesBFS(); // Menjalankan algoritma Brute-Force (BFS)
                           // Keduanya akan memperbarui this->bridges

    void printBridges() const; // Mencetak bridge yang terakhir ditemukan
    void exportToDot(const std::string& filename) const; // Ekspor graf dengan bridge terakhir ke DOT
    void exportToPNG(const std::string& dotFile, const std::string& pngFile);

private:
    const Graph& graph; 
    int n;              
    int timer; // Digunakan oleh DFS
    std::vector<bool> visited; // Digunakan oleh DFS dan mungkin helper BFS
    std::vector<int> disc; // Digunakan oleh DFS
    std::vector<int> low;  // Digunakan oleh DFS
    std::vector<std::pair<int, int>> bridges; // Menyimpan bridge yang terakhir ditemukan (oleh DFS atau BFS)

    void dfs(int u, int parent); // Helper DFS untuk algoritma Tarjan

    // Metode statik helper, bisa tetap statik karena tidak bergantung state BridgeFinder
    static bool isConnectedAfterRemoving(const Graph& currentGraph, int removeU, int removeV);
};

#endif