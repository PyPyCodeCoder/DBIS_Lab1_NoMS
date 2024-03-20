#ifndef LAB1_NOMS_INDEX_H
#define LAB1_NOMS_INDEX_H
#include <cstdint>
#include <utility>
#include <fstream>
#include <vector>
#include <algorithm>
//get file and positions?
class Index {
private:
    std::pair<uint32_t, uint32_t> record;

public:
    Index(uint32_t id, uint32_t address) : record(id, address) {}

    bool insertRecord(std::fstream& file, const std::streampos& pos);
    bool deleteRecord(std::fstream& file);
    std::pair<uint32_t, uint32_t> getRecordAt(std::fstream& file, uint32_t index);
    void sortRecords(std::fstream& file);
};

#endif //LAB1_NOMS_INDEX_H
