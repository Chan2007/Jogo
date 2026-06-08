//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PROJETIL_H
#define JOGO_PROJETIL_H

#include "Ente/Entidade/Entidade.h"

namespace Entidades {
    class Projetil: public Entidade{
    protected:
        int dano;
        sf::Vector2f velocidade;
        float alcanceMaximo;
        float distanciaPercorrida;
        float tempoVida;
        bool perfurante;
    public:
        Projetil();
        ~Projetil();

        void mover(float dt);
        void executar(float dt);
        void salvar();

        void setVelocidade(sf::Vector2f v) {velocidade.x = v.x; velocidade.y = v.y;}
        void setDano(int valor) {if (valor >= 0) dano = valor;}
        void setAlcanceMaximo(float valor) {if (valor > 0.0f) alcanceMaximo = valor;}
        void setTempoVida(float valor) {if (valor >= 0.0f) tempoVida = valor;}
        void setPerfurante(bool valor) {perfurante = valor;}

        sf::Vector2f getVelocidade() const {return velocidade;}
        int getDano() const {return dano;}
        float getAlcanceMaximo() const {return alcanceMaximo;}
        float getDistanciaPercorrida() const {return distanciaPercorrida;}
        float getTempoVida() const {return tempoVida;}
        bool getPerfurante() const {return perfurante;}

        bool expirou() const;
        void registrarDeslocamento(float delta);

        void aoColidir(Entidade* E) {
            E->interagir_Colisao(this);
        }
        void interagir_Colisao(Personagens::Inimigo* I);
        void interagir_Colisao(Obstaculos::Obstaculo* O);
        void interagir_Colisao(Projetil* P);
        void interagir_Colisao(Personagens::Jogador* J);
    };
} // Entidades

#endif //JOGO_PROJETIL_H
