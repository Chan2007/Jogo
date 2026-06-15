//
// Created by Henrique on 12/06/2026.
//

#ifndef JOGO_VISITOR_COLISAO_H
#define JOGO_VISITOR_COLISAO_H

namespace Personagens {
    class Jogador;
    class Inimigo;
}
namespace Obstaculos {
    class Obstaculo;
}
namespace Entidades {
    class Projetil;
}

class VisitorColisao {
    public:
        virtual ~VisitorColisao(){};

        virtual void visitar(Personagens::Jogador* j) = 0;
        virtual void visitar(Personagens::Inimigo* i) = 0;
        virtual void visitar(Obstaculos::Obstaculo* o) = 0;
        virtual void visitar(Entidades::Projetil* p) = 0;
};

#endif //JOGO_VISITOR_COLISAO_H