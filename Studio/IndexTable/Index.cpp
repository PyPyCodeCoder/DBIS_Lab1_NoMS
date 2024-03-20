#include "../IndexTable/Index.h"

bool Index::insertRecord(const std::streampos& pos) {
    if (!IND_FILE)
        return false;

    IND_FILE.seekp(pos);
    IND_FILE.write(reinterpret_cast<const char*>(&record), sizeof(record));
    IND_FILE.flush();

    return !IND_FILE.fail();
}

bool Index::deleteRecord() {//сумнівно, але най буде
    IND_FILE.seekg(0, std::ios::end);
    int numRecords = IND_FILE.tellg() / sizeof(record);
    IND_FILE.seekg(0, std::ios::beg);

    for (int i = 0; i < numRecords; ++i) {
        uint32_t currID;
        IND_FILE.read(reinterpret_cast<char*>(&currID), sizeof(currID));
        if (currID == record.first) {
            std::pair<uint32_t, uint32_t> emptyRecord = {0, 0};
            IND_FILE.seekp(i * sizeof(record));
            IND_FILE.write(reinterpret_cast<const char*>(&emptyRecord), sizeof(record));
            IND_FILE.flush();

            return !IND_FILE.fail();
        }
        IND_FILE.seekg((i + 1) * sizeof(record));
    }
    return false;
}

std::pair<uint32_t, uint32_t> Index::getRecordAt(uint32_t index) {
    IND_FILE.seekg(index * sizeof(record));
    uint32_t id, studioAddress;
    IND_FILE.read(reinterpret_cast<char*>(&id), sizeof(id));
    IND_FILE.read(reinterpret_cast<char*>(&studioAddress), sizeof(studioAddress));
    return std::make_pair(id, studioAddress);
}

void Index::sortRecords() {
    IND_FILE.seekg(0, std::ios::end);
    int numRecords = IND_FILE.tellg() / sizeof(record);
    IND_FILE.seekg(0, std::ios::beg);

    std::vector<std::pair<uint32_t, uint32_t>> records;
    for (int i = 0; i < numRecords; ++i) {
        uint32_t id, studioAddress;
        IND_FILE.read(reinterpret_cast<char*>(&id), sizeof(id));
        IND_FILE.read(reinterpret_cast<char*>(&studioAddress), sizeof(studioAddress));
        records.push_back(std::make_pair(id, studioAddress));
    }

    std::sort(records.begin(), records.end());

    IND_FILE.clear();
    IND_FILE.seekg(0);
    for (const auto& rec : records) {
        IND_FILE.write(reinterpret_cast<const char*>(&rec.first), sizeof(rec.first));
        IND_FILE.write(reinterpret_cast<const char*>(&rec.second), sizeof(rec.second));
    }
}