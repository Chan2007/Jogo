//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_OBSTACULO_FACIL_H
#define JOGO_OBSTACULO_FACIL_H
#include "Ente/Entidade/Obstaculo/Obstaculo.h"

namespace Obstaculos {
    class Obstaculo_Facil: public Obstaculo {
        private:
            float altura;
            float dano;
        public:
            Obstaculo_Facil();
            ~Obstaculo_Facil();
            void executar(float dt);
            float getAltura();
            float getDano();
            void setPlataforma(Personagens::Jogador* J);
    };
} // Obstaculo

#endif //JOGO_OBSTACULO_FACIL_H