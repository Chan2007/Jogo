//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PLATAFORMA_H
#define JOGO_PLATAFORMA_H


class Plataforma {
private:
    float altura;
    float largura;
public:
    Plataforma();
    ~Plataforma();
    void executar();
    float getAltura();
    float getLargura();
    void setObstaculo(Jogador* J);
};


#endif //JOGO_PLATAFORMA_H