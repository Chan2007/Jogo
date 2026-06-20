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

    class Entidade : public Ente {
        private:
            sf::Sprite sprite;
            sf::Texture textura;
            bool colisao;
            std::string nome;
            bool vigente;

        protected:
            std::ostream* buffer;
            Gerenciadores::Gerenciador_Colisao* gerenciadorColisao;
            Gerenciadores::Gerenciador_Gravidade& gerenciadorGravidade;
            void salvarDataBuffer();
        public:

            Entidade(const std::string &n);

            virtual ~Entidade();

            void desenhar();
            virtual void executar() = 0;
            virtual void salvar() = 0;
            void setBuffer(std::ostream* stream) { buffer = stream; }
            void salvarEm(std::ostream& output);

            // virtual void aceitar(VisitorColisao* visitor) = 0;

            sf::Vector2f getPosicao() const {return sprite.getPosition();}
            virtual sf::FloatRect getTamanho() const { return sprite.getGlobalBounds(); }

            sf::Sprite& getSprite() {return sprite;}
            const sf::Sprite& getSprite() const {return sprite;} // Tem funções que aceitam a textura somente como sendo parâmetro constante

            sf::Texture& getTextura() {return textura;}
            const sf::Texture& getTextura() const {return textura;} // Tem funções que aceitam a textura somente como sendo parâmetro constante


            bool getColisao() const {return colisao;}
            std::string getNome() const {return nome;}
            bool getVigente() const {return vigente;}

            Gerenciadores::Gerenciador_Colisao* getGerenciadorColisao() const { return gerenciadorColisao; }
            Gerenciadores::Gerenciador_Gravidade& getGerenciadorGravidade() const { return gerenciadorGravidade; }

            void setPosicao(sf::Vector2f pos) {sprite.setPosition(pos);}
            void setColisao(bool col) {colisao = col;}
            void setNome(const std::string& n) {if (!n.empty()) nome = n;}
            void setVigente(bool a) {vigente = a;}

            bool estaDisponivel() const {return vigente && !colisao;}

    };
} // Entidade

#endif // ENTIDADE_H
