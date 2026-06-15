//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_OBSTACULO_MEDIO_H
#define JOGO_OBSTACULO_MEDIO_H
#include "Ente/Entidade/Obstaculo/Obstaculo.h"

namespace Personagens {
    class Jogador;
}

namespace Obstaculos {
    class Obstaculo_Medio: public Obstaculo {
        private:
            float altura;
            float largura;
        public:
            Obstaculo_Medio();
            ~Obstaculo_Medio();
            void executar();
            void mover(){}
            void salvar(){}
            float getAltura() { return altura; }
            float getLargura() { return largura; }
            float getDano() { return 0; }
            void obstaculizar(Personagens::Jogador* p);
    };
} // Obstaculo

#endif //JOGO_OBSTACULO_MEDIO_H