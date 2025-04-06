#include "file_open.h"
#include "command_parse.h"
#include <iostream>
#include <assert.h>

int main(int argc, char** argv) {
    CommandLineParser parser;
    auto args = parser.parseArguments(argc, argv);    
    Archiver archiver;
    if (args.command == "-c") {
        archiver.Packing(args.archive_name, args.files);
    } else if (args.command == "-d") {
        archiver.Unpacking(args.archive_name);
    } else if (args.command == "-h") {
        parser.ShowHelp();
    } else {
        exit(111);
    }
}
