//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PERSONAGEM_H
#define JOGO_PERSONAGEM_H
#include "Jogo_Principal/Entidade/Entidade.h"

class Personagem:public Entidade {
    protected:
        int vida;
        int ataque;
        int estado;
    public:
        Personagem();
        ~Personagem();
        void salvarDataBuffer();
        virtual void executar() = 0;
        virtual void salvar() = 0;
        virtual void mover() = 0;
};


#endif //JOGO_PERSONAGEM_H