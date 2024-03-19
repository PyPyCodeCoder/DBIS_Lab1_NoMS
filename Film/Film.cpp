#include <fstream>
#include "../Studio/Studio.h"
#include "../Film/Film.h"
#include "../Studio/IndexTable/Index.h"

bool Film::insertFilm(const Film& film, std::fstream& file, const std::streampos& pos) {
    if (!file)
        return false;

    file.seekp(pos);
    file.write(reinterpret_cast<const char*>(&film), sizeof(Film));
    file.flush();

    return !file.fail();
}