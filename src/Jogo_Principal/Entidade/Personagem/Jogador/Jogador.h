//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_JOGADOR_H
#define JOGO_JOGADOR_H
#include "Observer_Jogador.h"
#include "Jogo_Principal/Entidade/Personagem/Personagem.h"

class Inimigo;
class Jogador: public Personagem {
    private:
        Observer_Jogador* observer_jogador;
    protected:
        int pontos;
        int escudo;
    public:
        Jogador();
        ~Jogador();
        bool colidir(Inimigo* I);
        void executar();
        void salvar();
        Observer_Jogador* get_observer_jogador();
};


#endif //JOGO_JOGADOR_H