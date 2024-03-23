#include "../Film/Film.h"

uint32_t Film::getFilmId() const {
    return filmId;
}

uint32_t Film::getStudioId() const {
    return studioId;
}

const char* Film::getFilmName() const {
    return filmName;
}

uint32_t Film::getBudget() const {
    return budget;
}

int64_t Film::getNext() const {
    return next;
}

void Film::setNext(int64_t nextFilmAddress) {
    next = nextFilmAddress;
}

bool Film::insert() {
    if (!FILM_FILE || !STUDIO_FILE)
        return false;

    STUDIO_FILE.seekg(0, std::ios::beg);
    int64_t studioAddress = -1;
    Studio studio;

    while (STUDIO_FILE.read(reinterpret_cast<char*>(&studio), sizeof(Studio))) {
        if (studio.getStudioId() == studioId) {
            studioAddress = studio.getStudioAddress();
            break;
        }
    }

    if (studioAddress == -1) {
        return false;
    }

    int64_t firstStudiosFilmAddress;
    STUDIO_FILE.seekg(sizeof(Studio) * studioAddress);
    STUDIO_FILE.read(reinterpret_cast<char*>(&firstStudiosFilmAddress), sizeof(firstStudiosFilmAddress));

    FILM_FILE.seekp(0, std::ios::end);
    uint32_t filmAddress = FILM_FILE.tellp() / sizeof(Film);
    FILM_FILE.write(reinterpret_cast<const char*>(this), sizeof(Film));
    FILM_FILE.flush();

    // Update the films address in the studio record
    if (firstStudiosFilmAddress == -1) {
        firstStudiosFilmAddress = filmAddress;
    } else {
        Film prevFilm;
        FILM_FILE.seekg(firstStudiosFilmAddress * sizeof(Film));
        while (FILM_FILE.read(reinterpret_cast<char*>(&prevFilm), sizeof(Film))) {
            if (prevFilm.getNext() == -1) {
                prevFilm.setNext(filmAddress);
                FILM_FILE.seekp(prevFilm.getNext() * sizeof(Film));
                FILM_FILE.write(reinterpret_cast<const char*>(&prevFilm), sizeof(Film));
                break;
            }
            firstStudiosFilmAddress = prevFilm.getNext();
        }
    }

    STUDIO_FILE.seekp(sizeof(Studio) * studioAddress);
    STUDIO_FILE.write(reinterpret_cast<const char*>(&firstStudiosFilmAddress), sizeof(firstStudiosFilmAddress));
    STUDIO_FILE.flush();

    return !FILM_FILE.fail() && !STUDIO_FILE.fail();
}

Film getFilm(uint32_t filmId, uint32_t studioId) {
    Studio studio = getStudio(studioId);
    if (studio.getStudioId() == 0) {
        return Film();
    }

    int64_t firstStudiosFilmAddress = studio.getFirstStudiosFilmAddress();

    Film film;
    FILM_FILE.seekg(firstStudiosFilmAddress * sizeof(Film));
    while (FILM_FILE.read(reinterpret_cast<char*>(&film), sizeof(Film))) {
        if (film.getFilmId() == filmId) {
            return film;
        }
    }

    return Film();
}