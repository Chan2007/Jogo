//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_SEGUNDA_FASE_H
#define JOGO_SEGUNDA_FASE_H
#include "jogo.h"
#include "Ente/Fase/Fase.h"

namespace Fases {
    class Segunda_Fase: public Fase {
        private:
            const int maxChefoes;

            void processarEventos(const sf::Event &evento);
            void desenhar();

            std::string diretorio_Frames_Fase;
        protected:
            void criarObstaculos() {
                criarObstDificeis();

            }
            void criarInimigos() {
                criarChefoes();
            }
            void criarProjeteis();
            void criarCenario();

            void criarObstDificeis();
            void criarChefoes();
        public:
            Segunda_Fase();
            ~Segunda_Fase();
            void executar();

    };
} // Fases

#endif //JOGO_SEGUNDA_FASE_H