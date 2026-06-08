//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_MEDIO_H
#define JOGO_INIMIGO_MEDIO_H
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"

class Inimigo_Medio:public Personagens::Inimigo{
    private:
        float raio;
        int tamanho;
        short int forca;
    public:
        Inimigo_Medio();
        ~Inimigo_Medio();
        void danificar(Personagens::Jogador* J);
        void executar(float dt);
        float getRaio() const {return raio;}
        int getTamanho(){return tamanho;}
        short int getForca() const {return forca;}
};


#endif //JOGO_INIMIGO_MEDIO_H
