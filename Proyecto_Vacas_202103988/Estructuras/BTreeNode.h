//
// Created by ser on 27/06/2024.
//
#ifndef BTREENODE_H
#define BTREENODE_H

#include "Avion.h"

const int MAXI = 4; // Order 5

struct BTreeNode {
    Avion* val[MAXI + 1];
    int num;
    BTreeNode* link[MAXI + 1];
    BTreeNode() : num(0) {
        for (int i = 0; i <= MAXI; ++i) {
            link[i] = nullptr;
            val[i] = nullptr;
        }
    }
};

#endif //BTREENODE_H

