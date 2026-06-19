#include "jogo.h"

#include <fstream>
#include <sstream>
#include <iostream>

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
               estadoTela(TelaMenu),
               carregandoSave(false)
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

        sf::Event evento;
        while (janela.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) janela.close();

            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::F5) {
                    salvarJogoAtual("save.txt");
                    continue;
                }

                if (evento.key.code == sf::Keyboard::F9) {
                    carregarJogo("save.txt");
                    continue;
                }
            }

            // Se estiver em uma fase, repassa os eventos para ela (ex: inputs)
            if (faseAtual) faseAtual->processarEventos(evento);
        }

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

    if (inicializado && faseAtual) {
        faseAtual->setMusica(ligada);
    }
}
void Jogo::setVolume(const float volume) const {
    gerenciadorAudio.setVolume(volume);

    if (inicializado && faseAtual)
        faseAtual->setVolume(volume);
}

bool Jogo::carregarJogo(const std::string& caminho) {
    std::ifstream arquivo(caminho.c_str());

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir save: " << caminho << std::endl;
        return false;
    }

    std::string cabecalho;
    std::getline(arquivo, cabecalho);

    if (cabecalho != "SAVE_PRIMEIRO_JOGO_V1") {
        std::cerr << "Arquivo de save invalido." << std::endl;
        arquivo.close();
        return false;
    }

    std::string linhaFase;
    std::getline(arquivo, linhaFase);

    std::istringstream iss(linhaFase);

    std::string palavraFase;
    int numeroFase = 0;

    iss >> palavraFase >> numeroFase;

    arquivo.close();

    if (palavraFase != "FASE") {
        std::cerr << "Linha de fase invalida no save." << std::endl;
        return false;
    }

    EstadoTela estadoDoSave;

    if (numeroFase == 1) {
        estadoDoSave = TelaFase1;
    }
    else if (numeroFase == 2) {
        estadoDoSave = TelaFase2;
    }
    else {
        std::cerr << "Numero de fase invalido: " << numeroFase << std::endl;
        return false;
    }
    setCarregandoSave(true);

    if (faseAtual == NULL || estadoTela != estadoDoSave) {
        mudarEstado(estadoDoSave);
    }

    if (!faseAtual) {
        std::cerr << "Erro: faseAtual nao foi criada." << std::endl;
        setCarregandoSave(false);
        return false;
    }

    bool resultado = faseAtual->carregarJogo(caminho);
    setCarregandoSave(false);

    return resultado;
}

bool Jogo::salvarJogoAtual(const std::string& caminho) {
    if (!faseAtual) {
        std::cerr << "Nenhuma fase atual para salvar." << std::endl;
        return false;
    }

    int numeroFase = 0;

    if (estadoTela == TelaFase1) {
        numeroFase = 1;
    }
    else if (estadoTela == TelaFase2) {
        numeroFase = 2;
    }
    else {
        std::cerr << "Estado atual nao e uma fase." << std::endl;
        return false;
    }

    return faseAtual->salvarJogo(caminho, numeroFase);
}