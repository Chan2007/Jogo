#include "animador_fundo.h"
#include "Jogo_Principal/Diretorio/Encontrar_Diretorio.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>

Animador_Fundo::FrameData::FrameData() :
    caminho_spSheet(),
    RectTextura(),
    textura(0) {
}

Animador_Fundo::Animador_Fundo(Gerenciador_Textura* gerenciadorTextura) :
    SpriteAtual(),
    ProxSprite(),
    frames_data(),
    gerenciadorTextura(gerenciadorTextura),
    FrameIndexAtual(0),
    frameAccumulator(0.0f),
    frameTime(1.0f / 60.0f),
    frameSize(),
    targetSize(),
    posicaoBase(0.f, 0.f),
    loaded(false)
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    targetSize.x = desktop.width;
    targetSize.y = desktop.height;
}

Animador_Fundo::~Animador_Fundo() {}

sf::Texture* Animador_Fundo::buscarTextura(const std::string& path) const {
    if (!gerenciadorTextura) return 0;
    sf::Texture* tex = gerenciadorTextura->buscar_textura(path);

    // "Lazy Loading" feita por padrão Proxy
    if (tex == NULL || (tex->getSize().x == 0 && tex->getSize().y == 0)) {
        if (gerenciadorTextura->carregar_textura(path))
            tex = gerenciadorTextura->buscar_textura(path);
    }
    return tex;
}

void Animador_Fundo::updateSpriteScale() {
    if (!loaded || frameSize.x == 0 || frameSize.y == 0 || targetSize.x == 0 || targetSize.y == 0)
        return;
    const float scaleX = static_cast<float>(targetSize.x) / static_cast<float>(frameSize.x);
    const float scaleY = static_cast<float>(targetSize.y) / static_cast<float>(frameSize.y);
    const float scale = std::max(scaleX, scaleY);
    const float larguraEscalada = frameSize.x * scale;
    const float alturaEscalada = frameSize.y * scale;
    const float offsetX = (targetSize.x - larguraEscalada) * 0.5f;
    const float offsetY = (targetSize.y - alturaEscalada) * 0.5f;

    SpriteAtual.setScale(scale, scale);
    ProxSprite.setScale(scale, scale);
    SpriteAtual.setPosition(posicaoBase.x + offsetX, posicaoBase.y + offsetY);
    ProxSprite.setPosition(posicaoBase.x + offsetX, posicaoBase.y + offsetY);
}

void Animador_Fundo::updateBlend(float blend) {
    if (!loaded || frames_data.empty()) return;
    blend = std::max(0.0f, std::min(blend, 1.0f));

    // A sobreposicao de quadros adjacentes fez com que as regioes brilhantes "cintilassem" no olho nu.
    // Para uma animacao de fundo estavel, apenas o quadro ativo aparece ("blend").
    (void) blend;
    SpriteAtual.setColor(sf::Color(255, 255, 255, 255));
    ProxSprite.setColor(sf::Color(255, 255, 255, 0));
}

bool Animador_Fundo::loadFrames(const std::string& pathPrefix, int numFrames, int startIndex, int frameStep,
                                int colunas, int linhas) {
    frames_data.clear();
    if (!gerenciadorTextura) return false;

    gerenciadorTextura->limpar_textura();
    FrameIndexAtual = 0;
    frameAccumulator = 0.0f;
    frameSize = sf::Vector2u();
    loaded = false;

    if (numFrames <= 0 || startIndex <= 0 || frameStep <= 0 || colunas <= 0 || linhas <= 0) return false;

    frameTime = static_cast<float>(frameStep) / 60.0f;

    const std::string path = Encontrar_Diretorio::concatenarEnderecos(pathPrefix, "/");
    char buffer[32];
    std::sprintf(buffer, "bg_menu%d.png", 1);
    const std::string primeiroSheet = Encontrar_Diretorio::concatenarEnderecos(path, buffer);
    sf::Texture* texturaBase = buscarTextura(primeiroSheet);
    if (texturaBase == 0) return false;

    const sf::Vector2u tamanhoSheet = texturaBase->getSize();
    if (tamanhoSheet.x == 0 || tamanhoSheet.y == 0) return false;
    if ((tamanhoSheet.x % static_cast<unsigned int>(colunas)) != 0 ||
        (tamanhoSheet.y % static_cast<unsigned int>(linhas)) != 0) return false;

    const int frameWidth = static_cast<int>(tamanhoSheet.x / colunas);
    const int frameHeight = static_cast<int>(tamanhoSheet.y / linhas);
    const int frames_SprSheet = colunas * linhas;

    const int endIndex = startIndex + numFrames;
    for (int i = startIndex; i < endIndex; i += frameStep) {
        const int indexFrame = i - 1;
        const int spSheetIndex = (indexFrame / frames_SprSheet) + 1;
        const int frameIndex_spSheet = indexFrame % frames_SprSheet;
        const int coluna = frameIndex_spSheet / linhas;
        const int linha = frameIndex_spSheet % linhas;

        FrameData frame;
        std::sprintf(buffer, "bg_menu%d.png", spSheetIndex);
        frame.caminho_spSheet = Encontrar_Diretorio::concatenarEnderecos(path, buffer);
        frame.RectTextura = sf::IntRect(coluna * frameWidth,linha * frameHeight, frameWidth, frameHeight);
        frame.textura = 0;
        frames_data.push_back(frame);
    }

    if (frames_data.empty()) return false;
    if (!cacheFrameTextures()) return false;
    if (!applyFrame(SpriteAtual, frames_data[0])) return false;

    frameSize = sf::Vector2u(frameWidth, frameHeight);

    if (frames_data.size() > 1) {
        if (!applyFrame(ProxSprite, frames_data[1])) return false;
    }
    else {
        if (!applyFrame(ProxSprite, frames_data[0])) return false;
    }

    loaded = true;
    updateSpriteScale();
    updateBlend(static_cast<float>(rand()) / RAND_MAX);
    clock.restart();
    return true;
}

void Animador_Fundo::update() {
    if (!loaded || frames_data.empty()) return;
    if (frames_data.size() == 1) {
        updateBlend(0.0f);
        return;
    }

    frameAccumulator += clock.restart().asSeconds();
    while (frameAccumulator >= frameTime) {
        FrameIndexAtual = (FrameIndexAtual + 1) % static_cast<int>(frames_data.size());
        frameAccumulator -= frameTime;

        if (!applyFrame(SpriteAtual, frames_data[FrameIndexAtual])) {
            loaded = false;
            return;
        }
        unsigned int nextFrameIndex = (FrameIndexAtual + 1) % frames_data.size();
        if (!applyFrame(ProxSprite, frames_data[nextFrameIndex])) {
            loaded = false;
            return;
        }
    }

    updateBlend(frameAccumulator / frameTime);
}

void Animador_Fundo::draw(sf::RenderWindow& window) const{
    if (!loaded) return;
    window.draw(SpriteAtual);
    window.draw(ProxSprite);
}

void Animador_Fundo::setPosicao(const sf::Vector2f& pos) {
    posicaoBase = pos;
    if (loaded) updateSpriteScale();
}

void Animador_Fundo::setTargetSize(const sf::Vector2u& size) {
    targetSize = size;
    updateSpriteScale();
}

bool Animador_Fundo::cacheFrameTextures() {
    std::vector<FrameData>::iterator it;
    for (it = frames_data.begin(); it != frames_data.end(); ++it) {
        it->textura = buscarTextura(it->caminho_spSheet);
        if (it->textura == 0) return false;
    }
    return true;
}

bool Animador_Fundo::applyFrame(sf::Sprite& sprite, const FrameData& frameData) {
    if (frameData.textura == 0) return false;
    sprite.setTexture(*frameData.textura, true);
    sprite.setTextureRect(frameData.RectTextura);
    return true;
}
