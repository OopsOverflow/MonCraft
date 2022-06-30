#pragma once

#include <SFML/Audio/Music.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <stdint.h>
#include <string>
#include <vector>


#define Music_ACTIVE

class Music {
public:
    Music();

    void update();
    void playNext();
private:
    void getNextSong();

    sf::Music music;
    sf::Time  duration;
    sf::Clock timer;

    std::vector<std::string> playlist;
    uint32_t currentSong = 0;
};
