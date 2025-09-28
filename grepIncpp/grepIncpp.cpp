#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <io.h>    
#define ISATTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>  
#define ISATTY isatty
#define FILENO fileno
#endif

std::string StringToLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return str;
}

void SearchFiles(std::string searchString, std::string searchPath, bool ignoreCase) {
    std::ifstream inputFile;
    std::string line;
    std::string searchLine;

    for (const auto& file : std::filesystem::directory_iterator(searchPath)) {
        inputFile.open(file.path());

        if (inputFile.is_open()) {
            while (getline(inputFile, line)) {
                if (ignoreCase) {
                    searchLine = StringToLower(line);
                }
                else {
                    searchLine = line;
                }
                if (searchLine.find(searchString) != std::string::npos) {
                    std::cout << line << std::endl;
                }
            }
        }
        else {
            std::cerr << "Unable to open file: " << file.path() << std::endl;
        }

        inputFile.close();
    }
}

void SearchOutput(std::string searchString, bool ignoreCase) {
    std::string inputLine;
    std::string searchLine;

    while (std::cin && getline(std::cin, inputLine)) {
        if (ignoreCase) {
            searchLine = StringToLower(inputLine);
        }
        else {
            searchLine = inputLine;
        }

        if (searchLine.find(searchString) != std::string::npos) {
            std::cout << inputLine << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    bool ignoreCase = false;
    std::string searchString;
    std::string searchPath;
    int validArgCount = 2;

    if (argc < 2) {
        std::cout << "Usage: grep [OPTION]... PATTERNS [FILE]...\n"
            "Try 'grep --help' for more information." << std::endl;
        return 1;
    }

    if (std::string("-i") == argv[1]) {
        ignoreCase = true;
        if (argc < 3) {
            std::cerr << "Error: Missing search string." << std::endl;
            return 1;
        }
        searchString = StringToLower(argv[2]);
        validArgCount++;
    }
    else {
        searchString = argv[1];
    }
    if (!ISATTY(FILENO(stdin))) {
        SearchOutput(searchString, ignoreCase);
    }
    else if (argc >= validArgCount + 1) {
        if (ignoreCase) {
            searchPath = argv[3];
        }
        else {
            searchPath = argv[2];
        }
        SearchFiles(searchString, searchPath, ignoreCase);
    }
    else {
        std::cout << "Usage: grep [OPTION]... PATTERNS [FILE]...\n"
            "Try 'grep --help' for more information." << std::endl;
    }

    return 0;
}

