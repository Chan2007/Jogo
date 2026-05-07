//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_JOGADOR_H
#define JOGO_JOGADOR_H
#include "Observer_Jogador/Observer_Jogador.h"
#include "Jogo_Principal/Entidade/Personagem/Personagem.h"

namespace Personagens {
    class Inimigo;
}
namespace Personagens {
    class Jogador: public Personagem {
    private:
        Observador_Jogador* observer_jogador;
    protected:
        float sorte;
        float pontos;
        float escudo;
    public:
        Jogador();
        ~Jogador();
        bool colidir(Personagens::Inimigo* I);
        void executar();
        void salvar();
        Observador_Jogador* get_observer_jogador();
    };
} // Personagens

#endif //JOGO_JOGADOR_H