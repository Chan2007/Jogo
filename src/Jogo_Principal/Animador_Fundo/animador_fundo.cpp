#include "animador_fundo.h"
#include "Jogo_Principal/Diretorio/Encontrar_Diretorio.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>

Animador_Fundo::Animador_Fundo() :
    currentSprite(),
    nextSprite(),
    currentTexture(),
    nextTexture(),
    frames(),
    currentspSheetPath(),
    nextspSheetPath(),
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

    // A sobreposicao de quadros adjacentes fez com que as regioes brilhantes "cintilassem" no olho nu.
    // Para uma animacao de fundo estavel, apenas o quadro ativo aparece ("blend").
    (void) blend;
    currentSprite.setColor(sf::Color(255, 255, 255, 255));
    nextSprite.setColor(sf::Color(255, 255, 255, 0));
}

bool Animador_Fundo::loadFrames(const std::string& pathPrefix, int numFrames, int startIndex, int frameStep) {
    const int frameWidth = 960;
    const int frameHeight = 540;

    frames.clear();
    currentspSheetPath.clear();
    nextspSheetPath.clear();
    currentFrameIndex = 0;
    frameAccumulator = 0.0f;
    frameSize = sf::Vector2u();
    loaded = false;

    if (numFrames <= 0 || startIndex <= 0 || frameStep <= 0) return false;

    frameTime = static_cast<float>(frameStep) / 60.0f;

    std::string sep = "";
    Encontrar_Diretorio diretorio;
    if (!pathPrefix.empty()) {
        char last = pathPrefix[pathPrefix.size() - 1];
        if (last != '/' && last != '\\') {
            if (pathPrefix.find('\\') != std::string::npos) sep = "\\";
            else sep = "/";
        }
    }

    const std::string path = diretorio.concatenarEnderecos(pathPrefix, sep);
    const int endIndex = startIndex + numFrames;
    for (int i = startIndex; i < endIndex; i += frameStep) {
        const int indexFrame = i - 1;
        const int frames_SprSheet = 12;
        const int colunas = 4;

        const int spSheetIndex = (indexFrame / frames_SprSheet) + 1;
        const int frameIndex_spSheet = indexFrame % frames_SprSheet;
        const int coluna = frameIndex_spSheet % colunas;
        const int linha = frameIndex_spSheet / colunas;

        FrameData frame;
        char buffer[32];
        std::sprintf(buffer, "bg_menu%d.png", spSheetIndex);
        frame.spSheetPath = diretorio.concatenarEnderecos(path, buffer);
        frame.textureRect = sf::IntRect(coluna * frameWidth,linha * frameHeight, frameWidth, frameHeight);
        frames.push_back(frame);
    }

    if (frames.empty()) return false;
    if (!applyFrame(currentSprite, currentTexture, currentspSheetPath, frames[0])) return false;

    frameSize = sf::Vector2u(frameWidth, frameHeight);

    if (frames.size() > 1) {
        if (!applyFrame(nextSprite, nextTexture, nextspSheetPath, frames[1])) return false;
    }
    else {
        if (!applyFrame(nextSprite, nextTexture, nextspSheetPath, frames[0])) return false;
    }

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

        std::swap(currentTexture, nextTexture);
        std::swap(currentspSheetPath, nextspSheetPath);
        currentSprite.setTexture(currentTexture, true);
        currentSprite.setTextureRect(frames[currentFrameIndex].textureRect);

        const size_t nextFrameIndex = (currentFrameIndex + 1) % frames.size();
        if (!applyFrame(nextSprite, nextTexture, nextspSheetPath, frames[nextFrameIndex])) {
            loaded = false;
            return;
        }
    }

    updateBlend(frameAccumulator / frameTime);
}

void Animador_Fundo::draw(sf::RenderWindow& window) const{
    if (!loaded) return;
    window.draw(currentSprite);
    window.draw(nextSprite);
}

void Animador_Fundo::setPosicao(const sf::Vector2f& pos) {
    if (!loaded) return;
    currentSprite.setPosition(pos);
    nextSprite.setPosition(pos);
}

void Animador_Fundo::setTargetSize(const sf::Vector2u& size) {
    targetSize = size;
}

bool Animador_Fundo::loadTextureFromPath(sf::Texture& texture, std::string& loadedPath, const std::string& path) {
    if (loadedPath == path) return true;
    if (!texture.loadFromFile(path)) return false;
    loadedPath = path;
    return true;
}

bool Animador_Fundo::applyFrame(sf::Sprite& sprite, sf::Texture& texture, std::string& loadedPath, const FrameData& frame) {
    if (!loadTextureFromPath(texture, loadedPath, frame.spSheetPath)) return false;
    sprite.setTexture(texture, true);
    sprite.setTextureRect(frame.textureRect);
    return true;
}
