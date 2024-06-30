//
// Created by ser on 28/06/2024.
//

#ifndef NODE_H
#define NODE_H

#include "Pilotos.h"

struct Node {
    Pilotos data;
    Node* left;
    Node* right;

    Node(const Pilotos& piloto) : data(piloto), left(nullptr), right(nullptr) {}
};

#endif // NODE_H

