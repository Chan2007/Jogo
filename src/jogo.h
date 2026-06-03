#ifndef JOGO_H
#define JOGO_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <windows.h>
#include "Sistema/UI/animador_fundo.h"
#include "Sistema/Audio/Audio.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Gerenciador_Textura.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Obstaculo/Plataforma/Plataforma.h"


namespace Gerenciadores {
    class Gerenciador_Gravidade;
}

class Jogo {
private:
    enum EstadoTela {
        TelaMenu,
        TelaFase,
        TelaPausa
    };

    Gerenciador_Textura gerenciadorTextura;
    Animador_Fundo animador;
    Audio musica;

    std::string diretorio_Frames_Fase1;
    std::string diretorio_Frames_Fase2;
    std::string diretorio_Audio;

    sf::Event event{};
    sf::Clock relogio_fisica;

    Personagens::Jogador* jogador;
    Gerenciadores::Gerenciador_Gravidade* gerenciadorGravidade;
    Listas::ListaEntidades listaEntidades;

    sf::RenderWindow janela;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::Font fonte;
    sf::Text titulo;
    std::vector<sf::Text> opcoesMenu;
    sf::RectangleShape painelMenu;
    sf::Clock relogio;
    bool inicializado;
    bool menuPronto;
    bool musicaLigada;
    EstadoTela estadoTela;
    std::size_t opcaoSelecionada;

    bool carregarInimigos();
    bool carregarJogadores();
    bool carregarObstaculos();
    bool carregarProjeteis();
    bool carregarMultimidia();

    void processarEventos();

    void processarEventoPausa(const sf::Event& evento) {}; // TODO
    void processarEventoJogo(const sf::Event& evento);
    void executarOpcaoMenu();
    void desenharMenu();
    void desenharFase();

public:
    Jogo();
    ~Jogo();

    bool inicializar();
    void iniciarFase();
    void setMusica(bool ligada);
    void setVolume(float volume);
    bool tocandoMusica() const;
    bool trocarMusica(int fase);
    void atualizar();
    bool estaAberto() const;
    void fechar();

    void executar();

    static void sementear() {
        rand();
        Sleep(100); time_t t;
        srand((unsigned)time(&t));
        rand();
    }
};

#endif // JOGO_H