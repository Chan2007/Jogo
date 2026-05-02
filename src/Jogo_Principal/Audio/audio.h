//
// Created by Henrique on 21/04/2026.
//

#ifndef JOGO_AUDIO_H
#define JOGO_AUDIO_H

#include <SFML/audio.hpp>
#include <string>

class audio {
    public:
        audio();
        ~audio();
        bool loadMusic(const std::string& filePath);
        void setVolume(float volumePercent);
        float getVolume() const;
        void play();
        void pause();
        void stop();
        void setLoop(bool loop);
        bool isPlaying() const;

    private:
        sf::Music music;
};

#endif //JOGO_AUDIO_H