//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_CHEFE_H
#define JOGO_CHEFE_H
#include "Jogo_Principal/Entidade/Personagem/Inimigo/Inimigo.h"

class Chefe: public Inimigo {
    private:
        float raio;
        int tamanho;
        short int forca;
    public:
        Chefe();
        ~Chefe();
        void danificar(Jogador* J);
        void executar();
        float getRaio(){return raio;}
        int getTamanho(){return tamanho;};
        short int getForca(){return forca;}
};


#endif //JOGO_CHEFE_H