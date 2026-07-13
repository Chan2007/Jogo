//
// Created by Henrique on 05/05/2026.
//

#ifndef ENTIDADE_H
#define ENTIDADE_H

#include <SFML/Graphics.hpp>
#include "Ente/Ente.h"
#include "Gerenciador/Gerenciador_Estado/Memento.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"


class VisitorColisao;

namespace Obstaculos {
    class Obstaculo;
}

namespace Personagens {
    class Inimigo;
    class Jogador;
    enum EscolhaCampeao;
}

namespace Entidades {
    class Projetil;

    class Entidade : public Ente {
        protected:
            sf::Sprite sprite;
            sf::Texture textura;
            bool colisao;
            std::string nome;
            bool vigente;
            std::ostream* buffer;
            Gerenciadores::Gerenciador_Colisao* gerenciadorColisao;
            Gerenciadores::Gerenciador_Gravidade& gerenciadorGravidade;
            void salvarDataBuffer();

            class EntidadeMemento : public EnteMemento {
                private:
                    std::ostream* bufferMemento;
                    sf::Sprite spriteMemento;
                    sf::Texture texturaMemento;
                    bool colisaoMemento;
                    std::string nomeMemento;
                    bool vigenteMemento;
                protected:
                    explicit EntidadeMemento(const Entidade& e) : EnteMemento(e),
                        bufferMemento(e.buffer),
                        spriteMemento(e.sprite),
                        texturaMemento(e.textura),
                        colisaoMemento(e.colisao),
                        nomeMemento(e.nome),
                        vigenteMemento(e.vigente) {}
                    virtual ~EntidadeMemento() {}
                    friend class Entidade;
                    friend class Personagens::Jogador;
            };
        public:

            Entidade(const std::string &n);

            virtual ~Entidade();

            void desenhar();
            virtual void executar() = 0;
            virtual void salvar() = 0;
            void setBuffer(std::ostream* stream) { buffer = stream; }
            void salvarEm(std::ostream& output);


            virtual Gerenciadores::Memento* salvarMemento() const;
            virtual void restaurarMemento(const Gerenciadores::Memento* memento);

            // virtual void aceitar(VisitorColisao* visitor) = 0;

            sf::Vector2f getPosicao() const { return sprite.getPosition(); }
            void setPosicao(const sf::Vector2f& pos) { sprite.setPosition(pos); }
            virtual sf::FloatRect getTamanho() const { return sprite.getGlobalBounds(); }

            sf::Sprite& getSprite() { return sprite; }
            const sf::Sprite& getSprite() const { return sprite; } // Tem funções que aceitam a textura somente como sendo parâmetro constante

            sf::Texture& getTextura() { return textura; }
            const sf::Texture& getTextura() const { return textura; } // Tem funções que aceitam a textura somente como sendo parâmetro constante

            std::string getNome() const { return nome; }
            void setNome(const std::string& n) { nome = n; }

            bool getVigente() const { return vigente; }
            void setVigente(bool v) { vigente = v; }

            void setColisao(bool col) { colisao = col; }

    };
} // Entidade

#endif // ENTIDADE_H
