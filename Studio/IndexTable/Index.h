#ifndef LAB1_NOMS_INDEX_H
#define LAB1_NOMS_INDEX_H
#include <cstdint>
#include <utility>
#include <fstream>
#include <vector>
#include <algorithm>
#include "../../Helpers/helpers.h"

//get positions
class Index {
private:
    std::pair<uint32_t, uint32_t> record;

public:
    Index(uint32_t id, uint32_t address) : record(id, address) {}

    bool insertRecord(const std::streampos& pos);
    bool deleteRecord();
    std::pair<uint32_t, uint32_t> getRecordAt(uint32_t index);
    void sortRecords();
};

#endif //LAB1_NOMS_INDEX_H
