#pragma once

#include <stdlib.h>
#include <assert.h>
#include <iostream>

class Node {
public:
    Node();

    ~Node();

    explicit Node(uint16_t value_);

    Node(uint16_t value_, size_t prior);

    Node(Node*& left, Node*& right);

    Node(size_t prior_, Node*& left, Node*& right);

    Node* GetLeft() const;

    Node* GetRight() const;

    bool IsTerminal() const;

    uint16_t GetValue() const;

    size_t GetPrior() const;

    void SetValue(uint16_t value_);

    void SetLeft(Node* left);

    void SetRight(Node* right);

private:
    Node* left_next;
    Node* right_next;
    uint16_t value;
    size_t prior;
};

