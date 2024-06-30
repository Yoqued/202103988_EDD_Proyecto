//
// Created by ser on 28/06/2024.
//

#ifndef BST_H
#define BST_H

#include "node.h"
#include "Pilotos.h"
#include <iostream>
#include <fstream>

class BST {
public:
    BST() : root(nullptr) {}

    void insert(const Pilotos& piloto) {
        root = insertRec(root, piloto);
    }

    void inorder() const {
        inorderRec(root);
        std::cout << std::endl;
    }

    void preorder() const {
        preorderRec(root);
        std::cout << std::endl;
    }

    void postorder() const {
        postorderRec(root);
        std::cout << std::endl;
    }

    bool search(int horas_de_vuelo) const {
        return searchRec(root, horas_de_vuelo);
    }

    void visualize() const {
        std::ofstream out("bst.dot");
        out << "digraph BST {" << std::endl;
        visualizeRec(out, root);
        out << "}";
        out.close();
        system("dot -Tpng bst.dot -o bst.png"); // Comando para generar el PNG usando Graphviz
        std::cout << "BST visualization generated as bst.png" << std::endl;
    }

    void remove(const std::string& numero_de_id) {
        root = removeRec(root, numero_de_id);
    }

private:
    Node* root;

    Node* insertRec(Node* node, const Pilotos& piloto) {
        if (node == nullptr) {
            return new Node(piloto);
        }
        if (piloto.horas_de_vuelo < node->data.horas_de_vuelo) {
            node->left = insertRec(node->left, piloto);
        } else if (piloto.horas_de_vuelo > node->data.horas_de_vuelo) {
            node->right = insertRec(node->right, piloto);
        }
        return node;
    }

    void inorderRec(Node* node) const {
        if (node != nullptr) {
            inorderRec(node->left);
            std::cout << node->data.nombre << " (" << node->data.horas_de_vuelo << " horas)" << "\n ";
            inorderRec(node->right);
        }
    }

    void preorderRec(Node* node) const {
        if (node != nullptr) {
            std::cout << node->data.nombre << " (" << node->data.horas_de_vuelo << " horas)" << "\n ";
            preorderRec(node->left);
            preorderRec(node->right);
        }
    }

    void postorderRec(Node* node) const {
        if (node != nullptr) {
            postorderRec(node->left);
            postorderRec(node->right);
            std::cout << node->data.nombre << " (" << node->data.horas_de_vuelo << " horas)" << "\n ";
        }
    }

    bool searchRec(Node* node, int horas_de_vuelo) const {
        if (node == nullptr) {
            return false;
        }
        if (node->data.horas_de_vuelo == horas_de_vuelo) {
            return true;
        }
        if (horas_de_vuelo < node->data.horas_de_vuelo) {
            return searchRec(node->left, horas_de_vuelo);
        } else {
            return searchRec(node->right, horas_de_vuelo);
        }
    }

    Node* removeRec(Node* node, const std::string& numero_de_id) {
        if (node == nullptr) {
            return node;
        }
        if (numero_de_id < node->data.numero_de_id) {
            node->left = removeRec(node->left, numero_de_id);
        } else if (numero_de_id > node->data.numero_de_id) {
            node->right = removeRec(node->right, numero_de_id);
        } else {
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = removeRec(node->right, temp->data.numero_de_id);
        }
        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    void visualizeRec(std::ofstream& out, Node* node) const {
        if (node != nullptr) {
            if (node->left != nullptr) {
                out << "\"" << node->data.nombre << " (" << node->data.horas_de_vuelo << " horas)" << "\" -> \"" << node->left->data.nombre << " (" << node->left->data.horas_de_vuelo << " horas)" << "\";" << std::endl;
            }
            if (node->right != nullptr) {
                out << "\"" << node->data.nombre << " (" << node->data.horas_de_vuelo << " horas)" << "\" -> \"" << node->right->data.nombre << " (" << node->right->data.horas_de_vuelo << " horas)" << "\";" << std::endl;
            }
            visualizeRec(out, node->left);
            visualizeRec(out, node->right);
        }
    }
};

#endif // BST_H
