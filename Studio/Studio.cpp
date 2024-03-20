#include "../Studio/Studio.h"

bool Studio::insert(const std::streampos& pos) {
    if (!STUDIO_FILE)
        return false;

    STUDIO_FILE.seekp(pos);
    STUDIO_FILE.write(reinterpret_cast<const char*>(this), sizeof(Studio));
    STUDIO_FILE.flush();

    Index index(this->id, this->studioAddress);
    index.insertRecord();

    return !STUDIO_FILE.fail();
}

uint32_t Studio::getStudioAddress() {
    return this->studioAddress;
}

uint32_t Studio::getFilmsAddress() {
    return this->filmsAddress;
}

uint32_t Studio::getStudioFilms() {
    return this->studioFilms;
}

void Studio::setStudioAddress(uint32_t address) {
    studioAddress = address;
}

void Studio::setFilmsAddress(uint32_t address) {
    filmsAddress = address;
}

void Studio::setStudioFilms(uint32_t count) {
    studioFilms = count;
}

uint32_t Studio::createStudioAddress() {
    //some logic here
}