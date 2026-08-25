//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_CHEFE_H
#define JOGO_CHEFE_H
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"

namespace Personagens {
    class Jogador;
}

namespace Personagens {
    class DragaoAnciao : public Inimigo {
        private:
            short int forca;
            Entidades::Projetil* bolaDeFogo;
            class DragaoAnciaoMemento : public InimigoMemento {
            private:
                short int forcaMemento;
                Entidades::Projetil* bolaDeFogoMemento;
                DragaoAnciaoMemento(const DragaoAnciao& d) : InimigoMemento(d),
                forcaMemento(d.forca), bolaDeFogoMemento(d.bolaDeFogo) {}

                ~DragaoAnciaoMemento() {}
                friend class DragaoAnciao; // Permite ao pai acessar os dados privados
            };
        public:
            DragaoAnciao();
            ~DragaoAnciao();

            Gerenciadores::Memento* salvarMemento() const;
            void restaurarMemento(const Gerenciadores::Memento *memento);

            void danificar(Jogador* J);
            void executar();
            void salvar();

            sf::FloatRect getTamanho() const;
            void setProjetil(Entidades::Projetil* p) {
                if (p) bolaDeFogo = p;
            }
            Entidades::Projetil* getProjetil() { return bolaDeFogo; }
            short int getForca() { return forca; }
    };

}
#endif //JOGO_CHEFE_H