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
            class InimigoMemento : public PersonagemMemento {
                private:
                    int nivelMaldadeMemento;
                    int ataqueMemento;
                    float tempoUltimoAtaqueMemento;
                    float cooldownAtaqueMemento;
                    float alcancePerseguicaoMemento;
                    float direcaoPatrulhaMemento;
                    float deslocamentoPatrulhaMemento;
                    float limiteDeslocamentoMemento;
                    bool eliteMemento;
                    bool interagindoMemento;
                    static std::vector<Jogador*> listaJogadoresMemento;
                protected:
                    InimigoMemento(const Inimigo& i) : PersonagemMemento(i),
                    nivelMaldadeMemento(i.nivelMaldade), ataqueMemento(i.ataque),
                    tempoUltimoAtaqueMemento(i.tempoUltimoAtaque), cooldownAtaqueMemento(i.cooldownAtaque),
                    alcancePerseguicaoMemento(i.alcancePerseguicao), direcaoPatrulhaMemento(i.direcaoPatrulha),
                    deslocamentoPatrulhaMemento(i.deslocamentoPatrulha),
                    limiteDeslocamentoMemento(i.limiteDeslocamento), eliteMemento(i.elite),
                    interagindoMemento(i.interagindo) {}

                    virtual ~InimigoMemento() {}
                    friend class Inimigo; // Permite ao pai acessar os dados privados
            };
        public:
            Inimigo();
            ~Inimigo();

            virtual Gerenciadores::Memento* salvarMemento() const;
            virtual void restaurarMemento(const Gerenciadores::Memento* memento);

            void setTempoUltimoAtaque(float tempo) { tempoUltimoAtaque = tempo; }
            void setDeslocamentoPatrulha(float deslocamento) { deslocamentoPatrulha = deslocamento; }
            void setElite(bool e) { elite = e; }
            void setInteragindo(bool i) { interagindo = i; }
            void setDirecaoPatrulha(float direcao) { direcaoPatrulha = direcao; }

            float getDirecaoPatrulha() const { return direcaoPatrulha; }
            bool estaEmAlcance(const sf::Vector2f& alvo) const;
            void inverterPatrulha();

            // Padrão visitor
            // void aceitar(VisitorColisao *visitor);

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
