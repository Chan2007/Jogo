//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PLATAFORMA_H
#define JOGO_PLATAFORMA_H
#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Obstaculo/Obstaculo.h"

namespace Personagens {
    class Jogador;
}

namespace Obstaculos{
        class Plataforma:public Obstaculo {
            public:
                enum TipoPlataforma {
                    NORMAL1,
                    NORMAL2,
                    NORMAL3,
                    CHAO
                };
            private:
                int altura;
                TipoPlataforma tipoPlataforma;
                class PlataformaMemento : public ObstaculoMemento {
                    private:
                        int alturaMemento;
                        TipoPlataforma tipoPlataformaMemento;
                        explicit PlataformaMemento(const Plataforma& p) : ObstaculoMemento(p),
                        alturaMemento(p.altura), tipoPlataformaMemento(p.tipoPlataforma) {}

                        friend class Plataforma; // Permite ao pai acessar os dados privados
                };
            public:
                Plataforma(TipoPlataforma tipo = NORMAL1);
                ~Plataforma();

                Gerenciadores::Memento* salvarMemento() const;
                void restaurarMemento(const Gerenciadores::Memento* memento);

                void criar(TipoPlataforma tipo = NORMAL1);
                void executar(){};
                void salvar();
                void obstaculizar(Personagens::Jogador* J);
                void setAltura(int a) { altura = a; }
                int getAltura() { return altura; }
                void setTipo(TipoPlataforma t) { tipoPlataforma = t; criar(t); }
        };
} // Obstaculo

#endif //JOGO_PLATAFORMA_H