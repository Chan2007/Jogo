#ifndef JOGO_H
#define JOGO_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Animador_Fundo/animador_fundo.h"
#include "Audio/Audio.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Gerenciador_Textura.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Entidade/Personagem/Jogador/Jogador.h"

namespace Gerenciadores {
    class Gerenciador_Gravidade;
}

class Jogo{
    private:
        enum EstadoTela {
            TelaMenu,
            TelaGameplay
        };

        int totalFrames;
        int max_intercalarFrames;
        int intercalarFrames;
        int Frames;

        Gerenciador_Textura gerenciadorTextura;
        Animador_Fundo bgAnimation;
        std::string diretorio;
        std::string diretorio_Frame;

        Audio bgMusic;
        std::string diretorio_Audio;
        std::string diretorio_Musica;
        std::string diretorio_Fonte;

        sf::Event event{};
        sf::Clock relogio_fisica;
        Personagens::Jogador* jogador{};
        Gerenciadores::Gerenciador_Gravidade* gerenciadorGravidade{};
        sf::RenderWindow m_window;
        sf::Font menuFont;
        sf::Text tituloText;
        std::vector<sf::Text> menuOptions;
        sf::RectangleShape menuPanel;
        bool inicializado;
        bool menuPronto;
        bool musicaLigada;
        EstadoTela estadoTela;
        std::size_t opcaoSelecionada;

        bool carregarRecursos();
        bool configurarMenu();
        void processarEventos();
        void processarEventoMenu(const sf::Event& evento);
        void processarEventoGameplay(const sf::Event& evento);
        void atualizarMenuVisual();
        void executarOpcaoMenu();
        void desenharMenu();
        void desenharGameplay();

    public:
        Jogo();
        ~Jogo();

        bool inicializar();
        void iniciarGameplay();
        void setMusicaLigada(bool ligada);
        void setVolumeMusica(float volume);
        bool musicaEstaLigada() const;
        void executar();
        void atualizar();
        bool estaAberto() const;
        void fechar();
};

#endif // JOGO_H