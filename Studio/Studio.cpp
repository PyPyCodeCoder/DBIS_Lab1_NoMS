#include "../Studio/Studio.h"

std::vector<uint32_t> Studio::deletedAddresses;

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
