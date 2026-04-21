#include "animador_fundo.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

BackgroundAnimator::BackgroundAnimator() :
    currentSprite(),
    nextSprite(),
    currentFrame(0),
    frameAccumulator(0.0f),
    frameTime(1.0f / 60.0f),
    frameSize(),
    targetSize(1920, 1080),
    loaded(false) {
}

BackgroundAnimator::~BackgroundAnimator() {
}

void BackgroundAnimator::updateSpriteScale() {
    if (!loaded || frameSize.x == 0 || frameSize.y == 0 || targetSize.x == 0 || targetSize.y == 0) {
        return;
    }

    const float scaleX = static_cast<float>(targetSize.x) / static_cast<float>(frameSize.x);
    const float scaleY = static_cast<float>(targetSize.y) / static_cast<float>(frameSize.y);
    currentSprite.setScale(scaleX, scaleY);
    nextSprite.setScale(scaleX, scaleY);
}

void BackgroundAnimator::updateBlend(float blend) {
    if (!loaded || frames.empty()) {
        return;
    }

    blend = std::max(0.0f, std::min(blend, 1.0f));

    const size_t nextFrameIndex = (currentFrame + 1) % frames.size();

    currentSprite.setTexture(frames[currentFrame], true);
    nextSprite.setTexture(frames[nextFrameIndex], true);

    // A sobreposição de quadros adjacentes fez com que as regiões brilhantes "cintilassem" no olho nu.
    // Para uma animação de fundo estável, apenas o quadro ativo aparece ("blend").
    (void)blend;
    currentSprite.setColor(sf::Color(255, 255, 255, 255));
    nextSprite.setColor(sf::Color(255, 255, 255, 0));
}

bool BackgroundAnimator::loadFrames(const std::string& pathPrefix, int numFrames, int startIndex, int frameStep) {
    framePaths.clear();
    frames.clear();
    currentFrame = 0;
    frameAccumulator = 0.0f;
    frameSize = sf::Vector2u();
    loaded = false;

    if (numFrames <= 0 || startIndex < 0 || frameStep <= 0) {
        return false;
    }

    frameTime = static_cast<float>(frameStep) / 60.0f;

    const int endIndex = startIndex + numFrames;
    for (int i = startIndex; i < endIndex; i += frameStep) {
        std::ostringstream pathBuilder;
        pathBuilder << pathPrefix << "frame" << std::setw(4) << std::setfill('0') << i << ".png";
        framePaths.push_back(pathBuilder.str());
    }

    std::vector<std::string>::const_iterator pathIt;
    for (pathIt = framePaths.begin(); pathIt != framePaths.end(); ++pathIt) {
        sf::Texture tex;
        if (!tex.loadFromFile(*pathIt)) {
            return false;
        }

        if (frames.empty()) {
            frameSize = tex.getSize();
        }

        frames.push_back(tex);
    }

    if (frames.empty()) {
        return false;
    }

    currentSprite.setTexture(frames[0], true);
    nextSprite.setTexture(frames[0], true);
    loaded = true;
    updateSpriteScale();
    updateBlend(0.0f);
    clock.restart();
    return true;
}

void BackgroundAnimator::update() {
    if (!loaded || frames.empty()) {
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
    if (!loaded) {
        return;
    }

    window.draw(currentSprite);
    window.draw(nextSprite);
}

void BackgroundAnimator::setPosition(const sf::Vector2f& pos) {
    if (!loaded) {
        return;
    }

    currentSprite.setPosition(pos);
    nextSprite.setPosition(pos);
}

void BackgroundAnimator::setTargetSize(const sf::Vector2u& size) {
    targetSize = size;
    updateSpriteScale();
}
