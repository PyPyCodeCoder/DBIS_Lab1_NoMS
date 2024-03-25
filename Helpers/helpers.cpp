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

//void delMaster(uint32_t studioId) {
//    // Get the studio by its ID
//    Studio studio = getStudio(studioId);
//
//    // If the studio is not found, exit the function
//    if (studio.getStudioId() == 0) {
//        std::cout << "Studio with ID " << studioId << " not found." << std::endl;
//        return;
//    }
//
//    // Delete all films associated with this studio
//    int64_t currentFilmAddress = studio.getFirstStudiosFilmAddress();
//    while (currentFilmAddress != -1) {
//        Film currentFilm;
//        FILM_FILE.seekg(currentFilmAddress * sizeof(Film));
//        FILM_FILE.read(reinterpret_cast<char*>(&currentFilm), sizeof(Film));
//
//        int64_t nextFilmAddress = currentFilm.getNext();
//        delFilm(currentFilm.getFilmId(), studioId);
//        currentFilmAddress = nextFilmAddress;
//    }
//
//    // Delete the studio from the index file
//    Index index(studioId, studio.getStudioAddress());
//    index.deleteRecord();
//
//    // Move the last record in the studio file to the position of the deleted studio
//    STUDIO_FILE.seekg(0, std::ios::end);
//    int64_t lastStudioAddress = (STUDIO_FILE.tellg() / sizeof(Studio)) - 1;
//    if (lastStudioAddress >= 0) {
//        Studio lastStudio;
//        STUDIO_FILE.seekg(lastStudioAddress * sizeof(Studio));
//        STUDIO_FILE.read(reinterpret_cast<char*>(&lastStudio), sizeof(Studio));
//
//        // Update the last studio's record in the index file
//        Index lastIndex(lastStudio.getStudioId(), studio.getStudioAddress());
//        lastIndex.deleteRecord();
//        lastIndex.insertRecord();
//
//        // Move the last record to the position of the deleted studio
//        STUDIO_FILE.seekp(studio.getStudioAddress() * sizeof(Studio));
//        STUDIO_FILE.write(reinterpret_cast<const char*>(&lastStudio), sizeof(Studio));
//    }
//
//    // Delete the last record in the studio file
//    STUDIO_FILE.seekp(0, std::ios::end);
//    STUDIO_FILE.seekp(std::max(static_cast<int64_t>(0), static_cast<int64_t>(STUDIO_FILE.tellp()) - static_cast<int64_t>(sizeof(Studio))));
//    Studio emptyStudio;
//    STUDIO_FILE.write(reinterpret_cast<const char*>(&emptyStudio), sizeof(Studio));
//
//    // Sort the index file
//    Index ind(0, 0);
//    ind.sortRecords();
//}

//void delMaster(uint32_t studioId) {
//    // Get the studio by its ID
//    Studio studio = getStudio(studioId);
//
//    // If the studio is not found, exit the function
//    if (studio.getStudioId() == 0) {
//        std::cout << "Studio with ID " << studioId << " not found." << std::endl;
//        return;
//    }
//
//    // Delete all films associated with this studio
//    int64_t currentFilmAddress = studio.getFirstStudiosFilmAddress();
//    while (currentFilmAddress != -1) {
//        Film currentFilm;
//        FILM_FILE.seekg(currentFilmAddress * sizeof(Film));
//        FILM_FILE.read(reinterpret_cast<char*>(&currentFilm), sizeof(Film));
//
//        int64_t nextFilmAddress = currentFilm.getNext();
//        delFilm(currentFilm.getFilmId(), studioId);
//        currentFilmAddress = nextFilmAddress;
//    }
//
//    // Delete the studio from the index file
//    IND_FILE.seekg(0, std::ios::beg);
//    std::pair<uint32_t, uint32_t> currRecord;
//    int64_t currRecordAddress = -1;
//    int64_t prevRecordAddress = -1;
//    bool found = false;
//
//    while (IND_FILE.read(reinterpret_cast<char*>(&currRecord), sizeof(currRecord))) {
//        if (currRecord.first == studioId) {
//            found = true;
//            currRecordAddress = IND_FILE.tellg() - static_cast<std::streamoff>(sizeof(currRecord));
//            break;
//        }
//        prevRecordAddress = IND_FILE.tellg() - static_cast<std::streamoff>(sizeof(currRecord));
//    }
//
//    if (!found) {
//        std::cout << "Studio with ID " << studioId << " not found in the Index file." << std::endl;
//        return;
//    }
//
//    // Remove the record from the file (mark it as deleted)
//    IND_FILE.seekp(currRecordAddress);
//    std::pair<uint32_t, uint32_t> emptyRecord = {0, 0};
//    IND_FILE.write(reinterpret_cast<const char*>(&emptyRecord), sizeof(currRecord));
//
//    // Update the previous record's next pointer if it's not the first record
//    if (prevRecordAddress != -1) {
//        IND_FILE.seekg(prevRecordAddress);
//        std::pair<uint32_t, uint32_t> prevRecord;
//        IND_FILE.read(reinterpret_cast<char*>(&prevRecord), sizeof(prevRecord));
//        int64_t nextRecordAddress = IND_FILE.tellg() + sizeof(currRecord);
//        IND_FILE.seekg(nextRecordAddress);
//        std::pair<uint32_t, uint32_t> nextRecord;
//        IND_FILE.read(reinterpret_cast<char*>(&nextRecord), sizeof(nextRecord));
//
//        // If the record being deleted is the last record, set the next pointer to -1
//        if (nextRecord.first == 0 && nextRecord.second == 0) {
//            prevRecord.second = -1;
//        } else {
//            prevRecord.second = nextRecordAddress;
//        }
//
//        IND_FILE.seekp(prevRecordAddress);
//        IND_FILE.write(reinterpret_cast<const char*>(&prevRecord), sizeof(prevRecord));
//    } else {
//        // First record, update the deletedAddresses vector
//        Index::updateDeletedAddresses(0);
//    }
//
//    IND_FILE.flush();
//
//    // Move the last record in the studio file to the position of the deleted studio
//    STUDIO_FILE.seekg(0, std::ios::end);
//    int64_t lastStudioAddress = (STUDIO_FILE.tellg() / sizeof(Studio)) - 1;
//    if (lastStudioAddress >= 0) {
//        Studio lastStudio;
//        STUDIO_FILE.seekg(lastStudioAddress * sizeof(Studio));
//        STUDIO_FILE.read(reinterpret_cast<char*>(&lastStudio), sizeof(Studio));
//
//        // Update the last studio's record in the index file
//        IND_FILE.seekg(0, std::ios::beg);
//        std::pair<uint32_t, uint32_t> lastRecord;
//        int64_t lastRecordAddress = -1;
//        while (IND_FILE.read(reinterpret_cast<char*>(&lastRecord), sizeof(lastRecord))) {
//            if (lastRecord.first == lastStudio.getStudioId()) {
//                lastRecordAddress = IND_FILE.tellg() - static_cast<std::streamoff>(sizeof(lastRecord));
//                break;
//            }
//        }
//
//        if (lastRecordAddress != -1) {
//            IND_FILE.seekp(lastRecordAddress);
//            std::pair<uint32_t, uint32_t> newRecord(lastStudio.getStudioId(), studio.getStudioAddress());
//            IND_FILE.write(reinterpret_cast<const char*>(&newRecord), sizeof(newRecord));
//        }
//
//        // Move the last record to the position of the deleted studio
//        STUDIO_FILE.seekp(studio.getStudioAddress() * sizeof(Studio));
//        STUDIO_FILE.write(reinterpret_cast<const char*>(&lastStudio), sizeof(Studio));
//    }
//
//    // Delete the last record in the studio file
//    STUDIO_FILE.seekp(0, std::ios::end);
//    STUDIO_FILE.seekp(std::max(static_cast<int64_t>(0), static_cast<int64_t>(STUDIO_FILE.tellp()) - static_cast<int64_t>(sizeof(Studio))));
//    Studio emptyStudio;
//    STUDIO_FILE.write(reinterpret_cast<const char*>(&emptyStudio), sizeof(Studio));
//
//    // Sort the index file
//    Index ind(0, 0);
//    ind.sortRecords();
//}

//void delMaster(uint32_t studioId) {
//    // Get the studio by its ID
//    Studio studio = getStudio(studioId);
//
//    // If the studio is not found, exit the function
//    if (studio.getStudioId() == 0) {
//        std::cout << "Studio with ID " << studioId << " not found." << std::endl;
//        return;
//    }
//
//    // Delete all films associated with this studio
//    int64_t currentFilmAddress = studio.getFirstStudiosFilmAddress();
//    while (currentFilmAddress != -1) {
//        Film currentFilm;
//        FILM_FILE.seekg(currentFilmAddress * sizeof(Film));
//        FILM_FILE.read(reinterpret_cast<char*>(&currentFilm), sizeof(Film));
//
//        int64_t nextFilmAddress = currentFilm.getNext();
//        delFilm(currentFilm.getFilmId(), studioId);
//        currentFilmAddress = nextFilmAddress;
//    }
//
//    // Delete the studio from the index file
//    IND_FILE.seekg(0, std::ios::beg);
//    std::pair<uint32_t, uint32_t> currRecord;
//    int64_t currRecordAddress = -1;
//    int64_t prevRecordAddress = -1;
//    bool found = false;
//
//    while (IND_FILE.read(reinterpret_cast<char*>(&currRecord), sizeof(currRecord))) {
//        if (currRecord.first == studioId) {
//            found = true;
//            currRecordAddress = IND_FILE.tellg() - static_cast<std::streamoff>(sizeof(currRecord));
//            break;
//        }
//        prevRecordAddress = IND_FILE.tellg() - static_cast<std::streamoff>(sizeof(currRecord));
//    }
//
//    if (!found) {
//        std::cout << "Studio with ID " << studioId << " not found in the Index file." << std::endl;
//        return;
//    }
//
//    // Remove the record from the file (mark it as deleted)
//    IND_FILE.seekp(currRecordAddress);
//    std::pair<uint32_t, uint32_t> emptyRecord = {0, 0};
//    IND_FILE.write(reinterpret_cast<const char*>(&emptyRecord), sizeof(currRecord));
//
//    // Update the previous record's next pointer if it's not the first record
//    if (prevRecordAddress != -1) {
//        IND_FILE.seekg(prevRecordAddress);
//        std::pair<uint32_t, uint32_t> prevRecord;
//        IND_FILE.read(reinterpret_cast<char*>(&prevRecord), sizeof(prevRecord));
//        int64_t nextRecordAddress = static_cast<int64_t>(IND_FILE.tellp()) + static_cast<int64_t>(sizeof(currRecord));
//
//        IND_FILE.seekg(nextRecordAddress);
//        std::pair<uint32_t, uint32_t> nextRecord;
//        IND_FILE.read(reinterpret_cast<char*>(&nextRecord), sizeof(nextRecord));
//
//        // If the record being deleted is the last record, set the next pointer to -1
//        if (nextRecord.first == 0 && nextRecord.second == 0) {
//            prevRecord.second = -1;
//        } else {
//            prevRecord.second = nextRecordAddress;
//        }
//
//        IND_FILE.seekp(prevRecordAddress);
//        IND_FILE.write(reinterpret_cast<const char*>(&prevRecord), sizeof(prevRecord));
//    } else {
//        // First record, update the deletedAddresses vector
//        //Index::updateDeletedAddresses(0);
//    }
//
//    IND_FILE.flush();
//
//    // Move the last record in the studio file to the position of the deleted studio
//    STUDIO_FILE.seekg(0, std::ios::end);
//    int64_t lastStudioAddress = (STUDIO_FILE.tellg() / sizeof(Studio)) - 1;
//    if (lastStudioAddress >= 0) {
//        Studio lastStudio;
//        STUDIO_FILE.seekg(lastStudioAddress * sizeof(Studio));
//        STUDIO_FILE.read(reinterpret_cast<char*>(&lastStudio), sizeof(Studio));
//
//        // Update the last studio's record in the index file
//        IND_FILE.seekg(0, std::ios::beg);
//        std::pair<uint32_t, uint32_t> lastRecord;
//        int64_t lastRecordAddress = -1;
//        while (IND_FILE.read(reinterpret_cast<char*>(&lastRecord), sizeof(lastRecord))) {
//            if (lastRecord.first == lastStudio.getStudioId()) {
//                lastRecordAddress = IND_FILE.tellg() - static_cast<std::streamoff>(sizeof(lastRecord));
//                break;
//            }
//        }
//
//        if (lastRecordAddress != -1) {
//            IND_FILE.seekp(lastRecordAddress);
//            std::pair<uint32_t, uint32_t> newRecord(lastStudio.getStudioId(), studio.getStudioAddress());
//            IND_FILE.write(reinterpret_cast<const char*>(&newRecord), sizeof(newRecord));
//        }
//
//        // Move the last record to the position of the deleted studio
//        STUDIO_FILE.seekp(studio.getStudioAddress() * sizeof(Studio));
//        STUDIO_FILE.write(reinterpret_cast<const char*>(&lastStudio), sizeof(Studio));
//    }
//
//    // Delete the last record in the studio file
//    STUDIO_FILE.seekp(0, std::ios::end);
//    STUDIO_FILE.seekp(std::max(static_cast<int64_t>(0), static_cast<int64_t>(STUDIO_FILE.tellp()) - static_cast<int64_t>(sizeof(Studio))));
//    Studio emptyStudio;
//    STUDIO_FILE.write(reinterpret_cast<const char*>(&emptyStudio), sizeof(Studio));
//
//    // Sort the index file
//    Index ind(0, 0);
//    ind.sortRecords();
//}


// Function to count the total number of master records
uint32_t countMasterRecords() {
    IND_FILE.seekg(0, std::ios::end);
    uint32_t numMasterRecords = IND_FILE.tellg() / sizeof(std::pair<uint32_t, uint32_t>);
    return numMasterRecords;
}

// Function to count the total number of slave records
uint32_t countSlaveRecords() {
    FILM_FILE.seekg(0, std::ios::end);
    uint32_t numSlaveRecords = FILM_FILE.tellg() / sizeof(Film);
    return numSlaveRecords;
}

// Function to count the number of slave records for a given master record
uint32_t countSlaveRecordsForMaster(uint32_t studioId) {
    Studio studio = getStudio(studioId);

    if (studio.getStudioId() == 0) {
        // Studio not found
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

// Function to print all master records (studios)
//void printAllMasterRecords() {
//    IND_FILE.seekg(0, std::ios::beg);
//    std::pair<uint32_t, uint32_t> recordPair;
//    while (IND_FILE.read(reinterpret_cast<char*>(&recordPair), sizeof(recordPair))) {
//        if (recordPair.first != 0) {
//            uint32_t studioId = recordPair.first;
//            Studio studio = getStudio(studioId);
//            printStudioDetails(studio);
//        }
//    }
//}

// Function to print all master records (studios)
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

    // Reposition the stream's position indicator back to the beginning of the file
    IND_FILE.clear(); // Clear any error flags that may have been set
    IND_FILE.seekg(0, std::ios::beg);
}


// Function to print all slave records (films)
void printAllSlaveRecords() {
    FILM_FILE.seekg(0, std::ios::beg);
    Film film;
    while (FILM_FILE.read(reinterpret_cast<char*>(&film), sizeof(Film))) {
        if (film.getFilmId() != 0) {
            printFilmDetails(film);
        }
    }

    // Reposition the stream's position indicator back to the beginning of the file
    FILM_FILE.clear(); // Clear any error flags that may have been set
    FILM_FILE.seekg(0, std::ios::beg);
}