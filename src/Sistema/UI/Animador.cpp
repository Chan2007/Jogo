#include <algorithm>
#include <sstream>

#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Gerenciador_Textura.h"
#include "Animador.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Proxy_Textura.h"

Animador::FrameData::FrameData()
    : caminho_spSheet(), RectTextura(), textura(NULL) {}

Animador::Animador(Gerenciadores::Gerenciador_Textura* gerenciadorTextura):
    SpriteAtual(),
    ProxSprite(),
    proxy(gerenciadorTextura),
    allowLoad(true),
    frames_data(),
    gerenciadorTextura(gerenciadorTextura),
    FrameIndexAtual(0),
    frameAccumulator(0.0f),
    frameTime(1.0f / 30.0f),
    frameSize(),
    targetSize(),
    position(),
    loaded(false)
{}

Animador::~Animador() {}

void Animador::applyFrame(sf::Sprite &sprite, FrameData &frameData) {
    // Ele faz o pedido diretamente através do Proxy
    sf::Texture* tex = proxy.getTexture(frameData.caminho_spSheet);

    // Se o Proxy retornou uma textura válida, aplica ela ao sprite
    if (tex != NULL && tex->getSize().x > 0 && tex->getSize().y > 0) {
        sprite.setTexture(*tex, true);
        sprite.setTextureRect(frameData.RectTextura);
    }
}


void Animador::updateSpriteScale() {
    if (!loaded || frameSize.x == 0 || frameSize.y == 0 || targetSize.x == 0 || targetSize.y == 0)
        return;
    const float scaleX = static_cast<float>(targetSize.x) / static_cast<float>(frameSize.x);
    const float scaleY = static_cast<float>(targetSize.y) / static_cast<float>(frameSize.y);
    const sf::Vector2f escala(scaleX, scaleY);

    SpriteAtual.setScale(escala);
    ProxSprite.setScale(escala);
    SpriteAtual.setPosition(position);
    ProxSprite.setPosition(position);
}

void Animador::updateBlend() {
    if (!loaded || frames_data.empty()) return;
    SpriteAtual.setColor(sf::Color(255, 255, 255, 255));
    ProxSprite .setColor(sf::Color(255, 255, 255,   0));
}

bool Animador::loadFrames(const std::string &pathPrefix, const std::string &name,
                                const int numFrames, const int frameStep,
                                unsigned int colunas, unsigned int linhas) {
    frames_data.clear();
    if (!gerenciadorTextura) return false;

    FrameIndexAtual = 0;
    frameAccumulator = 0.0f;
    frameSize = sf::Vector2u();
    loaded = false;

    if (numFrames <= 0 || frameStep <= 0 || colunas <= 0 || linhas <= 0) return false;

    frameTime = static_cast<float>(frameStep) / 60.0f;

    const std::string path = Encontrar_Caminho::concatenarEnderecos(pathPrefix, "/");

    // Lê dimensões do primeiro sheet para calcular frameWidth e frameHeight
    // O ponteiro fica no cache do vetor em frames_data[0].textura depois
    std::ostringstream bufferIn;
    bufferIn << name << 1 << ".png";
    const std::string primeiroSheet = Encontrar_Caminho::concatenarEnderecos(path, bufferIn.str());

    sf::Texture* texturaTemplate = proxy.getTexture(primeiroSheet, true);
    if (!texturaTemplate) return false;

    const sf::Vector2u tamanhoSheet = texturaTemplate->getSize();

    if (tamanhoSheet.x == 0 || tamanhoSheet.y == 0) return false;
    if (tamanhoSheet.x % colunas != 0 || (tamanhoSheet.y % linhas) != 0) return false;

    const int frameWidth = static_cast<int>(tamanhoSheet.x / colunas);
    const int frameHeight = static_cast<int>(tamanhoSheet.y / linhas);
    const unsigned int frames_SprSheet = colunas * linhas;

    for (int i = 1; i <= numFrames; i += frameStep) {
        const int indexFrame = i - 1;
        const unsigned int spSheetIndex = indexFrame / frames_SprSheet + 1;
        const unsigned int frameIndex_spSheet = indexFrame % frames_SprSheet;
        const unsigned int coluna = frameIndex_spSheet / linhas;
        const unsigned int linha = frameIndex_spSheet % linhas;

        FrameData frame;
        std::ostringstream buffer;
        buffer << name << spSheetIndex << ".png";
        frame.caminho_spSheet = Encontrar_Caminho::concatenarEnderecos(path, buffer.str());
        frame.RectTextura = sf::IntRect(frameWidth * coluna, linha * frameHeight, frameWidth, frameHeight);

        // "Lazy Loading"
        frame.textura = NULL;
        frames_data.push_back(frame);
    }

    if (frames_data.empty()) return false;
    frameSize = sf::Vector2u(frameWidth, frameHeight);

    sf::Texture* texInicial = proxy.getTexture(frames_data[0].caminho_spSheet);
    if (texInicial) {
        SpriteAtual.setTexture(*texInicial);
        SpriteAtual.setTextureRect(frames_data[0].RectTextura);
    }

    loaded = true;
    updateSpriteScale();
    updateBlend();
    clock.restart();
    return true;
}


void Animador::update() {
    if (!loaded || frames_data.empty()) return;
    if (frames_data.size() == 1) return;

    const float delta = clock.getElapsedTime().asSeconds();
    frameAccumulator += std::min(delta, frameTime * 4.0f);
    clock.restart();

    if (frameAccumulator >= frameTime) {
        clock.restart(); // só reinicia quando vai avançar

        const int framesSaltados = static_cast<int>(frameAccumulator / frameTime);
        const unsigned int total = static_cast<unsigned int>(frames_data.size());
        const unsigned int newIndex = (FrameIndexAtual + static_cast<unsigned int>(framesSaltados)) % total;
        const unsigned int nextIndex = (newIndex + 1) % total;

        applyFrame(SpriteAtual, frames_data[newIndex]);
        applyFrame(ProxSprite,  frames_data[nextIndex]);


        FrameIndexAtual = newIndex;
        frameAccumulator -= frameTime * static_cast<float>(framesSaltados);
        updateBlend(); // só chama quando o frame realmente mudou
    }
    sf::Texture* texAtual = proxy.getTexture(frames_data[FrameIndexAtual].caminho_spSheet);

    if (texAtual) {
        SpriteAtual.setTexture(*texAtual);
        SpriteAtual.setTextureRect(frames_data[FrameIndexAtual].RectTextura);
    }
}

void Animador::draw(sf::RenderTarget& target) const {
    if (!loaded) return;
    target.draw(SpriteAtual);
    target.draw(ProxSprite);
}

void Animador::setSheetPosition(const sf::Vector2f& pos) {
    position = pos;
    if (loaded) updateSpriteScale();
}

void Animador::setSheetTargetSize(const sf::Vector2u& size) {
    targetSize = size;
    if (loaded) updateSpriteScale();
}

void Animador::atualizarSpriteEntidade(
    sf::Sprite& sprite, sf::IntRect& rectAtual,
    int numFrames, float dt,
    float& tempoAcumulado, int& indexFrameAtual,
    unsigned int cols, unsigned int rows, float tempoPorFrame)
{
    if (numFrames <= 0 || cols <= 0 || rows <= 0) return;
    const sf::Texture* textura = sprite.getTexture();
    if (!textura) return;

    if (indexFrameAtual >= numFrames) {
        indexFrameAtual = 0;
        tempoAcumulado = 0.0f;
    }

    tempoAcumulado += dt;
    if (tempoAcumulado >= tempoPorFrame) {
        tempoAcumulado = 0.0f;
        indexFrameAtual = (indexFrameAtual + 1) % numFrames;
    }

    const int frameW = textura->getSize().x / cols;
    const int frameH = textura->getSize().y / rows;

    int colunaAtual = indexFrameAtual % rows;
    int linhaAtual = indexFrameAtual / rows;

    rectAtual.width = frameW;
    rectAtual.height = frameH;
    rectAtual.left = colunaAtual * frameW;
    rectAtual.top = linhaAtual * frameH;

    sprite.setTextureRect(rectAtual);
}
