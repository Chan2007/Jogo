#ifndef JOGO_H
#define JOGO_H

#include <string>

#include "Ente/Fase/Fase.h"
#include "Gerenciador/Gerenciador_Estado/Caretaker.h"


class Pausa;

namespace Fases {
    class Fase;
}

namespace Gerenciadores {
    class Gerenciador_Estado;
    class Gerenciador_Gravidade;
}

class Jogo {
    private:
        static Jogo* jogo;
        Fases::Fase* faseAtual;
        Caretaker* zelador;
        Pausa* telaPausa;

        Personagens::Jogador* jogador1;
        Personagens::Jogador* jogador2;
        Gerenciadores::Gerenciador_Textura gerenciadorTextura;
        Gerenciadores::Gerenciador_Audio& gerenciadorAudio;
        Gerenciadores::Gerenciador_Grafico& gerenciadorGrafico;
        Gerenciadores::Gerenciador_Estado& gerenciadorEstado;

        sf::Clock relogio;

        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

        bool multiplayer;
        static bool inicializado;
        bool musicaLigada;
        bool carregandoSave;
        Jogo();
    public:
        enum EstadoTela {
            TelaMenu,
            TelaFase1,
            TelaFase2,
            TelaPausa
        };
        static Jogo* getJogo() {
            if (!jogo) jogo = new Jogo();
            return jogo;
        }
        EstadoTela getEstadoTela() const { return estadoTela; }
        ~Jogo();

        void inicializar();
        void mudarEstado(EstadoTela novoEstado);

        void setMusica(bool ligada);
        void setVolume(float volume) const;

        void setJogador1(Personagens::Jogador* j1) {jogador1 = j1;}
        void setJogador2(Personagens::Jogador* j2) {jogador2 = j2;}
        void setJogador2Ativo(bool ativo) {
            if (ativo) multiplayer = true;
            else multiplayer = false;
        }
        int getJogador2Ativo() const {
            return multiplayer;
        }
        Personagens::Jogador* getJogador1() const { return jogador1; }
        Personagens::Jogador* getJogador2() const { return jogador2; }

        static bool estaAberto() {
            return inicializado && Gerenciadores::Gerenciador_Grafico::getGerenciador().isOpen();
        }
        void executar();

        bool carregarJogo(const std::string& caminho);
        bool salvarJogoAtual(const std::string& caminho);

        void setCarregandoSave(bool carregando) { carregandoSave = carregando; }
        bool getCarregandoSave() const { return carregandoSave; }
        void conferirJogadores();
        void solicitarPausa();
        void processarCliqueBotaoPausa(int indice);

    private:
        EstadoTela estadoTela;
        EstadoTela telaAnterior;
};

#endif // JOGO_H