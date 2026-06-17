//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_FASE_H
#define JOGO_FASE_H

#include "Ente/Ente.h"
#include "Gerenciador/Gerenciador_Audio/Gerenciador_Audio.h"
#include "Listas/ListaEntidades.h"

class Jogo;

namespace Gerenciadores {
    class Gerenciador_Input;
    class Gerenciador_Colisao;
    class Gerenciador_Grafico;
    class Gerenciador_Gravidade;
    class Gerenciador_Audio;
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

            sf::Vector2u tamanhoJanela;

            virtual void criarObstaculos() = 0;
            virtual void criarInimigos() = 0;
            virtual void criarCenario() = 0;

            void criarInimFaceis();
            void criarJogadores();

            void definirLimitesJanela();

            void criarPlataformas();

            Jogo* jogo;
            Gerenciadores::Gerenciador_Gravidade& gerenciadorGravidade;
            Gerenciadores::Gerenciador_Colisao* gerenciadorColisao;
            Gerenciadores::Gerenciador_Audio& gerenciadorAudio;
            Gerenciadores::Gerenciador_Input& gerenciadorInput;
            std::string diretorio_Audio;

        public:
            explicit Fase();
            virtual ~Fase();
            void setMusica(const bool ligada) const { gerenciadorAudio.ativarMusica(ligada);}
            void setVolume(const float volume) const { gerenciadorAudio.setVolume(volume);}
            bool tocandoMusica() const { return gerenciadorAudio.isPlaying();}

            bool trocarMusica(int fase) const;
            virtual void processarEventos(const sf::Event &evento) = 0;
            virtual void executar() = 0;
            virtual void desenhar() = 0;

    };
} // Fases

#endif //JOGO_FASE_H