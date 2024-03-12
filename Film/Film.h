#ifndef LAB1_FILM_H
#define LAB1_FILM_H
#include <cstdint>

class Film {
private:
    uint32_t id;
    uint32_t studio_id;
    char name[50] = {};
    uint32_t budget;

public:
    bool insertFilm(const Film& film, std::fstream& file, const std::streampos& pos);
};

#endif //LAB1_FILM_H
