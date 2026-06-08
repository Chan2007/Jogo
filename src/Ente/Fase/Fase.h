//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_FASE_H
#define JOGO_FASE_H

#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Gerenciador_Textura.h"
#include "Gerenciador/Gerenciador_Audio/Gerenciador_Audio.h"
#include "Listas/ListaEntidades.h"

namespace Listas {
    class ListaEntidades;
}
namespace Fases {
    class Fase: public Ente {
        protected:
            Listas::ListaEntidades LEntidades;

            sf::VideoMode desktop;

            virtual void criarObstaculos() = 0;
            virtual void criarInimigos() = 0;
            virtual void criarProjeteis() = 0;
            virtual void criarCenario() = 0;

            void criarInimFaceis();
            void criarPlataformas();

            Personagens::Jogador jogador;
            Gerenciadores::Gerenciador_Gravidade& gerenciadorGravidade;
            Gerenciadores::Gerenciador_Colisao& gerenciadorColisao;
            Gerenciadores::Gerenciador_Audio& audio;
            std::string diretorio_Audio;

        public:
            explicit Fase();
            virtual ~Fase();
            virtual void executar(){};
            void setMusica(const bool ligada) const { audio.ativarMusica(ligada);}
            void setVolume(const float volume) const { audio.setVolume(volume);}
            bool tocandoMusica() const { return audio.isPlaying();}

            bool trocarMusica(int fase) const;
            virtual void processarEventos(const sf::Event &evento) = 0;
            virtual void atualizar(float dt) = 0;
            virtual void renderizar(sf::RenderWindow &janela) = 0;

    };
} // Fases

#endif //JOGO_FASE_H