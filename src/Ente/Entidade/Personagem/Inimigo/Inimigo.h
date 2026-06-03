//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_H
#define JOGO_INIMIGO_H

#include "Ente/Entidade/Personagem/Personagem.h"

class Ente;
namespace Personagens {
    class Jogador;

    class Inimigo: public Personagem {
    protected:
        int nivelMaldade;
        int ataque;
        float tempoUltimoAtaque;
        float cooldownAtaque;
        int escudo;
        int ouroConcedido;
        int experienciaConcedida;
        float alcancePerseguicao;
        bool elite;
    public:
        Inimigo();
        ~Inimigo();
        void salvarDataBuffer();
        virtual void atualizar() = 0;
        virtual void salvar() = 0;
        virtual void mover() = 0;
        void desenhar(sf::RenderWindow& window);
        virtual void danificar(Jogador* J) = 0;
        int getAtaque() const {return ataque;}
        int getEscudo() const {return escudo;}
        int getOuroConcedido() const {return ouroConcedido;}
        int getExperienciaConcedida() const {return experienciaConcedida;}
        float getAlcancePerseguicao() const {return alcancePerseguicao;}
        bool getElite() const {return elite;}
        void setAtaque(int valor) {if (valor >= 0) ataque = valor;}
        void setEscudo(int valor) {escudo = (valor < 0) ? 0 : valor;}
        void setOuroConcedido(int valor) {ouroConcedido = (valor < 0) ? 0 : valor;}
        void setExperienciaConcedida(int valor) {experienciaConcedida = (valor < 0) ? 0 : valor;}
        void setAlcancePerseguicao(float valor) {if (valor >= 0.0f) alcancePerseguicao = valor;}
        void setElite(bool valor) {elite = valor;}
        bool estaEmAlcance(const sf::Vector2f& alvo) const;
        int absorverDano(int dano);
        void concederRecompensa(Jogador* J);
        void aoColidir(Entidades::Entidade* E) {E->interagir_Colisao(this);}
        void interagir_Colisao(Inimigo* I);
        void interagir_Colisao(Obstaculos::Obstaculo* O);
        void interagir_Colisao(Entidades::Projetil* P);
        void interagir_Colisao(Jogador* J);
    };
}

#endif //JOGO_INIMIGO_H
