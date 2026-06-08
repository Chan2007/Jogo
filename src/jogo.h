#ifndef JOGO_H
#define JOGO_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <windows.h>
#include "Gerenciador/Gerenciador_Audio/Gerenciador_Audio.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Gerenciador_Textura.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Obstaculo/Plataforma/Plataforma.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Grafico.h"


namespace Fases {
    class Fase;
}

namespace Gerenciadores {
    class Gerenciador_Gravidade;
}

class Jogo {
    private:
        Fases::Fase* faseAtual;
        Personagens::Jogador* jogador;
        Gerenciadores::Gerenciador_Textura gerenciadorTextura;
        Gerenciadores::Gerenciador_Audio& gerenciadorAudio;
        Gerenciadores::Gerenciador_Grafico& gerenciadorGrafico;

        sf::Event event{};
        sf::Clock relogio;

        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        std::vector<sf::Text> opcoesMenu;
        bool inicializado;
        bool musicaLigada;
        std::size_t opcaoSelecionada;

        static void processarEventos();

        void executarOpcaoMenu();

    public:
        Jogo();
        ~Jogo();
        enum EstadoTela {
            TelaMenu,
            TelaFase1,
            TelaFase2,
            TelaPausa
        };
        void inicializar();
        void mudarEstado(EstadoTela novoEstado);

        void setMusica(bool ligada);
        void setVolume(float volume) const;
        bool trocarMusica(int fase) const;

        void setJogador(Personagens::Jogador* j) {jogador = j;}
        Personagens::Jogador* getJogador() const {
            if(jogador) return jogador;
            return NULL;
        }

        static bool estaAberto() {
            return Gerenciadores::Gerenciador_Grafico::getGerenciador().isOpen();
        }
        void executar();

        static void sementear() {
            rand();
            Sleep(100); time_t t;
            srand(static_cast<unsigned>(time(&t)));
            rand();
        }
    private:
        EstadoTela estadoTela;
};

#endif // JOGO_H