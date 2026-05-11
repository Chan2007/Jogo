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
        private:
            sf::Vector2f velocidade;
            sf::Vector2f aceleracao;
        protected:
            int vida;
            int forca;
            int estado;
        public:
            Personagem();
            sf::Vector2f getVelocidade() const {return velocidade;}
            sf::Vector2f getAceleracao() const {return aceleracao;}
            sf::Vector2f setVelocidade(sf::Vector2f v) {velocidade.x = v.x; velocidade.y = v.y;}
            void setVy(float vy) {velocidade.y = vy;}
            void setVx(float vx) {velocidade.y = vx;}
            virtual ~Personagem();
            virtual void salvarDataBuffer();
            virtual void atualizar() = 0;
            virtual void salvar() = 0;
            virtual void mover() = 0;
    };
} // Personagens

#endif //JOGO_PERSONAGEM_H