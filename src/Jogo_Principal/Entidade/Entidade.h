//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_ENTIDADE_H
#define JOGO_ENTIDADE_H

#include <SFML/Graphics.hpp>
#include "Jogo_Principal/Listas/ListaEntidades.h"

namespace Obstaculos {
    class Obstaculo;
}

namespace Personagens {
    class Inimigo;
    class Jogador;
}

namespace Gerenciadores {
    class Mediador_Colisao;
}

namespace Entidades {
    class Projetil;

    class Entidade {
        private:
            sf::Sprite corpo;
            sf::Texture textura;
            static Listas::ListaEntidades* listaEntidades;
            bool colisao;
            std::string nome;
            bool ativo;
        protected:
            virtual void salvarDataBuffer();
            static Gerenciadores::Mediador_Colisao* mediador_colisao;

        public:
            Entidade(std::string nam, Gerenciadores::Mediador_Colisao* mediador);
            virtual ~Entidade();

            virtual void atualizar() = 0;
            virtual void salvar() = 0;
            virtual void mover() = 0;
            virtual void desenhar(sf::RenderWindow& window) = 0;

            // Padrão visitor
            virtual void aoColidir(Entidade* entidade) = 0;
            virtual void interagir_Colisao(Personagens::Jogador* J) = 0;
            virtual void interagir_Colisao(Personagens::Inimigo* I) = 0;
            virtual void interagir_Colisao(Obstaculos::Obstaculo* O) = 0;
            virtual void interagir_Colisao(Projetil* P) = 0;

            sf::Vector2f getPosicao() const {return corpo.getPosition();}
            sf::FloatRect getTamanho() const {return corpo.getGlobalBounds();}
            void setPosicao(sf::Vector2f pos) {corpo.setPosition(pos);}
            void setColisao(bool col) {colisao = col;}
            void setNome(const std::string n) {if (!n.empty()) nome = n;}
            std::string getNome() {return nome;}
            void setAtivo(bool a) {ativo = a;};
            bool getAtivo() const {return ativo;}
            static Listas::ListaEntidades* getListaEntidades() {return listaEntidades;}

    };
} // Entidade

#endif // JOGO_ENTIDADE_H