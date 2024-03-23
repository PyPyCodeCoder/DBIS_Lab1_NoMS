#ifndef LAB1_FILM_H
#define LAB1_FILM_H
#include <cstdint>
#include <fstream>
#include "../Helpers/helpers.h"
#include "../Studio/Studio.h"
#include "../Studio/IndexTable/Index.h"

class Film {
private:
    uint32_t filmId;
    uint32_t studioId;
    char filmName[50] = {};
    uint32_t budget;

    int64_t next;

public:
    Film() : filmId(0), studioId(0), budget(0), next(-1) {
        filmName[0] = '\0';
    }

    Film(uint32_t _id, uint32_t _studio_id, const char* _name, uint32_t _budget) : filmId(_id), studioId(_studio_id), budget(_budget), next(-1) {
        strncpy(filmName, _name, sizeof(filmName) - 1);
        filmName[sizeof(filmName) - 1] = '\0'; // Ensure null-termination
    }

    uint32_t getFilmId() const;

    uint32_t getStudioId() const;

    const char* getFilmName() const;

    uint32_t getBudget() const;

    int64_t getNext() const;
    void setNext(int64_t nextFilmAddress);

    bool insert();
};

Film getFilm(uint32_t filmId, uint32_t studioId);
#endif //LAB1_FILM_H
