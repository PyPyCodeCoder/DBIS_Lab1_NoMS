#include <iostream>
#include <vector>

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

bool createStudioIndexFile() {
    std::string ind_filepath = R"(D:\Valera\122_22_2\DB\Lab1_NoMS\Studio\Studio.ind)";

    std::fstream ind_file(ind_filepath, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    auto err = errno;

    if (err == ENOENT)
    {
        ind_file = std::fstream(ind_filepath, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    }

    if (!ind_file) {
        std::cerr << "Unable to open file from " << ind_filepath << std::endl;

        return -1;
    }
}