//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_FACIL_H
#define JOGO_INIMIGO_FACIL_H
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"
#include "Ente/Entidade/Projetil/Projetil.h"

namespace Personagens {
    class Jogador;
}

class Minion : public Personagens::Inimigo {
    private:
        float raio;
        Entidades::Projetil* tiro;
    public:
        Minion();
        ~Minion();
        void danificar(Personagens::Jogador* J);

        sf::FloatRect getTamanho() const;
        void executar();
        void mover();
        void salvar();
        void setProjetil(Entidades::Projetil* p) { if (p) { tiro = p; } }
        Entidades::Projetil* getProjetil() { return tiro; }
        float getRaio() { return raio; }
};


#endif //JOGO_INIMIGO_FACIL_H

