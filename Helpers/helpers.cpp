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

void updateFilm(uint32_t id, uint32_t studio_id, const char* name, const char* budget) {
    Film film = getFilm(id, studio_id);

    if (strcmp(name, "_") != 0) {
        film.updateFilmName(name);
    }

    if (strcmp(budget, "_") != 0) {
        film.updateFilmBudget(std::stoul(budget));
    }
}

bool delFilm(uint32_t filmId, uint32_t studioId) {
    if (!FILM_FILE)
        return false;

    // Get the master record from the master file
    Studio studio = getStudio(studioId);

    // Check if the studio exists
    if (studio.getStudioId() == 0) {
        return false; // Studio doesn't exist
    }

    // Get the address of the first film in the studio's list of films
    int64_t firstStudiosFilmAddress = studio.getFirstStudiosFilmAddress();
    int64_t prevFilmAddress = -1;
    int64_t currentFilmAddress = firstStudiosFilmAddress;

    // Search for the specific film in the list of films
    Film currentFilm, lastFilm;
    while (currentFilmAddress != -1) {
        FILM_FILE.seekg(currentFilmAddress * sizeof(Film));
        FILM_FILE.read(reinterpret_cast<char*>(&currentFilm), sizeof(Film));

        if (currentFilm.getFilmId() == filmId && currentFilm.getStudioId() == studioId) {
            // Film found, now delete it
            if (prevFilmAddress != -1) {
                // Not the first film in the list, so update previous film's next pointer
                lastFilm.setNext(currentFilm.getNext());
                FILM_FILE.seekp(prevFilmAddress * sizeof(Film));
                FILM_FILE.write(reinterpret_cast<const char*>(&lastFilm), sizeof(Film));
            } else {
                // First film in the list, update studio's first film address
                studio.updateStudiosFilmAddress(studioId, currentFilm.getNext());
            }

            // Remove the film from the file (mark it as deleted)
            currentFilm = Film(); // Clear the current film data
            FILM_FILE.seekp(currentFilmAddress * sizeof(Film));
            FILM_FILE.write(reinterpret_cast<const char*>(&currentFilm), sizeof(Film));

            return !FILM_FILE.fail();
        }

        prevFilmAddress = currentFilmAddress;
        currentFilmAddress = currentFilm.getNext();
        lastFilm = currentFilm; // Store the last film in case it's needed for updating next pointer
    }

    return false; // Film not found
}
