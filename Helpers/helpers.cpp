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

    if (studio.getStudioId() == 0) {
        return;
    }

    if (clarifier == "full") {
        std::cout << "Studio ID: " << studio.getStudioId() << std::endl;
        std::cout << "Studio Name: " << studio.getStudioName() << std::endl;
        std::cout << "Studio Address: " << studio.getNextStudioAddress() << std::endl;
        std::cout << "Films Address: " << studio.getFirstStudiosFilmAddress() << std::endl;
    } else if (clarifier == "studioName") {
        std::cout << "Studio Name: " << studio.getStudioName() << std::endl;
    } else {
        std::cout << "Invalid clarifier. Please use 'full' or 'studioName'." << std::endl;
    }
}

void getSlave(uint32_t filmId, uint32_t studioId, std::string clarifier) {
    Film film = getFilm(filmId, studioId);

    if (clarifier == "full") {
        std::cout << "Name: " << film.getFilmName() << std::endl;
        std::cout << "Studio ID: " << film.getStudioId() << std::endl;
        std::cout << "Budget: " << film.getBudget() << std::endl;
    } else if (clarifier == "studioName") {
        std::cout << "Name: " << film.getFilmName() << std::endl;
    } else if (clarifier == "studioId") {
        std::cout << "Studio ID: " << film.getStudioId() << std::endl;
    } else if (clarifier == "budget") {
        std::cout << "Budget: " << film.getBudget() << std::endl;
    } else {
        std::cout << "Invalid clarifier: " << clarifier << std::endl;
    }
}

bool delFilm(uint32_t id, uint32_t studioId) {
    Film film = getFilm(id, studioId);

    /**/

    return false;
}