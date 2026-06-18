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
            bool interagindo;
            static std::vector<Jogador*> listaJogadores;
        public:
            Inimigo();
            ~Inimigo();

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
    };
}

#endif //JOGO_INIMIGO_H
