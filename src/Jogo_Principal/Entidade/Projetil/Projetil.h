//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PROJETIL_H
#define JOGO_PROJETIL_H
#include "Jogo_Principal/Entidade/Entidade.h"

namespace Entidades {
    class Projetil: public Entidade{
    protected:
        int dano;
        sf::Vector2f velocidade;
    public:
        Projetil();
        ~Projetil();
        void mover();
        void atualizar();
        void salvar();
        void setVelocidade(sf::Vector2f v) {velocidade.x = v.x; velocidade.y = v.y;}
        sf::Vector2f getVelocidade() const {return velocidade;}
        void setVy(float vy) {velocidade.y = vy;}
        void setVx(float vx) {velocidade.y = vx;}
        // Padrão visitor
        void aoColidir(Entidade* E) {E->interagir_Colisao(this);}
        void interagir_Colisao(Personagens::Inimigo* I);
        void interagir_Colisao(Obstaculos::Obstaculo* O);
        void interagir_Colisao(Projetil* P);
        void interagir_Colisao(Personagens::Jogador* J);
    };
} // Entidades


#endif //JOGO_PROJETIL_H