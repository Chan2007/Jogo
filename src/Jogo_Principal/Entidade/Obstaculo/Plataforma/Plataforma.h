//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PLATAFORMA_H
#define JOGO_PLATAFORMA_H
#include "Jogo_Principal/Entidade/Obstaculo/Obstaculo.h"

namespace Obstaculos{
        class Plataforma:public Obstaculo {
            private:
                float altura;
                float largura;
            public:
                Plataforma();
                ~Plataforma();
                void executar();
                float getAltura();
                float getLargura();
                void setPlataforma(Jogador* J);
        };
} // Obstaculo

#endif //JOGO_PLATAFORMA_H