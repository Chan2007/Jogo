//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_DIFICIL_H
#define JOGO_INIMIGO_DIFICIL_H
#include "Jogo_Principal/Entidade/Personagem/Inimigo/Inimigo.h"

class Inimigo_Dificil:public Personagens::Inimigo {
    private:
        float raio;
        int tamanho;
        short int forca;
    public:
        Inimigo_Dificil();
        ~Inimigo_Dificil();
        void danificar(Jogador* J);
        void executar();
        float getRaio(){return raio;}
        int getTamanho(){return tamanho;};
        short int getForca(){return forca;}
};


#endif //JOGO_INIMIGO_DIFICIL_H