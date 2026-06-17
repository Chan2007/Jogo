//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_CHEFE_H
#define JOGO_CHEFE_H
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"

namespace Personagens {
    class Jogador;
}

class DragaoAnciao : public Personagens::Inimigo {
private:
    short int forca;
    Entidades::Projetil* bolaDeFogo;
public:
    DragaoAnciao();
    ~DragaoAnciao();
    void danificar(Personagens::Jogador* J);
    void executar();
    void salvar();
    void mover(){}
    sf::FloatRect getTamanho() const;
    void setProjetil(Entidades::Projetil* p) { if (p) { bolaDeFogo = p; } }
    Entidades::Projetil* getProjetil() { return bolaDeFogo; }
    short int getForca() { return forca; }
};


#endif //JOGO_CHEFE_H