#include "../Studio/Studio.h"

std::vector<uint32_t> Studio::deletedStudiosAddresses;

uint32_t Studio::getStudioId() {
    return studioId;
}

const char* Studio::getStudioName() {
    return studioName;
}

int64_t Studio::getFirstStudiosFilmAddress() {
    return firstStudiosFilmAddress;
}

int64_t Studio::getStudioAddress() {
    return studioAddress;
}

int64_t Studio::getNextStudioAddress() {
    if (deletedStudiosAddresses.empty()) {
        uint32_t fileSize = STUDIO_FILE.tellg();
        return fileSize / (sizeof(Studio));
    } else {
        uint32_t address = deletedStudiosAddresses.back();
        deletedStudiosAddresses.pop_back();
        return address;
    }
}

bool Studio::insert() {
    if (!STUDIO_FILE)
        return false;

    studioAddress = getNextStudioAddress();

    STUDIO_FILE.seekp(sizeof(Studio) * studioAddress);
    STUDIO_FILE.write(reinterpret_cast<const char*>(this), sizeof(Studio));
    STUDIO_FILE.flush();

    Index index(this->studioId, this->studioAddress);
    index.insertRecord();

    return !STUDIO_FILE.fail();
}

bool Studio::updateStudioName(uint32_t studioId, const char* newName) {
    Studio studio = getStudio(studioId);
    if (studio.getStudioId() == 0) {
        return false;
    }

    uint32_t studioAddress = studio.getStudioAddress();

    size_t nameOffset = offsetof(Studio, studioName);

    STUDIO_FILE.seekp(sizeof(Studio) * studioAddress + nameOffset);

    STUDIO_FILE.write(newName, sizeof(studio.studioName));
    STUDIO_FILE.flush();

    return !STUDIO_FILE.fail();
}

bool Studio::updateStudiosFilmAddress(uint32_t studioId, int64_t newAddress) {
    Studio studio = getStudio(studioId);
    if (studio.getStudioId() == 0) {
        return false;
    }

    uint32_t studioAddress = studio.getStudioAddress();

    size_t addressOffset = offsetof(Studio, firstStudiosFilmAddress);

    STUDIO_FILE.seekp(sizeof(Studio) * studioAddress + addressOffset);

    STUDIO_FILE.write(reinterpret_cast<const char*>(&newAddress), sizeof(newAddress));
    STUDIO_FILE.flush();

    return !STUDIO_FILE.fail();
}

bool Studio::deleteStudio() {
    if (!STUDIO_FILE || !FILM_FILE)
        return false;

    int64_t firstStudiosFilmAddress = getFirstStudiosFilmAddress();
    int64_t currentFilmAddress = firstStudiosFilmAddress;

    while (currentFilmAddress != -1) {
        Film film;
        FILM_FILE.seekg(currentFilmAddress * sizeof(Film));
        FILM_FILE.read(reinterpret_cast<char*>(&film), sizeof(Film));

        delFilm(film.getFilmId(), studioId);

        currentFilmAddress = film.getNext();
    }

    STUDIO_FILE.seekg(0, std::ios::beg);
    Studio studio;
    int64_t studioAddress = -1;
    while (STUDIO_FILE.read(reinterpret_cast<char*>(&studio), sizeof(Studio))) {
        if (studio.getStudioId() == studioId) {
            studioAddress = STUDIO_FILE.tellg() - static_cast<std::streamoff>(sizeof(Studio));
            break;
        }
    }

    if (studioAddress == -1) {
        return false;
    }

    deletedStudiosAddresses.push_back(studioAddress / sizeof(Studio));

    STUDIO_FILE.seekp(studioAddress);
    Studio zeroStudio;
    STUDIO_FILE.write(reinterpret_cast<const char*>(&zeroStudio), sizeof(Studio));
    STUDIO_FILE.flush();

    Index index(studioId, studioAddress);
    index.deleteRecord();

    return !STUDIO_FILE.fail();
}

Studio getStudio(uint32_t studioID) {
    IND_FILE.seekg(0, std::ios::beg);
    std::pair<uint32_t, uint32_t> recordPair;
    bool found = false;
    while (IND_FILE.read(reinterpret_cast<char*>(&recordPair), sizeof(recordPair))) {
        if (recordPair.first == studioID) {
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Studio with ID " << studioID << " not found in the Index file." << std::endl;
        return Studio();
    }

    STUDIO_FILE.seekg(recordPair.second * sizeof(Studio));
    Studio studio;
    STUDIO_FILE.read(reinterpret_cast<char*>(&studio), sizeof(Studio));
    return studio;
}

void printStudioDetails(Studio& studio) {
    std::cout << "Studio ID: " << studio.getStudioId() << std::endl;
    std::cout << "Studio Name: " << studio.getStudioName() << std::endl;
    std::cout << "First Studio's Film Address: " << studio.getFirstStudiosFilmAddress() << std::endl;
    std::cout << "Studio Address: " << studio.getStudioAddress() << std::endl;
    std::cout << std::endl;
}
