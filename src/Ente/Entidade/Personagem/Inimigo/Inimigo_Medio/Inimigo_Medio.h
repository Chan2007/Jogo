//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_MEDIO_H
#define JOGO_INIMIGO_MEDIO_H
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"

namespace Personagens {
    class Jogador;
}

class Inimigo_Medio :public Personagens::Inimigo {
private:
    int tamanho;
public:
    Inimigo_Medio();
    ~Inimigo_Medio();
    void danificar(Personagens::Jogador* J);
    void atualizar();
    void executar();
    void salvar();
    void mover();
    sf::FloatRect getTamanho() const;
    int gettamanho() { return tamanho; };
};


#endif //JOGO_INIMIGO_MEDIO_H
