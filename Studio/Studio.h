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
    Studio(uint32_t id, const char* _name) : id(id), studioAddress(0), filmsAddress(0), studioFilms(0), isDeleted(false) {
        strncpy(name, _name, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    }

    bool insert(std::fstream& file, const std::streampos& pos);

    uint32_t getStudioAddress();
    uint32_t getFilmsAddress();
    uint32_t getStudioFilms();

    void setStudioAddress(uint32_t address);
    void setFilmsAddress(uint32_t address);
    void setStudioFilms(uint32_t count);

    uint32_t createStudioAddress();
};
#endif //LAB1_STUDIO_H