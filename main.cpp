#include <iostream>
#include <vector>
#include "Helpers/helpers.h"
#include "Studio/Studio.h"
#include "Film/Film.h"

int main() {
    try {
        //std::cout << "Enter the path for the file with studios:" << std::endl;
        std::string studio_filepath = R"(D:\Valera\122_22_2\DB\Lab1_NoMS\Studio\Studio.fl)";
        //std::cin >> studio_filepath;
        STUDIO_FILE = getFile(studio_filepath);

        IND_FILE = getFile(R"(D:\Valera\122_22_2\DB\Lab1_NoMS\Studio\IndexTable\Studio.ind)");

        //std::cout << "Enter the path for the file with films:" << std::endl;
        std::string film_filepath = R"(D:\Valera\122_22_2\DB\Lab1_NoMS\Film\Film.fl)";
        //std::cin >> film_filepath;
        FILM_FILE = getFile(film_filepath);

        while(true) {
            std::cout << "Enter the command" << std::endl;

            std::string input;
            std::getline(std::cin, input);

            std::vector<std::string> command = parseInput(input);

            //perform commands actions
            if (command.at(0) == "insert-m") {
                Studio studio(std::stoul(command.at(1)), command.at(2).c_str());

                studio.insert();
            } else if (command.at(0) == "insert-s") {


            } else if (command.at(0) == "exit") {
                return 0;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return -1;
    }
}
