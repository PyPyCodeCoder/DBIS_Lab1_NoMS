#ifndef LAB1_STUDIO_H
#define LAB1_STUDIO_H
#include <cstdint>

class Studio {
private:
    uint32_t id;
    char name[25] = {};

public:
    bool insertStudio(const Studio& studio, std::fstream& file, const std::streampos& pos);
};

#endif //LAB1_STUDIO_H