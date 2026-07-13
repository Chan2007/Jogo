//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_DIFICIL_H
#define JOGO_INIMIGO_DIFICIL_H
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
namespace Personagens {
    class Inimigo_Dificil:public Inimigo {
        private:
            float raio;
            int tamanho;
            short int forca;
        public:
            Inimigo_Dificil();
            ~Inimigo_Dificil();
            void danificar(Jogador* J);
            void executar(float dt);
            float getRaio() const {return raio;}
            int getTamanho(){return tamanho;}
            short int getForca() const {return forca;}
    };
}

#endif //JOGO_INIMIGO_DIFICIL_H
