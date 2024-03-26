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

    int64_t firstStudiosFilmAddress = studio.getFirstStudiosFilmAddress();

    FILM_FILE.seekp(0, std::ios::end);
    uint32_t filmAddress = FILM_FILE.tellp() / sizeof(Film);
    FILM_FILE.write(reinterpret_cast<const char*>(this), sizeof(Film));
    FILM_FILE.flush();

    if (firstStudiosFilmAddress == -1) {
        studio.updateStudiosFilmAddress(studioId, filmAddress);
    } else {
        Film prevFilm;
        FILM_FILE.seekg(firstStudiosFilmAddress * sizeof(Film));
        while (FILM_FILE.read(reinterpret_cast<char*>(&prevFilm), sizeof(Film))) {
            if (prevFilm.getNext() == -1) {
                prevFilm.setNext(filmAddress);
                FILM_FILE.seekp(firstStudiosFilmAddress * sizeof(Film));
                FILM_FILE.write(reinterpret_cast<const char*>(&prevFilm), sizeof(Film));
                break;
            }
            firstStudiosFilmAddress = prevFilm.getNext();
        }
    }

    return !FILM_FILE.fail() && !STUDIO_FILE.fail();
}

bool Film::updateFilmName(const char* newName) {
    if (!FILM_FILE)
        return false;

    FILM_FILE.seekg(0, std::ios::beg);
    Film film;
    int64_t filmAddress = -1;
    while (FILM_FILE.read(reinterpret_cast<char*>(&film), sizeof(Film))) {
        if (film.getFilmId() == filmId && film.getStudioId() == studioId) {
            filmAddress = FILM_FILE.tellg() - static_cast<std::streamoff>(sizeof(Film));
            break;
        }
    }

    if (filmAddress == -1) {
        return false;
    }

    size_t nameOffset = offsetof(Film, filmName);
    FILM_FILE.seekp(filmAddress + nameOffset);
    FILM_FILE.write(newName, sizeof(filmName));
    FILM_FILE.flush();

    return !FILM_FILE.fail();
}

bool Film::updateFilmBudget(uint32_t newBudget) {
    if (!FILM_FILE)
        return false;

    FILM_FILE.seekg(0, std::ios::beg);
    Film film;
    int64_t filmAddress = -1;
    while (FILM_FILE.read(reinterpret_cast<char*>(&film), sizeof(Film))) {
        if (film.getFilmId() == filmId && film.getStudioId() == studioId) {
            filmAddress = FILM_FILE.tellg() - static_cast<std::streamoff>(sizeof(Film));
            break;
        }
    }

    if (filmAddress == -1) {
        return false;
    }

    size_t budgetOffset = offsetof(Film, budget);
    FILM_FILE.seekp(filmAddress + budgetOffset);
    FILM_FILE.write(reinterpret_cast<const char*>(&newBudget), sizeof(newBudget));
    FILM_FILE.flush();

    return !FILM_FILE.fail();
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
        if (film.getNext() != -1) {
            FILM_FILE.seekg(film.getNext() * sizeof(Film));
        } else {
            break;
        }
    }

    return Film();
}

void printFilmDetails(const Film& film) {
    std::cout << "Film ID: " << film.getFilmId() << std::endl;
    std::cout << "Studio ID: " << film.getStudioId() << std::endl;
    std::cout << "Film Name: " << film.getFilmName() << std::endl;
    std::cout << "Budget: " << film.getBudget() << std::endl;
    std::cout << "Next Film Address: " << film.getNext() << std::endl;
    std::cout << std::endl;
}