//
// Created by Henrique on 12/06/2026.
//

#ifndef JOGO_VISITOR_COLISAO_JOGADOR_H
#define JOGO_VISITOR_COLISAO_JOGADOR_H

#include "Visitor_Colisao.h"

class VisitorColisaoJogador : public VisitorColisao {
    private:
        Personagens::Jogador* jogador;
    public:
        VisitorColisaoJogador(Personagens::Jogador* J);
        ~VisitorColisaoJogador();

        void visitar(Personagens::Jogador* J);
        void visitar(Personagens::Inimigo* I);
        void visitar(Obstaculos::Obstaculo* O);
        void visitar(Entidades::Projetil* p);
};


#endif //JOGO_VISITOR_COLISAO_JOGADOR_H