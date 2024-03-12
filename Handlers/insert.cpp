#include <fstream>
#include "../Studio/Studio.h"
#include "../Film/Film.h"

bool Studio::insert(std::fstream& file, const std::streampos& pos) {
    if (!file)
        return false;

    file.seekp(pos);
    file.write(reinterpret_cast<const char*>(this), sizeof(Studio));
    file.flush();

    return !file.fail();
}

bool Film::insertFilm(const Film& film, std::fstream& file, const std::streampos& pos) {
    if (!file)
        return false;

    file.seekp(pos);
    file.write(reinterpret_cast<const char*>(&film), sizeof(Studio));
    file.flush();

    return !file.fail();
}