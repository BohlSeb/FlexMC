#include <iostream>
#include "i_file.h"


namespace flexMC {
    std::vector<std::string> readLinesFromFile(const std::string &filename) {
        std::vector<std::string> lines;
        std::ifstream stream(filename);
        if (!stream.is_open()) {
            std::cerr << "Error opening file: " << filename << "\n";
            return {};
        }
        std::string line;
        while (std::getline(stream, line)) {
            lines.push_back(line);
        }
        if (stream.bad()) {
            stream.close();
            std::cerr << "Error reading file: " << filename << "\n";
            return {};
        }
        stream.close();
        return lines;
    }
}
