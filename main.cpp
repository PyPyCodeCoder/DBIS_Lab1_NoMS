#include <iostream>
#include <fstream>
#include <vector>
#include "Handlers/handlers.h"

int main() {
    //std::cout << "Enter the path for the file with studios:" << std::endl;

    std::string studio_filepath = "D:\\Valera\\122_22_2\\DB\\Lab1_NoMS\\Studio\\Studio.fl";
    //std::cin >> studio_filepath;

    std::fstream studio_file(studio_filepath, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    auto err = errno;

    if (err == ENOENT)
    {
        studio_file = std::fstream(studio_filepath, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    }

    if (!studio_file) {
        std::cerr << "Unable to open file from " << studio_filepath << std::endl;

        return -1;
    }


    //std::cout << "Enter the path for the file with films:" << std::endl;

    std::string film_filepath = "D:\\Valera\\122_22_2\\DB\\Lab1_NoMS\\Film\\Film.fl";
    //std::cin >> film_filepath;

    std::fstream film_file(film_filepath, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    auto err1 = errno;

    if (err1 == ENOENT)
    {
        film_file = std::fstream(film_filepath, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    }

    if (!film_file) {
        std::cerr << "Unable to open file from " << film_filepath << std::endl;

        return -1;
    }


    std::cout << "Enter the command" << std::endl;

    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> command = parseInput(input);

    if(command.at(0) == "insert-m") {
        Studio studio(std::stoul(command.at(1)), command.at(2).c_str());
        studio.insert(studio_file, 0);
    }
    else if (command.at(0) == "insert-s") {

    }

    //perform commands actions

    return 0;
}
