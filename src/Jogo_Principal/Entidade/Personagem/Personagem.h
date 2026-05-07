//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PERSONAGEM_H
#define JOGO_PERSONAGEM_H

#include "Jogo_Principal/Entidade/Entidade.h"
namespace Entidades {
    class Entidade;
}
namespace Personagens {
    class Personagem : public Entidades::Entidade {
    protected:
        int vida;
        int forca;
        int estado;
    public:
        Personagem();
        virtual ~Personagem();
        virtual void salvarDataBuffer();
        virtual void executar() = 0;
        virtual void salvar() = 0;
        virtual void mover() = 0;
    };
} // Personagens

#endif //JOGO_PERSONAGEM_H