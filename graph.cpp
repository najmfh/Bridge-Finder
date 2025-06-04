#include "graph.h"
#include <algorithm>

Graph::Graph(int n) : numNodes(n), adj(n + 1) {} //Variabel yang digunakan di sini.

//Fungsi untuk mengatur jumlah node
void Graph::setNumNodes(int n) {
    numNodes = n;
    adj.assign(n + 1, std::vector<int>());
    edgeSet.clear();
}

//Fungsi untuk mereset edge yang sebelumnya telah diinput
void Graph::clearEdges() {
    adj.assign(numNodes + 1, std::vector<int>());
    edgeSet.clear();
}

//Fungsi untuk menambahkan edge
bool Graph::addEdge(int u, int v) {
    if (u == v) {
        std::cout << "Don't input self-loop.\n";
        return false;
    }
    if (u < 1 || v < 1 || u > numNodes || v > numNodes) {
        std::cout << "Node must be in the interval 1 until " << numNodes << ".\n";
        return false;
    }
    std::pair<int, int> edge = std::minmax(u, v);
    if (edgeSet.count(edge)) {
        std::cout << "Edge has been created.\n";
        return false;
    }
    adj[u].push_back(v);
    adj[v].push_back(u);
    edgeSet.insert(edge);
    return true;
}

//Fungsi untuk menampilkan graf yang sudah kita input
void Graph::showGraph() const{
    std::cout << "Graph saat ini :\n";
    for (int u = 1; u <= numNodes; ++u){    // u sebagai batas atas node
        std::cout << u << " : ";
        for (int v : adj[u]){               // v sebagai tetangga node
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
}

//Fungsi untuk menyimpan graf menjadi file (.txt)
void Graph::saveToFile(const std::string& filename) const{
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Failed to save the graph.\n";
        return;
    }
    
    out << numNodes << "\n";
    for (const auto& [u, v] : edgeSet){
        out << u << " " << v << "\n";
    }

    out.close();
    std::cout << "Graph has been successfully saved into :" << filename << "\n";
}

//Fungsi untuk memuat file (.txt) yang berisi graf
void Graph::loadFromFile(const std::string& filename){
    std::ifstream in(filename);
    if (!in){
        std::cerr << "Failed to load the file.\n";
        return;
    }

    in >> numNodes;
    adj.assign(numNodes +1, {});
    edgeSet.clear();

    int u, v;
    while (in >> u >> v){
        addEdge(u, v);
    }

    in.close();
    std::cout << "Graph has been loaded from : " << filename << "\n";
}

//Fungsi untuk memanggil batas node yang telah kita input
int Graph::getNumNodes() const {
    return numNodes;
}

//Fungsi untuk memanggil node apakah bersebelahan atau tidak
const std::vector<std::vector<int>>& Graph::getAdjList() const {
    return adj;
}