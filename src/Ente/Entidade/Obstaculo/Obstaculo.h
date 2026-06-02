//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_OBSTACULO_H
#define JOGO_OBSTACULO_H
#include "Ente/Entidade/Entidade.h"

namespace Entidades {
    class Entidade;
    class Projetil;
}
namespace Personagens {
    class Jogador;
    class Inimigo;
}
namespace Obstaculos {
    class Obstaculo : public Entidades::Entidade {
        protected:
            bool perigoso;
        public:
            Obstaculo();
            virtual ~Obstaculo();
            void salvarDataBuffer();
            virtual void desenhar(sf::RenderWindow& window) = 0;
            virtual void atualizar() = 0;
            virtual void salvar() = 0;
            virtual void mover() = 0;
            virtual void setPlataforma(Personagens::Jogador* J) = 0;
            // Padrão visitor
            void aoColidir(Entidade* E) {E->interagir_Colisao(this);}
            void interagir_Colisao(Personagens::Inimigo* I);
            void interagir_Colisao(Obstaculo* O);
            void interagir_Colisao(Entidades::Projetil* P);
            void interagir_Colisao(Personagens::Jogador* J);
    };
} // Obstaculo

#endif //JOGO_OBSTACULO_H