#include <iostream>
#include <string>
#include "headers/gameclub.h"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        std::cerr << "Example: " << argv[0] << " file_test.txt" << std::endl;
        return 1;
    }

    try {
        GameClub club(argv[1]);
        club.start();
        club.end();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}