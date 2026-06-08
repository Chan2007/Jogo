//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_FACIL_H
#define JOGO_INIMIGO_FACIL_H
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"

namespace Personagens {
    class Jogador;
    class Inimigo_Facil : public Inimigo {
    private:
        float raio;
        int tamanho;
        void atualizar(float dt);
    public:
        Inimigo_Facil();
        ~Inimigo_Facil();
        void danificar(Jogador* J);
        void mover(float dt){}
        void executar(float dt);
        void salvar(){}
        float getRaio() const {return raio;}
        int getTamanho() {return tamanho;}
    };
}


#endif //JOGO_INIMIGO_FACIL_H
