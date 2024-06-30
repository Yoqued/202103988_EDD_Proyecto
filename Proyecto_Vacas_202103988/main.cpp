#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <nlohmann/json.hpp>

#include "Estructuras/ListaDobleCircular.h"
#include "Estructuras/BTree.h"
#include "Estructuras//bst.h"
#include "Estructuras/HashTablePilotos.h"
#include "Estructuras/Graph.h"

using json = nlohmann::json;
using namespace std;

void cargarAviones();
void cargarPilotos();
void cargarRutas();
void cargarMovimientos();
void consultarHorasDeVuelo();
void recomendarRuta();
void visualizarReportes();

ListaDobleCircular avionesMantenimiento;
BTree tree;
BST bstPilotos;
HashTablePilotos hashTable(18);
Graph graph;

int main() {
    int opcion;

    do {
        cout << "-----------------Menu-----------------" << endl;
        cout << "1. Carga de aviones" << endl;
        cout << "2. Carga de pilotos" << endl;
        cout << "3. Carga de rutas" << endl;
        cout << "4. Carga de movimientos" << endl;
        cout << "5. Consulta de horas de vuelo (Pilotos)" << endl;
        cout << "6. Recomendar ruta" << endl;
        cout << "7. Visualizar reportes" << endl;
        cout << "8. Salir" << endl;
        cout << "--------------------------------------" << endl;
        cout << "Ingrese una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cargarAviones();
                break;
            case 2:
                cargarPilotos();
                break;
            case 3:
                cargarRutas();
                break;
            case 4:
                cargarMovimientos();
                break;
            case 5:
                consultarHorasDeVuelo();
                break;
            case 6:
                recomendarRuta();
                break;
            case 7:
                visualizarReportes();
                break;
            case 8:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
        }

        cout << endl;
    } while (opcion != 8);

    return 0;
}

void cargarAviones() {
    cout << "--------------------------------------" << endl;
    cout << "Cargando aviones..." << endl;


    std::ifstream input("../aviones.json");
    if (!input.is_open()) {
        std::cerr << "No se pudo abrir el archivo data.json" << std::endl;
    }else {
        // Leer el contenido del archivo en un objeto JSON
        json j;
        input >> j;

        // Asegurarse de que el JSON es una lista de objetos
        if (!j.is_array()) {
            std::cerr << "El archivo JSON no contiene una lista de objetos" << std::endl;
        }else {
            // Iterar a través de la lista y mostrar el contenido de cada objeto
            for (const auto& item : j) {
                std::string cadena = item["estado"];

                // Convertir la cadena a minúsculas
                for (char &c : cadena) {
                    c = std::tolower(c);
                }

                if(cadena.compare("disponible")==0) {
                    std::string alfanumerico = item["numero_de_registro"];
                    std::string numeros;
                    int valor;
                    // Verificar que la cadena comienza con 'N'
                    if (alfanumerico[0] == 'N') {
                        // Eliminar la 'N' y obtener la subcadena con los números
                        numeros = alfanumerico.substr(1);

                        // Convertir la subcadena a entero
                        valor = std::stoi(numeros);

                    }

                    Avion* avion = new Avion{item["vuelo"], item["numero_de_registro"], item["modelo"], item["capacidad"],
                        item["aerolinea"], item["ciudad_destino"], item["estado"], valor};
                    tree.insert(avion);
                }
                if(cadena.compare("mantenimiento")==0) {

                    std::string alfanumerico = item["numero_de_registro"];
                    std::string numeros;
                    int valor;
                    // Verificar que la cadena comienza con 'N'
                    if (alfanumerico[0] == 'N') {
                        // Eliminar la 'N' y obtener la subcadena con los números
                        numeros = alfanumerico.substr(1);

                        // Convertir la subcadena a entero
                        valor = std::stoi(numeros);

                    }

                    Avion avion = {item["vuelo"], item["numero_de_registro"], item["modelo"], item["capacidad"],
                        item["aerolinea"], item["ciudad_destino"], item["estado"], valor};
                    avionesMantenimiento.agregarAvion(avion);
                }
            }
        }
    }
    /*avionesMantenimiento.imprimirLista();
    tree.traversal();
    tree.exportToGraphviz("btree.dot");
    avionesMantenimiento.generarDot("listaDoble");*/
}

void cargarPilotos() {
    cout << "--------------------------------------" << endl;
    cout << "Cargando pilotos..." << endl;


    std::ifstream input("../pilotos.json");
    if (!input.is_open()) {
        std::cerr << "No se pudo abrir el archivo data.json" << std::endl;
    }else {
        // Leer el contenido del archivo en un objeto JSON
        json j;
        input >> j;

        // Asegurarse de que el JSON es una lista de objetos
        if (!j.is_array()) {
            std::cerr << "El archivo JSON no contiene una lista de objetos" << std::endl;
        }else {
            // Iterar a través de la lista y mostrar el contenido de cada objeto
            for (const auto& item : j) {
                Pilotos piloto = {item["nombre"], item["nacionalidad"],item["numero_de_id"],item["vuelo"],item["horas_de_vuelo"],item["tipo_de_licencia"]};
                bstPilotos.insert(piloto);
                hashTable.insert(piloto);
            }
        }
    }

    /*std::cout << "Inorder traversal: \n";
    bstPilotos.inorder();

    std::cout << endl;
    std::cout << endl;

    std::cout << "Preorder traversal: \n";
    bstPilotos.preorder();

    std::cout << endl;
    std::cout << endl;

    std::cout << "Postorder traversal: \n";
    bstPilotos.postorder();

    std::cout << endl;
    std::cout << endl;

    int horas_de_vuelo = 300;
    if (bstPilotos.search(horas_de_vuelo)) {
        std::cout << "Piloto con " << horas_de_vuelo << " horas de vuelo encontrado." << std::endl;
    } else {
        std::cout << "Piloto con " << horas_de_vuelo << " horas de vuelo no encontrado." << std::endl;
    }

    bstPilotos.visualize();

    hashTable.display();

    std::string searchID = "5678";
    Pilotos* foundPiloto = hashTable.search(searchID);
    if (foundPiloto) {
        std::cout << "Piloto encontrado: " << foundPiloto->nombre << std::endl;
    } else {
        std::cout << "Piloto con ID " << searchID << " no encontrado." << std::endl;
    }
    */
    // Generar el archivo Graphviz
    // hashTable.generateGraphvizFile("hash_table.dot");

}

void cargarRutas() {
    graph.readFromFile("../rutas.txt");
}

void cargarMovimientos() {
    cout << "--------------------------------------" << endl;
    cout << "Cargando movimientos..." << endl;
    // Logica para cargar movimientos

    std::ifstream file("../movimientos.txt");
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string part;
        if (std::getline(iss, part, ',')) {
            if (part == "MantenimientoAviones") {
                std::string tipo, codigo;
                if (std::getline(iss, tipo, ',') && std::getline(iss, codigo, ',')) {
                    if (tipo == "Ingreso") {
                        codigo.erase(std::remove(codigo.begin(), codigo.end(), ';'), codigo.end());
                        std::cout << "Ingreso de avión: " << codigo << std::endl;
                        Avion* avi = tree.deleteAvion(codigo);
                        Avion aviN = {avi->vuelo, avi->numero_de_registro, avi->modelo,avi->capacidad,avi->aerolinea,avi->ciudad_destino,avi->estado,avi->numero_orden};
                        avionesMantenimiento.agregarAvion(aviN);
                    } else if (tipo == "Salida") {
                        codigo.erase(std::remove(codigo.begin(), codigo.end(), ';'), codigo.end());
                        std::cout << "Salida de avión: " << codigo << std::endl;
                        Avion avi = avionesMantenimiento.eliminar(codigo);
                        Avion* aviN = new Avion{avi.vuelo, avi.numero_de_registro,avi.modelo, avi.capacidad,avi.aerolinea,avi.ciudad_destino,avi.estado,avi.numero_orden};
                        tree.insert(aviN);
                    }
                }
            } else if (part.find("DarDeBaja") != std::string::npos) {
                size_t start = part.find('(') + 1;
                size_t end = part.find(')');
                if (start != std::string::npos && end != std::string::npos) {
                    std::string codigo = part.substr(start, end - start);
                    std::cout << "Dar de baja el PILOTO con código: " << codigo << std::endl;
                    hashTable.remove(codigo);
                    bstPilotos.remove(codigo);
                }
            }
        }
    }

    file.close();
    /*tree.exportToGraphviz("btree.dot");
    avionesMantenimiento.imprimirLista();
    avionesMantenimiento.generarDot("listaDoble");*/
}

void consultarHorasDeVuelo() {
    cout << "--------------------------------------" << endl;
    cout << "Consultando horas de vuelo de pilotos..." << endl;
    // Logica para consultar horas de vuelo de pilotos

    cout << "--------------------------------------" << endl;
    cout << "Consultando horas de vuelo de pilotos..." << endl;

    int opcion;
    do {
        cout << "Seleccione el tipo de recorrido del árbol BST:" << endl;
        cout << "1. Preorden" << endl;
        cout << "2. Inorden" << endl;
        cout << "3. Postorden" << endl;
        cout << "4. Salir" << endl;
        cout << "Ingrese una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Recorrido Preorden: " << endl;
            bstPilotos.preorder();
            break;
            case 2:
                cout << "Recorrido Inorden: " << endl;
            bstPilotos.inorder();
            break;
            case 3:
                cout << "Recorrido Postorden: " << endl;
            bstPilotos.postorder();
            break;
            case 4:
                cout << "Saliendo del menú de consulta de horas de vuelo..." << endl;
            break;
            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
        }

        cout << endl;
    } while (opcion != 4);
}

void recomendarRuta() {
    cout << "--------------------------------------" << endl;
    cout << "Recomendando ruta..." << endl;
    // Logica para recomendar una ruta
}

void visualizarReportes() {
    cout << "--------------------------------------" << endl;
    cout << "Visualizando reportes..." << endl;
    // Logica para visualizar reportes

    int opcion;

    do {
        cout << "----------------- Reportes -----------------" << endl;
        cout << "1. Árbol B con aviones disponibles" << endl;
        cout << "2. Lista de aviones en mantenimiento" << endl;
        cout << "3. Árbol binario de búsqueda con horas de vuelo de pilotos" << endl;
        cout << "4. Tabla hash de pilotos" << endl;
        cout << "5. Grafo dirigido con las rutas" << endl;
        cout << "6. Matriz dispersa de vuelos y ciudades" << endl;
        cout << "7. Volver al menú principal" << endl;
        cout << "---------------------------------------------" << endl;
        cout << "Ingrese una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Árbol B con aviones disponibles:" << endl;
                // Lógica para mostrar el árbol B con aviones disponibles
                tree.traversal(); // Por ejemplo, puedes llamar a una función de tu árbol B
                tree.exportToGraphviz("btree.dot");
                break;
            case 2:
                cout << "Lista de aviones en mantenimiento:" << endl;
                //avionesMantenimiento.imprimirLista(); // Por ejemplo, muestra la lista de aviones en mantenimiento
                avionesMantenimiento.generarDot("listaDoble");
                break;
            case 3:
                cout << "Árbol binario de búsqueda con horas de vuelo de pilotos:" << endl;
                //bstPilotos.inorder(); // Por ejemplo, muestra el árbol binario de búsqueda con horas de vuelo de pilotos
                bstPilotos.visualize();
                break;
            case 4:
                cout << "Tabla hash de pilotos:" << endl;
                hashTable.display(); // Por ejemplo, muestra la tabla hash de pilotos
                hashTable.generateGraphvizFile("hash_table.dot");
                break;
            case 5:
                cout << "Grafo dirigido con las rutas:" << endl;
                graph.printGraph(); // Por ejemplo, muestra el grafo dirigido con las rutas
                graph.exportToGraphviz("output.dot");
                break;
            case 6:
                cout << "Matriz dispersa de vuelos y ciudades:" << endl;
                // Lógica para mostrar la matriz dispersa de vuelos y ciudades
                // Implementación según cómo tengas estructurada la matriz dispersa
                break;
            case 7:
                cout << "Volviendo al menú principal..." << endl;
                break;
            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
        }

        cout << endl;
    } while (opcion != 7);
}
