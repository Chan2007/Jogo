//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PROJETIL_H
#define JOGO_PROJETIL_H
#include "Jogo_Principal/Entidade/Entidade.h"

class Projetil:public Entidade{
protected:
    int dano;
    float velocidade;
public:
    Projetil();
    ~Projetil();
    void mover();
    void executar();
    void salvar();
};


#endif //JOGO_PROJETIL_H