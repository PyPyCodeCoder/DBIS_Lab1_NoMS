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
    Studio() : id(0), studioAddress(0), filmsAddress(0), studioFilms(0), isDeleted(false) {
        name[0] = '\0'; // Initialize name to an empty string
    }

    Studio(uint32_t id, const char* _name) : id(id), studioAddress(0), filmsAddress(0), studioFilms(0), isDeleted(false) {
        strncpy(name, _name, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    }

    uint32_t getId() const {
        return id;
    }

    uint32_t getStudioAddress() const {
        return studioAddress;
    }

    uint32_t getFilmsAddress() const {
        return filmsAddress;
    }

    uint32_t getStudioFilms() const {
        return studioFilms;
    }

    bool getIsDeleted() const {
        return isDeleted;
    }

    const char* getName();
    bool insert();
    uint32_t getStudioAddress();
};
Studio getStudio(uint32_t studioID);
#endif //LAB1_STUDIO_H