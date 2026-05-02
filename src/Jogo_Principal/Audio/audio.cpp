//
// Created by Henrique on 21/04/2026.
//

#include "audio.h"

#include <algorithm>

audio::audio() {}

audio::~audio() {
    music.stop();
}

bool audio::loadMusic(const std::string& filePath) {
    if (music.openFromFile(filePath)) {
        music.setLoop(true);
        return true;
    }
    return false;
}

void audio::setVolume(float volumePercent) {
    volumePercent = std::max(0.0f, std::min(volumePercent, 100.0f));
    music.setVolume(volumePercent);
}

float audio::getVolume() const {
    return music.getVolume();
}

void audio::play() {
    music.play();
}

void audio::pause() {
    music.pause();
}

void audio::stop() {
    music.stop();
}

void audio::setLoop(bool loop) {
    music.setLoop(loop);
}

bool audio::isPlaying() const {
    return music.getStatus() == sf::Music::Playing;
}