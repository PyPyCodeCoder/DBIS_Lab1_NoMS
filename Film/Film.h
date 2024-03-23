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
    Film(uint32_t _id, uint32_t _studio_id, const char* _name, uint32_t _budget)
            : id(_id), studio_id(_studio_id), budget(_budget) {
        strncpy(name, _name, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0'; // Ensure null-termination
    }

    bool insert();
};
#endif //LAB1_FILM_H
