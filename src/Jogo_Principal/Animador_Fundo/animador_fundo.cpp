#include "animador_fundo.h"
#include "Jogo_Principal/Diretorio/Encontrar_Diretorio.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>

// ──────────────────────────────────────────────
// FrameData
// ──────────────────────────────────────────────
Animador_Fundo::FrameData::FrameData()
    : caminho_spSheet()
    , RectTextura()
    , textura(0)
{}

// ──────────────────────────────────────────────
// Ctor / Dtor
// ──────────────────────────────────────────────
Animador_Fundo::Animador_Fundo(Gerenciador_Textura* gerenciadorTextura)
    : SpriteAtual()
    , ProxSprite()
    , frames_data()
    , gerenciadorTextura(gerenciadorTextura)
    , FrameIndexAtual(0)
    , frameAccumulator(0.0f)
    , frameTime(1.0f / 60.0f)
    , frameSize()
    , targetSize()
    , posicaoBase(0.f, 0.f)
    , loaded(false)
{
    const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    targetSize.x = desktop.width;
    targetSize.y = desktop.height;
}

Animador_Fundo::~Animador_Fundo() {}

// ──────────────────────────────────────────────
// buscarTextura — lazy load via Gerenciador
// ──────────────────────────────────────────────
sf::Texture* Animador_Fundo::buscarTextura(const std::string& path) const {
    if (!gerenciadorTextura) return 0;

    sf::Texture* tex = gerenciadorTextura->buscar_textura(path);
    if (tex && (tex->getSize().x > 0 || tex->getSize().y > 0))
        return tex;

    // Proxy: carrega sob demanda, somente se ainda não está em memória
    if (gerenciadorTextura->carregar_textura(path))
        return gerenciadorTextura->buscar_textura(path);

    return 0;
}

// ──────────────────────────────────────────────
// applyFrame — usa cache no FrameData
// ──────────────────────────────────────────────
// Agora o ponteiro resolvido é guardado em frameData.textura para
// que chamadas futuras não precisem passar pelo Gerenciador novamente.
bool Animador_Fundo::applyFrame(sf::Sprite& sprite, FrameData& frameData) {
    if (frameData.textura == 0)
        frameData.textura = buscarTextura(frameData.caminho_spSheet);

    if (frameData.textura == 0) return false;

    sprite.setTexture(*frameData.textura, true);
    sprite.setTextureRect(frameData.RectTextura);
    return true;
}

// ──────────────────────────────────────────────
// updateSpriteScale
// ──────────────────────────────────────────────
void Animador_Fundo::updateSpriteScale() {
    if (!loaded
        || frameSize.x == 0 || frameSize.y == 0
        || targetSize.x == 0 || targetSize.y == 0)
        return;

    const float scaleX = static_cast<float>(targetSize.x) / static_cast<float>(frameSize.x);
    const float scaleY = static_cast<float>(targetSize.y) / static_cast<float>(frameSize.y);
    const sf::Vector2f escala(scaleX, scaleY);

    SpriteAtual.setScale(escala);
    ProxSprite .setScale(escala);
    SpriteAtual.setPosition(posicaoBase);
    ProxSprite .setPosition(posicaoBase);
}

// ──────────────────────────────────────────────
// updateBlend — parâmetro removido (era ignorado)
// ──────────────────────────────────────────────
void Animador_Fundo::updateBlend() {
    if (!loaded || frames_data.empty()) return;
    SpriteAtual.setColor(sf::Color(255, 255, 255, 255));
    ProxSprite .setColor(sf::Color(255, 255, 255,   0));
}

// ──────────────────────────────────────────────
// loadFrames
// ──────────────────────────────────────────────
bool Animador_Fundo::loadFrames(const std::string& pathPrefix, int numFrames,
                                int startIndex, int frameStep,
                                int colunas,    int linhas)
{
    frames_data.clear();
    if (!gerenciadorTextura) return false;

    gerenciadorTextura->limpar_textura();
    FrameIndexAtual  = 0;
    frameAccumulator = 0.0f;
    frameSize        = sf::Vector2u();
    loaded           = false;

    if (numFrames <= 0 || startIndex <= 0 || frameStep <= 0
        || colunas <= 0 || linhas <= 0)
        return false;

    frameTime = static_cast<float>(frameStep) / 60.0f;

    const std::string path = Encontrar_Diretorio::concatenarEnderecos(pathPrefix, "/");

    // Lê dimensões do primeiro sheet para calcular frameWidth/Height
    // O ponteiro já fica cacheado em frames_data[0].textura depois
    char buffer[32];
    std::sprintf(buffer, "bg_menu%d.png", 1);
    const std::string primeiroSheet = Encontrar_Diretorio::concatenarEnderecos(path, buffer);

    sf::Texture* texturaBase = buscarTextura(primeiroSheet);
    if (!texturaBase) return false;

    const sf::Vector2u tamanhoSheet = texturaBase->getSize();
    if (tamanhoSheet.x == 0 || tamanhoSheet.y == 0) return false;

    const unsigned int ucolunas = static_cast<unsigned int>(colunas);
    const unsigned int ulinhas  = static_cast<unsigned int>(linhas);
    if ((tamanhoSheet.x % ucolunas) != 0 || (tamanhoSheet.y % ulinhas) != 0)
        return false;

    const int frameWidth      = static_cast<int>(tamanhoSheet.x / ucolunas);
    const int frameHeight     = static_cast<int>(tamanhoSheet.y / ulinhas);
    const int frames_SprSheet = colunas * linhas;
    const int endIndex        = startIndex + numFrames;

    for (int i = startIndex; i < endIndex; i += frameStep) {
        const int indexFrame        = i - 1;
        const int spSheetIndex      = (indexFrame / frames_SprSheet) + 1;
        const int frameIndex_spSheet = indexFrame % frames_SprSheet;
        const int coluna            = frameIndex_spSheet / linhas;
        const int linha             = frameIndex_spSheet % linhas;

        FrameData frame;
        std::sprintf(buffer, "bg_menu%d.png", spSheetIndex);
        frame.caminho_spSheet = Encontrar_Diretorio::concatenarEnderecos(path, buffer);
        frame.RectTextura     = sf::IntRect(coluna * frameWidth, linha * frameHeight,
                                            frameWidth, frameHeight);
        // Textura do primeiro sheet já está carregada: cacheia o ponteiro
        // Os demais sheets serão carregados pelo proxy na primeira exibição
        frame.textura = (spSheetIndex == 1) ? texturaBase : 0;
        frames_data.push_back(frame);
    }

    if (frames_data.empty()) return false;

    // Aplica frame 0 e pré-carrega frame 1 (evita hitch no primeiro update)
    if (!applyFrame(SpriteAtual, frames_data[0])) return false;

    frameSize = sf::Vector2u(frameWidth, frameHeight);

    const std::size_t idxProx = (frames_data.size() > 1) ? 1 : 0;
    if (!applyFrame(ProxSprite, frames_data[idxProx])) return false;

    loaded = true;
    updateSpriteScale();
    updateBlend();
    clock.restart();
    return true;
}

// ──────────────────────────────────────────────
// update
// ──────────────────────────────────────────────
void Animador_Fundo::update() {
    if (!loaded || frames_data.empty()) return;

    if (frames_data.size() == 1) {
        updateBlend();
        return;
    }

    const float delta = clock.restart().asSeconds();

    // Proteção contra spike de I/O, alt+tab ou breakpoint:
    // descarta qualquer delta maior que 4 frames
    frameAccumulator += std::min(delta, frameTime * 4.0f);

    if (frameAccumulator >= frameTime) {
        const int framesSaltados =
            static_cast<int>(frameAccumulator / frameTime);

        const unsigned int total = static_cast<unsigned int>(frames_data.size());

        const unsigned int novoFrameAtual =
            (FrameIndexAtual + static_cast<unsigned int>(framesSaltados)) % total;

        const unsigned int idxProx = (novoFrameAtual + 1) % total;

        // Tenta aplicar — só avança o índice se AMBOS tiverem textura válida.
        // Assim o sprite nunca fica em estado parcialmente atualizado.
        const bool okAtual = applyFrame(SpriteAtual, frames_data[novoFrameAtual]);
        const bool okProx  = applyFrame(ProxSprite,  frames_data[idxProx]);

        if (okAtual && okProx) {
            FrameIndexAtual  = novoFrameAtual;
            frameAccumulator -= frameTime * static_cast<float>(framesSaltados);
        } else {
            // Textura ainda não disponível: mantém frame atual visível
            // e reseta o acumulador para tentar de novo no próximo update
            frameAccumulator = 0.0f;
        }
    }

    updateBlend();
}

// ──────────────────────────────────────────────
// draw / setters
// ──────────────────────────────────────────────
void Animador_Fundo::draw(sf::RenderWindow& window) const {
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
