//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PROJETIL_H
#define JOGO_PROJETIL_H

#include "Ente/Entidade/Entidade.h"

namespace Entidades {
    class Projetil : public Entidade {
    protected:
        int dano;
        sf::Vector2f velocidade;
        bool doJogador;
        float tempoUltimoAtaque;
        float cooldownAtaque;
    public:
        Projetil();
        ~Projetil();
        void mover();
        void atualizar();
        void salvar();
        void executar();
        void setVelocidade(sf::Vector2f v) { velocidade.x = v.x; velocidade.y = v.y; }
        sf::Vector2f getVelocidade() const { return velocidade; }
        void setVy(float vy) { velocidade.y = vy; }
        void setVx(float vx) { velocidade.x = vx; }
        int getDano() const { return dano; }
        void setDano(int valor) { if (valor >= 0) dano = valor; }
        sf::FloatRect getTamanho() const;

        void aceitar(VisitorColisao *visitor);

        void setDoJogador(bool valor) { doJogador = valor; }
        bool getDoJogador() const { return doJogador; }

    };
} // Entidades

#endif //JOGO_PROJETIL_H