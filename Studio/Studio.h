#ifndef LAB1_STUDIO_H
#define LAB1_STUDIO_H
#include <cstdint>
#include <cstring>

class Studio {
private:
    uint32_t id;
    char name[50] = {};
public:
    Studio() : id(0), name{} {}
    Studio(uint32_t _id, const char* _name) : id(_id) {
        strncpy(name, _name, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    }
    bool insert(std::fstream& file, const std::streampos& pos);
};

#endif //LAB1_STUDIO_H