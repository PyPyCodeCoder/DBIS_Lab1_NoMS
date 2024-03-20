#include "../Film/Film.h"

bool Film::insertFilm(const Film& film, const std::streampos& pos) {
    if (!FILM_FILE)
        return false;

    FILM_FILE.seekp(pos);
    FILM_FILE.write(reinterpret_cast<const char*>(&film), sizeof(Film));
    FILM_FILE.flush();

    return !FILM_FILE.fail();
}