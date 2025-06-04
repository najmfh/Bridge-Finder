#include <iostream>
#include "graph.h"
#include "bridgeAlgorithm.h"
#include <cstdlib>

using namespace std;

void separatorTitle(){
    std::cout << "\n" << string(43, '=') << endl;
}

int main(){
    Graph* graph = nullptr;
    BridgeFinder* bridgeFinder = nullptr;

    int choice;

    do {
        system("cls");
        separatorTitle();
        std::cout << ">>>\t\tBridge Finder\t\t<<<";
        separatorTitle();
        std::cout << "\n\t>>>>>-|   Menu   |-<<<<<\n";
        std::cout << "(1) Input Range Node\n";
        std::cout << "(2) Input Edge\n";
        std::cout << "(3) Show Graph\n";
        std::cout << "(4) Find Bridge\n";
        std::cout << "(5) Graph Visualization\n";
        std::cout << "(6) Convert .dot to .png\n";
        std::cout << "(7) Save Graph\n";
        std::cout << "(8) Load Graph\n";
        std::cout << "(0) Quit\n";
        std::cout << "Select the Option : ";
        std::cin >> choice;

        switch (choice){
            case 1:{
                int n;
                std::cout << "Input the maximum value of node : ";
                std::cin >> n;
                delete graph;
                delete bridgeFinder;
                graph = new Graph(n);
                bridgeFinder = nullptr;
                std::cout << "Press enter to continue ...";
                std::cin.ignore();
                std::cin.get();
                break;
            }

            case 2:{
                if (!graph) {
                    std::cout << "Input the range of node or load the file first!\n";
                    std::cout << "Press enter to continue ...";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                graph->clearEdges();
                int m;
                std::cout << "Input the amount of edge : ";
                std::cin >> m;
                std::cout << "Input node in the range 1 to " <<graph->getNumNodes()<< " (Format : Node-1 Node-2) : \n";
                for (int i = 0; i < m; ++i){
                    int u, v;
                    std::cout << "Edge-" << i + 1 << " : ";
                    std::cin >> u >> v;
                    if (graph->addEdge(u, v) == false){
                        --i;
                    }
                }
                std::cout << "Press enter to continue ...";
                std::cin.ignore();
                std::cin.get();
                break;
            }

            case 3: {
                if (!graph){
                    std::cout << "Graph hasn't created yet\n";
                } else {
                    graph->showGraph();
                }
                std::cout << "Press enter to continue ...";
                std::cin.ignore();
                std::cin.get();
                break;
            }

            case 4: {
                if (!graph){
                    std::cout << "Graph hasn't created yet\n";
                    std::cout << "Press enter to continue ...";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                delete bridgeFinder;
                bridgeFinder = new BridgeFinder(*graph);
                bridgeFinder->findBridges();
                bridgeFinder->printBridges();
                std::cout << "Press enter to continue ...";
                std::cin.ignore();
                std::cin.get();
                break;
            }

            case 5: {
                if (!bridgeFinder) {
                    std::cout << "Find the bridge first!\n";
                    std::cout << "Press enter to continue ...";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                string filename;
                std::cout << "Input visualization file name(.dot) : ";
                std::cin >> filename;
                bridgeFinder->exportToDot(filename);
                std::cout << "Visualization has been saved in " << filename << "\n";
                std::cout << "Press enter to continue ...";
                std::cin.ignore();
                std::cin.get();
                break;
            }
            
            case 6:{
                string dotFile, pngFile;
                std::cout << "Input (.dot) filename : ";
                std::cin >> dotFile;
                std::cout << "Input picture name(.png) : ";
                std::cin >> pngFile;

                bridgeFinder->exportToPNG(dotFile, pngFile);

                std::cout << "Press enter to continue ...";
                std::cin.ignore();
                std::cin.get();
                break;
            }

            case 7: {
                if (graph) {
                    string filename;
                    std::cout << "Input save file name (.txt): ";
                    std::cin >> filename;
                    graph->saveToFile(filename);
                } else {
                    std::cout << "Graph hasn't created yet\n";
                }
                std::cout << "Press enter to continue ...";
                std::cin.ignore();
                std::cin.get();
                break;
            }

            case 8: {
                string filename;
                std::cout << "Input the file name to load the graph (.txt): ";
                std::cin >> filename;
                delete graph;
                graph = new Graph(0);
                graph->loadFromFile(filename);
                std::cout << "Press enter to continue ...";
                std::cin.ignore();
                std::cin.get();
                break;
            }
            
            case 0: {
                std::cout << "Thank you for using this program >.<";
                std::cin.ignore();
                std::cin.get();
                system("cls");
                break;
            }

            default:
                std::cout << "Please select a valid option.\n";
                std::cout << "Press enter to continue ...";
                std::cin.ignore();
                std::cin.get();
        }
    } while (choice != 0);
    delete graph;
    return 0;
}