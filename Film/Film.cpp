#include "../Film/Film.h"

bool Film::insert() {
    if (!FILM_FILE || !STUDIO_FILE)
        return false;

    // Find the studio record
    STUDIO_FILE.seekg(0, std::ios::beg); // Move to the beginning of the file
    uint32_t studioAddress = -1;
    Studio studio;

    while (STUDIO_FILE.read(reinterpret_cast<char*>(&studio), sizeof(Studio))) {
        if (studio.getId() == studio_id) {
            studioAddress = studio.getStudioAddress();
            break;
        }
    }

    if (studioAddress == -1) {
        // Studio not found
        return false;
    }

    // Get the films address
    uint32_t filmsAddress;
    STUDIO_FILE.seekg(sizeof(Studio) * studioAddress);
    STUDIO_FILE.read(reinterpret_cast<char*>(&filmsAddress), sizeof(filmsAddress));

    // Append the new film record to the end of the FILM_FILE
    FILM_FILE.seekp(0, std::ios::end);
    uint32_t filmAddress = FILM_FILE.tellp() / sizeof(Film);
    FILM_FILE.write(reinterpret_cast<const char*>(this), sizeof(Film));
    FILM_FILE.flush();

    // Update the films address in the studio record
    STUDIO_FILE.seekp(sizeof(Studio) * studioAddress);
    STUDIO_FILE.write(reinterpret_cast<const char*>(&filmAddress), sizeof(filmsAddress));
    STUDIO_FILE.flush();

    return !FILM_FILE.fail() && !STUDIO_FILE.fail();
}