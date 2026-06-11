//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_CHEFE_H
#define JOGO_CHEFE_H
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"

namespace Personagens {
    class Jogador;
}

class Chefe : public Personagens::Inimigo {
private:
    short int forca;
public:
    Chefe();
    ~Chefe();
    void danificar(Personagens::Jogador* J);
    void executar();
    void salvar();
    void mover(){};
    sf::FloatRect getTamanho() const;
    short int getForca() { return forca; }
};


#endif //JOGO_CHEFE_H