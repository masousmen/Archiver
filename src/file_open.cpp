#include "file_open.h"
#include "decoder.h"
#include <fstream>

std::vector<bool> Archiver::BitToVector(uint16_t code) const {
    std::vector<bool> result;
    while (code > 0) {
        result.push_back(static_cast<bool>(code % 2));
        code >>= 1;
    }
    return result;
}

void Archiver::ArchiveFile(const std::string& file_name, BitStreamOutput& output, bool is_last) {
    std::unordered_map<uint16_t, size_t> symbols_count;
    symbols_count[filename_end] = 1;
    symbols_count[one_more_file] = 1;
    symbols_count[archive_end] = 1;
    std::ifstream input(file_name, std::ios::binary);
    char c;
    while (input.get(c)) {
        symbols_count[static_cast<uint8_t>(c)]++;
    }
    input.close();

    Encoder encoder(symbols_count);
    std::unordered_map<uint16_t, std::vector<bool>> codes = encoder.GetCodes();
    for (auto [a, b] : codes) {
        std::cout << a << " ";
        for (auto i : b) {
            std::cout << (int) i << " ";
        }
        std::cout << "\n";
    }
    auto codes_info = encoder.GetCodeInfo();
    output.Write(BitToVector(static_cast<uint16_t>(codes_info.size())), 9);
    for (const auto& [code_length, symbol] : codes_info) {
        output.Write(BitToVector(symbol), 9);
    }
    uint16_t left_idx = 0;
    for (uint16_t length = 0; length < codes_info.back().first; ++length) {
        uint16_t right_idx = left_idx;
        while (codes_info[right_idx].first == length + 1) {
            ++right_idx;
        }
        output.Write(BitToVector(right_idx - left_idx), 9);
    }

    for (const char& c : file_name) {
        std::vector<bool> code = codes[static_cast<uint16_t>(c)];
        output.Write(code, static_cast<uint8_t>(code.size()));
    }
    output.Write(codes[filename_end], static_cast<uint8_t>(codes[filename_end].size()));
    input.open(file_name, std::ios::binary);
    while (input.get(c)) {
        output.Write(codes[c], static_cast<uint8_t>(codes[c].size()));
    }
    output.Write(codes[one_more_file], static_cast<uint8_t>(codes[one_more_file].size()));

    if (is_last) {
        output.Write(codes[archive_end], static_cast<uint8_t>(codes[archive_end].size()));
    }
}

void Archiver::Packing(const std::string& name, const std::vector<std::string>& files) {
    std::ofstream file_archive(name, std::ios::binary);
    BitStreamOutput output(file_archive);
    for (size_t idx = 0 ; idx < files.size() - 1; ++idx) {
        ArchiveFile(files[idx], output, false);
    }
    ArchiveFile(files.back(), output, true);
    output.ClearBuffer();
    file_archive.close();
}

bool Archiver::FileUnpack(BitStreamInput& input) {
    uint16_t alphabet_size = input.Read(9);
    std::vector<uint16_t> alphabet;
    for (uint16_t idx = 0; idx < alphabet_size; ++idx) {
        alphabet.push_back(input.Read(9));
    }
    std::unordered_map<uint16_t, uint16_t> codes;
    uint16_t cur_code = 0;
    uint16_t cur_symbol = 0;
    uint16_t elems_sum = 0;
    while (elems_sum < alphabet_size) {
        cur_code <<= 1;
        uint16_t symbols_count = input.Read(9);
        elems_sum += symbols_count;
        for (uint16_t idx = 0; idx < symbols_count; ++idx) {
            codes[alphabet[cur_symbol]] = cur_code;
            ++cur_code;
        }
    }
    std::unordered_map<uint16_t, std::vector<bool>> vector_codes;
    for (const auto& [symbol, code] : codes) {
        vector_codes[symbol] = BitToVector(code);
    }

    Decoder decoder(vector_codes);
    std::string file_name;
    while (true) {
        uint16_t cur_symbol = decoder.GetSymbol(input.Read(1));
        while (cur_symbol > archive_end) {
            cur_symbol = decoder.GetSymbol(input.Read(1));
        }
        if (cur_symbol == filename_end) {
            break;
        }
        file_name += static_cast<char>(cur_symbol);
    }

    std::fstream output(file_name, std::ios::binary);
    while (true) {
        uint16_t cur_symbol = decoder.GetSymbol(input.Read(1));
        while (cur_symbol > archive_end) {
            cur_symbol = decoder.GetSymbol(input.Read(1));
        }
        if (cur_symbol > filename_end) {
            output.close();
            if (cur_symbol == one_more_file) {
                return true;
            }
            return false;
        }
        output.put(static_cast<char>(cur_symbol));
    }
    return true;
}

void Archiver::Unpacking(const std::string& archive_name) {
    std::ifstream file_archive(archive_name, std::ios::binary);
    BitStreamInput input(file_archive);
    while (FileUnpack(input)) {
        continue;
    }
}