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
            class PortalMemento : public ObstaculoMemento {
                private:
                    float alturaMemento;
                    float larguraMemento;

                    explicit PortalMemento(const Portal& p) : ObstaculoMemento(p),
                    alturaMemento(p.altura), larguraMemento(p.largura){}
                    friend class Portal; // Permite ao pai acessar os dados privados
            };
        public:
            Portal();
            ~Portal();

            Memento *salvarMemento() const;
            void restaurarMemento(const Memento *memento);

            void executar();
            void mover() {
                // Não se move
            };
            void salvar();

            float getAltura() { return altura; }
            float getLargura() { return largura; }
            void obstaculizar(Personagens::Jogador* p);
    };
} // Obstaculo

#endif //JOGO_OBSTACULO_MEDIO_H