//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_FACIL_H
#define JOGO_INIMIGO_FACIL_H
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Ente/Entidade/Projetil/Projetil.h"

namespace Personagens {
    class Jogador;
}

namespace Personagens {
    class Minion : public Personagens::Inimigo {
        private:
            float raio;
            class MinionMemento : public InimigoMemento {
                private:
                    int raioMemento;
                    MinionMemento(const Minion& m) : InimigoMemento(m), raioMemento(m.raio) {}

                    ~MinionMemento() {}
                    friend class Minion; // Permite ao pai acessar os dados privados
            };
        public:
            Minion();
            ~Minion();

            Gerenciadores::Memento* salvarMemento() const;
            void restaurarMemento(const Gerenciadores::Memento *memento);

            void executar();
            void salvar();
            void danificar(Jogador* J);

            sf::FloatRect getTamanho() const;
    };
}


#endif //JOGO_INIMIGO_FACIL_H

