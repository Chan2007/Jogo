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
    class Portal: public Obstaculo {
        private:
            float altura;
            float largura;
        public:
            Portal();
            ~Portal();
            void executar();
            void salvar();
            float getAltura() { return altura; }
            float getLargura() { return largura; }
            void obstaculizar(Personagens::Jogador* p);
    };
} // Obstaculo

#endif //JOGO_OBSTACULO_MEDIO_H