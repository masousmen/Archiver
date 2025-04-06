#include "bit_stream.h"
#include <algorithm>

BitStreamOutput::BitStreamOutput(std::ostream& out) : stream_out(out), buffer(0), bits_used(0) {
}

BitStreamOutput::~BitStreamOutput() {
    ClearBuffer();
}

void BitStreamOutput::Write(std::vector<bool> value, uint8_t bits_count) {
    uint8_t cur_bit = 0;
    while (bits_count > 0) {
        uint8_t bits_to_use = std::min(bits_count, static_cast<uint8_t>(8 - bits_used));
        for (uint8_t idx = cur_bit; idx < cur_bit + bits_to_use; ++idx) {
            buffer <<= 1;
            if (idx < value.size() && value[idx]) {
                buffer++;
            }
        }
        bits_count -= bits_to_use;
        bits_used += bits_to_use;
        cur_bit += bits_to_use;
        if (bits_used == 8) {
            ClearBuffer();
        }
    }
}

void BitStreamOutput::ClearBuffer() {
    if (bits_used > 0) {
        buffer <<= (8 - bits_used);
        stream_out << buffer;
        buffer = 0;
        bits_used = 0;
    }
}

BitStreamInput::BitStreamInput(std::istream& in) : stream_in(in), buffer(0), bits_used(8) {
};

BitStreamInput::~BitStreamInput() {
    FillBuffer();
}

uint16_t BitStreamInput::Read(uint8_t bits_count) {
    uint16_t result = 0;
    while (bits_count > 0) {
        if (bits_used == 8) {
            FillBuffer();
        }
        uint8_t bits_to_use = std::min(bits_count, static_cast<uint8_t>(8 - bits_used));
        result <<= bits_to_use;
        result += ((buffer >> (8 - bits_used - bits_to_use)) & ((1 << bits_to_use) - 1));
        bits_used += bits_to_use;
        bits_count -= bits_to_use;
    }
    return result;
}

void BitStreamInput::FillBuffer() {
    buffer = stream_in.get();
    bits_used = 0;
}

