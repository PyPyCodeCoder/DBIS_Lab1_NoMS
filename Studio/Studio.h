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
    const uint32_t studioId;
    char studioName[50] = {};

    //for internal usage
    int64_t firstStudiosFilmAddress;

    int64_t studioAddress;//адреса студії (може змінюватись)
    static std::vector<int64_t> deletedStudiosAddresses;

public:
    Studio() : studioId(0), firstStudiosFilmAddress(-1) {
        studioName[0] = '\0';
    }

    Studio(uint32_t id, const char* _name) : studioId(id), firstStudiosFilmAddress(-1) {
        strncpy(studioName, _name, sizeof(studioName) - 1);
        studioName[sizeof(studioName) - 1] = '\0';
    }

    uint32_t getStudioId();

    const char* getStudioName();
    void setStudioName(const char* newName);

    int64_t getFirstStudiosFilmAddress();
    void setFirstStudiosFilmAddress(int64_t newAddress);

    int64_t getStudioAddress();

    int64_t getNextStudioAddress();

    bool insert();
    bool updateStudioName(uint32_t studioId, const char* newName);
    bool updateStudiosFilmAddress(uint32_t studioId, int64_t newAddress);
};

Studio getStudio(uint32_t studioId);

#endif //LAB1_STUDIO_H