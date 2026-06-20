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
            virtual void executar() = 0;
            virtual void salvar() = 0;
            virtual void obstaculizar(Personagens::Jogador* J) = 0;

            // Padrão visitor
            // void aceitar(VisitorColisao *visitor);
            void salvarObstaculo();
    };
} // Obstaculo

#endif //JOGO_OBSTACULO_H