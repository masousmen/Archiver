#include "encoder.h"
#include "bit_stream.h"

class Archiver {
public:
    void Packing(const std::string& name, const std::vector<std::string>& files);

    void Unpacking(const std::string& archive_name);
private:
    const uint16_t filename_end = 256;
    const uint16_t one_more_file = 257;
    const uint16_t archive_end = 258;

    std::vector<bool> BitToVector(uint16_t code) const;

    bool FileUnpack(BitStreamInput& input);

    void ArchiveFile(const std::string& file_name, BitStreamOutput& output, bool is_last);
};
