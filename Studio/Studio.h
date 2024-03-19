#ifndef LAB1_STUDIO_H
#define LAB1_STUDIO_H
#include <cstdint>
#include <cstring>

class Studio {
private:
    //seen by user
    const uint32_t id;
    char name[50] = {};

    //for internal usage
    uint32_t studioAddress;//адреса студії (може змінюватись)

    uint32_t filmsAddress; //вказівник на перший фільм
    uint32_t studioFilms; //кількість фільмів студії

    bool isDeleted = false;

public:
    Studio(uint32_t _id, const char* _name) : id(_id) {
        strncpy(name, _name, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    }
    bool insert(std::fstream& file, const std::streampos& pos);
};

#endif //LAB1_STUDIO_H