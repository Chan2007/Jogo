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
        private:
            float tempoUltimoAtaque;
            float cooldownAtaque;
            int pontosConcedidos;
            float alcancePerseguicao;
            bool elite;
        protected:
            Jogador* jogador;
        public:
            Inimigo();
            virtual ~Inimigo();

            bool devePerseguir(const sf::Vector2f& alvo) const;
            bool deveAtacar(const sf::Vector2f &alvo) const;

            int receberDano(int dano);
            void concederRecompensa(Jogador* J) const;

            float getTempoUltimoAtaque() const{ return tempoUltimoAtaque; }
            float getCooldownAtaque() const { return cooldownAtaque; }
            int getPontosConcedidos() const { return pontosConcedidos; }
            float getAlcancePerseguicao() const { return alcancePerseguicao; }
            bool getElite() const { return elite; }

            void setTempoUltimoAtaque(const float tempo) { tempoUltimoAtaque = tempo; }
            void setCooldownAtaque(const float cooldown) { cooldownAtaque = cooldown; }
            void setPontosConcedidos(const int pontos) { pontosConcedidos = pontos; }
            void setAlcancePerseguicao(const float alcance) { alcancePerseguicao = alcance; }
            void setElite(const bool e) { elite = e; }
            void setJogadorAlvo(Jogador* J) { jogador = J; }

            void aoColidir(Entidade* E) {
                E->interagir_Colisao(this);
            }
            void interagir_Colisao(Inimigo* I);
            void interagir_Colisao(Obstaculos::Obstaculo* O);
            void interagir_Colisao(Entidades::Projetil* P);
            void interagir_Colisao(Jogador* J);

    };
}

#endif //JOGO_INIMIGO_H
