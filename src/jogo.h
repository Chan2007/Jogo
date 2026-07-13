#ifndef JOGO_H
#define JOGO_H

#include <string>

#include "Ente/Entidade/Personagem/Inimigo/Inimigo_Medio/Azulo.h"
#include "Ente/Fase/Fase.h"

class Pausa;

namespace Fases {
    class Fase;
}

namespace Gerenciadores {
    class Gerenciador_Estado;
}

class Jogo: public Gerenciadores::Observador_Input {
    private:
        Fases::Fase* faseAtual;
        Pausa* telaPausa;

        Gerenciadores::Gerenciador_Textura gerenciadorTextura;
        Gerenciadores::Gerenciador_Audio& gerenciadorAudio;
        Gerenciadores::Gerenciador_Grafico& gerenciadorGrafico;
        Gerenciadores::Gerenciador_Estado& gerenciadorEstado;
        Gerenciadores::Gerenciador_Input& gerenciadorInput;

        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

        static bool inicializado;
        bool musicaLigada;
        bool carregandoSave;

    public:
        enum EstadoTela {
            TelaMenu,
            TelaFase1,
            TelaFase2,
            TelaPausa
        };
        EstadoTela estadoTela;
        EstadoTela telaAnterior;

        EstadoTela getEstadoTela() const { return estadoTela; }
        Jogo();
        ~Jogo();

        void inicializar();
        void mudarEstado(EstadoTela novoEstado, const std::string& nomeJ1 = "", const QString& campeaoJ1 = "",
                         const std::string& nomeJ2 = "", const QString& campeaoJ2 = "", bool multiplayer = false);
        void setMusica(bool ligada);
        void setVolume(float volume) const;

        static bool estaAberto() { return inicializado && Gerenciadores::Gerenciador_Grafico::getGerenciador().isOpen(); }
        void executar();

        bool carregarJogo(const std::string& caminho);
        bool salvarJogoAtual(const std::string& caminho);

        void setCarregandoSave(bool carregando) { carregandoSave = carregando; }
        bool getCarregandoSave() const { return carregandoSave; }

        void gerenciarPausa();
        void onClickPausa(int indice);
        void aoApertarTecla(const Gerenciadores::Tecla& evento);
};

#endif // JOGO_H