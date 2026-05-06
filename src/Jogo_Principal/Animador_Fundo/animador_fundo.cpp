#include "animador_fundo.h"

#include <iomanip>
#include <sstream>
#include <cstdlib>

Animador_Fundo::Animador_Fundo() :
    currentSprite(),
    nextSprite(),
    currentFrameIndex(0),
    frameAccumulator(0.0f),
    frameTime(1.0f / 60.0f),
    frameSize(),
    targetSize(1920, 1080),
    loaded(false) {
}

Animador_Fundo::~Animador_Fundo() {}

void Animador_Fundo::updateSpriteScale() {
    if (!loaded || frameSize.x == 0 || frameSize.y == 0 || targetSize.x == 0 || targetSize.y == 0)
        return;
    const float scaleX = static_cast<float>(targetSize.x) / static_cast<float>(frameSize.x);
    const float scaleY = static_cast<float>(targetSize.y) / static_cast<float>(frameSize.y);
    currentSprite.setScale(scaleX, scaleY);
    nextSprite.setScale(scaleX, scaleY);
}

void Animador_Fundo::updateBlend(float blend) {
    if (!loaded || frames.empty()) return;
    blend = std::max(0.0f, std::min(blend, 1.0f));
    const size_t nextFrameIndex = (currentFrameIndex + 1) % frames.size();

    currentSprite.setTexture(frames[currentFrameIndex], true);
    nextSprite.setTexture(frames[nextFrameIndex], true);

    // A sobreposição de quadros adjacentes fez com que as regiões brilhantes "cintilassem" no olho nu.
    // Para uma animação de fundo estável, apenas o quadro ativo aparece ("blend").
    (void) blend;
    currentSprite.setColor(sf::Color(255, 255, 255, 255));
    nextSprite.setColor(sf::Color(255, 255, 255, 0));
    // currentSprite.setColor(sf::Color(255, 255, 255, blend));
    // nextSprite.setColor(sf::Color(255, 255, 255, 255 - blend));
}

bool Animador_Fundo::loadFrames(const std::string& pathPrefix, int numFrames, int startIndex, int frameStep) {
    framePaths.clear();
    frames.clear();
    currentFrameIndex = 0;
    frameAccumulator = 0.0f;
    frameSize = sf::Vector2u();
    loaded = false;
    if (numFrames <= 0 || startIndex < 0 || frameStep <= 0) return false;
    frameTime = static_cast<float>(frameStep) / 60.0f;
    const int endIndex = startIndex + numFrames;
    for (int i = startIndex; i < endIndex; i += frameStep) {
        std::ostringstream pathBuilder;
        pathBuilder << pathPrefix << "frame" << std::setw(4) << std::setfill('0') << i << ".png";
        framePaths.push_back(pathBuilder.str());
    }
    std::vector<std::string>::const_iterator it;
    for (it = framePaths.begin(); it != framePaths.end(); it++) {
        sf::Texture tex;
        if (!tex.loadFromFile(*it)) return false;
        if (frames.empty()) frameSize = tex.getSize();
        frames.push_back(tex);
    }
    if (frames.empty()) return false;
    currentSprite.setTexture(frames[0], true);
    nextSprite.setTexture(frames[0], true);
    loaded = true;
    updateSpriteScale();
    updateBlend(static_cast<float>(rand()) / RAND_MAX);
    clock.restart();
    return true;
}

void Animador_Fundo::update() {
    if (!loaded || frames.empty()) return;
    if (frames.size() == 1) {
        updateBlend(0.0f);
        return;
    }
    frameAccumulator += clock.restart().asSeconds();
    while (frameAccumulator >= frameTime) {
        currentFrameIndex = (currentFrameIndex + 1) % frames.size();
        frameAccumulator -= frameTime;
    }
    updateBlend(frameAccumulator / frameTime);
}

void Animador_Fundo::draw(sf::RenderWindow& window) {
    if (!loaded) return;
    window.draw(currentSprite);
    window.draw(nextSprite);
}

void Animador_Fundo::setPosition(const sf::Vector2f& pos) {
    if (!loaded) return;
    currentSprite.setPosition(pos);
    nextSprite.setPosition(pos);
}

void Animador_Fundo::setTargetSize(const sf::Vector2u& size) {
    targetSize = size;
    updateSpriteScale();
}
