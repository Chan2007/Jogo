//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_FASE_H
#define JOGO_FASE_H

#include <iosfwd>
#include <QStringList>
#include <string>

#include "Ente/Ente.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Gerenciador/Gerenciador_Audio/Gerenciador_Audio.h"
#include "Listas/ListaEntidades.h"

class Memento;
class Jogo;

namespace Gerenciadores {
    class Caretaker;
    class Gerenciador_Input;
    class Gerenciador_Colisao;
    class Gerenciador_Grafico;
    class Gerenciador_Gravidade;
    class Gerenciador_Audio;
}
namespace Entidades {
    class Entidade;
}
namespace Personagens {
    class Personagem;
    class Jogador;
    class Inimigo;
}
namespace Obstaculos {
    class Obstaculo;
}
namespace Listas {
    class ListaEntidades;
}
namespace Fases {
    class Fase: public Ente {
        private:
            bool verificarLimitesJanela(Entidades::Entidade *entidade);
        protected:
            Jogo* jogo;
            Listas::ListaEntidades LEntidades;
            sf::VideoMode tamanhoJanela;
            std::string diretorio_Audio;

            virtual void criarObstaculos() = 0;
            virtual void criarInimigos() = 0;
            virtual void criarCenario() = 0;

            Personagens::Jogador* jogador1;
            Personagens::Jogador* jogador2;
            bool multiplayer;
            Gerenciadores::Caretaker* CaretakerFase;

            void criarInimFaceis();
            void criarPlataformas();

            void definirLimitesJanela();

            void limparJogo();
            void registrarEntidade(Entidades::Entidade* e);
            void registrarJogador(Personagens::Jogador* j);
            bool lerDadosEntidade(std::istream& entrada, Entidades::Entidade* e);
            bool lerDadosPersonagem(std::istream& entrada, Personagens::Personagem* p);
            bool lerDadosInimigo(std::istream& entrada, Personagens::Inimigo* i);
            bool lerDadosObstaculos(std::istream& entrada, Obstaculos::Obstaculo* o);
            bool carregarLinhaEntidade(const std::string& linha);

            Gerenciadores::Gerenciador_Gravidade& gerenciadorGravidade;
            Gerenciadores::Gerenciador_Colisao* gerenciadorColisao;
            Gerenciadores::Gerenciador_Audio& gerenciadorAudio;
            Gerenciadores::Gerenciador_Input& gerenciadorInput;


            class FaseMemento: public EnteMemento {
                private:
                    Listas::ListaEntidades LEntidadesMemento;
                    sf::VideoMode tamanhoJanelaMemento;
                    std::string diretorio_AudioMemento;
                protected:
                    explicit FaseMemento(const Fase& f) : EnteMemento(f),
                    LEntidadesMemento(f.LEntidades), tamanhoJanelaMemento(f.tamanhoJanela),
                    diretorio_AudioMemento(f.diretorio_Audio) {}
                    virtual ~FaseMemento() {}
                    friend class Fase;
            };

        public:
            Fase(Jogo* pJogo, const std::string& nomeJ1 = "", const QString& campeaoJ1 = "",
                 const std::string& nomeJ2 = "", const QString& campeaoJ2 = "", bool jogador2Ativo = false);
            virtual ~Fase();

            static const QStringList CAMPEOES;
            static Personagens::Jogador::Campeao defCampeao(const QString& texto);
            static Personagens::Jogador::Campeao randomCampeao();
            void criarJogadores(const std::string& nomeJ1, const QString& campeaoJ1, const std::string& nomeJ2, const QString& campeaoJ2, bool m);

            virtual Gerenciadores::Memento* salvarMemento() const;
            virtual void restaurarMemento(const Gerenciadores::Memento* memento);

            void setMusica(const bool ligada) const { gerenciadorAudio.ativarMusica(ligada);}
            void setVolume(const float volume) const { gerenciadorAudio.setVolume(volume);}
            bool tocandoMusica() const { return gerenciadorAudio.isPlaying();}

            bool trocarMusica(int fase) const;
            virtual void processarEventos(const sf::Event &evento) = 0;
            virtual void executar() = 0;
            virtual void desenhar() = 0;

            bool salvarJogo(const std::string& caminho, int numeroFase);
            bool carregarJogo(const std::string& caminho);

            Personagens::Jogador* getJogador1() const { return jogador1; }
            Personagens::Jogador* getJogador2() const { return jogador2; }
            bool getMultiplayer() const { return multiplayer; }
    };
} // Fases

#endif //JOGO_FASE_H