cmake_minimum_required(VERSION 3.10)
project(Proyecto_Vacas_202103988)

set(CMAKE_CXX_STANDARD 11)

# Incluye el archivo de configuración de vcpkg
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Encuentra el paquete nlohmann_json
find_package(nlohmann_json CONFIG REQUIRED)

# Crear el ejecutable
add_executable(Proyecto_Vacas_202103988 main.cpp
        Estructuras/Avion.h
        Estructuras/Nodo_Avion.h
        Estructuras/ListaDobleCircular.h
        Estructuras/BTree.h
        Estructuras/BTreeNode.h
        Estructuras/node.h
        Estructuras/bst.h
        Estructuras/Pilotos.h
        Estructuras/HashTablePilotos.h
        Estructuras/Rutas.h
        Estructuras/Graph.h
)

# Vincular la biblioteca nlohmann_json con el ejecutable
target_link_libraries(Proyecto_Vacas_202103988 PRIVATE nlohmann_json::nlohmann_json)