#include "trie_node.h"
#include <unordered_map>
#include <queue>

class Encoder {
public:
    Encoder();

    explicit Encoder(const std::unordered_map<uint16_t, size_t>& symbols_count);

    ~Encoder();

    void Encode(const std::unordered_map<uint16_t, size_t>& symbols_count);

    std::unordered_map<uint16_t, std::vector<bool>> GetCodes();

    std::vector<std::pair<uint16_t, uint16_t>> GetCodeInfo() const;
    

private:
    Node* root;

    void Search(Node* vertex, uint16_t cur_length, std::unordered_map<uint16_t, uint16_t>& codes) const;

    void Clear(Node* vertex);

    std::vector<std::pair<uint16_t, uint16_t>> sorted_symbols;

    std::unordered_map<uint16_t, uint16_t> ToCanonicCode(std::unordered_map<uint16_t, uint16_t>& codes_length);
};