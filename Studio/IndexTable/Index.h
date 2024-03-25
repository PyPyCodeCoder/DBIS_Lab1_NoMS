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

    uint32_t RecordAddress;
    static std::vector<uint32_t> deletedAddresses;
public:
    Index(uint32_t StudioId, uint32_t StudioAddress) : record(StudioId, StudioAddress) {}

    uint32_t getRecordAddress();
    bool insertRecord();
    bool deleteRecord();
    void sortRecords();
    std::pair<uint32_t, uint32_t> getRecordAt(uint32_t index);//не треба

    static void updateDeletedAddresses(uint32_t address) {
        deletedAddresses.push_back(address);
    }
};

#endif //LAB1_NOMS_INDEX_H
