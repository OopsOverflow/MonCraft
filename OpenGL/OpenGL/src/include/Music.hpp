#pragma once
#ifndef Music_HPP_INCLUDED
#define Music_HPP_INCLUDED

#include <SFML/Audio.hpp>
#include <vector>
#include <SFML/Audio/Music.hpp>


#define Music_ACTIVE

class Music
{
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

#endif // MusicPlayer_HPP_INCLUDED
