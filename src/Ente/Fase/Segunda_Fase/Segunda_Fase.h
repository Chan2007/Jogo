//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_SEGUNDA_FASE_H
#define JOGO_SEGUNDA_FASE_H
#include "jogo.h"
#include "Ente/Fase/Fase.h"
#include "Gerenciador/Gerenciador_Audio/Gerenciador_Audio.h"

namespace Fases {
    class Segunda_Fase: public Fase {
        private:
            const int maxChefoes;

            void processarEventos(const sf::Event &evento);
            void renderizar(sf::RenderWindow &janela);

            Gerenciadores::Gerenciador_Textura gerenciadorTextura;
            Animador animadorFase;
            Jogo jogo;

            std::string diretorio_Frames_Fase;
        protected:
            void criarObstaculos(){}
            void criarInimigos(){}
            void criarProjeteis(){}
            void criarCenario();

            void criarObstMedios(){}
            void criarChefoes(){}
        public:
            Segunda_Fase();
            ~Segunda_Fase();
            void executar();

    };
} // Fases

#endif //JOGO_SEGUNDA_FASE_H