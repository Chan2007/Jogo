//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_FASE_H
#define JOGO_FASE_H

#include "jogo.h"
#include "Ente/Ente.h"
#include "Gerenciador/Gerenciador_Audio/Gerenciador_Audio.h"
#include "Gerenciador/Gerenciador_Input/Gerenciador_Input.h"
#include "Listas/ListaEntidades.h"

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
            virtual void criarProjeteis() = 0;
            virtual void criarCenario() = 0;

            void criarInimFaceis();
            void criarJogadores();

            void definirLimitesJanela();

            void criarPlataformas();

            Jogo* jogo;
            Gerenciadores::Gerenciador_Gravidade& gerenciadorGravidade;
            Gerenciadores::Gerenciador_Colisao* gerenciadorColisao;
            Gerenciadores::Gerenciador_Audio& audio;
            Gerenciadores::Gerenciador_Input& gerenciadorInput;
            std::string diretorio_Audio;

        public:
            explicit Fase();
            virtual ~Fase();
            void setMusica(const bool ligada) const { audio.ativarMusica(ligada);}
            void setVolume(const float volume) const { audio.setVolume(volume);}
            bool tocandoMusica() const { return audio.isPlaying();}

            bool trocarMusica(int fase) const;
            virtual void processarEventos(const sf::Event &evento) = 0;
            virtual void executar() = 0;
            virtual void renderizar() = 0;

    };
} // Fases

#endif //JOGO_FASE_H