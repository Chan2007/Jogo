#include "jogo.h"
#include "Ente/Fase/Fase.h"
#include "Ente/Fase/Primeira_Fase/Primeira_Fase.h"
#include "Ente/Fase/Segunda_Fase/Segunda_Fase.h"

Jogo::Jogo() : faseAtual(NULL),
               jogador(),
               gerenciadorTextura(),
               gerenciadorAudio(Gerenciadores::Gerenciador_Audio::getGerenciador()),
               gerenciadorGrafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
               opcoesMenu(),
               inicializado(false),
               musicaLigada(true),
               opcaoSelecionada(0),
               estadoTela(TelaMenu)
{}

Jogo::~Jogo() {
    if (gerenciadorGrafico.isOpen()) gerenciadorGrafico.close();
    if (faseAtual) delete faseAtual;
    inicializado = false;
}

void Jogo::inicializar() {
    if (inicializado && gerenciadorGrafico.getJanela().isOpen()) return;

    sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();

    gerenciadorGrafico.setSize(videoMode);

    gerenciadorGrafico.getJanela().create(videoMode, "Jogo cHAN E Huang", sf::Style::Default);
    gerenciadorGrafico.getJanela().setFramerateLimit(60);

    inicializado = true;
    estadoTela = TelaFase1;
    relogio.restart();
}
void Jogo::mudarEstado(const EstadoTela novoEstado) {
    estadoTela = novoEstado;

    if (faseAtual) {
        delete faseAtual;
        faseAtual = NULL;
    }

    switch (novoEstado) {
        case TelaFase1:
            faseAtual = new Fases::Primeira_Fase();
            faseAtual->executar();
            break;
        case TelaFase2:
            faseAtual = new Fases::Segunda_Fase();
            break;
        default:
            break;
    }
}

void Jogo::executar() {
    sf::RenderWindow& janela = gerenciadorGrafico.getJanela();

    while (janela.isOpen()) {
        // Calcula dt (tempo que levou o frame anterior)
        float dt = relogio.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f; // Proteção contra travamentos abruptos

        // Capturar eventos
        sf::Event evento;
        while (janela.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                janela.close();
            }

            // Se estivermos em uma fase, repassa os eventos para ela (ex: inputs)
            if (estadoTela == TelaFase1 && faseAtual != NULL)
                faseAtual->processarEventos(evento);
        }

        // Atualização da lógica da janela
        if (estadoTela == TelaFase1 && faseAtual != NULL) {
            faseAtual->executar();
            // IMPORTANTE!!! Se o(s) jogador(es) mudou(aram) de fase, o Jogo que tem que decidir o que fazer
        }
        else if (estadoTela == TelaMenu)
            executarOpcaoMenu();

        // Renderização
        janela.clear();

        if (estadoTela == TelaFase1 && faseAtual != NULL) {
            faseAtual->renderizar(janela);
        }
        janela.display();
    }
    gerenciadorAudio.stop();
    inicializado = false;
    if (faseAtual) {
        delete faseAtual;
        faseAtual = NULL;
    }
}

void Jogo::processarEventos() {
    sf::Event evento;

    sf::RenderWindow& janela = Gerenciadores::Gerenciador_Grafico::getGerenciador().getJanela();

    while (janela.pollEvent(evento)) {

        if (evento.type == sf::Event::Closed) {
            janela.close();
        }

        if (evento.type == sf::Event::Resized) {

            sf::FloatRect areaVisivel(0.f, 0.f, sf::VideoMode().getDesktopMode().width, sf::VideoMode().getDesktopMode().height);
            janela.setView(sf::View(areaVisivel));
        }

    }
}

void Jogo::executarOpcaoMenu() {
    switch (opcaoSelecionada) {
        case 0: // Iniciar jogo
            mudarEstado(TelaFase1);
            break;
        case 1: // Configurações
            // Tela de configurações
            break;
        case 2: // Sair
            estadoTela = TelaPausa;
            break;
        default:
            break;
    }
}

void Jogo::setMusica(const bool ligada) {
    // Altera o estado diretamente no Gerenciador Global
    gerenciadorAudio.ativarMusica(ligada);
    musicaLigada = ligada;
    // Propaga a mudança para a fase atual se ela estiver rodando
    if (inicializado && faseAtual) {
        faseAtual->setMusica(ligada);
    }
}
void Jogo::setVolume(const float volume) const {
    gerenciadorAudio.setVolume(volume);

    if (inicializado && faseAtual)
        faseAtual->setVolume(volume);
}

bool Jogo::trocarMusica(const int fase) const {
    if (!faseAtual) return false;
    return faseAtual->trocarMusica(fase);
}
