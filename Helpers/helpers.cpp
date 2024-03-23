#include "../Helpers/helpers.h"

std::fstream STUDIO_FILE;
std::fstream IND_FILE;
std::fstream FILM_FILE;

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



void getMaster(uint32_t studioID, std::string clarifier) {
    Studio studio = getStudio(studioID);

    if (studio.getId() == 0) {
        // Studio not found
        return;
    }

    if (clarifier == "full") {
        // Display all fields of the Studio record
        std::cout << "Studio ID: " << studio.getId() << std::endl;
        std::cout << "Studio Name: " << studio.getName() << std::endl;
        std::cout << "Studio Address: " << studio.getStudioAddress() << std::endl;
        std::cout << "Films Address: " << studio.getFilmsAddress() << std::endl;
        std::cout << "Number of Films: " << studio.getStudioFilms() << std::endl;
        std::cout << "Is Deleted: " << (studio.getIsDeleted() ? "Yes" : "No") << std::endl;
    } else if (clarifier == "name") {
        // Display only the Studio name
        std::cout << "Studio Name: " << studio.getName() << std::endl;
    } else {
        std::cout << "Invalid clarifier. Please use 'full' or 'name'." << std::endl;
    }
}

