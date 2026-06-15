//
// Created by Henrique on 12/06/2026.
//

#ifndef JOGO_VISITOR_COLISAO_INIMIGO_H
#define JOGO_VISITOR_COLISAO_INIMIGO_H

#include "Visitor_Colisao.h"

namespace Personagens {
    class Inimigo;
    class Jogador;
}
namespace Obstaculos {
    class Obstaculo;
}
namespace Entidades {
    class Projetil;
}

class VisitorColisaoInimigo : public VisitorColisao {
    private:
        Personagens::Inimigo* inimigo;

    public:
        VisitorColisaoInimigo(Personagens::Inimigo* i);
        ~VisitorColisaoInimigo();

        void visitar(Personagens::Jogador* J){};
        void visitar(Personagens::Inimigo* I);
        void visitar(Obstaculos::Obstaculo* O);
        void visitar(Entidades::Projetil* P);
};


#endif //JOGO_VISITOR_COLISAO_INIMIGO_H