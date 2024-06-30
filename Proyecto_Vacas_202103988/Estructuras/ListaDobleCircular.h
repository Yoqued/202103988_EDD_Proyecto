//
// Created by ser on 27/06/2024.
//

#ifndef LISTADOBLECIRCULAR_H
#define LISTADOBLECIRCULAR_H

#include "Nodo_avion.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

class ListaDobleCircular {
private:
    Nodo_avion* cabeza;

public:
    ListaDobleCircular() : cabeza(nullptr) {}

    ~ListaDobleCircular() {
        if (!cabeza) return;

        Nodo_avion* actual = cabeza;
        do {
            Nodo_avion* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        } while (actual != cabeza);
    }

    void agregarAvion(Avion a) {
        Nodo_avion* nuevo = new Nodo_avion(a);

        if (!cabeza) {
            cabeza = nuevo;
            cabeza->siguiente = cabeza;
            cabeza->anterior = cabeza;
        } else {
            Nodo_avion* ultimo = cabeza->anterior;
            ultimo->siguiente = nuevo;
            nuevo->anterior = ultimo;
            nuevo->siguiente = cabeza;
            cabeza->anterior = nuevo;
        }
    }

    void imprimirLista() {
        if (!cabeza) return;

        Nodo_avion* actual = cabeza;
        do {
            std::cout << std::endl;
            std::cout << "--------------------------------------" << std::endl;
            std::cout << "Vuelo: " << actual->avion.vuelo << std::endl;
            std::cout << "Numero de Registro: " << actual->avion.numero_de_registro << std::endl;
            std::cout << "Modelo: " << actual->avion.modelo << std::endl;
            std::cout << "Capacidad: " << actual->avion.capacidad << std::endl;
            std::cout << "Aerolinea: " << actual->avion.aerolinea << std::endl;
            std::cout << "Ciudad Destino: " << actual->avion.ciudad_destino << std::endl;
            std::cout << "Estado: " << actual->avion.estado << std::endl;
            std::cout << "Numero Orden: " << actual->avion.numero_orden << std::endl;
            actual = actual->siguiente;
        } while (actual != cabeza);
    }

    bool estaVacia() const {
        return cabeza == nullptr;
    }

    Avion eliminar(const std::string& numero_de_registro) {
        if (!cabeza) return Avion();

        Nodo_avion* actual = cabeza;
        Nodo_avion* previo = nullptr;

        do {
            if (actual->avion.numero_de_registro == numero_de_registro) {
                Avion avionEliminado = actual->avion;

                if (actual->siguiente == actual) {
                    delete actual;
                    cabeza = nullptr;
                } else {
                    if (actual == cabeza) {
                        cabeza = actual->siguiente;
                    }
                    previo = actual->anterior;
                    previo->siguiente = actual->siguiente;
                    actual->siguiente->anterior = previo;

                    delete actual;
                }

                return avionEliminado;
            }
            actual = actual->siguiente;
        } while (actual != cabeza);

        return Avion();
    }

    void generarDot(const std::string& nombreArchivo) const {
        std::ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            std::cerr << "No se pudo abrir el archivo para escribir." << std::endl;
            return;
        }

        archivo << "digraph G {" << std::endl;
        archivo << "    node [shape=record];" << std::endl;

        if (cabeza) {
            Nodo_avion* actual = cabeza;
            int id = 0;
            do {
                archivo << "    nodo" << id << " [label=\"{|Vuelo: "
                        << actual->avion.vuelo << "\\nNúmero de Registro: "
                        << actual->avion.numero_de_registro << "\\nModelo: "
                        << actual->avion.modelo << "\\nEstado: "
                        << actual->avion.estado << "\\nAero línea: "
                        << actual->avion.aerolinea << "\\nCapacidad: "
                        << actual->avion.capacidad << "|}\"];" << std::endl;
                if (actual->siguiente != cabeza) {
                    archivo << "    nodo" << id << " -> nodo" << id + 1 << ";" << std::endl;
                    archivo << "    nodo" << id + 1 << " -> nodo" << id << ";" << std::endl;
                } else {
                    archivo << "    nodo" << id << " -> nodo0;" << std::endl;
                    archivo << "    nodo0 -> nodo" << id << ";" << std::endl;
                }
                actual = actual->siguiente;
                id++;
            } while (actual != cabeza);
        }

        archivo << "}" << std::endl;
        archivo.close();

        // Ejecutar el comando dot para convertir el archivo .dot en una imagen .png
        std::string command = "dot -Tpng " + nombreArchivo + " -o listaDoble.png";
        system(command.c_str());
    }

};

#endif //LISTADOBLECIRCULAR_H

