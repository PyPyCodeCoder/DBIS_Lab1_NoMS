#ifndef LAB1_FILM_H
#define LAB1_FILM_H
#include <cstdint>
#include <fstream>
#include "../Helpers/helpers.h"
#include "../Studio/Studio.h"
#include "../Studio/IndexTable/Index.h"

class Film {
private:
    uint32_t id;
    uint32_t studio_id;
    char name[50] = {};
    uint32_t budget;

public:
    bool insertFilm(const Film& film, const std::streampos& pos);
};
#endif //LAB1_FILM_H
