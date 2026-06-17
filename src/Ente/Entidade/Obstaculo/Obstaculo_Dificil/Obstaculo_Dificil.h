//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_OBSTACULO_DIFICIL_H
#define JOGO_OBSTACULO_DIFICIL_H
#include "Ente/Entidade/Obstaculo/Obstaculo.h"

namespace Personagens {
    class Jogador;
}

namespace Obstaculos {
    class Obstaculo_Dificil: public Obstaculo {
        private:
            float impulso;
            short int danosidade;
        public:
            Obstaculo_Dificil();
            ~Obstaculo_Dificil();

            void executar();
            void mover() {
                // Não se move
            }
            void salvar() {
                // TODO
            }

            float getDano() { return danosidade; }
            void obstaculizar(Personagens::Jogador* p);
    };
} // Obstaculo

#endif //JOGO_OBSTACULO_DIFICIL_H