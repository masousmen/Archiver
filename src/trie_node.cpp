#include "trie_node.h"

Node::Node(uint16_t value_, size_t prior_) : left_next(nullptr), right_next(nullptr), value(value_), prior(prior_) {
};

Node::~Node() {
    delete left_next;
    delete right_next;
}

Node::Node() : left_next(nullptr), right_next(nullptr), value(0), prior(0) {
}

Node::Node(uint16_t value_) : left_next(nullptr), right_next(nullptr), value(value_), prior(0) {
}

Node::Node(Node*& left, Node*& right) : left_next(nullptr), right_next(nullptr), value(0), prior(0) {
}

Node::Node(size_t prior_, Node*& left, Node*& right) : left_next(nullptr), right_next(nullptr), value(0), prior(prior_) {
}

Node* Node::GetLeft() const {
    return left_next;
}

Node* Node::GetRight() const {
    return right_next;
}

bool Node::IsTerminal() const {
    return left_next == nullptr && right_next == nullptr;
}

uint16_t Node::GetValue() const {
    return value;
}

size_t Node::GetPrior() const {
    return prior;
}

void Node::SetValue(uint16_t value_) {
    value = value_;
}

void Node::SetLeft(Node* left) {
    left_next = left;
}

void Node::SetRight(Node* right) {
    right_next = right;
}