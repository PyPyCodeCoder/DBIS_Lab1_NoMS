#ifndef LAB1_NOMS_INDEX_H
#define LAB1_NOMS_INDEX_H
#include <cstdint>
#include <utility>
#include <fstream>

class Index {
private:
    uint32_t id;
    uint32_t studioAddress;
public:
    Index(uint32_t _id, uint32_t _studioAddress) : id(_id), studioAddress(_studioAddress) {}

    //write, delete, get

    //get file and positions?
    bool insertRecord(std::fstream& file, const std::streampos& pos);
    bool deleteRecord(std::fstream& file, const std::streampos& pos);
};
#endif //LAB1_NOMS_INDEX_H
