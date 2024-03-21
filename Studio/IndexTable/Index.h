#ifndef LAB1_NOMS_INDEX_H
#define LAB1_NOMS_INDEX_H
#include <cstdint>
#include <utility>
#include <fstream>
#include <vector>
#include <algorithm>
#include "../../Helpers/helpers.h"

class Index {
private:
    std::pair<uint32_t, uint32_t> record;
    uint32_t RecordAddress = 0;

    static std::vector<uint32_t> deletedAddresses;
public:
    Index(uint32_t StudioId, uint32_t StudioAddress) : record(StudioId, StudioAddress) {}

    uint32_t getRecordAddress();
    bool insertRecord();
    bool deleteRecord();
    std::pair<uint32_t, uint32_t> getRecordAt(uint32_t index);
    void sortRecords();
};

#endif //LAB1_NOMS_INDEX_H
