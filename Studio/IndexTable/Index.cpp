#include "../IndexTable/Index.h"

bool Index::insertRecord() {
    if (!IND_FILE)
        return false;

    IND_FILE.seekg(0, std::ios::end);
    uint32_t numRecords = IND_FILE.tellg() / sizeof(record);
    IND_FILE.seekg(0, std::ios::beg);

    std::vector<std::pair<uint32_t, uint32_t>> records(numRecords);
    IND_FILE.read(reinterpret_cast<char*>(records.data()), numRecords * sizeof(record));

    auto it = std::upper_bound(records.begin(), records.end(), record);
    int64_t insertPos = it - records.begin();

    IND_FILE.seekp(insertPos * sizeof(record));
    IND_FILE.write(reinterpret_cast<const char*>(&record), sizeof(record));
    IND_FILE.flush();

    return !IND_FILE.fail();
}

bool Index::deleteRecord() {
    if (!IND_FILE)
        return false;

    IND_FILE.seekg(0, std::ios::beg);
    uint32_t id, studioAddress;
    int64_t recordAddress = -1;
    while (IND_FILE.read(reinterpret_cast<char*>(&id), sizeof(id))) {
        IND_FILE.read(reinterpret_cast<char*>(&studioAddress), sizeof(studioAddress));
        if (id == record.first) {
            recordAddress = IND_FILE.tellg() - static_cast<std::streamoff>(sizeof(id) + sizeof(studioAddress));
            break;
        }
    }

    if (recordAddress == -1) {
        return false;
    }

    IND_FILE.seekp(recordAddress);
    uint32_t zeroValue = 0;
    IND_FILE.write(reinterpret_cast<const char*>(&zeroValue), sizeof(zeroValue));
    IND_FILE.write(reinterpret_cast<const char*>(&zeroValue), sizeof(zeroValue));
    IND_FILE.flush();

    return !IND_FILE.fail();
}