#include <iostream>
#include <string>
#include <vector>
#include <limits> 
#include <cstdlib> 
#include "graph.h"
#include "bridgeAlgorithm.h"

void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

void separatorTitle() {
    std::cout << "\n" << std::string(43, '=') << std::endl;
}

void pressEnterToContinue() {
    std::cout << "Tekan Enter untuk melanjutkan...";
    std::cin.get(); 
}

int main() {
    Graph* graph = nullptr;
    BridgeFinder* bridgeFinder = nullptr; // Satu instance BridgeFinder

    int choice;

    do {
        clearScreen();
        separatorTitle();
        std::cout << ">>>\t\tBridge Finder\t\t<<<";
        separatorTitle();
        std::cout << "\t>>>>>-|   Menu   |-<<<<<\n";
        std::cout << "(1) Input Jumlah Node\n";
        std::cout << "(2) Input Edge\n";
        std::cout << "(3) Tampilkan Adjacency List Graf\n";
        std::cout << "(4) Cari Bridge (DFS - Algoritma Tarjan)\n";
        std::cout << "(5) Cari Bridge (BFS - Brute Force)\n";
        std::cout << "(6) Visualisasi Graf & Bridge (Hasil Terakhir) ke .dot\n";
        std::cout << "(7) Konversi .dot ke .png\n";
        std::cout << "(8) Simpan Graf ke .txt\n";
        std::cout << "(9) Muat Graf dari .txt\n";
        std::cout << "(0) Keluar\n";
        std::cout << "Pilih Opsi : ";

        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Input tidak valid. Mohon masukkan angka.\n";
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            choice = -1; 
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: { // Input Jumlah Node
                int n_nodes;
                std::cout << "Masukkan jumlah maksimum node: ";
                std::cin >> n_nodes;
                if (std::cin.fail() || n_nodes < 0) {
                    std::cout << "Input jumlah node tidak valid.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    delete graph;
                    delete bridgeFinder;
                    graph = new Graph(n_nodes);
                    if (n_nodes > 0) {
                        bridgeFinder = new BridgeFinder(*graph);
                    } else {
                        bridgeFinder = nullptr; // Tidak ada bridge finder untuk graf kosong
                    }
                    std::cout << "Jumlah node diatur ke " << n_nodes << ". Graf & Bridge Finder direset.\n";
                }
                break;
            }

            case 2: { // Input Edge
                if (!graph) {
                    std::cout << "Harap input jumlah node (opsi 1) atau muat graf (opsi 9) terlebih dahulu!\n";
                    break;
                }
                if (graph->getNumNodes() == 0) {
                     std::cout << "Jumlah node adalah 0. Harap input jumlah node (opsi 1) yang valid.\n";
                     break;
                }
                // Jika ada bridgeFinder, hapus bridges lama karena edge berubah
                if(bridgeFinder) {
                    // Cara "mereset" bridge di bridgeFinder bisa dengan memanggil salah satu findBridges... dengan graph kosong,
                    // atau menambah fungsi clearBridges() di BridgeFinder, atau buat ulang bridgeFinder.
                    // Untuk simpelnya, buat ulang:
                    delete bridgeFinder;
                    bridgeFinder = new BridgeFinder(*graph); // Re-create dengan graph yang ada (yang akan diisi edge)
                } else if (graph->getNumNodes() > 0) { // Jika graph ada tapi bridgeFinder belum
                     bridgeFinder = new BridgeFinder(*graph);
                }

                graph->clearEdges(); // Hapus edge lama di graph
                int m_edges;
                std::cout << "Masukkan jumlah edge yang akan diinput: ";
                std::cin >> m_edges;
                if (std::cin.fail() || m_edges < 0) {
                    std::cout << "Input jumlah edge tidak valid.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    std::cout << "Input edge (Format: Node1 Node2) untuk node dari 1 s/d " << graph->getNumNodes() << ":\n";
                    for (int i = 0; i < m_edges; ++i) {
                        int u, v;
                        std::cout << "Edge-" << i + 1 << " : ";
                        std::cin >> u >> v;
                        if (std::cin.fail()) {
                            std::cout << "Input edge tidak valid.\n";
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            --i; 
                        } else {
                             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                            if (!graph->addEdge(u, v)) {
                                --i; 
                            }
                        }
                    }
                }
                break;
            }

            case 3: { // Tampilkan Graf
                if (!graph) std::cout << "Graf belum dibuat.\n";
                else if (graph->getNumNodes() == 0) std::cout << "Graf kosong (tidak ada node).\n";
                else graph->showGraph();
                break;
            }

            case 4: { // Cari Bridge (DFS)
                if (!bridgeFinder) {
                    std::cout << "Graf belum siap atau kosong. Tidak bisa mencari bridge.\n";
                    break;
                }
                bridgeFinder->findBridgesDFS(); // Memperbarui bridgeFinder->bridges
                bridgeFinder->printBridges();   // Mencetak dari bridgeFinder->bridges
                break;
            }

            case 5: { // Cari Bridge (BFS)
                if (!bridgeFinder) {
                    std::cout << "Graf belum siap atau kosong. Tidak bisa mencari bridge.\n";
                    break;
                }
                bridgeFinder->findBridgesBFS(); // Memperbarui bridgeFinder->bridges
                bridgeFinder->printBridges();   // Mencetak dari bridgeFinder->bridges
                break;
            }

            case 6: { // Visualisasi Graf & Bridge (Hasil Terakhir) ke .dot
                if (!bridgeFinder) { 
                    std::cout << "Graf belum siap atau belum ada hasil pencarian bridge.\n";
                    std::cout << "Jalankan pencarian bridge (opsi 4 atau 5) terlebih dahulu.\n";
                    break;
                }
                std::string filename_dot;
                std::cout << "Masukkan nama file untuk visualisasi (.dot), misal 'graf_visual.dot': ";
                std::getline(std::cin, filename_dot);
                if (filename_dot.empty()) filename_dot = "graph_output.dot";
                bridgeFinder->exportToDot(filename_dot); // Menggunakan bridgeFinder->bridges
                break;
            }

            case 7: { // Konversi .dot ke .png
                std::string dot_file, png_file;
                std::cout << "Masukkan nama file .dot yang akan dikonversi: ";
                std::getline(std::cin, dot_file);
                std::cout << "Masukkan nama file output .png yang diinginkan: ";
                std::getline(std::cin, png_file);

                if (dot_file.empty() || png_file.empty()) {
                    std::cout << "Nama file tidak boleh kosong.\n";
                } else {
                    if (bridgeFinder) { // Gunakan instance yang ada jika tersedia
                         bridgeFinder->exportToPNG(dot_file, png_file);
                    } else if (graph && graph->getNumNodes() > 0) { // Buat temp jika graph ada tapi finder tidak
                         BridgeFinder tempBf(*graph); 
                         tempBf.exportToPNG(dot_file, png_file);
                    } else { // Jika tidak ada graph sama sekali
                        std::cout << "Tidak ada konteks graf aktif untuk konversi. "
                                  << "Memanggil konversi tanpa referensi graf...\n";
                        // Jika exportToPNG mau dibuat statik di masa depan:
                        // BridgeFinder::exportToPNG(dot_file, png_file);
                        // Untuk sekarang, panggil dengan dummy jika tidak ada bridgeFinder
                        Graph dummyGraph(0); // Graph kosong
                        BridgeFinder dummyBf(dummyGraph);
                        dummyBf.exportToPNG(dot_file, png_file);
                    }
                }
                break;
            }

            case 8: { // Simpan Graf ke .txt
                if (!graph) std::cout << "Graf belum dibuat.\n";
                else {
                    std::string filename_save;
                    std::cout << "Masukkan nama file untuk menyimpan graf (.txt): ";
                    std::getline(std::cin, filename_save);
                     if (filename_save.empty()) filename_save = "graph_data.txt";
                    graph->saveToFile(filename_save);
                }
                break;
            }

            case 9: { // Muat Graf dari .txt
                std::string filename_load;
                std::cout << "Masukkan nama file untuk memuat graf (.txt): ";
                std::getline(std::cin, filename_load);
                if (filename_load.empty()) {
                    std::cout << "Nama file tidak boleh kosong.\n";
                } else {
                    delete graph;
                    delete bridgeFinder;
                    graph = new Graph(0); 
                    graph->loadFromFile(filename_load);
                    
                    if (graph->getNumNodes() > 0) {
                        bridgeFinder = new BridgeFinder(*graph);
                        std::cout << "Graf dimuat dengan " << graph->getNumNodes() << " node. Bridge Finder direset.\n";
                    } else {
                        bridgeFinder = nullptr;
                         std::cout << "Graf yang dimuat tidak memiliki node, atau file tidak valid. Bridge Finder tidak dibuat.\n";
                    }
                }
                break;
            }

            case 0:
                std::cout << "Terima kasih telah menggunakan program Bridge Finder! ^.^\n";
                break;

            default:
                std::cout << "Pilihan tidak valid. Mohon pilih opsi dari menu.\n";
        }

        if (choice != 0) {
            pressEnterToContinue();
        }

    } while (choice != 0);

    delete graph;
    delete bridgeFinder;
    return 0;
}