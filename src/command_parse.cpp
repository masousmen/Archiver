#include "command_parse.h"
#include <iostream>

ParsedArgs CommandLineParser::parseArguments(int argc, char** argv) {
    ParsedArgs args;
    args.command = argv[1];
    if (args.command == "-c") {
        if (argc < 4) {
            std::cerr << "Usage: archiver -c archive_name file1 [file2 ...]" << std::endl;
            exit(111);
        }
        args.archive_name = argv[2];
        for (int i = 3; i < argc; ++i) {
            args.files.push_back(argv[i]);
        }
    } else if (args.command == "-d") {
        if (argc != 3) {
            std::cerr << "Usage: archiver -d archive_name" << std::endl;
            exit(111);
        }
        args.archive_name = argv[2];
    } else {
        std::cerr << "Unknown command." << std::endl;
        ShowHelp();
        exit(111);
    }
    return args;
}

void CommandLineParser::ShowHelp() const {
    std::cout << "Usage:" << std::endl;
    std::cout << "  archiver -c archive_name file1 [file2 ...] - Compress files into archive." << std::endl;
    std::cout << "  archiver -d archive_name - Decompress archive into current directory." << std::endl;
    std::cout << "  archiver -h - Show help." << std::endl;
}
