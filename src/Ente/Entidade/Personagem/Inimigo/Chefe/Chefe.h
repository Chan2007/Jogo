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
    float raio;
    int tamanho;
    short int forca;
public:
    Chefe();
    ~Chefe();
    void danificar(Personagens::Jogador* J);
    void atualizar();
    void executar();
    void salvar();
    float getRaio() { return raio; }
    int getTamanho() { return tamanho; };
    short int getForca() { return forca; }
};


#endif //JOGO_CHEFE_H