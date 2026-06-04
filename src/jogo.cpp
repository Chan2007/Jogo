#include "jogo.h"
#include <iostream>

#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Grafico.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

Jogo::Jogo() :
    gerenciadorTextura(),
    animadorFase1(&gerenciadorTextura),
    animadorFase2(&gerenciadorTextura),
    jogador(NULL),
    gerenciadorGravidade(NULL),
    listaEntidades(),
    janela(),
    fonte(),
    titulo(),
    opcoesMenu(),
    painelMenu(),
    inicializado(false),
    menuPronto(false),
    musicaLigada(true),
    estadoTela(TelaMenu),
    opcaoSelecionada(0)
{}

Jogo::~Jogo() {
    fechar();
}

bool Jogo::carregarMultimidia() {
    const sf::Vector2u tamanhoJanela = janela.getSize();

    diretorio_Frames_Fase1 = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_frames/fase1");
    diretorio_Frames_Fase2 = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_frames/fase2");

    diretorio_Audio = Encontrar_Caminho::acharDiretorio_Arquivo("assets/bg_audios/bg_music");

    animadorFase1.setTargetSize(tamanhoJanela);
    animadorFase2.setTargetSize(tamanhoJanela);
    if (tamanhoJanela.x == 0 || tamanhoJanela.y == 0) {
        std::cerr << "Erro: Tamanho da janela é inválido para configurar o fundo." << std::endl;
        return false;
    }

    if (!diretorio_Frames_Fase1.empty()) {
        if (animadorFase1.loadFrames(diretorio_Frames_Fase1, "bg_fase1_", 376, 1, 4, 3))
            std::cout << "Background da fase 1 carregado com sucesso!" << std::endl;
        else
            {std::cerr << "Falha ao carregar o Background da fase 1." << std::endl; return false;}
    }
    /*
    if (!diretorio_Frames_Fase2.empty()) {
        if (animadorFase2.loadFrames(diretorio_Frames_Fase2, "bg_fase2_", 451, 2, 4, 7))
            std::cout << "Background da fase 2 carregado com sucesso!" << std::endl;
        else
            {std::cerr << "Falha ao carregar o Background da fase 2." << std::endl; return false;}
    }
    */
    if (!diretorio_Audio.empty()) {
        const std::string caminho_musica1 =
            Encontrar_Caminho::concatenarEnderecos(diretorio_Audio, "Aurora_s-Theme.ogg");
        if (musica.loadMusic(caminho_musica1)) {
            musica.setVolume(50.0f);
            musica.setLoop(true);
            if (musicaLigada)
                musica.play();
        }
        else {std::cerr << "Falha ao carregar a música da fase 1." << std::endl; return false;}
    }
    if (!diretorio_Audio.empty()) {
        const std::string caminho_musica2 =
            Encontrar_Caminho::concatenarEnderecos(diretorio_Audio, "Down-to-a-Dusty-Plain.ogg");
        if (musica.loadMusic(caminho_musica2)) {
            musica.setVolume(50.0f);
            musica.setLoop(true);
            //if (musicaLigada)
            //    bgMusic.play();
        }
        else {std::cerr << "Falha ao carregar a música da fase 2." << std::endl; return false;}
    }
    return true;
}
bool Jogo::trocarMusica(int fase) {
    if (diretorio_Audio.empty()) {
        std::cerr << "Sem música disponível! " << std::endl;
        return false;
    }

    // 1. Para a música atual antes de trocar o arquivo
    musica.stop();

    std::string nomeArquivo = "";
    if (fase == 1) {
        nomeArquivo = "Aurora_s-Theme.ogg";
    } else if (fase == 2) {
        nomeArquivo = "Down-to-a-Dusty-Plain.ogg";
    }

    const std::string caminho_musica = Encontrar_Caminho::concatenarEnderecos(
                                        diretorio_Audio, nomeArquivo);

    // 2. Carrega a nova música na mesma variável
    if (musica.loadMusic(caminho_musica)) {
        musica.setVolume(50.0f);
        musica.setLoop(true);

        if (musicaLigada) {
            musica.play();
        }
        return true;
    }
    else {std::cerr << "Falha ao carregar a música da fase " << fase << std::endl; return false;}

}

bool Jogo::carregarObstaculos() {
    Obstaculos::Plataforma* chao = new Obstaculos::Plataforma(Obstaculos::Plataforma::CHAO);
    if (chao) {
        Gerenciadores::Gerenciador_Colisao::getInstancia().incluirEntidade(chao);
        chao->getCorpo().setPosition(sf::Vector2f(desktop.width * 0.5, desktop.height));
        listaEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(chao));
    }
    else {std::cerr << "Falha ao criar plataforma" << std::endl; return false;}

    Obstaculos::Plataforma* novaPlat;
    sementear();
    for (int i = 0; i < (rand()%8)+3; i++) {
        if ((rand()%10)<5)
            novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL1);
        else if ((rand()%10)<4)
            novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL2);
        else
            novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL3);

        if (!novaPlat) {std::cerr << "Falha ao criar nova plataforma." << std::endl; return false;}

        bool posicaoValida = false;
        int tentativas = 0;

        while (!posicaoValida && tentativas < 500) {
            int sizex = ((janela.getSize().x) - novaPlat->getTamanho().width);
            int sizey = ((janela.getSize().y) - novaPlat->getTamanho().height - (chao->getAltura()) / 2);
            novaPlat->getCorpo().setPosition((rand() % sizex) + (novaPlat->getTamanho().width) / 2, (rand() % sizey) + (novaPlat->getTamanho().height) / 2);

            sf::FloatRect hitboxExpandida = novaPlat->getCorpo().getGlobalBounds();
            hitboxExpandida.left -= 20.f;
            hitboxExpandida.top -= 20.f;
            hitboxExpandida.width += 40.f;
            hitboxExpandida.height += 40.f;
            if (Gerenciadores::Gerenciador_Colisao::getInstancia().verificarPosicaoLivre(hitboxExpandida))
                posicaoValida = true;

            tentativas++;
        }
        if (posicaoValida) {
            listaEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(novaPlat));
            Gerenciadores::Gerenciador_Colisao::getInstancia().incluirEntidade(novaPlat);
        }
        else delete novaPlat;
    }
    novaPlat = NULL;
    return true;
}

bool Jogo::carregarJogadores() {
    if (!jogador) {
        jogador = new Personagens::Jogador();
        if (jogador) {
            Gerenciadores::Gerenciador_Colisao::getInstancia().incluirEntidade(jogador);

            // Mudar futuramente para escolha do jogador
            jogador->setCampeao(Personagens::CAMPEAO_NAAFIRI);
            jogador->setPosicao(sf::Vector2f(100.f, 800.f));
            std::cout << "Boneco criado: " << jogador->getNome() << std::endl;

            gerenciadorGravidade = &Gerenciadores::Gerenciador_Gravidade::getInstancia();
            if (gerenciadorGravidade) {
                gerenciadorGravidade->aplicarGravidade(jogador, true);
                jogador->setGerenciadorGravidade(gerenciadorGravidade);
                return true;
            }
            std::cerr << "Falha ao aplicar gravidade."  << std::endl;
            return false;
        }
        std::cerr << "Falha ao carregar boneco."  << std::endl;
        return false;
    }
    return true;
}

bool Jogo::inicializar() {
    if (inicializado && janela.isOpen()) return true;

    if (desktop.isValid())
        janela.create(desktop, "Jogo", Style::Default);
    else
        janela.create(VideoMode(desktop.width, desktop.height), "Jogo", Style::Default);

    janela.setFramerateLimit(60);

    if (!carregarMultimidia() || !carregarObstaculos() || !carregarJogadores()) {
        janela.close();
        return false;
    }

    inicializado = true;
    estadoTela = TelaFase;
    relogio_fisica.restart();
    if (tocandoMusica()) trocarMusica(1);
    return true;
}

void Jogo::executar() {
    if (!inicializar()) return;
    while (estaAberto())
        atualizar();
}
void Jogo::iniciarFase() {
    if (!inicializado) return;
    estadoTela = TelaFase;
}

void Jogo::setMusica(bool ligada) {
    musicaLigada = ligada;

    if (inicializado && janela.isOpen()) {
        if (musicaLigada) musica.play();
        else musica.pause();
    }
}

void Jogo::setVolume(float volume) { musica.setVolume(volume);}

bool Jogo::tocandoMusica() const { return musicaLigada;}

// Para tela de pausa
void Jogo::processarEventoJogo(const sf::Event& evento) {
    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
        estadoTela = TelaPausa;
    }
}

void Jogo::processarEventos() {
    while (janela.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            janela.close();
            inicializado = false;
            return;
        }

        if (event.type == sf::Event::Resized) {
            animadorFase1.setTargetSize(sf::Vector2u(desktop.width, desktop.height));
            animadorFase2.setTargetSize(sf::Vector2u(desktop.width, desktop.height));
        }

        if (estadoTela == TelaPausa)
            processarEventoPausa(event);
        else
            processarEventoJogo(event);
    }
}
void Jogo::executarOpcaoMenu()
{
    switch (opcaoSelecionada) {
        case 0:
            estadoTela = TelaFase;
            break;
        case 1:
            musicaLigada = !musicaLigada;
            setMusica(musicaLigada);
            break;
        case 2:
            janela.close();
            break;
        default:
            break;
    }
}


void Jogo::desenharFase() {
    float dt = relogio_fisica.restart().asSeconds();

    if (dt > 0.1f) { dt = 0.1f; }

    animadorFase1.update();
    janela.clear(sf::Color::Black);
    animadorFase1.draw(janela);

    listaEntidades.desenharTodas(janela);
    if (jogador) {
        gerenciadorGravidade->executar(dt);

        sf::Vector2u tamanhoMundo(desktop.width, desktop.height);

        Gerenciadores::Gerenciador_Colisao::getInstancia().executar(jogador, tamanhoMundo, gerenciadorGravidade);
        jogador->atualizar();
        jogador->desenhar(janela);
    }
}

void Jogo::atualizar() {
    if (!inicializado || !janela.isOpen()) {
        return;
    }

    processarEventos();
    if (!janela.isOpen()) {
        return;
    }

    janela.clear(sf::Color::Black);

    if (estadoTela == TelaMenu) {
        // Por enquanto não tem nada aqui (QT faz essa parte)
    }
    else if (estadoTela == TelaPausa)
        desenharPausa();
    else
        desenharFase();

    janela.display();
}

bool Jogo::estaAberto() const {
    return janela.isOpen();
}

void Jogo::fechar() {
    if (janela.isOpen()) janela.close();

    if (jogador) {
        delete jogador;
        jogador = NULL;
    }

    if (gerenciadorGravidade) {
        delete gerenciadorGravidade;
        gerenciadorGravidade = NULL;
    }

    inicializado = false;
}
