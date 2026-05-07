//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_FACIL_H
#define JOGO_INIMIGO_FACIL_H
#include "Jogo_Principal/Entidade/Personagem/Inimigo/Inimigo.h"

class Inimigo_Facil:public Personagens::Inimigo {
    private:
        float raio;
        int tamanho;
        short int forca;
    public:
        Inimigo_Facil();
        ~Inimigo_Facil();
        void danificar(Jogador* J);
        void executar();
        float getRaio(){return raio;}
        int getTamanho(){return tamanho;};
        short int getForca(){return forca;}
};


#endif //JOGO_INIMIGO_FACIL_H