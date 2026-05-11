//
// Created by Henrique on 09/05/2026.
//

#ifndef JOGO_PRIMEIRA_FASE_H
#define JOGO_PRIMEIRA_FASE_H
#include "Jogo_Principal/Fase/Fase.h"

namespace Listas {
    class ListaEntidades;
};

namespace Fases {
    class Primeira_Fase: public Fase {
    private:
        const int maxInimigos;
    protected:
        void criarPlataformas();
        void criarObstaculos();
        void criarInimigos();
        void criarProjetil();
    public:
        Primeira_Fase();
        ~Primeira_Fase();
    };
} // Fases

#endif //JOGO_PRIMEIRA_FASE_H