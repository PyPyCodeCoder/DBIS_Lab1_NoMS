#include "../IndexTable/Index.h"

bool Index::insertRecord(std::fstream& file, const std::streampos& pos) {
    if (!file)
        return false;

    file.seekp(pos);
    file.write(reinterpret_cast<const char*>(this), sizeof(Index));
    file.flush();

    return !file.fail();
}

bool Index::deleteRecord(std::fstream& file, const std::streampos& pos) {

}