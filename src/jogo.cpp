#include "jogo.h"
#include <iostream>

#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Grafico.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

Jogo::Jogo() :
    gerenciadorTextura(),
    animadorFase1(&gerenciadorTextura),
    animadorFase2(&gerenciadorTextura),
    jogador(),
    gerenciadorGravidade(NULL),
    listaEntidades(),
    janela(),
    opcoesMenu(),
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
        if (audio.loadMusic(caminho_musica1)) {
            audio.setVolume(50.0f);
            audio.setLoop(true);
            if (musicaLigada)
                audio.play();
        }
        else {std::cerr << "Falha ao carregar a música da fase 1." << std::endl; return false;}
    }
    if (!diretorio_Audio.empty()) {
        const std::string caminho_musica2 =
            Encontrar_Caminho::concatenarEnderecos(diretorio_Audio, "Down-to-a-Dusty-Plain.ogg");
        if (audio.loadMusic(caminho_musica2)) {
            audio.setVolume(50.0f);
            audio.setLoop(true);
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
    audio.stop();

    std::string nomeArquivo = "";
    if (fase == 1) {
        nomeArquivo = "Aurora_s-Theme.ogg";
    } else if (fase == 2) {
        nomeArquivo = "Down-to-a-Dusty-Plain.ogg";
    }

    const std::string caminho_musica = Encontrar_Caminho::concatenarEnderecos(
                                        diretorio_Audio, nomeArquivo);

    // 2. Carrega a nova música na mesma variável
    if (audio.loadMusic(caminho_musica)) {
        audio.setVolume(50.0f);
        audio.setLoop(true);

        if (musicaLigada) {
            audio.play();
        }
        return true;
    }
    else {std::cerr << "Falha ao carregar a música da fase " << fase << std::endl; return false;}

}



void Jogo::criarJogadores() {

    jogador = Personagens::Jogador();
    Gerenciadores::Gerenciador_Colisao::getInstancia().incluirEntidade(&jogador);

    // Mudar futuramente para escolha do jogador
    jogador.setCampeao(Personagens::CAMPEAO_NAAFIRI);
    jogador.setPosicao(sf::Vector2f(100.f, 800.f));
    std::cout << "Boneco criado: " << jogador.getNome() << std::endl;

    gerenciadorGravidade = &Gerenciadores::Gerenciador_Gravidade::getInstancia();
    if (gerenciadorGravidade) {
        gerenciadorGravidade->aplicarGravidade(&jogador, true);
        jogador.setGerenciadorGravidade(gerenciadorGravidade);
    }
    std::cerr << "Falha ao aplicar gravidade."  << std::endl;


}

bool Jogo::inicializar() {
    if (inicializado && janela.isOpen()) return true;

    if (desktop.isValid())
        janela.create(desktop, "Jogo", sf::Style::Default);
    else
        janela.create(sf::VideoMode(desktop.width, desktop.height), "Jogo", sf::Style::Default);

    janela.setFramerateLimit(60);

    if (!carregarMultimidia()) {
        janela.close();
        return false;
    }

    inicializado = true;
    estadoTela = TelaFase;
    relogio.restart();
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
        if (musicaLigada) audio.play();
        else audio.pause();
    }
}

void Jogo::setVolume(float volume) { audio.setVolume(volume);}

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
    float dt = relogio.restart().asSeconds();

    if (dt > 0.1f) { dt = 0.1f; }

    animadorFase1.update();
    janela.clear(sf::Color::Black);
    animadorFase1.draw(janela);

    listaEntidades.desenharTodas(janela);
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
