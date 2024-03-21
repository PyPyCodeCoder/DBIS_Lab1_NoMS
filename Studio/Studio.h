#ifndef LAB1_STUDIO_H
#define LAB1_STUDIO_H
#include <cstdint>
#include <cstring>
#include <fstream>
#include "../Helpers/helpers.h"
#include "../Film/Film.h"
#include "../Studio/IndexTable/Index.h"

class Studio {
private:
    //seen by user
    const uint32_t id;
    char name[50] = {};

    //for internal usage
    uint32_t studioAddress;//адреса студії (може змінюватись)
    static std::vector<uint32_t> deletedAddresses;

    uint32_t filmsAddress; //вказівник на перший фільм
    uint32_t studioFilms; //кількість фільмів студії

    bool isDeleted = false;

public:
    Studio(uint32_t id, const char* _name) : id(id), studioAddress(0), filmsAddress(0), studioFilms(0), isDeleted(false) {
        strncpy(name, _name, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    }

    bool insert();
    uint32_t getStudioAddress();
};
#endif //LAB1_STUDIO_H