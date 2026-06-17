#include "jogo.h"

#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Fase/Primeira_Fase/Primeira_Fase.h"
#include "Ente/Fase/Segunda_Fase/Segunda_Fase.h"

Jogo* Jogo::jogo = NULL;
bool Jogo::inicializado = false;

Jogo::Jogo() : faseAtual(NULL),
               jogador1(),
               jogador2(),
               gerenciadorTextura(),
               gerenciadorAudio(Gerenciadores::Gerenciador_Audio::getGerenciador()),
               gerenciadorGrafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
               multiplayer(false),
               musicaLigada(true),
               estadoTela(TelaMenu)
{
    jogador1 = new Personagens::Jogador(sf::Vector2f(50.0, 50.0), sf::Color::Green);
    jogador2 = new Personagens::Jogador(sf::Vector2f(
            desktop.width - 800.0, 50), sf::Color::Blue);

}

Jogo::~Jogo() {
    if (gerenciadorGrafico.getJanela().isOpen()) {
        gerenciadorGrafico.getJanela().close();
    }
    if (faseAtual) {
        delete faseAtual;
        faseAtual = NULL;
    }
    if (jogador1) {
        delete jogador1;
        jogador1 = NULL;
    }
    if (jogador2) {
        delete jogador2;
        jogador2 = NULL;
    }
    inicializado = false;
}

void Jogo::inicializar() {
    if (inicializado && gerenciadorGrafico.getJanela().isOpen()) return;

    sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();

    gerenciadorGrafico.setSize(videoMode);

    gerenciadorGrafico.getJanela().create(videoMode, "Jogo cHAN E Huang", sf::Style::Default);
    gerenciadorGrafico.getJanela().setFramerateLimit(60);

    inicializado = true;
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
            // Garante que a janela SFML exista e esteja configurada
            inicializar();
            faseAtual = new Fases::Primeira_Fase();
            break;
        case TelaFase2:
            inicializar();
            faseAtual = new Fases::Segunda_Fase();
            break;
        default:
            break;
    }
}
void Jogo::executar() {
    sf::RenderWindow& janela = gerenciadorGrafico.getJanela();

    while (janela.isOpen()) {
        // Capturar eventos
        sf::Event evento;
        while (janela.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) janela.close();

            // Se estiver em uma fase, repassa os eventos para ela (ex: inputs)
            if (faseAtual) faseAtual->processarEventos(evento);
        }

        // Atualização da lógica da janela

        // IMPORTANTE!!! Se o(s) jogador(es) mudou(aram) de fase, o Jogo que tem que decidir o que fazer
        if (faseAtual) faseAtual->executar();

        // Renderização
        janela.clear();
        if (faseAtual) faseAtual->desenhar();
        janela.display();
    }
    gerenciadorAudio.stop();
    inicializado = false;
    if (faseAtual) {
        delete faseAtual;
        faseAtual = NULL;
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