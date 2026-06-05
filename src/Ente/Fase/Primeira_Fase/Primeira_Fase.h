//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_PRIMEIRA_FASE_H
#define JOGO_PRIMEIRA_FASE_H
#include "Ente/Fase/Fase.h"

namespace Fases {
    class Primeira_Fase: public Fase {
        private:
            sf::Clock relogio;
            sf::RenderWindow janela;
            sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
            const int maxInimigos;
        protected:
            void criarPlataformas();
            void criarObstaculos();
            void criarInimigos();
            void criarProjetil();
            void criarCenario();

            void criarObstFaceis();
            void criarInimMedios();
        public:
            Primeira_Fase();
            ~Primeira_Fase();
        };
} // Fases

#endif //JOGO_PRIMEIRA_FASE_H