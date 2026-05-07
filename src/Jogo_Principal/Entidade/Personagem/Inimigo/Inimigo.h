//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_H
#define JOGO_INIMIGO_H
#include "Jogo_Principal/Entidade/Personagem/Personagem.h"

class Jogador;
namespace Personagens {
    class Inimigo: public Personagem {
    protected:
        int vida;
        int ataque;
        int estado;
        int escudo;
    public:
        Inimigo();
        ~Inimigo();
        void salvarDataBuffer();
        virtual void executar() = 0;
        virtual void salvar() = 0;
        virtual void danificar(Jogador* J) = 0;
    };
}


#endif //JOGO_INIMIGO_H