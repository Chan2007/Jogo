//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_MEDIO_H
#define JOGO_INIMIGO_MEDIO_H
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"

namespace Personagens {
    class Jogador;
}

class Azulo :public Personagens::Inimigo {
private:
    int tamanho;
public:
    Azulo();
    ~Azulo();
    void danificar(Personagens::Jogador* J);
    void executar();
    void salvar();
    sf::FloatRect getTamanho() const;
    int gettamanho() { return tamanho; };
};


#endif //JOGO_INIMIGO_MEDIO_H
