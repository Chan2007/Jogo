//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_ENTIDADE_H
#define JOGO_ENTIDADE_H
#include "Jogo_Principal/Listas/ListaEntidades.h"

class Entidade {
    private:
        ListaEntidades* listaEntidades;
        float x, y, vx, vy, ax, ay;
    protected:
        void salvarDataBuffer();
    public:
        Entidade();
        ~Entidade();
        virtual void executar() = 0;
        virtual void salvar() = 0;
        virtual void mover() = 0;
        float getX() const {return x;}
        float getY() const {return y;}
        float getVx() const {return vx;}
        float getVy() const {return vy;}
        float getAx() const {return ax;}
        float getAy() const {return ay;}
};


#endif //JOGO_ENTIDADE_H