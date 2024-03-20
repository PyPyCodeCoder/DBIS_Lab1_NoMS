#include "../IndexTable/Index.h"

bool Index::insertRecord(std::fstream& file, const std::streampos& pos) {
    if (!file)
        return false;

    file.seekp(pos);
    file.write(reinterpret_cast<const char*>(&record), sizeof(record));
    file.flush();

    return !file.fail();
}

bool Index::deleteRecord(std::fstream& file) {//сумнівно, але най буде
    file.seekg(0, std::ios::end);
    int numRecords = file.tellg() / sizeof(record);
    file.seekg(0, std::ios::beg);

    for (int i = 0; i < numRecords; ++i) {
        uint32_t currID;
        file.read(reinterpret_cast<char*>(&currID), sizeof(currID));
        if (currID == record.first) {
            std::pair<uint32_t, uint32_t> emptyRecord = {0, 0};
            file.seekp(i * sizeof(record));
            file.write(reinterpret_cast<const char*>(&emptyRecord), sizeof(record));
            file.flush();

            return !file.fail();
        }
        file.seekg((i + 1) * sizeof(record));
    }
    return false;
}

std::pair<uint32_t, uint32_t> Index::getRecordAt(std::fstream& file, uint32_t index) {
    file.seekg(index * sizeof(record));
    uint32_t id, studioAddress;
    file.read(reinterpret_cast<char*>(&id), sizeof(id));
    file.read(reinterpret_cast<char*>(&studioAddress), sizeof(studioAddress));
    return std::make_pair(id, studioAddress);
}

void Index::sortRecords(std::fstream& file) {
    file.seekg(0, std::ios::end);
    int numRecords = file.tellg() / sizeof(record);
    file.seekg(0, std::ios::beg);

    std::vector<std::pair<uint32_t, uint32_t>> records;
    for (int i = 0; i < numRecords; ++i) {
        uint32_t id, studioAddress;
        file.read(reinterpret_cast<char*>(&id), sizeof(id));
        file.read(reinterpret_cast<char*>(&studioAddress), sizeof(studioAddress));
        records.push_back(std::make_pair(id, studioAddress));
    }

    std::sort(records.begin(), records.end());

    file.clear();
    file.seekg(0);
    for (const auto& rec : records) {
        file.write(reinterpret_cast<const char*>(&rec.first), sizeof(rec.first));
        file.write(reinterpret_cast<const char*>(&rec.second), sizeof(rec.second));
    }
}