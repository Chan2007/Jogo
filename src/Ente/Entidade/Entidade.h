//
// Created by Henrique on 05/05/2026.
//

#ifndef ENTIDADE_H
#define ENTIDADE_H

#include <SFML/Graphics.hpp>
#include "Ente/Ente.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"
#include "Listas/ListaEntidades.h"

class VisitorColisao;

namespace Obstaculos {
    class Obstaculo;
}

namespace Personagens {
    class Inimigo;
    class Jogador;
    enum EscolhaCampeao;
}


class Observer_Colisao;

namespace Entidades {
    class Projetil;

    enum TipoEntidade {
        ENTIDADE_JOGADOR,
        ENTIDADE_INIMIGO,
        ENTIDADE_OBSTACULO,
        ENTIDADE_PROJETIL
    };

    class Entidade : public Ente {
        private:
            sf::Sprite sprite;
            sf::Texture textura;
            static Listas::ListaEntidades* listaEntidades;
            bool colisao;
            std::string nome;
            bool ativo;
            TipoEntidade tipo;
        protected:
            void salvarDataBuffer();
            Gerenciadores::Gerenciador_Colisao* gerenciadorColisao;
            Gerenciadores::Gerenciador_Gravidade& gerenciadorGravidade;

        public:

            Entidade(const std::string &n);

            virtual ~Entidade();

            void desenhar();
            virtual void executar() = 0;
            virtual void salvar() = 0;
            virtual void mover() = 0;

            virtual void aceitar(VisitorColisao* visitor) = 0;

            sf::Vector2f getPosicao() const {return sprite.getPosition();}
            virtual sf::FloatRect getTamanho() const { return sprite.getGlobalBounds(); }

            sf::Sprite& getSprite() {return sprite;}
            const sf::Sprite& getSprite() const {return sprite;} // Tem funções que aceitam a textura somente como sendo parâmetro constante

            sf::Texture& getTextura() {return textura;}
            const sf::Texture& getTextura() const {return textura;} // Tem funções que aceitam a textura somente como sendo parâmetro constante

            TipoEntidade getTipo() const {return tipo;}

            bool getColisao() const {return colisao;}
            std::string getNome() const {return nome;}
            bool getAtivo() const {return ativo;}

            Gerenciadores::Gerenciador_Colisao* getGerenciadorColisao() { return gerenciadorColisao; }
            Gerenciadores::Gerenciador_Gravidade& getGerenciadorGravidade() { return gerenciadorGravidade; }

            void setPosicao(sf::Vector2f pos) {sprite.setPosition(pos);}
            void setTipo(TipoEntidade t) {tipo = t;}
            void setColisao(bool col) {colisao = col;}
            void setNome(const std::string& n) {if (!n.empty()) nome = n;}
            void setAtivo(bool a) {ativo = a;}
            bool estaDisponivel() const {return ativo && !colisao;}
            static Listas::ListaEntidades* getListaEntidades() {return listaEntidades;}
    };
} // Entidade

#endif // ENTIDADE_H
