//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_OBSTACULO_H
#define JOGO_OBSTACULO_H
#include "Jogo_Principal/Entidade/Entidade.h"

class Jogador;
namespace Obstaculos {
    class Obstaculo : public Entidades::Entidade {
        protected:
            bool perigoso;
        public:
            Obstaculo();
            virtual ~Obstaculo();
            void salvarDataBuffer();
            virtual void executar() = 0;
            virtual void salvar() = 0;
            virtual void mover() = 0;
            virtual void setPlataforma(Jogador* J) = 0;
    };
} // Obstaculo

#endif //JOGO_OBSTACULO_H