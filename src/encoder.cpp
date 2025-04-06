#include "encoder.h"
#include <vector>
#include <algorithm>

Encoder::Encoder() : root(nullptr) {}

Encoder::Encoder(const std::unordered_map<uint16_t, size_t>& symbols_count) : root(nullptr) {
    Encode(symbols_count);
}

Encoder::~Encoder() {
    Clear(root);
}

void Encoder::Encode(const std::unordered_map<uint16_t, size_t>& symbols_count) {
    auto comp = [](Node* a, Node* b) -> bool { return a->GetPrior() > b->GetPrior(); };
    std::priority_queue<Node*, std::vector<Node*>, decltype(comp)> queue(comp);

    for (const auto& [symbol, count] : symbols_count) {
        queue.push(new Node(symbol, count));
    }

    while (queue.size() > 1) {
        Node* left_next = queue.top();
        queue.pop();
        Node* right_next = queue.top();
        queue.pop();

        Node* unite_node = new Node(left_next->GetPrior() + right_next->GetPrior(), left_next, right_next);
        queue.push(unite_node);
    }

    if (!queue.empty()) {
        root = queue.top();
    }
}

std::unordered_map<uint16_t, std::vector<bool>> Encoder::GetCodes() {
    std::unordered_map<uint16_t, uint16_t> codes_length;
    uint16_t cur_length = 0;

    if (root) {
        Search(root, cur_length, codes_length);
    }

    auto codes = ToCanonicCode(codes_length);
    std::unordered_map<uint16_t, std::vector<bool>> result;

    for (const auto& [symbol, code] : codes) {
        std::vector<bool> binary_code;
        uint16_t temp_code = code;
        while (temp_code > 0) {
            binary_code.push_back(temp_code % 2);
            temp_code >>= 1;
        }
        std::reverse(binary_code.begin(), binary_code.end());
        result[symbol] = binary_code;
    }

    return result;
}

void Encoder::Search(Node* vertex, uint16_t cur_length, std::unordered_map<uint16_t, uint16_t>& codes) const {
    if (vertex->IsTerminal()) {
        codes[vertex->GetValue()] = cur_length;
    } else {
        if (vertex->GetLeft()) {
            Search(vertex->GetLeft(), cur_length + 1, codes);
        }
        if (vertex->GetRight()) {
            Search(vertex->GetRight(), cur_length + 1, codes);
        }
    }
}

void Encoder::Clear(Node* vertex) {
    if (vertex) {
        Clear(vertex->GetLeft());
        Clear(vertex->GetRight());
        delete vertex;
    }
}

std::unordered_map<uint16_t, uint16_t> Encoder::ToCanonicCode(std::unordered_map<uint16_t, uint16_t>& codes_length) {
    std::unordered_map<uint16_t, uint16_t> result;

    sorted_symbols.clear();
    for (const auto& [symbol, length] : codes_length) {
        sorted_symbols.emplace_back(length, symbol);
    }

    std::sort(sorted_symbols.begin(), sorted_symbols.end());

    uint16_t current_code = 0;
    uint8_t current_code_size = 0;

    for (const auto& [length, symbol] : sorted_symbols) {
        while (current_code_size < length) {
            current_code <<= 1;
            current_code_size++;
        }
        result[symbol] = current_code;
        current_code++;
    }

    return result;
}

std::vector<std::pair<uint16_t, uint16_t>> Encoder::GetCodeInfo() const {
    return sorted_symbols;
}
