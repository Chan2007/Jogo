#include "animador_fundo.h"

#include <cstdio>

BackgroundAnimator::BackgroundAnimator() : currentSprite(NULL), nextSprite(NULL), currentFrame(0), frameAccumulator(0.0f), frameTime(1.0f / 60.0f), frameSize() {
}

BackgroundAnimator::~BackgroundAnimator() {
    delete currentSprite;
    delete nextSprite;
}

void BackgroundAnimator::updateSpriteScale() {
    if (currentSprite != NULL) {
        currentSprite->setScale(1920.0f / frameSize.x, 1080.0f / frameSize.y);
    }

    if (nextSprite != NULL) {
        nextSprite->setScale(1920.0f / frameSize.x, 1080.0f / frameSize.y);
    }
}

void BackgroundAnimator::updateBlend(float blend) {
    if (currentSprite == NULL || nextSprite == NULL || frames.empty()) {
        return;
    }

    if (blend < 0.0f) {
        blend = 0.0f;
    } else if (blend > 1.0f) {
        blend = 1.0f;
    }

    const size_t nextFrameIndex = (currentFrame + 1) % frames.size();
    currentSprite->setTexture(frames[currentFrame], true);
    nextSprite->setTexture(frames[nextFrameIndex], true);

    // Avoid blending the last frame into the first one, which causes a visible flash.
    currentSprite->setColor(sf::Color(255, 255, 255, 255));
    nextSprite->setColor(sf::Color(255, 255, 255, 0));
}

bool BackgroundAnimator::loadFrames(const std::string& pathPrefix, int numFrames, int startIndex, int frameStep) {
    framePaths.clear();
    frames.clear();
    delete currentSprite;
    delete nextSprite;
    currentSprite = NULL;
    nextSprite = NULL;
    currentFrame = 0;
    frameAccumulator = 0.0f;

    if (numFrames <= 0 || startIndex < 0 || frameStep <= 0) {
        return false;
    }

    frameTime = static_cast<float>(frameStep) / 60.0f;

    const int endIndex = startIndex + numFrames;
    for (int i = startIndex; i < endIndex; i += frameStep) {
        char buf[64];
        sprintf_s(buf, "%sframe%04d.png", pathPrefix.c_str(), i);
        framePaths.push_back(buf);
    }

    std::vector<std::string>::const_iterator pathIt;
    for (pathIt = framePaths.begin(); pathIt != framePaths.end(); ++pathIt) {
        sf::Texture tex;
        if (!tex.loadFromFile(*pathIt)) {
            return false;
        }

        frames.push_back(tex);
        if (frames.size() == 1) {
            frameSize = tex.getSize();
        }
    }

    if (frames.empty()) {
        return false;
    }

    currentSprite = new sf::Sprite(frames[0]);
    nextSprite = new sf::Sprite(frames[0]);
    updateSpriteScale();
    updateBlend(0.0f);
    clock.restart();
    return true;
}

void BackgroundAnimator::update() {
    if (currentSprite == NULL || nextSprite == NULL || frames.empty()) {
        return;
    }

    if (frames.size() == 1) {
        updateBlend(0.0f);
        return;
    }

    frameAccumulator += clock.restart().asSeconds();

    while (frameAccumulator >= frameTime) {
        currentFrame = (currentFrame + 1) % frames.size();
        frameAccumulator -= frameTime;
    }

    updateBlend(frameAccumulator / frameTime);
}

void BackgroundAnimator::draw(sf::RenderWindow& window) {
    if (currentSprite != NULL && nextSprite != NULL) {
        window.draw(*currentSprite);
        window.draw(*nextSprite);
    }
}

void BackgroundAnimator::setPosition(const sf::Vector2f& pos) {
    if (currentSprite != NULL) {
        currentSprite->setPosition(pos);
    }

    if (nextSprite != NULL) {
        nextSprite->setPosition(pos);
    }
}
