//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_OBSTACULO_DIFICIL_H
#define JOGO_OBSTACULO_DIFICIL_H
#include "Ente/Entidade/Obstaculo/Obstaculo.h"

namespace Obstaculos {
    class Obstaculo;
    class Obstaculo_Dificil:public Obstaculo {
    private:
        float altura;
        float largura;
        float dano;
        float velocidade;
    public:
        Obstaculo_Dificil();
        ~Obstaculo_Dificil();
        void executar(float dt);
        float getAltura();
        float getLargura();
        float getDano();
        float getVelocidade();
        void setPlataforma(Personagens::Jogador* J);
    };
} // Obstaculo

#endif //JOGO_OBSTACULO_DIFICIL_H