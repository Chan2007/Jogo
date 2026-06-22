//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_MEDIO_H
#define JOGO_INIMIGO_MEDIO_H
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"

namespace Personagens {
    class Jogador;
}

namespace Personagens {
    class Azulo : public Personagens::Inimigo {
        private:
            int tamanho;
            class AzuloMemento : public InimigoMemento {
                private:
                    int tamanhoMemento;
                    AzuloMemento(const Azulo& a) : InimigoMemento(a), tamanhoMemento(a.tamanho) {}

                    ~AzuloMemento() {}
                    friend class Azulo; // Permite ao pai acessar os dados privados
            };
        public:
            Azulo();
            ~Azulo();
            Memento* salvarMemento() const;
            void restaurarMemento(const Memento* memento);
            void danificar(Personagens::Jogador* J);
            void executar();
            void salvar();
            sf::FloatRect getTamanho() const;
            int getTamanho() { return tamanho; }
    };
}


#endif //JOGO_INIMIGO_MEDIO_H
