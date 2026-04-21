//
// Created by Henrique on 21/04/2026.
//

#include "Audio.h"

#include <algorithm>

Audio::Audio() {}

Audio::~Audio() {
    music.stop();
}

bool Audio::loadMusic(const std::string& filePath) {
    if (music.openFromFile(filePath)) {
        music.setLoop(true);
        return true;
    }
    return false;
}

void Audio::setVolume(float volumePercent) {
    volumePercent = std::max(0.0f, std::min(volumePercent, 100.0f));
    music.setVolume(volumePercent);
}

float Audio::getVolume() const {
    return music.getVolume();
}

void Audio::play() {
    music.play();
}

void Audio::pause() {
    music.pause();
}

void Audio::stop() {
    music.stop();
}

void Audio::setLoop(bool loop) {
    music.setLoop(loop);
}

bool Audio::isPlaying() const {
    return music.getStatus() == sf::Music::Playing;
}