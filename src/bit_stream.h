#include <iostream>

class BitStreamOutput {
public:
    explicit BitStreamOutput(std::ostream& out);

    ~BitStreamOutput();

    void Write(std::vector<bool> value, uint8_t bits_count);

    void ClearBuffer();
private:
    std::ostream& stream_out;
    uint8_t buffer;
    uint8_t bits_used;

};

class BitStreamInput {
public:
    explicit BitStreamInput(std::istream& in);

    ~BitStreamInput();

    uint16_t Read(uint8_t bits_count);

    void FillBuffer();
private:
    std::istream& stream_in;
    uint8_t buffer;
    uint8_t bits_used;

};
