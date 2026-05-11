//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_H
#define JOGO_INIMIGO_H
#include "Jogo_Principal/Entidade/Personagem/Personagem.h"

class Jogador;
namespace Personagens {
    class Inimigo: public Personagem {
    protected:
        int vida;
        int ataque;
        int estado;
        int escudo;
    public:
        Inimigo();
        ~Inimigo();
        void salvarDataBuffer();
        virtual void atualizar() = 0;
        virtual void salvar() = 0;
        virtual void danificar(Jogador* J) = 0;
        // Padrão visitor
        void aoColidir(Entidade* E) {E->interagir_Colisao(this);}
        void interagir_Colisao(Inimigo* I);
        void interagir_Colisao(Obstaculos::Obstaculo* O);
        void interagir_Colisao(Entidades::Projetil* P);
        void interagir_Colisao(Jogador* J);
    };
}


#endif //JOGO_INIMIGO_H