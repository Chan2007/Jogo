//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_FASE_H
#define JOGO_FASE_H

#include <iosfwd>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Ente/Ente.h"
#include "Gerenciador/Gerenciador_Audio/Gerenciador_Audio.h"
#include "Listas/ListaEntidades.h"

class Memento;
class Jogo;

namespace Gerenciadores {
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
}
namespace Obstaculos {
    class Obstaculo;
}
namespace Personagens {
    class Jogador;
}
namespace Personagens {
    class Inimigo;
}
namespace Listas {
    class ListaEntidades;
}
namespace Fases {
    class Fase: public Ente {
        private:
            bool verificarLimitesJanela(Entidades::Entidade *entidade);
        protected:
            Listas::ListaEntidades LEntidades;
            sf::VideoMode tamanhoJanela;
            std::string diretorio_Audio;

            virtual void criarObstaculos() = 0;
            virtual void criarInimigos() = 0;
            virtual void criarCenario() = 0;

            void criarInimFaceis();
            void criarJogadores();

            void definirLimitesJanela();

            void criarPlataformas();

            void limparJogo();
            void registrarEntidade(Entidades::Entidade* e);
            void registrarJogador(Personagens::Jogador* j);
            bool lerDadosEntidade(std::istream& entrada, Entidades::Entidade* e);
            bool lerDadosPersonagem(std::istream& entrada, Personagens::Personagem* p);
            bool lerDadosInimigo(std::istream& entrada, Personagens::Inimigo* i);
            bool lerDadosObstaculos(std::istream& entrada, Obstaculos::Obstaculo* o);
            bool carregarLinhaEntidade(const std::string& linha);

            Jogo* jogo;
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
            Fase();
            virtual ~Fase();

            virtual Memento* salvarMemento() const;
            virtual void restaurarMemento(const Memento* memento);

            void setMusica(const bool ligada) const { gerenciadorAudio.ativarMusica(ligada);}
            void setVolume(const float volume) const { gerenciadorAudio.setVolume(volume);}
            bool tocandoMusica() const { return gerenciadorAudio.isPlaying();}

            bool trocarMusica(int fase) const;
            virtual void processarEventos(const sf::Event &evento) = 0;
            virtual void executar() = 0;
            virtual void desenhar() = 0;

            bool salvarJogo(const std::string& caminho, int numeroFase);
            bool carregarJogo(const std::string& caminho);
    };
} // Fases

#endif //JOGO_FASE_H