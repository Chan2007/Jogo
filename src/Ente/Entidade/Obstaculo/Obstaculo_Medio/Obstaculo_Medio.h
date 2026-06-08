//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_OBSTACULO_MEDIO_H
#define JOGO_OBSTACULO_MEDIO_H
#include "Ente/Entidade/Obstaculo/Obstaculo.h"

namespace Obstaculos {
    class Obstaculo_Medio: public Obstaculo {
        private:
            float altura;
            float largura;
            float dano;
        public:
            Obstaculo_Medio();
            ~Obstaculo_Medio();
            void executar(float dt);
            float getAltura();
            float getLargura();
            float getDano();
            void setPlataforma(Personagens::Jogador* J);
    };
} // Obstaculo

#endif //JOGO_OBSTACULO_MEDIO_H