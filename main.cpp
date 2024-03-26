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
            std::cout << "Enter the command:" << std::endl;

            std::string input;
            std::getline(std::cin, input);

            std::vector<std::string> command = parseInput(input);

            //perform commands actions
            if (command.at(0) == "insert-m") {
                bool found = false;
                IND_FILE.seekg(0, std::ios::beg);
                std::pair<uint32_t, uint32_t> tempPair;
                while (IND_FILE.read(reinterpret_cast<char*>(&tempPair), sizeof(tempPair))) {
                    if (tempPair.first == std::stoul(command.at(1))) {
                        found = true;
                    }
                }

                IND_FILE.clear();
                IND_FILE.seekg(0, std::ios::beg);

                if(found) {
                    std::cout << "Studio with ID " << std::stoul(command.at(1)) << " already exists." << std::endl;
                }
                else {
                    Studio studio(std::stoul(command.at(1)), command.at(2).c_str());
                    studio.insert();
                }
            }
            else if (command.at(0) == "insert-s") {
                bool found = false;
                FILM_FILE.seekg(0, std::ios::beg);

                Studio studio = getStudio(std::stoul(command.at(2)));
                if (studio.getStudioId() == 0) {
                    found = false;
                }
                int64_t firstStudiosFilmAddress = studio.getFirstStudiosFilmAddress();
                Film film;
                FILM_FILE.seekg(firstStudiosFilmAddress * sizeof(Film));
                while (FILM_FILE.read(reinterpret_cast<char*>(&film), sizeof(Film))) {
                    if (film.getFilmId() == std::stoul(command.at(1))) {
                        found = true;
                    }
                    if (film.getNext() != -1) {
                        FILM_FILE.seekg(film.getNext() * sizeof(Film));
                    } else {
                        break;
                    }
                }

                FILM_FILE.clear();
                FILM_FILE.seekg(0, std::ios::beg);

                if(found) {
                    std::cout << "Film with ID " << std::stoul(command.at(1)) << " already exists." << std::endl;
                }
                else {
                    Film film(std::stoul(command.at(1)), std::stoul(command.at(2)), command.at(3).c_str(), std::stoul(command.at(4)));
                    film.insert();
                }
            }
            else if (command.at(0) == "get-m") {
                getMaster(std::stoul(command.at(1)), command.at(2));
            }
            else if (command.at(0) == "get-s") {
                getSlave(std::stoul(command.at(1)), std::stoul(command.at(2)), command.at(3));
            }
            else if (command.at(0) == "update-m") {
                Studio studio;
                studio.updateStudioName(std::stoul(command.at(1)), command.at(2).c_str());
            }
            else if (command.at(0) == "update-s") {
                updateFilm(std::stoul(command.at(1)), std::stoul(command.at(2)), command.at(3).c_str(), command.at(4).c_str());
            }
            else if (command.at(0) == "del-m") {
                Studio st = getStudio(std::stoul(command.at(1)));
                st.deleteStudio();
            }
            else if (command.at(0) == "del-s") {
                delFilm(std::stoul(command.at(1)), std::stoul(command.at(2)));
            }
            else if (command.at(0) == "calc-m") {
                std::cout << "Number of master records: " << countMasterRecords() << std::endl;
            }
            else if (command.at(0) == "calc-s") {
                if(command.size() == 1) {
                    std::cout << "Number of slave records: " << countSlaveRecords() << std::endl;
                }
                else {
                    std::cout << "Number of films for studio ID " << std::stoul(command.at(1)) << ": " << countSlaveRecordsForMaster(std::stoul(command.at(1))) << std::endl;
                }
            }
            else if (command.at(0) == "ut-m") {
                printAllMasterRecords();
            }
            else if (command.at(0) == "ut-s") {
                printAllSlaveRecords();
            }
            else if (command.at(0) == "exit") {
                return 0;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return -1;
    }
}
