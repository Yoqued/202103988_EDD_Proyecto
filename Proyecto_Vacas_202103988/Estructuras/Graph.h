//
// Created by ser on 30/06/2024.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "Rutas.h"
#include <set>

// Estructura para representar una lista de adyacencia
struct AdjacencyList {
    Rutas ruta;
    AdjacencyList* next;
};

// Clase para representar un grafo
class Graph {
private:
    std::map<std::string, int> cityIndexMap; // Mapa de nombres de ciudades a índices
    std::vector<std::string> cities; // Vector de nombres de ciudades
    std::vector<AdjacencyList*> adjLists; // Vector de listas de adyacencia

    // Función para crear un nuevo nodo en la lista de adyacencia
    AdjacencyList* createNode(Rutas ruta) {
        AdjacencyList* newNode = new AdjacencyList;
        newNode->ruta = ruta;
        newNode->next = nullptr;
        return newNode;
    }

    // Función para obtener el índice de una ciudad, agregándola si es necesario
    int getCityIndex(const std::string& city) {
        if (cityIndexMap.find(city) == cityIndexMap.end()) {
            cityIndexMap[city] = cities.size();
            cities.push_back(city);
            adjLists.push_back(nullptr); // Agregar nueva lista de adyacencia
        }
        return cityIndexMap[city];
    }

public:
    // Constructor
    Graph() {}

    // Función para agregar una arista al grafo
    void addEdge(const std::string& origen, const std::string& destino, int distancia) {
        int src = getCityIndex(origen);
        int dest = getCityIndex(destino);

        // Crear ruta
        Rutas ruta;
        ruta.origen = origen;
        ruta.destino = destino;
        ruta.distancia = distancia;

        // Agregar arista de src a dest
        AdjacencyList* newNode = createNode(ruta);
        newNode->next = adjLists[src];
        adjLists[src] = newNode;

        // No crear ruta inversa para mantener el grafo dirigido
    }

    // Función para imprimir el grafo
    void printGraph() {
        for (size_t i = 0; i < cities.size(); i++) {
            AdjacencyList* temp = adjLists[i];
            std::cout << "City " << cities[i] << ":";
            while (temp) {
                std::cout << " -> (" << temp->ruta.origen << ", " << temp->ruta.destino << ", " << temp->ruta.distancia << ")";
                temp = temp->next;
            }
            std::cout << std::endl;
        }
    }

    // Función para leer el archivo y agregar las aristas
    void readFromFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string origen, destino, distanciaStr;
            int distancia;

            std::getline(ss, origen, '/');
            std::getline(ss, destino, '/');
            std::getline(ss, distanciaStr, ';');
            distancia = std::stoi(distanciaStr);

            addEdge(origen, destino, distancia);
        }
    }

    // Función para exportar el grafo a un archivo DOT para Graphviz
    void exportToGraphviz(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Error opening file " << filename << std::endl;
            return;
        }

        file << "digraph G {\n";

        std::set<std::pair<std::string, std::string>> edges;

        for (size_t i = 0; i < cities.size(); i++) {
            AdjacencyList* temp = adjLists[i];
            while (temp) {
                std::pair<std::string, std::string> edge(temp->ruta.origen, temp->ruta.destino);
                if (edges.find(edge) == edges.end()) {
                    file << "    \"" << temp->ruta.origen << "\" -> \"" << temp->ruta.destino << "\" [label=\"" << temp->ruta.distancia << "\"];\n";
                    edges.insert(edge);
                }
                temp = temp->next;
            }
        }

        file << "}\n";
        file.close();
    }

    // Destructor
    ~Graph() {
        for (AdjacencyList* list : adjLists) {
            while (list) {
                AdjacencyList* next = list->next;
                delete list;
                list = next;
            }
        }
    }
};

#endif // GRAPH_H
