//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_JOGADOR_H
#define JOGO_JOGADOR_H
#include "Jogo_Principal/Sistema/Input/Observador_Teclas.h"
#include "Jogo_Principal/Entidade/Personagem/Personagem.h"

namespace Obstaculos {
    class Plataforma;
}

namespace Personagens {
    class Inimigo;
}
namespace Personagens {
    class Jogador: public Personagem {
    private:
        Observador_Teclas* observer_jogador;
    protected:
        float sorte;
        float pontos;
        float escudo;
    public:
        Jogador();
        ~Jogador();
        bool colidir(Inimigo* I);
        void atualizar();
        void salvar();
        Observador_Teclas* get_observer_jogador();
        // Padrão visitor
        void aoColidir(Entidade* E) {E->interagir_Colisao(this);}
        void interagir_Colisao(Inimigo* I);
        void interagir_Colisao(Obstaculos::Obstaculo* O);
        void interagir_Colisao(Entidades::Projetil* P);
        void interagir_Colisao(Jogador* J);
    };
} // Personagens

#endif //JOGO_JOGADOR_H