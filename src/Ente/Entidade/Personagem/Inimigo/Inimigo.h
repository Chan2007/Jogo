//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_H
#define JOGO_INIMIGO_H

#include "Ente/Entidade/Personagem/Personagem.h"

class Ente;
namespace Personagens {
    class Jogador;

    class Inimigo : public Personagem {
    protected:
        int nivelMaldade;
        int ataque;
        float tempoUltimoAtaque;
        float cooldownAtaque;
        float alcancePerseguicao;
        float direcaoPatrulha;
        float deslocamentoPatrulha;
        float limiteDeslocamento;
        float tempoPatrulha;
        bool elite;
        static std::vector<Jogador*> listaJogadores;
    public:
        Inimigo();
        ~Inimigo();
        void desenhar(sf::RenderWindow& window);
        int getAtaque() const { return ataque; }
        float getAlcancePerseguicao() const { return alcancePerseguicao; }
        bool getElite() const { return elite; }
        void setAtaque(int valor) { if (valor >= 0) ataque = valor; }
        void setAlcancePerseguicao(float valor) { if (valor >= 0.0f) alcancePerseguicao = valor; }
        void setElite(bool valor) { elite = valor; }
        bool estaEmAlcance(const sf::Vector2f& alvo) const;
        void inverterPatrulha();
        void aoColidir(Entidades::Entidade* E) { E->interagir_Colisao(this); }
        void interagir_Colisao(Inimigo* I);
        void interagir_Colisao(Obstaculos::Obstaculo* O);
        void interagir_Colisao(Entidades::Projetil* P);
        void interagir_Colisao(Jogador* J);
        void salvarDataBuffer();
        virtual void atualizar() = 0;
        virtual void salvar() = 0;
        virtual void mover() = 0;
        virtual void danificar(Jogador* J) = 0;
        static void incluirJogador(Jogador* J) { listaJogadores.push_back(J); }
    };
}

#endif //JOGO_INIMIGO_H
