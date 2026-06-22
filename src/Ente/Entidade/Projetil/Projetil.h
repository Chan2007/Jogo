//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PROJETIL_H
#define JOGO_PROJETIL_H

#include "Ente/Entidade/Entidade.h"
#include "Ente/Entidade/Personagem/Personagem.h"

namespace Entidades {
    class Projetil : public Entidade {
    protected:
        float dano;
        bool ativo;
        sf::Vector2f velocidade;
        bool doJogador;
        class ProjetilMemento : public EntidadeMemento {
            private:
                float danoMemento;
                bool ativoMemento;
                sf::Vector2f velocidadeMemento;
                bool doJogadorMemento;

                explicit ProjetilMemento(const Projetil& p) :
                    EntidadeMemento(p),danoMemento(p.dano), ativoMemento(p.ativo),
                    velocidadeMemento(p.velocidade), doJogadorMemento(p.doJogador) {}
                ~ProjetilMemento() {}
                friend class Projetil; // Permite ao pai acessar os dados privados
        };
    public:
        Projetil();
        ~Projetil();
        void mover();
        void atualizar();
        void salvar();
        void executar();

        Memento* salvarMemento() const;
        void restaurarMemento(const Memento* memento);


        void setVelocidade(sf::Vector2f v) {
            velocidade.x = v.x;
            velocidade.y = v.y;
        }
        void setVelocidade(const float* vx = NULL, const float* vy = NULL) {
            if (vx != NULL) velocidade.x = *vx;
            if (vy != NULL) velocidade.y = *vy;
        }
        sf::Vector2f getVelocidade() const { return velocidade; }
        void setAtivo(bool a) {
            ativo = a;
            setVigente(ativo);
        }
        bool getAtivo() { return ativo; }
        int getDano() const { return dano; }
        void setDano(int valor) { if (valor >= 0) dano = valor; }

        sf::FloatRect getTamanho() const;

        // void aceitar(VisitorColisao *visitor);

        void setDoJogador(bool valor) { doJogador = valor; }
        bool getDoJogador() const { return doJogador; }

    };
} // Entidades

#endif //JOGO_PROJETIL_H