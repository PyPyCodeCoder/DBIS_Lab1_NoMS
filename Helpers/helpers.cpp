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

    Studio studio = getStudio(studioId);

    if (studio.getStudioId() == 0) {
        return false;
    }

    int64_t firstStudiosFilmAddress = studio.getFirstStudiosFilmAddress();
    int64_t prevFilmAddress = -1;
    int64_t currentFilmAddress = firstStudiosFilmAddress;

    Film currentFilm, lastFilm;
    while (currentFilmAddress != -1) {
        FILM_FILE.seekg(currentFilmAddress * sizeof(Film));
        FILM_FILE.read(reinterpret_cast<char*>(&currentFilm), sizeof(Film));

        if (currentFilm.getFilmId() == filmId && currentFilm.getStudioId() == studioId) {
            if (prevFilmAddress != -1) {
                lastFilm.setNext(currentFilm.getNext());
                FILM_FILE.seekp(prevFilmAddress * sizeof(Film));
                FILM_FILE.write(reinterpret_cast<const char*>(&lastFilm), sizeof(Film));
            } else {
                studio.updateStudiosFilmAddress(studioId, currentFilm.getNext());
            }

            currentFilm = Film();
            FILM_FILE.seekp(currentFilmAddress * sizeof(Film));
            FILM_FILE.write(reinterpret_cast<const char*>(&currentFilm), sizeof(Film));

            return !FILM_FILE.fail();
        }

        prevFilmAddress = currentFilmAddress;
        currentFilmAddress = currentFilm.getNext();
        lastFilm = currentFilm;
    }

    return false;
}

uint32_t countMasterRecords() {
    IND_FILE.seekg(0, std::ios::end);
    uint32_t numMasterRecords = IND_FILE.tellg() / sizeof(std::pair<uint32_t, uint32_t>);
    return numMasterRecords;
}

uint32_t countSlaveRecords() {
    FILM_FILE.seekg(0, std::ios::end);
    uint32_t numSlaveRecords = FILM_FILE.tellg() / sizeof(Film);
    return numSlaveRecords;
}

uint32_t countSlaveRecordsForMaster(uint32_t studioId) {
    Studio studio = getStudio(studioId);

    if (studio.getStudioId() == 0) {
        return 0;
    }

    int64_t firstStudiosFilmAddress = studio.getFirstStudiosFilmAddress();
    uint32_t slaveRecordCount = 0;

    FILM_FILE.seekg(firstStudiosFilmAddress * sizeof(Film));
    Film film;
    while (FILM_FILE.read(reinterpret_cast<char*>(&film), sizeof(Film))) {
        if (film.getStudioId() == studioId) {
            slaveRecordCount++;
        }
        int64_t nextFilmAddress = film.getNext();
        if (nextFilmAddress == -1) {
            break;
        }
        FILM_FILE.seekg(nextFilmAddress * sizeof(Film));
    }

    return slaveRecordCount;
}

void printAllMasterRecords() {
    IND_FILE.seekg(0, std::ios::beg);
    std::pair<uint32_t, uint32_t> recordPair;
    while (IND_FILE.read(reinterpret_cast<char*>(&recordPair), sizeof(recordPair))) {
        if (recordPair.first != 0) {
            uint32_t studioId = recordPair.first;
            Studio studio = getStudio(studioId);
            printStudioDetails(studio);
        }
    }

    IND_FILE.clear();
    IND_FILE.seekg(0, std::ios::beg);
}

void printAllSlaveRecords() {
    FILM_FILE.seekg(0, std::ios::beg);
    Film film;
    while (FILM_FILE.read(reinterpret_cast<char*>(&film), sizeof(Film))) {
        if (film.getFilmId() != 0) {
            printFilmDetails(film);
        }
    }

    FILM_FILE.clear();
    FILM_FILE.seekg(0, std::ios::beg);
}