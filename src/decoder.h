#include "trie_node.h"
#include <unordered_map>
#include <vector>

class Decoder{
public:
    Decoder();

    explicit Decoder(const std::unordered_map<uint16_t, std::vector<bool>>& codes);
    
    ~Decoder();

    void Decode(const std::unordered_map<uint16_t, std::vector<bool>>& codes);

    uint16_t GetSymbol(bool code);
private:
    Node* root;
    Node* cur_vertex;
    const uint16_t max_elem = 300;

    void Add(Node* vertex, uint16_t value, uint16_t depth, const std::vector<bool>& code);

    void ClearFind();

    void Clear(Node* vertex);
};