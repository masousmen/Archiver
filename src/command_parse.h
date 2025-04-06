#include <string>
#include <vector>

struct ParsedArgs {
    std::string command;
    std::string archive_name;
    std::vector<std::string> files;
};

class CommandLineParser {
public:
    ParsedArgs parseArguments(int argc, char** argv);
    void ShowHelp() const;
};