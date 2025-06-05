#include "graph.h"
#include <algorithm> // Untuk std::minmax

// Konstruktor: Menginisialisasi graph dengan n node.
Graph::Graph(int n) : numNodes(n), adj(n + 1) {}

// Fungsi untuk mengatur jumlah node
void Graph::setNumNodes(int n) {
    numNodes = n;
    adj.assign(n + 1, std::vector<int>());
    edgeSet.clear();
}

// Fungsi untuk mereset edge yang sebelumnya telah diinput
void Graph::clearEdges() {
    adj.assign(numNodes + 1, std::vector<int>());
    edgeSet.clear();
}

// Fungsi untuk menambahkan edge
bool Graph::addEdge(int u, int v) {
    if (u == v) {
        std::cout << "Input diterima, namun saat operasi pencarian bridge, edge ini akan diabaikan.\n";
        
    }
    if (u < 1 || v < 1 || u > numNodes || v > numNodes) {
        std::cout << "Input tidak valid: Node harus dalam rentang [1, " << numNodes << "].\n";
        return false;
    }
    std::pair<int, int> edge = std::minmax(u, v); // Simpan edge dalam format standar (min, max)
    if (edgeSet.count(edge)) {
        std::cout << "Input tidak valid: Edge (" << u << "-" << v << ") sudah ada.\n";
        return false;
    }
    adj[u].push_back(v);
    adj[v].push_back(u);
    edgeSet.insert(edge);
    return true;
}

// Fungsi untuk menampilkan graf yang sudah kita input
void Graph::showGraph() const {
    std::cout << "Representasi Adjacency List Graf Saat Ini:\n";
    for (int u = 1; u <= numNodes; ++u) {
        std::cout << u << " : ";
        for (int v_neighbor : adj[u]) {
            std::cout << v_neighbor << " ";
        }
        std::cout << "\n";
    }
}

// Fungsi untuk menyimpan graf menjadi file (.txt)
void Graph::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error: Gagal membuka file '" << filename << "' untuk menyimpan graf.\n";
        return;
    }

    out << numNodes << "\n";
    for (const auto& edge : edgeSet) { // Iterasi melalui edgeSet untuk konsistensi
        out << edge.first << " " << edge.second << "\n";
    }

    out.close();
    std::cout << "Graf berhasil disimpan ke: " << filename << "\n";
}

// Fungsi untuk memuat file (.txt) yang berisi graf
void Graph::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Error: Gagal membuka file '" << filename << "' untuk memuat graf.\n";
        return;
    }

    int n_from_file;
    in >> n_from_file;
    if (in.fail() || n_from_file < 0) {
        std::cerr << "Error: Format file tidak valid atau jumlah node negatif di '" << filename << "'.\n";
        in.close();
        return;
    }
    
    // Inisialisasi ulang graph
    this->setNumNodes(n_from_file); // Gunakan setNumNodes untuk mereset adj dan edgeSet

    int u, v;
    while (in >> u >> v) {
        if (in.fail()) {
            std::cerr << "Error: Format edge tidak valid dalam file '" << filename << "'.\n";
            // Mungkin ingin menghentikan pemuatan atau melewati baris yang salah
            break; 
        }
        addEdge(u, v); // addEdge sudah memiliki validasi
    }

    in.close();
    std::cout << "Graf berhasil dimuat dari: " << filename << "\n";
}

// Fungsi untuk memanggil batas node yang telah kita input
int Graph::getNumNodes() const {
    return numNodes;
}

// Fungsi untuk memanggil node apakah bersebelahan atau tidak (adjacency list)
const std::vector<std::vector<int>>& Graph::getAdjList() const {
    return adj;
}