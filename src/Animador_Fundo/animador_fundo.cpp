#include "animador_fundo.h"
#include "Diretorio/Encontrar_Diretorio.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>

Animador_Fundo::FrameData::FrameData(): caminho_spSheet(), RectTextura(), textura(NULL){}

Animador_Fundo::Animador_Fundo(Gerenciador_Textura* gerenciadorTextura):
    SpriteAtual(),
    ProxSprite(),
    m_bufferPronto(false),
    m_threadRodando(false),
    m_loadThread(NULL),
    frames_data(),
    gerenciadorTextura(gerenciadorTextura),
    FrameIndexAtual(0),
    frameAccumulator(0.0f),
    frameTime(1.0f / 30.0f),
    frameSize(),
    targetSize(),
    posicaoBase(0.f, 0.f),
    loaded(false),
    m_scaleAtualizacao(false),
    m_blendAtualizacao(false),
    m_permitirCarregamento(true)
{
    const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    targetSize.x = desktop.width;
    targetSize.y = desktop.height;
}

Animador_Fundo::Animador_Fundo():
    SpriteAtual(),
    ProxSprite(),
    m_bufferPronto(false),
    m_threadRodando(false),
    m_loadThread(NULL),
    frames_data(),
    gerenciadorTextura(NULL),
    FrameIndexAtual(0),
    frameAccumulator(0.0f),
    frameTime(1.0f / 30.0f),
    frameSize(),
    targetSize(),
    posicaoBase(0.f, 0.f),
    loaded(false),
    m_scaleAtualizacao(false),
    m_blendAtualizacao(false),
    m_permitirCarregamento(true)
{
    const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    targetSize.x = desktop.width;
    targetSize.y = desktop.height;
}

Animador_Fundo::~Animador_Fundo() {
    if (m_loadThread) {
        m_loadThread->wait();
        delete m_loadThread;
        m_loadThread = NULL;
    }
}

void Animador_Fundo::threadCarregar() {
    // Carrega a textura e coloca direto no cache do gerenciador
    gerenciadorTextura->carregar_textura(m_pathParaCarregar);

    // Sinaliza que a textura está pronta
    m_bufferMutex.lock();
    m_bufferPronto = true;
    m_threadRodando = false;
    m_bufferMutex.unlock();
}

void Animador_Fundo::preCarregarProximoFrame(unsigned int idxProx) {
    // Verifica se há thread rondando COM LOCK para evitar race condition
    m_bufferMutex.lock();
    bool threadEstaRodando = m_threadRodando;
    m_bufferMutex.unlock();

    // Se thread anterior ainda está rodando, não dispara uma nova
    if (threadEstaRodando) {
        return;
    }

    // Limpa thread anterior se terminou
    if (m_loadThread) {
        delete m_loadThread;
        m_loadThread = NULL;
    }

    // Prepara o caminho a carregar
    m_pathParaCarregar = frames_data[idxProx].caminho_spSheet;

    // Sinaliza que thread vai rodar
    m_bufferMutex.lock();
    m_bufferPronto = false;
    m_threadRodando = true;
    m_bufferMutex.unlock();

    // Dispara thread para carregar a textura
    m_loadThread = new sf::Thread(&Animador_Fundo::threadCarregar, this);
    m_loadThread->launch();
}
sf::Texture* Animador_Fundo::buscarTextura(const std::string& path) const {
    if (!gerenciadorTextura) return NULL;

    // Verifica cache primeiro
    sf::Texture* tex = gerenciadorTextura->buscar_textura(path);
    if (tex && (tex->getSize().x > 0 || tex->getSize().y > 0))
        return tex;

    // Durante atualização (m_permitirCarregamento = false), não carrega sincrônamente
    // pois a thread de pré-carregamento cuidará disso
    if (!m_permitirCarregamento)
        return NULL;

    // Durante inicialização, carrega conforme necessário
    if (gerenciadorTextura->carregar_textura(path))
        return gerenciadorTextura->buscar_textura(path);

    return NULL;
}

bool Animador_Fundo::applyFrame(sf::Sprite& sprite, FrameData& frameData) {
    // Se a textura já foi carregada, usa ela
    if (frameData.textura != NULL && frameData.textura->getSize().x > 0) {
        sprite.setTexture(*frameData.textura, true);
        sprite.setTextureRect(frameData.RectTextura);
        return true;
    }

    // Tenta carregar se ainda não foi carregada
    if (frameData.textura == NULL) {
        frameData.textura = buscarTextura(frameData.caminho_spSheet);
    }

    // Se conseguiu carregar, aplica
    if (frameData.textura != NULL && frameData.textura->getSize().x > 0) {
        sprite.setTexture(*frameData.textura, true);
        sprite.setTextureRect(frameData.RectTextura);
        return true;
    }

    // Se ainda não conseguiu carregar (durante update), retorna true mesmo assim
    // para continuar a animação com o sprite anterior
    return true;
}

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

    m_scaleAtualizacao = false;
}

void Animador_Fundo::updateBlend() {
    if (!loaded || frames_data.empty()) return;
    SpriteAtual.setColor(sf::Color(255, 255, 255, 255));
    ProxSprite .setColor(sf::Color(255, 255, 255,   0));

    m_blendAtualizacao = false;
}

bool Animador_Fundo::loadFrames(const std::string &pathPrefix, const int numFrames,
                                const int frameStep, unsigned int colunas, unsigned int linhas) {
    frames_data.clear();
    if (!gerenciadorTextura) return false;

    gerenciadorTextura->limpar_textura();
    FrameIndexAtual = 0;
    frameAccumulator = 0.0f;
    frameSize = sf::Vector2u();
    loaded = false;

    if (numFrames <= 0 || frameStep <= 0 || colunas <= 0 || linhas <= 0)
        return false;

    frameTime = static_cast<float>(frameStep) / 60.0f;

    const std::string path = Encontrar_Diretorio::concatenarEnderecos(pathPrefix, "/");

    // Lê dimensões do primeiro sheet para calcular frameWidth e frameHeight
    // O ponteiro fica no cache do vetor em frames_data[0].textura depois
    char buffer[256];
    std::sprintf(buffer, "bg_menu%d.png", 1);
    const std::string primeiroSheet = Encontrar_Diretorio::concatenarEnderecos(path, buffer);

    sf::Texture* texturaTemplate = buscarTextura(primeiroSheet);
    if (!texturaTemplate) return false;

    const sf::Vector2u tamanhoSheet = texturaTemplate->getSize();
    if (tamanhoSheet.x == 0 || tamanhoSheet.y == 0) return false;

    colunas = static_cast<unsigned int>(colunas);
    linhas = static_cast<unsigned int>(linhas);
    if ((tamanhoSheet.x % colunas) != 0 || (tamanhoSheet.y % linhas) != 0)
        return false;

    const int frameWidth = static_cast<int>(tamanhoSheet.x / colunas);
    const int frameHeight = static_cast<int>(tamanhoSheet.y / linhas);
    const unsigned int frames_SprSheet = colunas * linhas;
    const int endIndex = numFrames + 1;

    for (int i = 1; i < endIndex; i += frameStep) {
        const int indexFrame = i - 1;
        const unsigned int spSheetIndex = (indexFrame / frames_SprSheet) + 1;
        const unsigned int frameIndex_spSheet = indexFrame % frames_SprSheet;
        const unsigned int coluna = frameIndex_spSheet / linhas;
        const unsigned int linha = frameIndex_spSheet % linhas;

        FrameData frame;
        std::sprintf(buffer, "bg_menu%d.png", spSheetIndex);
        frame.caminho_spSheet = Encontrar_Diretorio::concatenarEnderecos(path, buffer);
        frame.RectTextura = sf::IntRect(coluna * frameWidth, linha * frameHeight, frameWidth, frameHeight);

    // Textura do primeiro sheet já está carregada: coloca o ponteiro no cache
    // Os demais sheets serão carregados pelo proxy na primeira exibição
    frame.textura = (spSheetIndex == 1) ? texturaTemplate : NULL;
        frames_data.push_back(frame);
    }

    if (frames_data.empty()) return false;

    // Aplica frame 0 e pré-carrega frame 1 (evita hitch no primeiro update)
    if (!applyFrame(SpriteAtual, frames_data[0])) return false;

    frameSize = sf::Vector2u(frameWidth, frameHeight);

    const unsigned int idxProx = (frames_data.size() > 1) ? 1 : 0;
    if (!applyFrame(ProxSprite, frames_data[idxProx])) return false;

    loaded = true;
    m_scaleAtualizacao = false;
    m_blendAtualizacao = false;
    updateSpriteScale();
    updateBlend();
    clock.restart();
    if (frames_data.size() > 1)
        preCarregarProximoFrame(1);
    return true;
}

void Animador_Fundo::update() {
    if (!loaded || frames_data.empty()) return;

    // Desabilita carregamento síncrono durante update (evita travamento)
    const bool permitirAnterior = m_permitirCarregamento;
    m_permitirCarregamento = false;

    // Atualiza escala/posição se foi sinalizado
    if (m_scaleAtualizacao) {
        // Reabilita temporariamente para updateSpriteScale (não usa texturas)
        updateSpriteScale();
    }

    if (frames_data.size() == 1) {
        m_permitirCarregamento = permitirAnterior;
        return;
    }

    const float delta = clock.getElapsedTime().asSeconds();
    frameAccumulator += std::min(delta, frameTime * 4.0f);

    if (frameAccumulator >= frameTime) {
        clock.restart(); // ✅ só reinicia quando vai avançar

        const int framesSaltados =
            static_cast<int>(frameAccumulator / frameTime);

        const unsigned int total =
            static_cast<unsigned int>(frames_data.size());

        const unsigned int novoIndice =
            (FrameIndexAtual + static_cast<unsigned int>(framesSaltados)) % total;

        const unsigned int idxProx = (novoIndice + 1) % total;

        const bool aplicouAtual = applyFrame(SpriteAtual, frames_data[novoIndice]);
        const bool aplicouProx  = applyFrame(ProxSprite,  frames_data[idxProx]);

        if (aplicouAtual && aplicouProx) {
            FrameIndexAtual  = novoIndice;
            frameAccumulator -= frameTime * static_cast<float>(framesSaltados);
            updateBlend(); // ✅ só chama quando o frame realmente mudou

            // ✅ pré-carrega o frame seguinte
            const unsigned int idxFuturo = (idxProx + 1) % total;
            preCarregarProximoFrame(idxFuturo);
        } else {
            frameAccumulator = 0.0f;
        }
    }

    // Restaura permissão de carregamento
    m_permitirCarregamento = permitirAnterior;
}

void Animador_Fundo::draw(sf::RenderWindow& target) const {
    if (!loaded) return;
    target.draw(SpriteAtual);
    target.draw(ProxSprite);
}

void Animador_Fundo::setPosicao(const sf::Vector2f& pos) {
    posicaoBase = pos;
    if (loaded) {
        m_scaleAtualizacao = true;
    }
}

void Animador_Fundo::setTargetSize(const sf::Vector2u& size) {
    targetSize = size;
    if (loaded) {
        m_scaleAtualizacao = true;
    }
}
