//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PERSONAGEM_H
#define JOGO_PERSONAGEM_H

#include "Ente/Entidade/Entidade.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"

namespace Entidades {
    class Entidade;
}

namespace Personagens {
    enum EstadoAnimacao {
        PARADO = 0,
        ANDANDO = 1,
        PULANDO = 2,
        CAINDO = 3,
        ATACANDO = 4,
        MORTO = 5
    };

    class Personagem : public Entidades::Entidade {
        private:
            sf::Vector2f velocidade;
            sf::Vector2f aceleracao;

            sf::Vector2f tamanhoHitbox;
            sf::Vector2f deslocamentoHitbox;

            float vida;
            float vidaMaxima;
            float ataque;
            float alcanceAtaque;
            float chanceCritica;
            float regeneracaoVida;
            int estado;
            float velocidadeMax;
        protected:
            std::string caminhoArquivoSprite;
            sf::Texture texturaPulo;
            sf::IntRect rectAtual;
            sf::Clock clockAnimacao;
            float tempoPorFrame;
            float tempoAcumulado;
            int indexFrameAtual;
            int totalFramesAnimacao;
            int colunasSpritesheet;
            int linhasSpritesheet;

            Observer_Colisao* ObserverColisao;
        public:
            Personagem();
            virtual ~Personagem();

            // Getters e Setters
            sf::Vector2f getVelocidade() const { return velocidade; }
            void setVelocidade(const sf::Vector2f vel) { velocidade = vel; }
            sf::Vector2f getAceleracao() const { return aceleracao; }
            void setAceleracao(const sf::Vector2f ace) { aceleracao = ace; }

            float getVida() const { return vida; }
            float getVidaMaxima() const { return vidaMaxima; }
            float getVidaPercentual() const;
            float getAtaque() const { return ataque; }
            float getAlcanceAtaque() const { return alcanceAtaque; }
            bool estaVivo() const { return vida > 0; }
            float getChanceCritica() const { return chanceCritica; }
            float getRegeneracao() const { return regeneracaoVida; }
            EstadoAnimacao getEstado() const { return static_cast<EstadoAnimacao>(estado); }

            void setVidaMaxima(int valor);
            void setVida(int valor);
            void setAtaque(const float valor) { if (valor >= 0) ataque = valor; }
            void setAlcanceAtaque(const float valor) { if (valor >= 0) alcanceAtaque = valor; }
            void setVelocidadeMax(const float vMax) { velocidadeMax = vMax; }
            sf::FloatRect getSize() const;
            void setChanceCritica(const float crit) { chanceCritica = crit; }
            void setRegeneracao(const float reg) { regeneracaoVida = reg; }
            void setEstado(const int e) { estado = e; }

            // Lógica de combate
            virtual int receberDano(int dano);
            virtual int causarDano() const;
            virtual void curar(int valor);
            virtual void regenerarVida(float dt);
            void moverHorizontal(float direcao);
            void parar();

            virtual void executar(float dt) = 0;
            virtual void salvar() = 0;
            virtual void mover(float dt) = 0;
    };
} // Personagens

#endif //JOGO_PERSONAGEM_H