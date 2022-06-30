#include "Music.hpp"

#include <vector>
#include <string>

#include "util/Random.hpp"
#include "save/ClientConfig.hpp"


/**For now music has to be added manually unforetunetly
* because of C++ 14, unsing <filesystem> (not supported until C++17)
* causes crashes
* */

std::vector<std::string> songList =
{
     "ES_Reconstruct-AmaranthCove.ogg",
     "ES_Mays-VanSandano.ogg",
     "ES_ANewSleep-ArdenForest.ogg",
     "FreeMinecraftMusicHappyBackground.ogg"
     //,"dev.ogg" insane music to implement
};

Music::Music()
    : playlist(songList)
    , currentSong(0)
{
    auto config = Config::getClientConfig();
    music.setVolume(config.mainVolume * config.musicVolume * 0.01f);
    getNextSong();
}

void Music::update()
{
    if (duration <= timer.getElapsedTime())
    {
        uint32_t song;
        do
        {
            song = (uint32_t)Random::intInRange(0, playlist.size() - 1);

        } while (currentSong == song);

        currentSong = song;
        getNextSong();
    }
}

// For controller imp.
void Music::playNext()
{
    if (currentSong == playlist.size() - 1)
        currentSong = 0;
    else currentSong += 1;
    getNextSong();
}

void Music::getNextSong()
{
    if (playlist.empty()) return;
    std::string songname = "data/music/" + playlist[currentSong];
    music.openFromFile(songname);
    music.stop();

    duration = music.getDuration() + sf::seconds(1);
    timer.restart();
    music.play();
}
