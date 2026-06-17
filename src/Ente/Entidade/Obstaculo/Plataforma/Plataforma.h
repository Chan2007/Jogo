//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PLATAFORMA_H
#define JOGO_PLATAFORMA_H
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
            public:
                Plataforma(TipoPlataforma tipo = NORMAL1);
                ~Plataforma();
                void criar(TipoPlataforma tipo = NORMAL1);
                void executar(){};
                void salvar(){};
                void mover(){};
                void obstaculizar(Personagens::Jogador* J);
        };
} // Obstaculo

#endif //JOGO_PLATAFORMA_H