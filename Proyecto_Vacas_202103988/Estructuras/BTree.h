#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <fstream>
#include "BTreeNode.h"

using namespace std;

class BTree {
private:
    BTreeNode* root;

    bool setValue(Avion*, Avion*&, BTreeNode*&, BTreeNode*&);
    void insertNode(Avion*, int, BTreeNode*&, BTreeNode*);
    void splitNode(Avion*, Avion*&, int, BTreeNode*&, BTreeNode*, BTreeNode*&);
    BTreeNode* createNode(Avion*, BTreeNode*);
    void generateDotFile(BTreeNode*, ofstream&);
    void traversal(BTreeNode*);
    bool remove(const string&, BTreeNode*&, Avion*&);

public:
    BTree() : root(nullptr) {}

    void insert(Avion*);
    void traversal();
    void exportToGraphviz(const string&);
    Avion* deleteAvion(const string& numero_de_registro);
};

void BTree::insert(Avion* avion) {
    Avion* i = nullptr;
    BTreeNode* child = new BTreeNode();
    if (setValue(avion, i, root, child)) {
        root = createNode(i, child);
    }
}

bool BTree::setValue(Avion* val, Avion*& pval, BTreeNode*& node, BTreeNode*& child) {
    int pos;
    bool res;
    BTreeNode* newnode = new BTreeNode();
    if (node == nullptr) {
        pval = val;
        child = nullptr;
        res = true;
        return res;
    }

    if (val->numero_de_registro < node->val[1]->numero_de_registro) {
        pos = 0;
    } else {
        pos = node->num;
        while (val->numero_de_registro < node->val[pos]->numero_de_registro && pos > 1) {
            pos--;
        }
        if (val->numero_de_registro == node->val[pos]->numero_de_registro) {
            cout << "Duplicates are not permitted" << endl;
            res = false;
            return res;
        }
    }

    if (setValue(val, pval, node->link[pos], child)) {
        if (node->num < MAXI) {
            insertNode(pval, pos, node, child);
        } else {
            splitNode(pval, pval, pos, node, child, newnode);
            child = newnode;
            res = true;
            return res;
        }
    }

    res = false;
    return res;
}

void BTree::insertNode(Avion* val, int pos, BTreeNode*& node, BTreeNode* child) {
    int j = node->num;
    while (j > pos) {
        node->val[j + 1] = node->val[j];
        node->link[j + 1] = node->link[j];
        j--;
    }
    node->val[j + 1] = val;
    node->link[j + 1] = child;
    node->num++;
}

void BTree::splitNode(Avion* val, Avion*& pval, int pos, BTreeNode*& node, BTreeNode* child, BTreeNode*& newnode) {
    int median, i, j;
    if (pos > 1) {
        median = 2;
    } else {
        median = 1;
    }
    newnode = new BTreeNode();
    j = median + 1;
    while (j <= MAXI) {
        newnode->val[j - median] = node->val[j];
        newnode->link[j - median] = node->link[j];
        j++;
    }
    node->num = median;
    newnode->num = MAXI - median;
    if (pos <= 1) {
        insertNode(val, pos, node, child);
    } else {
        insertNode(val, pos - median, newnode, child);
    }
    pval = node->val[node->num];
    newnode->link[0] = node->link[node->num];
    node->num--;
}

BTreeNode* BTree::createNode(Avion* val, BTreeNode* child) {
    BTreeNode* newNode = new BTreeNode();
    newNode->val[1] = val;
    newNode->num = 1;
    newNode->link[0] = root;
    newNode->link[1] = child;
    return newNode;
}

void BTree::traversal() {
    traversal(root);
    cout << endl;
}

void BTree::traversal(BTreeNode* myNode) {
    if (myNode != nullptr) {
        cout << " [ ";
        int i = 0;
        while (i < myNode->num) {
            cout << myNode->val[i + 1]->numero_de_registro << " ";
            i++;
        }
        for (int j = 0; j <= myNode->num; ++j) {
            traversal(myNode->link[j]);
        }
        cout << "] ";
    }
}

void BTree::generateDotFile(BTreeNode* myNode, ofstream& outFile) {
    if (myNode != nullptr) {
        outFile << "  node" << myNode << " [label=\"";
        for (int i = 1; i <= myNode->num; ++i) {
            outFile << myNode->val[i]->numero_de_registro ;
            if (i < myNode->num) {
                outFile << "|";
            }
        }
        outFile << "\"];" << endl;
        for (int i = 0; i <= myNode->num; ++i) {
            if (myNode->link[i] != nullptr) {
                outFile << "  node" << myNode << " -> node" << myNode->link[i] << ";" << endl;
                generateDotFile(myNode->link[i], outFile);
            }
        }
    }
}

void BTree::exportToGraphviz(const string& filename) {
    ofstream outFile(filename);
    outFile << "digraph BTree {" << endl;
    outFile << "  node [shape=record];" << endl;
    generateDotFile(root, outFile);
    outFile << "}" << endl;
    outFile.close();
    system("dot -Tpng btree.dot -o btree.png"); // Comando para generar el PNG usando Graphviz
    cout << "Graphviz file created: " << filename << endl;
}

Avion* BTree::deleteAvion(const string& numero_de_registro) {
    Avion* removedAvion = nullptr;
    if (!remove(numero_de_registro, root, removedAvion)) {
        cout << "Avion with numero_de_registro " << numero_de_registro << " not found." << endl;
    }
    return removedAvion;
}

bool BTree::remove(const string& numero_de_registro, BTreeNode*& node, Avion*& removedAvion) {
    if (!node) {
        return false;
    }

    int pos;
    bool found = false;

    if (numero_de_registro < node->val[1]->numero_de_registro) {
        pos = 0;
    } else {
        pos = node->num;
        while (numero_de_registro < node->val[pos]->numero_de_registro && pos > 1) {
            pos--;
        }
        if (numero_de_registro == node->val[pos]->numero_de_registro) {
            found = true;
        }
    }

    if (found) {
        removedAvion = node->val[pos];
        if (node->link[pos]) {
            BTreeNode* predNode = node->link[pos];
            while (predNode->link[predNode->num]) {
                predNode = predNode->link[predNode->num];
            }
            node->val[pos] = predNode->val[predNode->num];
            remove(predNode->val[predNode->num]->numero_de_registro, node->link[pos], removedAvion);
        } else {
            for (int i = pos; i < node->num; i++) {
                node->val[i] = node->val[i + 1];
                node->link[i] = node->link[i + 1];
            }
            node->num--;
            if (node->num == 0) {
                delete node;
                node = nullptr;
            }
        }
    } else {
        if (remove(numero_de_registro, node->link[pos], removedAvion)) {
            if (node->link[pos] && node->link[pos]->num < (MAXI / 2)) {
                if (pos > 0 && node->link[pos - 1]->num > (MAXI / 2)) {
                    BTreeNode* leftSibling = node->link[pos - 1];
                    for (int i = node->link[pos]->num; i > 0; i--) {
                        node->link[pos]->val[i + 1] = node->link[pos]->val[i];
                        node->link[pos]->link[i + 1] = node->link[pos]->link[i];
                    }
                    node->link[pos]->link[1] = node->link[pos]->link[0];
                    node->link[pos]->val[1] = node->val[pos];
                    node->link[pos]->link[0] = leftSibling->link[leftSibling->num];
                    node->val[pos] = leftSibling->val[leftSibling->num];
                    leftSibling->num--;
                } else if (pos < node->num && node->link[pos + 1]->num > (MAXI / 2)) {
                    BTreeNode* rightSibling = node->link[pos + 1];
                    node->link[pos]->val[node->link[pos]->num + 1] = node->val[pos + 1];
                    node->link[pos]->link[node->link[pos]->num + 1] = rightSibling->link[0];
                    node->link[pos]->num++;
                    node->val[pos + 1] = rightSibling->val[1];
                    rightSibling->link[0] = rightSibling->link[1];
                    for (int i = 1; i < rightSibling->num; i++) {
                        rightSibling->val[i] = rightSibling->val[i + 1];
                        rightSibling->link[i] = rightSibling->link[i + 1];
                    }
                    rightSibling->num--;
                } else {
                    if (pos > 0) {
                        BTreeNode* leftSibling = node->link[pos - 1];
                        leftSibling->val[leftSibling->num + 1] = node->val[pos];
                        leftSibling->num++;
                        for (int i = 1; i <= node->link[pos]->num; i++) {
                            leftSibling->val[leftSibling->num + 1] = node->link[pos]->val[i];
                            leftSibling->link[leftSibling->num] = node->link[pos]->link[i - 1];
                            leftSibling->num++;
                        }
                        leftSibling->link[leftSibling->num] = node->link[pos]->link[node->link[pos]->num];
                        delete node->link[pos];
                        for (int i = pos; i < node->num; i++) {
                            node->val[i] = node->val[i + 1];
                            node->link[i] = node->link[i + 1];
                        }
                        node->num--;
                    } else {
                        BTreeNode* rightSibling = node->link[pos + 1];
                        node->link[pos]->val[node->link[pos]->num + 1] = node->val[pos + 1];
                        node->link[pos]->num++;
                        for (int i = 1; i <= rightSibling->num; i++) {
                            node->link[pos]->val[node->link[pos]->num + 1] = rightSibling->val[i];
                            node->link[pos]->link[node->link[pos]->num] = rightSibling->link[i - 1];
                            node->link[pos]->num++;
                        }
                        node->link[pos]->link[node->link[pos]->num] = rightSibling->link[rightSibling->num];
                        delete rightSibling;
                        for (int i = pos + 1; i < node->num; i++) {
                            node->val[i] = node->val[i + 1];
                            node->link[i] = node->link[i + 1];
                        }
                        node->num--;
                    }
                }
            }
        } else {
            return false;
        }
    }
    return true;
}

#endif //BTREE_H

