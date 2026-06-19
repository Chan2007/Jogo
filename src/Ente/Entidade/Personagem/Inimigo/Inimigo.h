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
            bool elite;
            bool interagindo;
            static std::vector<Jogador*> listaJogadores;
        public:
            Inimigo();
            ~Inimigo();

            void setTempoUltimoAtaque(float tempo) { tempoUltimoAtaque = tempo; }
            void setDeslocamentoPatrulha(float deslocamento) { deslocamentoPatrulha = deslocamento; }
            void setElite(bool e) { elite = e; }
            void setInteragindo(bool i) { interagindo = i; }
            void setDirecaoPatrulha(float direcao) { direcaoPatrulha = direcao; }
            float getDirecaoPatrulha() const { return direcaoPatrulha; }

            bool estaEmAlcance(const sf::Vector2f& alvo) const;
            void inverterPatrulha();

            // Padrão visitor
            void aceitar(VisitorColisao *visitor);

            virtual void executar() = 0;
            virtual void salvar() = 0;
            void mover();
            virtual void danificar(Jogador* J) = 0;

            static void incluirJogador(Jogador* J) { listaJogadores.push_back(J); }
            static void limparJogadores() { listaJogadores.clear(); }

            void salvarInimigo();
    };
}

#endif //JOGO_INIMIGO_H
