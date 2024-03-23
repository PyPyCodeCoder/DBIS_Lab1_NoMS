#include "../Studio/Studio.h"

std::vector<uint32_t> Studio::deletedAddresses;

const char* Studio::getName() {
    return name;
}

bool Studio::insert() {
    if (!STUDIO_FILE)
        return false;

    studioAddress = getStudioAddress();

    STUDIO_FILE.seekp(sizeof(Studio) * studioAddress);
    STUDIO_FILE.write(reinterpret_cast<const char*>(this), sizeof(Studio));
    STUDIO_FILE.flush();

    Index index(this->id, this->studioAddress);
    index.insertRecord();

    return !STUDIO_FILE.fail();
}

uint32_t Studio::getStudioAddress() {
    if (deletedAddresses.empty()) {
        uint32_t fileSize = STUDIO_FILE.tellg();
        return fileSize / (sizeof(Studio));
    } else {
        uint32_t address = deletedAddresses.back();
        deletedAddresses.pop_back(); // Remove it from the list
        return address;
    }
}

Studio getStudio(uint32_t studioID) {
    // Find the record in the Index file
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
        return Studio(); // Return an empty Studio object
    }

    // Retrieve the corresponding Studio record from the Studio.fl file
    STUDIO_FILE.seekg(recordPair.second * sizeof(Studio));
    Studio studio;
    STUDIO_FILE.read(reinterpret_cast<char*>(&studio), sizeof(Studio));
    return studio;
}