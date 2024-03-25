#include "../IndexTable/Index.h"

std::vector<uint32_t> Index::deletedAddresses;

uint32_t Index::getRecordAddress() {
    if (deletedAddresses.empty()) {
        uint32_t fileSize = IND_FILE.tellg();
        return fileSize / (sizeof(record));
    } else {
        uint32_t address = deletedAddresses.back(); // Get the last deleted address
        deletedAddresses.pop_back(); // Remove it from the list
        return address;
    }
}

bool Index::insertRecord() {
    if (!IND_FILE)
        return false;

    const std::streampos& pos = sizeof(record) * getRecordAddress();
    IND_FILE.seekp(pos);
    IND_FILE.write(reinterpret_cast<const char*>(&record), sizeof(record));
    IND_FILE.flush();

    return !IND_FILE.fail();
}

//bool Index::deleteRecord() {//сумнівно, але най буде
//    IND_FILE.seekg(0, std::ios::end);
//    int numRecords = IND_FILE.tellg() / sizeof(record);
//    IND_FILE.seekg(0, std::ios::beg);
//
//    for (int i = 0; i < numRecords; ++i) {
//        uint32_t currID;
//        IND_FILE.read(reinterpret_cast<char*>(&currID), sizeof(currID));
//        if (currID == record.first) {
//            std::pair<uint32_t, uint32_t> emptyRecord = {0, 0};
//            IND_FILE.seekp(i * sizeof(record));
//            IND_FILE.write(reinterpret_cast<const char*>(&emptyRecord), sizeof(record));
//            IND_FILE.flush();
//
//            return !IND_FILE.fail();
//        }
//        IND_FILE.seekg((i + 1) * sizeof(record));
//    }
//    return false;
//}
bool Index::deleteRecord() {
    if (!IND_FILE)
        return false;

    IND_FILE.seekg(0, std::ios::beg);
    std::pair<uint32_t, uint32_t> currRecord;
    int64_t currRecordAddress = -1;
    int64_t prevRecordAddress = -1;

    while (IND_FILE.read(reinterpret_cast<char*>(&currRecord), sizeof(currRecord))) {
        if (currRecord == record) {
            // Record found, now delete it
            if (prevRecordAddress != -1) {
                // Not the first record, update previous record's next pointer
                IND_FILE.seekp(prevRecordAddress * sizeof(record));
                IND_FILE.write(reinterpret_cast<const char*>(&currRecordAddress), sizeof(currRecordAddress));
            } else {
                // First record, update the deletedAddresses vector
                deletedAddresses.push_back(0);
            }

            // Remove the record from the file (mark it as deleted)
            IND_FILE.seekp(currRecordAddress * sizeof(record));
            std::pair<uint32_t, uint32_t> emptyRecord = {0, 0};
            IND_FILE.write(reinterpret_cast<const char*>(&emptyRecord), sizeof(record));

            IND_FILE.flush();
            return !IND_FILE.fail();
        }

        prevRecordAddress = currRecordAddress;
        currRecordAddress = IND_FILE.tellg() / sizeof(record);
    }

    return false; // Record not found
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
