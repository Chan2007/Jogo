//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_SEGUNDA_FASE_H
#define JOGO_SEGUNDA_FASE_H
#include "Ente/Fase/Fase.h"

namespace Fases {
    class Segunda_Fase: public Fase {
        private:
            const int maxInimigos;
        protected:
            void criarPlataformas();
            void criarObstaculos();
            void criarInimigos();
            void criarProjetil();
        public:
            Segunda_Fase();
            ~Segunda_Fase();
    };
} // Fases

#endif //JOGO_SEGUNDA_FASE_H