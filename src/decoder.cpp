#include "decoder.h"

Decoder::Decoder() : root(new Node()), cur_vertex(root) {}

Decoder::Decoder(const std::unordered_map<uint16_t, std::vector<bool>>& codes) : root(new Node()), cur_vertex(root) {
    Decode(codes);
}

Decoder::~Decoder() {
    Clear(root);
}

void Decoder::Decode(const std::unordered_map<uint16_t, std::vector<bool>>& codes) {
    for (const auto& [symbol, code] : codes) {
        Add(root, symbol, 0, code);
    }
    cur_vertex = root;
}

void Decoder::Add(Node* vertex, uint16_t value, uint16_t depth, const std::vector<bool>& code) {
    if (depth == code.size()) { 
        vertex->SetValue(value);
        return;
    }
    if (code[depth] == false) {
        if (vertex->GetLeft() == nullptr) {
            vertex->SetLeft(new Node());
        }
        Add(vertex->GetLeft(), value, depth + 1, code);
    } else {
        if (vertex->GetRight() == nullptr) {
            vertex->SetRight(new Node());
        }
        Add(vertex->GetRight(), value, depth + 1, code);
    }
}

uint16_t Decoder::GetSymbol(bool code) {
    cur_vertex = code ? cur_vertex->GetRight() : cur_vertex->GetLeft();
    if (cur_vertex->IsTerminal()) {
        uint16_t result = cur_vertex->GetValue();
        cur_vertex = root;
        return result;
    }
    return max_elem; 
}

void Decoder::Clear(Node* vertex) {
    if (vertex == nullptr) return;
    Clear(vertex->GetLeft());
    Clear(vertex->GetRight());
    delete vertex;
}
