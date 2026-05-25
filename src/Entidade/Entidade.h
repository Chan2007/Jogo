//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_ENTIDADE_H
#define JOGO_ENTIDADE_H

#include <SFML/Graphics.hpp>
#include "Listas/ListaEntidades.h"

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

    enum TipoEntidade {
        ENTIDADE_GENERICA = 0,
        ENTIDADE_JOGADOR,
        ENTIDADE_INIMIGO,
        ENTIDADE_OBSTACULO,
        ENTIDADE_PROJETIL
    };

    class Entidade {
        private:
            sf::Sprite corpo;
            sf::Texture textura;
            static Listas::ListaEntidades* listaEntidades;
            bool colisao;
            std::string nome;
            bool ativo;
            TipoEntidade tipo;
            int nivel;
            int prioridadeRenderizacao;
            float escalaColisao;
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

            virtual void aoColidir(Entidade* entidade) = 0;
            virtual void interagir_Colisao(Personagens::Jogador* J) = 0;
            virtual void interagir_Colisao(Personagens::Inimigo* I) = 0;
            virtual void interagir_Colisao(Obstaculos::Obstaculo* O) = 0;
            virtual void interagir_Colisao(Projetil* P) = 0;

            sf::Vector2f getPosicao() const {return corpo.getPosition();}
            sf::FloatRect getTamanho() const {return corpo.getGlobalBounds();}
            void setPosicao(sf::Vector2f pos) {corpo.setPosition(pos);}
            sf::Sprite& getCorpo() {return corpo;}
            const sf::Sprite& getCorpo() const {return corpo;}
            sf::Texture& getTextura() {return textura;}
            const sf::Texture& getTextura() const {return textura;}
            void setTipo(TipoEntidade t) {tipo = t;}
            TipoEntidade getTipo() const {return tipo;}
            void setNivel(int n) {if (n > 0) nivel = n;}
            int getNivel() const {return nivel;}
            void subirNivel() {++nivel;}
            void setPrioridadeRenderizacao(int prioridade) {prioridadeRenderizacao = prioridade;}
            int getPrioridadeRenderizacao() const {return prioridadeRenderizacao;}
            void setEscalaColisao(float escala) {if (escala > 0.0f) escalaColisao = escala;}
            float getEscalaColisao() const {return escalaColisao;}
            void setColisao(bool col) {colisao = col;}
            bool getColisao() const {return colisao;}
            void setNome(const std::string& n) {if (!n.empty()) nome = n;}
            std::string getNome() const {return nome;}
            void setAtivo(bool a) {ativo = a;}
            bool getAtivo() const {return ativo;}
            bool estaDisponivel() const {return ativo && !colisao;}
            static Listas::ListaEntidades* getListaEntidades() {return listaEntidades;}
    };
} // Entidade

#endif // JOGO_ENTIDADE_H
