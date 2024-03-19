#include <fstream>
#include "../Studio/Studio.h"
#include "../Film/Film.h"
#include "../Studio/IndexTable/Index.h"

bool Studio::insert(std::fstream& file, const std::streampos& pos) {
    if (!file)
        return false;

    file.seekp(pos);
    file.write(reinterpret_cast<const char*>(this), sizeof(Studio));
    file.flush();

    Index index(this->id, this->studioAddress);
    index.insertRecord();

    return !file.fail();
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