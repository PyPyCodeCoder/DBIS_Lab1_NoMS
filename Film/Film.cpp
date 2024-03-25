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

Film getFilm(uint32_t filmId, uint32_t studioId) {
    // Get the master record from the master file
    Studio studio = getStudio(studioId);

    // Check if the studio exists
    if (studio.getStudioId() == 0) {
        return Film(); // Return an empty Film object if the studio doesn't exist
    }

    // Get the address of the first film in the studio's list of films
    int64_t firstStudiosFilmAddress = studio.getFirstStudiosFilmAddress();

    // Search for the specific film in the list of films
    Film film;
    FILM_FILE.seekg(firstStudiosFilmAddress * sizeof(Film)); // Move to the beginning of the films list
    while (FILM_FILE.read(reinterpret_cast<char*>(&film), sizeof(Film))) {
        if (film.getFilmId() == filmId) { // Check if the film ID matches
            return film; // Return the found film
        }
        // Move to the next film using the next pointer
        if (film.getNext() != -1) {
            FILM_FILE.seekg(film.getNext() * sizeof(Film));
        } else {
            break; // If next pointer is -1, it means we've reached the end of the list
        }
    }

    return Film(); // Return an empty Film object if the film is not found
}

bool Film::updateFilmName(const char* newName) {
    if (!FILM_FILE)
        return false;

    // Find the film's address
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
        // Film not found
        return false;
    }

    // Update the film name
    size_t nameOffset = offsetof(Film, filmName);
    FILM_FILE.seekp(filmAddress + nameOffset);
    FILM_FILE.write(newName, sizeof(filmName));
    FILM_FILE.flush();

    return !FILM_FILE.fail();
}

bool Film::updateFilmBudget(uint32_t newBudget) {
    if (!FILM_FILE)
        return false;

    // Find the film's address
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
        // Film not found
        return false;
    }

    // Update the film budget
    size_t budgetOffset = offsetof(Film, budget);
    FILM_FILE.seekp(filmAddress + budgetOffset);
    FILM_FILE.write(reinterpret_cast<const char*>(&newBudget), sizeof(newBudget));
    FILM_FILE.flush();

    return !FILM_FILE.fail();
}
