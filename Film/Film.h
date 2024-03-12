#ifndef LAB1_FILM_H
#define LAB1_FILM_H
#include <cstdint>

class Film {
private:
    uint32_t id;
    char name[25] = {};
    uint32_t budget;

    uint32_t studio_id;

public:
    bool insertFilm(const Film& film, std::fstream& file, const std::streampos& pos);
};

#endif //LAB1_FILM_H
