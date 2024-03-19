#include <iostream>
#include <vector>
#include <fstream>

std::fstream getFile(const std::string& filepath) {
    std::fstream file(filepath, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    auto err = errno;

    if (err == ENOENT)
    {
        file = std::fstream(filepath, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    }

    if (!file) {
        std::cerr << "Unable to open file from " << filepath << std::endl;
        throw std::runtime_error("Failed to open file");
    }

    return file;
}

std::vector<std::string> parseInput(std::string input) {
    std::vector<std::string> command;
    int prev_pos = 0;
    for(int i = 0; i <= input.length(); ++i) {
        if(i == input.length() || input[i] == ' ') {
            command.push_back(input.substr(prev_pos, i - prev_pos));
            prev_pos = i + 1;
        }
    }
    return command;
}
