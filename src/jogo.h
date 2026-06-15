#ifndef JOGO_H
#define JOGO_H

#include "Gerenciador/Gerenciador_Audio/Gerenciador_Audio.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Gerenciador_Textura.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Grafico.h"


namespace Fases {
    class Fase;
}

namespace Gerenciadores {
    class Gerenciador_Gravidade;
}

class Jogo {
    private:
        static Jogo* jogo;
        Fases::Fase* faseAtual;
        Personagens::Jogador* jogador1;
        Personagens::Jogador* jogador2;
        Gerenciadores::Gerenciador_Textura gerenciadorTextura;
        Gerenciadores::Gerenciador_Audio& gerenciadorAudio;
        Gerenciadores::Gerenciador_Grafico& gerenciadorGrafico;

        sf::Clock relogio;

        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        static bool inicializado;
        bool musicaLigada;

    Jogo();
    public:
        static Jogo* getJogo() {
            if (!jogo) jogo = new Jogo();
            return jogo;
        }
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

        void setJogador1(Personagens::Jogador* j1) {jogador1 = j1;}
        void setJogador2(Personagens::Jogador* j2) {jogador2 = j2;}
        Personagens::Jogador* getJogador1() const { return jogador1; }
        Personagens::Jogador* getJogador2() const { return jogador2; }

        static bool estaAberto() {
            return inicializado && Gerenciadores::Gerenciador_Grafico::getGerenciador().isOpen();
        }
        void executar();

    private:
        EstadoTela estadoTela;
};

#endif // JOGO_H