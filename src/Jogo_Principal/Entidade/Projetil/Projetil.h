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
        float alcanceMaximo;
        float distanciaPercorrida;
        float tempoVida;
        bool perfurante;
    public:
        Projetil();
        ~Projetil();
        void mover();
        void atualizar();
        void salvar();
        void desenhar(sf::RenderWindow& window);
        void setVelocidade(sf::Vector2f v) {velocidade.x = v.x; velocidade.y = v.y;}
        sf::Vector2f getVelocidade() const {return velocidade;}
        void setVy(float vy) {velocidade.y = vy;}
        void setVx(float vx) {velocidade.x = vx;}
        int getDano() const {return dano;}
        float getAlcanceMaximo() const {return alcanceMaximo;}
        float getDistanciaPercorrida() const {return distanciaPercorrida;}
        float getTempoVida() const {return tempoVida;}
        bool getPerfurante() const {return perfurante;}
        void setDano(int valor) {if (valor >= 0) dano = valor;}
        void setAlcanceMaximo(float valor) {if (valor > 0.0f) alcanceMaximo = valor;}
        void setTempoVida(float valor) {if (valor >= 0.0f) tempoVida = valor;}
        void setPerfurante(bool valor) {perfurante = valor;}
        bool expirou() const;
        void registrarDeslocamento(float delta);
        void aoColidir(Entidades::Entidade* E) {E->interagir_Colisao(this);}
        void interagir_Colisao(Personagens::Inimigo* I);
        void interagir_Colisao(Obstaculos::Obstaculo* O);
        void interagir_Colisao(Projetil* P);
        void interagir_Colisao(Personagens::Jogador* J);
    };
} // Entidades

#endif //JOGO_PROJETIL_H
