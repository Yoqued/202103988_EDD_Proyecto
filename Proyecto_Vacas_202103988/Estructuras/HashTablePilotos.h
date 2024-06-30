//
// Created by ser on 29/06/2024.
//

#ifndef HASHTABLEPILOTOS_H
#define HASHTABLEPILOTOS_H

#include "Pilotos.h"
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>

// Función para comparar los pilotos ignorando la letra 'X' en el número de ID
bool comparePilotos(const Pilotos& a, const Pilotos& b) {
    return std::stoi(a.numero_de_id.substr(1)) < std::stoi(b.numero_de_id.substr(1));
}

class HashTablePilotos {
private:
    int M; // Tamaño de la tabla hash
    std::vector<std::list<Pilotos>> table; // La tabla hash es un vector de listas

    // Función para extraer y convertir los dígitos de numero_de_id a un número entero
    int extractDigits(const std::string& id) {
        std::string digits;
        for (char c : id) {
            if (isdigit(c)) {
                digits += c;
            }
        }
        return std::stoi(digits);
    }

    // Función de dispersión
    int hashFunction(const std::string& id) {
        int int_id = extractDigits(id);
        return int_id % M;
    }

public:
    // Constructor
    HashTablePilotos(int size) : M(size), table(size) {}

    // Función para insertar un piloto en la tabla hash
    void insert(const Pilotos& piloto) {
        int index = hashFunction(piloto.numero_de_id);
        table[index].push_back(piloto);
    }

    // Función para buscar un piloto por su ID
    Pilotos* search(const std::string& id) {
        int index = hashFunction(id);
        for (auto& piloto : table[index]) {
            if (piloto.numero_de_id == id) {
                return &piloto;
            }
        }
        return nullptr; // Si no se encuentra el piloto
    }

    // Función para eliminar un piloto por su ID
    bool remove(const std::string& id) {
        int index = hashFunction(id);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->numero_de_id == id) {
                table[index].erase(it);
                return true; // Piloto eliminado exitosamente
            }
        }
        return false; // Si no se encuentra el piloto
    }

    // Función para mostrar la tabla hash
    void display() {
        for (int i = 0; i < M; ++i) {
            std::cout << "Index " << i << ": ";
            for (const auto& piloto : table[i]) {
                std::cout << piloto.nombre << " (" << piloto.numero_de_id << "), ";
            }
            std::cout << std::endl;
        }
    }

    // Función para generar un archivo DOT para Graphviz
    void generateGraphvizFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error al abrir el archivo para escribir." << std::endl;
            return;
        }

        file << "digraph HashTable {\n";
        file << "node [shape=record];\n";

        for (int i = 0; i < M; ++i) {
            file << "index" << i << " [label=\"Index " << i << "\", shape=box, style=filled, fillcolor=lightcoral];\n";
            if (!table[i].empty()) {
                // Ordenar la lista de pilotos en el índice actual
                table[i].sort(comparePilotos);

                int count = 0;
                std::string prevNode;
                for (const auto& piloto : table[i]) {
                    std::string nodeName = "node" + std::to_string(i) + "_" + std::to_string(count);
                    file << nodeName << " [label=\"" << piloto.nombre << " (" << piloto.numero_de_id << ")\", shape=box, style=filled, fillcolor=lightblue];\n";
                    if (count == 0) {
                        file << "index" << i << " -> " << nodeName << ";\n";
                    } else {
                        file << prevNode << " -> " << nodeName << ";\n";
                    }
                    prevNode = nodeName;
                    ++count;
                }
            }
        }

        file << "}\n";
        file.close();
        system("dot -Tpng hash_table.dot -o hash_table.png");
        std::cout << "Archivo Graphviz generado: " << filename << std::endl;
    }
};

#endif // HASHTABLEPILOTOS_H





