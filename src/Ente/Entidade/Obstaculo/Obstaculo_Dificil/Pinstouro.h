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
    class Pinstouro: public Obstaculo {
        private:
            float impulso;
            short int danosidade;
            class PinstouroMemento : public ObstaculoMemento {
                private:
                    float impulsoMemento;
                    short int danosidadeMemento;

                    explicit PinstouroMemento(const Pinstouro& p) : ObstaculoMemento(p),
                    impulsoMemento(p.impulso), danosidadeMemento(p.danosidade){}

                    friend class Pinstouro; // Permite ao pai acessar os dados privados
            };
        public:
            Pinstouro();
            ~Pinstouro();

            Memento* salvarMemento() const;
            void restaurarMemento(const Memento *memento);

            void executar();
            void mover() {
                // Não se move
            }
            void salvar();

            float getDano() { return danosidade; }
            void obstaculizar(Personagens::Jogador* p);
    };
} // Obstaculo

#endif //JOGO_OBSTACULO_DIFICIL_H