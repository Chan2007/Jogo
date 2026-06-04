#include "animador_fundo.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <sstream>

Animador_Fundo::FrameData::FrameData(): caminho_spSheet(), RectTextura(), textura(NULL){}

Animador_Fundo::Animador_Fundo(Gerenciador_Textura* gerenciadorTextura):
    SpriteAtual(),
    ProxSprite(),
    bufferReady(false),
    threadRunning(false),
    thread(NULL),
    allowLoad(true),
    frames_data(),
    gerenciadorTextura(gerenciadorTextura),
    FrameIndexAtual(0),
    frameAccumulator(0.0f),
    frameTime(1.0f / 30.0f),
    frameSize(),
    targetSize(),
    posicaoBase(0.f, 0.f),
    loaded(false)
{}

Animador_Fundo::~Animador_Fundo() {
    if (thread) {
        thread->wait();
        delete thread;
        thread = NULL;
    }
}

void Animador_Fundo::loadThread() {
    // Carrega a textura e coloca direto no cache do gerenciador
    gerenciadorTextura->carregarTextura(pathToLoad);

    // Sinaliza que a textura está pronta
    mutex.lock();
    bufferReady = true;
    threadRunning = false;
    mutex.unlock();
}

void Animador_Fundo::preLoadNextFrame(unsigned int index) {
    // Verifica se há thread rondando COM LOCK para evitar race condition
    mutex.lock();
    bool threadEstaRodando = threadRunning;
    mutex.unlock();

    // Se thread anterior continua rodando, não dispara uma nova
    if (threadEstaRodando) return;

    // Limpa thread anterior se terminou
    if (thread) {
        delete thread;
        thread = NULL;
    }

    // Prepara o caminho a carregar
    pathToLoad = frames_data[index].caminho_spSheet;

    // Sinaliza que thread vai rodar
    mutex.lock();
    bufferReady = false;
    threadRunning = true;
    mutex.unlock();

    // Dispara thread para carregar a textura
    thread = new sf::Thread(&Animador_Fundo::loadThread, this);
    thread->launch();
}

sf::Texture* Animador_Fundo::findTexture(const std::string& path) const {
    if (!gerenciadorTextura) return NULL;

    // Verifica cache primeiro (padrão de projeto Proxy)
    sf::Texture* tex = gerenciadorTextura->buscarTextura(path);
    if (tex && tex->getSize().x > 0 && tex->getSize().y > 0)
        return tex;

    // Durante atualização (allowLoad = false), não carrega sincronamente
    // pois a thread de pré-carregamento cuidará disso
    if (!allowLoad)
        return NULL;

    // Durante inicialização, carrega conforme necessário
    if (gerenciadorTextura->carregarTextura(path))
        return gerenciadorTextura->buscarTextura(path);

    return NULL;
}

bool Animador_Fundo::applyFrame(sf::Sprite& sprite, FrameData& frameData) {
    // Se a textura já foi carregada, usa ela
    if (frameData.textura != NULL && frameData.textura->getSize().x > 0 && frameData.textura->getSize().y > 0) {
        sprite.setTexture(*frameData.textura, true);
        sprite.setTextureRect(frameData.RectTextura);
        return true;
    }

    // Tenta carregar se ainda não foi carregada
    if (frameData.textura == NULL)
        frameData.textura = findTexture(frameData.caminho_spSheet);

    // Se conseguiu carregar, aplica
    if (frameData.textura != NULL && frameData.textura->getSize().x > 0 && frameData.textura->getSize().y > 0) {
        sprite.setTexture(*frameData.textura, true);
        sprite.setTextureRect(frameData.RectTextura);
        return true;
    }

    // Se ainda não conseguiu carregar (durante update), retorna true mesmo assim
    // para continuar a animação com o sprite anterior
    return true;
}


void Animador_Fundo::updateSpriteScale() {
    if (!loaded || frameSize.x == 0 || frameSize.y == 0 || targetSize.x == 0 || targetSize.y == 0)
        return;
    const float scaleX = static_cast<float>(targetSize.x) / static_cast<float>(frameSize.x);
    const float scaleY = static_cast<float>(targetSize.y) / static_cast<float>(frameSize.y);
    const sf::Vector2f escala(scaleX, scaleY);

    SpriteAtual.setScale(escala);
    ProxSprite.setScale(escala);
    SpriteAtual.setPosition(posicaoBase);
    ProxSprite.setPosition(posicaoBase);
}

void Animador_Fundo::updateBlend() {
    if (!loaded || frames_data.empty()) return;
    SpriteAtual.setColor(sf::Color(255, 255, 255, 255));
    ProxSprite .setColor(sf::Color(255, 255, 255,   0));
}

bool Animador_Fundo::loadFrames(const std::string &pathPrefix, const std::string &name,
                                const int numFrames, const int frameStep,
                                unsigned int colunas, unsigned int linhas) {
    frames_data.clear();
    if (!gerenciadorTextura) return false;

    gerenciadorTextura->limparTextura();
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

    sf::Texture* texturaTemplate = findTexture(primeiroSheet);
    if (!texturaTemplate) return false;

    const sf::Vector2u tamanhoSheet = texturaTemplate->getSize();

    if (tamanhoSheet.x == 0 || tamanhoSheet.y == 0) return false;
    if ((tamanhoSheet.x % colunas) != 0 || (tamanhoSheet.y % linhas) != 0) return false;

    const int frameWidth = static_cast<int>(tamanhoSheet.x / colunas);
    const int frameHeight = static_cast<int>(tamanhoSheet.y / linhas);
    const unsigned int frames_SprSheet = colunas * linhas;

    for (int i = 1; i <= numFrames; i += frameStep) {
        const int indexFrame = i - 1;
        const unsigned int spSheetIndex = (indexFrame / frames_SprSheet) + 1;
        const unsigned int frameIndex_spSheet = indexFrame % frames_SprSheet;
        const unsigned int coluna = frameIndex_spSheet / linhas;
        const unsigned int linha = frameIndex_spSheet % linhas;

        FrameData frame;
        std::ostringstream buffer;
        buffer << name << spSheetIndex << ".png";
        frame.caminho_spSheet = Encontrar_Caminho::concatenarEnderecos(path, buffer.str());
        frame.RectTextura = sf::IntRect(coluna * frameWidth, linha * frameHeight, frameWidth, frameHeight);

        // "Lazy Loading"
        // Os demais sheets serão carregados pelo proxy na primeira exibição
        frame.textura = (spSheetIndex == 1) ? texturaTemplate : NULL;
        frames_data.push_back(frame);
    }

    if (frames_data.empty()) return false;

    // Aplica frame 0 e pré-carrega frame 1 (evita hitch no primeiro update)
    if (!applyFrame(SpriteAtual, frames_data[0])) return false;

    frameSize = sf::Vector2u(frameWidth, frameHeight);

    const unsigned int nextIndex = (frames_data.size() > 1) ? 1 : 0;
    if (!applyFrame(ProxSprite, frames_data[nextIndex])) return false;

    loaded = true;
    updateSpriteScale();
    updateBlend();
    clock.restart();
    if (frames_data.size() > 1)
        preLoadNextFrame(1);
    return true;
}

void Animador_Fundo::update() {
    if (!loaded || frames_data.empty()) return;

    // Desabilita carregamento síncrono durante update (evita travamento)
    const bool temp = allowLoad;
    allowLoad = false;

    if (frames_data.size() == 1) {
        allowLoad = temp;
        return;
    }

    const float delta = clock.getElapsedTime().asSeconds();
    frameAccumulator += std::min(delta, frameTime * 4.0f);

    if (frameAccumulator >= frameTime) {
        clock.restart(); // só reinicia quando vai avançar

        const int framesSaltados = static_cast<int>(frameAccumulator / frameTime);
        const unsigned int total = static_cast<unsigned int>(frames_data.size());
        const unsigned int newIndex = (FrameIndexAtual + static_cast<unsigned int>(framesSaltados)) % total;
        const unsigned int nextIndex = (newIndex + 1) % total;

        const bool aplicouAtual = applyFrame(SpriteAtual, frames_data[newIndex]);
        const bool aplicouProx  = applyFrame(ProxSprite,  frames_data[nextIndex]);

        if (aplicouAtual && aplicouProx) {
            FrameIndexAtual  = newIndex;
            frameAccumulator -= frameTime * static_cast<float>(framesSaltados);
            updateBlend(); // só chama quando o frame realmente mudou

            // Pré-carrega o frame seguinte
            const unsigned int temp_nextIndex = (nextIndex + 1) % total;
            preLoadNextFrame(temp_nextIndex);
        }
        else frameAccumulator = 0.0f;

    }
    // Restaura permissão de carregamento
    allowLoad = temp;
}


void Animador_Fundo::draw(sf::RenderTarget& target) const {
    if (!loaded) return;
    target.draw(SpriteAtual);
    target.draw(ProxSprite);
}

void Animador_Fundo::setPosicao(const sf::Vector2f& pos) {
    posicaoBase = pos;
    if (loaded) updateSpriteScale();
}

void Animador_Fundo::setTargetSize(const sf::Vector2u& size) {
    targetSize = size;
    if (loaded) updateSpriteScale();
}
