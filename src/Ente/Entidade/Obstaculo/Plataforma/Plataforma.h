//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PLATAFORMA_H
#define JOGO_PLATAFORMA_H
#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"

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
                float altura;
                float largura;
                TipoPlataforma tipoPlataforma;
            public:
                Plataforma(TipoPlataforma tipo = NORMAL1);
                ~Plataforma();
                void criar(TipoPlataforma tipo = NORMAL1);
                void executar(float dt){};
                void salvar(){};
                void mover(const float dt){};
                float getAltura() const {return altura;}
                float getLargura() const {return largura;}
                void setPlataforma(Personagens::Jogador* J);
        };
} // Obstaculo

#endif //JOGO_PLATAFORMA_H