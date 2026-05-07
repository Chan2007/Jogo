//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_ENTIDADE_H
#define JOGO_ENTIDADE_H

#include <SFML/Graphics.hpp> // Necessário para sf::Vector2f
#include "Jogo_Principal/Listas/ListaEntidades.h"

namespace Entidades {
    class Entidade {
        private:
            static ListaEntidades* listaEntidades;
            float x, y, vx, vy, ax, ay;
            bool colisao;
            sf::Vector2f tamanho;
            std::string nome;

        protected:
            virtual void salvarDataBuffer();

        public:
            Entidade();
            virtual ~Entidade();

            virtual void executar() = 0;
            virtual void salvar() = 0;
            virtual void mover() = 0;

            sf::Vector2f getPosicao() const {return sf::Vector2f(x, y);}
            sf::Vector2f getVelocidade() const {return sf::Vector2f(vx, vy);}
            sf::Vector2f getAceleracao() const {return sf::Vector2f(ax, ay);}
            sf::Vector2f getTamanho() const {return tamanho;}
            void setPosicao(sf::Vector2f pos) { x = pos.x; y = pos.y; }
            void setColisao(bool col);
            void setNome(std::string n){nome = n;}
            std::string getNome(){return nome;}
            static ListaEntidades* getListaEntidades() {return listaEntidades;}
    };
} // Entidade

#endif // JOGO_ENTIDADE_H