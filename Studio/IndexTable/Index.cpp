#include "../IndexTable/Index.h"

//std::vector<uint32_t> Index::deletedAddresses;
//
//uint32_t Index::getRecordAddress() {
//    if (deletedAddresses.empty()) {
//        uint32_t fileSize = IND_FILE.tellg();
//        return fileSize / (sizeof(record));
//    } else {
//        uint32_t address = deletedAddresses.back(); // Get the last deleted address
//        deletedAddresses.pop_back(); // Remove it from the list
//        return address;
//    }
//}

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

//std::pair<uint32_t, uint32_t> Index::getRecordAt(uint32_t index) {
//    IND_FILE.seekg(index * sizeof(record));
//    uint32_t id, studioAddress;
//    IND_FILE.read(reinterpret_cast<char*>(&id), sizeof(id));
//    IND_FILE.read(reinterpret_cast<char*>(&studioAddress), sizeof(studioAddress));
//    return std::make_pair(id, studioAddress);
//}

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

bool Index::deleteRecord() {
    if (!IND_FILE)
        return false;

    // Find the record with the given studioId
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
        // Record not found
        return false;
    }

    // Mark the record as deleted
    //uint32_t deletedAddress = recordAddress / sizeof(record);
    //deletedAddresses.push_back(deletedAddress);

    // Move to the record's address and overwrite it with zeros
    IND_FILE.seekp(recordAddress);
    uint32_t zeroValue = 0;
    IND_FILE.write(reinterpret_cast<const char*>(&zeroValue), sizeof(zeroValue));
    IND_FILE.write(reinterpret_cast<const char*>(&zeroValue), sizeof(zeroValue));
    IND_FILE.flush();

    return !IND_FILE.fail();
}