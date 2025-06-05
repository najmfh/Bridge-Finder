#include "bridgeAlgorithm.h"
#include <iostream>
#include <fstream>
#include <algorithm> // Untuk std::min, std::max, std::fill, std::minmax
#include <set>       // Untuk std::set di exportToDot dan findBridgesBFS (internal)
#include <cstdlib>   // Untuk system()
#include <queue>     // Untuk std::queue di isConnectedAfterRemoving
#include <vector>    // Untuk std::vector
#include <chrono>

// Konstruktor BridgeFinder
BridgeFinder::BridgeFinder(const Graph& g) : graph(g), n(g.getNumNodes()), timer(0) {
    if (n > 0) {
        visited.assign(n + 1, false); // 'visited' bisa digunakan oleh kedua algo jika perlu reset
        disc.assign(n + 1, -1);    // Khusus DFS
        low.assign(n + 1, -1);     // Khusus DFS
    }
    bridges.clear(); // Selalu mulai dengan daftar bridge kosong
}

// --- Implementasi DFS (Tarjan) ---
void BridgeFinder::dfs(int u, int parent) {
    visited[u] = true;
    disc[u] = low[u] = timer++;

    const auto& adj = graph.getAdjList();
    if (static_cast<size_t>(u) >= adj.size()) return;

    for (int v : adj[u]) {
        if (v == parent) continue;
        if (v < 1 || v > n) continue;

        if (!visited[v]) {
            dfs(v, u);
            low[u] = std::min(low[u], low[v]);
            if (low[v] > disc[u]) {
                bridges.emplace_back(std::min(u, v), std::max(u, v));
            }
        } else {
            low[u] = std::min(low[u], disc[v]);
        }
    }
}

void BridgeFinder::findBridgesDFS() {
    auto start_time = std::chrono::high_resolution_clock::now();
    if (n == 0) {
        std::cout << "Graf kosong, tidak ada bridge untuk dicari.\n";
        bridges.clear(); // Pastikan bridges kosong
        return;
    }
    // Reset state khusus DFS
    timer = 0;
    std::fill(visited.begin(), visited.end(), false); // Reset visited
    std::fill(disc.begin(), disc.end(), -1);
    std::fill(low.begin(), low.end(), -1);
    bridges.clear(); // Hapus bridge dari pencarian sebelumnya

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            dfs(i, -1);
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Waktu eksekusi DFS: " << duration.count() << " mikrosekon\n";
}

bool BridgeFinder::isConnectedAfterRemoving(const Graph& currentGraph, int u_of_edge, int v_of_edge) {
    int num_nodes_in_graph = currentGraph.getNumNodes();

    // Kasus dasar: graf terlalu kecil untuk memiliki edge yang signifikan atau bridge
    if (num_nodes_in_graph < 2) {
        return true; // Dianggap "terhubung" atau tidak relevan untuk bridge
    }

    // Pastikan u_of_edge dan v_of_edge adalah node yang valid
    if (u_of_edge < 1 || u_of_edge > num_nodes_in_graph || 
        v_of_edge < 1 || v_of_edge > num_nodes_in_graph || u_of_edge == v_of_edge) {
        // Seharusnya tidak terjadi jika edge valid, tapi sebagai pengaman
        return true; // Atau throw error, anggap bukan bridge jika input aneh
    }

    std::vector<bool> visited_bfs(num_nodes_in_graph + 1, false);
    std::queue<int> q;

    // Mulai BFS dari salah satu endpoint edge yang "dihapus"
    q.push(u_of_edge);
    visited_bfs[u_of_edge] = true;

    const auto& adj_list = currentGraph.getAdjList();

    while (!q.empty()) {
        int u_curr = q.front();
        q.pop();

        // Jika kita berhasil mencapai endpoint lain (v_of_edge),
        // berarti mereka masih terhubung melalui path lain.
        if (u_curr == v_of_edge) {
            return true; // Masih terhubung
        }

        // Pemeriksaan batas untuk u_curr sebelum mengakses adj_list[u_curr]
        // (menggunakan static_cast untuk perbandingan aman dengan size_type)
        if (u_curr < 1 || static_cast<size_t>(u_curr) >= adj_list.size()) {
            continue; 
        }

        for (int v_neighbor : adj_list[u_curr]) {
            // Ini adalah langkah krusial: ABAIKAN edge (u_of_edge, v_of_edge) itu sendiri.
            if ((u_curr == u_of_edge && v_neighbor == v_of_edge) || \
                (u_curr == v_of_edge && v_neighbor == u_of_edge)) {
                continue;
            }

            // Pemeriksaan batas untuk v_neighbor
            if (v_neighbor < 1 || v_neighbor > num_nodes_in_graph) {
                continue;
            }

            if (!visited_bfs[v_neighbor]) {
                visited_bfs[v_neighbor] = true;
                q.push(v_neighbor);
            }
        }
    }

    // Jika loop BFS selesai dan v_of_edge tidak pernah tercapai dari u_of_edge (tanpa edge langsung mereka),
    // maka mereka menjadi terpisah.
    return false; // Tidak terhubung lagi, berarti edge tersebut adalah bridge
}

// --- Implementasi BFS Brute-Force (sekarang non-statik) ---
void BridgeFinder::findBridgesBFS() {
    auto start_time = std::chrono::high_resolution_clock::now();
    if (n < 2) {
        std::cout << "Graf terlalu kecil untuk memiliki bridge (kurang dari 2 node).\n";
        bridges.clear(); // Pastikan bridges kosong
        return;
    }
    bridges.clear(); // Hapus bridge dari pencarian sebelumnya

    const auto& adj_list = graph.getAdjList(); // Gunakan this->graph
    std::set<std::pair<int, int>> processed_edges;

    for (int u = 1; u <= n; ++u) {
        if (static_cast<size_t>(u) >= adj_list.size()) continue;
        for (int v : adj_list[u]) {
            if (v < 1 || v > n) continue;
            
            std::pair<int, int> current_edge = std::minmax(u, v);
            if (processed_edges.count(current_edge)) continue;
            processed_edges.insert(current_edge);

            if (!isConnectedAfterRemoving(this->graph, u, v)) { // Kirim this->graph
                bridges.push_back(current_edge); // Simpan ke this->bridges
            }
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Waktu eksekusi DFS: " << duration.count() << " mikrosekon\n";
}

void BridgeFinder::printBridges() const {
    if (n == 0 && bridges.empty()) { // Jika graf memang kosong dari awal
        // Pesan "Graf kosong" sudah dihandle di findBridgesDFS/BFS
        return;
    }
    if (bridges.empty()) {
        std::cout << "Tidak ada bridge yang ditemukan dalam graf ini (berdasarkan operasi terakhir).\n";
    } else {
        std::cout << "Bridge ditemukan (berdasarkan operasi terakhir):\n";
        for (const auto& bridge_pair : bridges) {
            std::cout << "  " << bridge_pair.first << " - " << bridge_pair.second << "\n";
        }
    }
}

void BridgeFinder::exportToDot(const std::string& filename) const {
    if (n == 0) {
        std::cout << "Graf kosong, tidak ada yang diekspor ke DOT.\n";
        return;
    }
    std::ofstream fout(filename);
    if (!fout) {
        std::cerr << "Error: Gagal membuka file '" << filename << "' untuk ekspor DOT.\n";
        return;
    }

    fout << "graph G {\n";
    fout << "  node [shape=circle];\n"; 

    std::set<std::pair<int, int>> bridgeSet(bridges.begin(), bridges.end()); // Gunakan this->bridges
    const auto& adj = graph.getAdjList();
    std::set<std::pair<int, int>> printed_edges;

    for (int u = 1; u <= n; ++u) {
        if (static_cast<size_t>(u) >= adj.size()) continue;
        for (int v : adj[u]) {
            if (v < 1 || v > n) continue;
            std::pair<int, int> edge = std::minmax(u, v);
            if (printed_edges.count(edge)) continue;
            printed_edges.insert(edge);

            fout << "  " << u << " -- " << v;
            if (bridgeSet.count(edge)) {
                fout << " [color=red, penwidth=2.0]";
            }
            fout << ";\n";
        }
    }
    if (printed_edges.empty() && n > 0) {
        for (int i = 1; i <= n; ++i) fout << "  " << i << ";\n";
    }
    fout << "}\n";
    fout.close();
    std::cout << "Visualisasi graf disimpan dalam format DOT ke: " << filename << "\n";
}

void BridgeFinder::exportToPNG(const std::string& dotFile, const std::string& pngFile) {
    std::string finalDotFile = dotFile;
    if (dotFile.rfind(".dot") == std::string::npos) finalDotFile += ".dot";
    std::string finalPngFile = pngFile;
    if (pngFile.rfind(".png") == std::string::npos) finalPngFile += ".png";

    std::string command = "dot -Tpng \"" + finalDotFile + "\" -o \"" + finalPngFile + "\"";
    std::cout << "Menjalankan perintah: " << command << std::endl;
    int result = system(command.c_str());

    if (result != 0) {
        std::cerr << "Error: Gagal mengkonversi '" << finalDotFile << "' ke '" << finalPngFile << "'.\n";
        std::cerr << "Pastikan Graphviz (dot) terinstal dan ada di PATH sistem Anda.\n";
    } else {
        std::cout << "Graf berhasil dikonversi ke: " << finalPngFile << "\n";
    }
}